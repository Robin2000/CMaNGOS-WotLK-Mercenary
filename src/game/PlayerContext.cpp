#include <hash_map>
#include "ObjectMgr.h"
#include "Player.h"
#include "SpellMgr.h"
#include "GridMap.h"
#include "MercenaryMgr.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_for_each.h"
#include "Log.h"
#include "PlayerContext.h"
#include "MovementGenerator.h"
#include "movement/MoveSplineInit.h"
#include "movement/MoveSplineInitArgs.h"
#include "movement/MoveSpline.h"
#include "DynamicObject.h"
#include "MoveMap.h"     
#include "Vehicle.h"
#include <algorithm>    // std::find
// for mmap manager

PlayerContext::PlayerContext(Player* _player) :mPlayer(_player), gamePointMgr(_player), delayActionQueue(0), eventPlugin(_player), prSpellPlugin(_player), prQuestPlugin(_player){
	questPOIVec = new tbb::concurrent_vector<QuestPOIPoint *>();
	questNpcGOVec = new tbb::concurrent_vector<QuestNpcGO *>();
	isMovesplineStopNow = false;
	isMovesplineRunning = false;
	mMercenary = nullptr;
}
PlayerContext::~PlayerContext(){
	DelayedAction *action;
	while (delayActionQueue.pop(action))
	{
		delete action;
	}

	if (transportStopAction != nullptr)
		delete transportStopAction;

	delete questPOIVec;
	delete questNpcGOVec;
}

void findFlyPath(Player* player, float x, float y, float z, PointsArray* result)
{
	float px = player->GetPositionX();
	float py = player->GetPositionY();
	//float stepX = 12.0f;
	//float stepY = 12.0f;

	float stepX = 3.0f;
	float stepY = 3.0f;

	//float stepX = (x-px)/50.0f;
	//float stepY = (y-py)/50.0f;

	bool dx = px - x < 0;//px<x时为正向x
	bool dy = py - y < 0;//py<y时为正向y

	result->push_back(Vector3(px, py, player->GetPositionZ()));

	float oldz = -200000.0f;
	
	for (float i = px, j = py; (dx ? i - x<0 : i - x>0) && (dy ? j - y<0 : j - y>0); i += dx ? stepX : -stepX, j += dy? stepY : -stepY)
	{
		const TerrainInfo* info = player->GetMap()->GetTerrain();
		float groundz = info->GetWaterOrGroundLevel(i, j, MAX_HEIGHT);
		if (groundz == -200000.0f)//VMAP_INVALID_HEIGHT_VALUE
			continue;
		if (groundz-oldz>0)
			oldz = groundz;

		result->push_back(Vector3(i, j, 50.0f + oldz)); //如果上一个z更大，就直接使用上一个z，避免颠簸

	}

	result->push_back(Vector3(x, y, z));
}
class TeleportAction :public DelayedAction{

public:
	TeleportAction(Player * _player, int _timelimit) : DelayedAction(_timelimit), player(_player){}
	void run() override{
		player->UpdateForQuestWorldObjects();
		player->SetCanFly(false);
		//player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);//设置传送结束允许攻击
	}
	Player * player;
};
class TransportStopAction :public StopAction{

public:
	TransportStopAction(Player * _player, PathFinder* _path) :player(_player), path(_path){};
	TransportStopAction(Player * _player, PointsArray* _points) :player(_player), points(_points){};
	~TransportStopAction(){ 
		if (points)
			delete points; 
		else if (path)
			delete path; 
	}
	void run() override{

		if (Pet *pet = player->GetPet())
		if (pet->isMercenary())
		{
			if (player->GetVehicleInfo()->HasOnBoard(pet))
			{
				player->GetVehicleInfo()->UnBoard(pet, false);
				player->GetMap()->AddUpdateObject(pet);
			}

			if (Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow()))
				mercenary->Create(mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetType(), mercenary->GetRole());
			//if (pet->IsMounted())
				//pet->Unmount(false);//取消骑乘
		}
			
		if (player->IsMounted())
			player->Unmount(false);//取消骑乘

		if (player->GetVehicleInfo()->HasOnBoard(player))
		{
			player->GetVehicleInfo()->UnBoard(player, false);
			player->GetVehicleInfo()->RemoveAccessoriesFromMap();
			player->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_PLAYER_VEHICLE);
			player->SetVehicleId(0, 0);
		}


		//delete player->GetVehicleInfo();

		player->UpdateForQuestWorldObjects();
		//player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);//设置传送结束允许攻击
		
	}
private:
	PathFinder* path;
	PointsArray* points;
	Player * player;
};
class TransportAction :public DelayedAction{

public:
	TransportAction(Player * _player, PathFinder* _path, int _timelimit) : DelayedAction(_timelimit), player(_player), path(_path){}
	TransportAction(Player * _player, PointsArray* _points, int _timelimit) : DelayedAction(_timelimit), player(_player), points(_points){}
	
	void run() override{

		PointsArray& pointPath = (points == nullptr) ? path->getPath() : *points;
		
		float speed = 24.0f;

		//mPlayer->AddMountSpellAura(tanMountSpell[rand() % 7]);//随机飞毯

		//TransportStopAction * transportStopAction = new TransportStopAction(mPlayer, x, y, z, orientation, speed, STOP_EVENT_FLY_TO_SKY);
		//addDelayedAction(new TransportAction(mPlayer, mPlayer->GetPositionX(), mPlayer->GetPositionY(), mPlayer->GetPositionZ() + 72.0f, orientation, 24.0f, 1000, transportStopAction));//0秒后，每秒 800.0f米
		//Movement::MoveSplineInit init(*mPlayer);
		//init.MoveTo(x, y, z, true, true);
		//init.MovebyPath(const PointsArray& controls, int32 path_offset)

		//init.SetVelocity(speed);
		//init.Launch();



		//Vector3 start = path.getStartPosition();
		//Vector3 end = path.getEndPosition();
		//Vector3 actualEnd = path.getActualEndPosition();


		if (!player->isGameMaster())
		{
			ChatHandler(player).SendSysMessage("Enable GM mode to see the path points.");

			for (uint32 i = 0; i < pointPath.size(); ++i)
				player->SummonCreature(VISUAL_WAYPOINT, pointPath[i].x, pointPath[i].y, pointPath[i].z, 0, TEMPSUMMON_TIMED_DESPAWN, 60000);
		}
		//mPlayer->SetVehicleId(774, 40725);//小型观光火箭VehicleTemplate=774,entry=40725

		/*uint32 size = pointPath.size();
		if (size>2){
			for (uint32 i = 1; i <pointPath.size() - 1; ++i)//增加高度防止掉地下
			pointPath[i].z += 20.0f;
		}
		else if (size==2)
		{
			Vector3 second(pointPath[0].x, pointPath[0].y, pointPath[0].z+20.0f);
			Vector3 third(pointPath[1].x, pointPath[1].y, pointPath[1].z + 20.0f);
			pointPath.insert(pointPath.begin() + 1, second);
			pointPath.insert(pointPath.begin() + 2, third);
		}*/

		if (player->IsMounted())
			player->Unmount();

		//此处为modelid
		//const static uint32 mount_model[5] = { 28890, 26611, 23647, 31992, 23656 };//[28890]X-53型观光火箭,[26611]火箭推进弹头,[23647]X-51虚空火箭特别加强版,[31992]X-53型观光火箭 [23656]X-51虚空火箭
		//const static uint32 mount_vehicle[5] = { 28890, 26611, 23647, 31992, 23656 };//[28890]X-53型观光火箭,[26611]火箭推进弹头,[23647]X-51虚空火箭特别加强版,[31992]X-53型观光火箭 [23656]X-51虚空火箭

		//uint32 model = mount_model[rand() % 5];
		player->Mount(31992);//随机

		if (player->GetVehicleInfo() == nullptr)
		{
			player->SetVehicleId(774, 40725);//小型观光火箭VehicleTemplate=774,entry=40725 model=31992
			player->GetVehicleInfo()->Initialize();
			player->GetVehicleInfo()->Board(player,0);
		}

		if (Pet *pet = player->GetPet()){
			if (pet->isMercenary()){

				//pet->Mount(model);//随机
				player->GetVehicleInfo()->Board(pet,1);
				player->GetMap()->RemoveUpdateObject(pet);
				/*Movement::MoveSplineInit init(*pet);
				init.MovebyPath(pointPath);
				init.SetVelocity(speed);
				init.SetFly();
				init.SetWalk(false);
				init.Launch();*/
			}
		}

		Movement::MoveSplineInit init(*player);
		init.MovebyPath(pointPath);
		init.SetVelocity(speed);
		init.SetFly();
		init.SetWalk(false);
		init.Launch();



		if (player->context.transportStopAction != nullptr)
			delete player->context.transportStopAction;//清理上次的

		if (points==nullptr)
			player->context.transportStopAction = new TransportStopAction(player, path);
		else
			player->context.transportStopAction = new TransportStopAction(player, points);


		player->context.isMovesplineStopNow = false;
		player->context.isMovesplineRunning = true;

		//player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);//设置传送中不得攻击
	}
	PathFinder* path;
	PointsArray* points;
	Player * player;
};

void PlayerContext::moveFast(uint32 mapid, uint32 zone, uint32 area, float x, float y, float z, float orientation){


	//if (area == mPlayer->GetAreaId()){
	//	addDelayedAction(new TransportAction(mPlayer, x, y, z, orientation,2000));
	//}
	//else 
	if (!disableFindPath&&zone == mPlayer->GetZoneId()){


		if (!MMAP::MMapFactory::createOrGetMMapManager()->GetNavMesh(mPlayer->GetMapId()))
		{
			teleport(mapid, x, y, z, orientation);
			return;
		}

		float dx = x - mPlayer->GetPositionX();
		float dy = y - mPlayer->GetPositionY();
		float dz = z - mPlayer->GetPositionZ();

		float dsqr = dx*dx + dy*dy+dz*dz;
		if (dsqr<3.0 * 3.0)
		{
			ChatHandler(mPlayer).SendSysMessage(-2800685);
			return;
		}
		else if (dsqr>50 * 50)//50码之外飞行
		{
			PointsArray* result = new PointsArray();

			findFlyPath(mPlayer, x, y+0.5, z+0.5, result);
			
			addDelayedAction(new TransportAction(mPlayer, result, 800));

		}
		else
		{
			PathFinder* path = new PathFinder(mPlayer);
			path->setUseStrightPath(false);
			//path->setPathLengthLimit(8.0f);
			path->calculate(x , y + 0.5, z + 0.5, true); // true为强制到达目标点
			//path->BuildPolyPath(path->getStartPosition(), path->getEndPosition());

			if (!(path->getPathType() & PATHFIND_NORMAL || path->getPathType() & PATHFIND_NOT_USING_PATH))
			{
				delete path;
				teleport(mapid, x, y, z, orientation);
				return;
			}
			
			PointsArray& result=path->getPath();
			for (int i = 0; i < result.size(); i++)
				result[i].z += 0.5; //避免掉地下

			Movement::MoveSplineInit init(*mPlayer); //50码之内跑过去
			init.MovebyPath(result);
			init.SetVelocity(10.0f);
			init.SetWalk(false);
			init.Launch();

			if (Pet * pet=mPlayer->GetPet())
			if (pet->isMercenary()){
				Movement::MoveSplineInit init(*pet); //50码之内跑过去
				init.MovebyPath(result);
				init.SetVelocity(12.0f);
				init.SetWalk(false);
				init.Launch();
			}

			mPlayer->context.isMovesplineStopNow = false;
			mPlayer->context.isMovesplineRunning = true;
		}
		//mPlayer->AddMountSpellAura(tanMountSpell[rand() % 7]);//随机飞毯

		//mPlayer->SetVehicleId(774, 40725);//小型观光火箭VehicleTemplate=774,entry=40725
		//mPlayer->GetVehicleInfo()->Initialize();
		//mPlayer->GetVehicleInfo()->Board(mPlayer);
		
		//
		
	}
else
	teleport(mapid, x, y, z, orientation);
}
void PlayerContext::teleport(uint32 mapid, float x, float y, float z, float orientation){

	mPlayer->UnsummonPetTemporaryIfAny();

	if (mapid == mPlayer->GetMapId())
	{
		mPlayer->NearTeleportTo(x, y+0.5, z + 0.5f, 0.0f - orientation);
	}
	else
	{
		mPlayer->TeleportTo(mapid, x, y + 0.5, z + 0.5f, 0.0f - orientation);
	}	

	//mPlayer->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);//设置传送中不得攻击
	this->addDelayedAction(new TeleportAction(mPlayer, 1000));//延迟1秒
	
}
void DelayedAction::Update(uint32 update_diff){
	
	if (timeout)
		return;

	timelimit -= update_diff;
	//DETAIL_LOG("remain:time: %d,diff:%u", timelimit, update_diff);
	if (timelimit < 0)
	{
		timeout = true;
		run();
	}
}

void PlayerContext::Update(uint32 update_diff, uint32 time){
	
	if (isMovesplineStopNow)
	{
		isMovesplineStopNow = false;
		isMovesplineRunning = false;
		if (transportStopAction!=nullptr)
			transportStopAction->run();
	}

	if (heart_stone_cooldown > 0)
		heart_stone_cooldown -= update_diff;

	DelayedAction *action;
	if (delayActionQueue.pop(action))
	{
		action->Update(update_diff);

		if (!action->timeout)
			delayActionQueue.push(action);
		else
			delete action;
	}

}
void PlayerContext::addDelayedAction(DelayedAction * action){
	delayActionQueue.push(action);
}

void PlayerContext::addMorphHistory(uint32 creature, uint32 displayid){

	if (find(displayhistory.begin(), displayhistory.end(), displayid) != displayhistory.end())
		return;

	displayhistory.insert(displayhistory.begin(), displayid);
	creaturehistory.insert(creaturehistory.begin(), creature);

	if (creaturehistory.size()>8)
		creaturehistory.pop_back();

	if (displayhistory.size()>8)
		displayhistory.pop_back();
}

void PlayerContext::checkFirstGuideQuest(){
	if (!firstQuestChecked){
		const Quest *quest = sObjectMgr.GetQuestTemplate(99999);/*新手指引任务*/
		if (mPlayer->CanRewardQuest(quest, false))
			mPlayer->RewardQuest(quest, 0, mPlayer, false);
		mPlayer->context.firstQuestChecked = true;
	}
}
void PlayerContext::loadMercenary(){
	mMercenary = sMercenaryMgr->LoadMercenarie(mPlayer);
}
void PlayerContext::OnSaveMercenary()
{
	sMercenaryMgr->OnSave(mPlayer);
}
bool IsSpellFitByClassAndRace(uint32 racemask, uint32 classmask, uint32 spell_id, uint32 level)
{
	SkillLineAbilityMapBounds bounds = sSpellMgr.GetSkillLineAbilityMapBounds(spell_id);
	if (bounds.first == bounds.second)
		return true;

	for (SkillLineAbilityMap::const_iterator _spell_idx = bounds.first; _spell_idx != bounds.second; ++_spell_idx)
	{
		SkillLineAbilityEntry const* abilityEntry = _spell_idx->second;
		// skip wrong race skills
		if (abilityEntry->racemask && (abilityEntry->racemask & racemask) == 0)
			continue;

		// skip wrong class skills
		if (abilityEntry->classmask && (abilityEntry->classmask & classmask) == 0)
			continue;

		SkillRaceClassInfoMapBounds bounds = sSpellMgr.GetSkillRaceClassInfoMapBounds(abilityEntry->skillId);
		for (SkillRaceClassInfoMap::const_iterator itr = bounds.first; itr != bounds.second; ++itr)
		{
			SkillRaceClassInfoEntry const* skillRCEntry = itr->second;
			if ((skillRCEntry->raceMask & racemask) && (skillRCEntry->classMask & classmask))/*仅race和class匹配的记录才可能被排除在外*/
			{
				if (skillRCEntry->flags & ABILITY_SKILL_NONTRAINABLE)/*不需要训练的技能被排除在外*/
					return false;

				if (skillRCEntry->reqLevel && level < skillRCEntry->reqLevel)/*等级不符合的被排除在外*/
					return false;
			}
		}

		return true;
	}

	return false;
}
void PlayerContext::getDefaultSpells(std::vector<uint32>& vec, uint8 race, uint8 cla, uint32 level)
{
	ChrClassesEntry const* clsEntry = sChrClassesStore.LookupEntry(cla);
	if (!clsEntry)
		return;

	std::hash_map<uint32, SpellEntry const*> map;
	uint32 family = clsEntry->spellfamily;

	for (uint32 i = 0; i < sSkillLineAbilityStore.GetNumRows(); ++i)
	{
		SkillLineAbilityEntry const* entry = sSkillLineAbilityStore.LookupEntry(i);
		if (!entry)
			continue;

		SpellEntry const* spellInfo = sSpellStore.LookupEntry(entry->spellId);
		if (!spellInfo)
			continue;

		// skip server-side/triggered spells
		if (spellInfo->spellLevel == 0)
			continue;


		// skip other spell families
		if (spellInfo->SpellFamilyName != family)
			continue;

		// skip spells with first rank learned as talent (and all talents then also)
		uint32 first_rank = sSpellMgr.GetFirstSpellInChain(spellInfo->Id);
		if (GetTalentSpellCost(first_rank) > 0)
			continue;

		// skip broken spells
		if (!SpellMgr::IsSpellValid(spellInfo, mPlayer, false))
			continue;

		if ((spellInfo->speed == 0 && spellInfo->DurationIndex == 0))
			continue;

		if (spellInfo->spellLevel >level)
			continue;

		// skip wrong class/race skills
		if (!IsSpellFitByClassAndRace(spellInfo->Id, race, cla, level))
			continue;


		auto it = map.find(spellInfo->SpellIconID);
		if (it != map.end())//找到
		{
			if (it->second->spellLevel <= spellInfo->spellLevel)
				//	map[spellInfo->SpellIconID] = spellInfo;
				it->second = spellInfo;
		}
		else//没找到
			map[spellInfo->SpellIconID] = spellInfo;
	}
	for (auto itr = map.begin(); itr != map.end(); ++itr)
		vec.push_back(itr->second->Id);
}

/*数据量太大，采用并行算法提高效率*/
struct Z_QuestNpcGo{
	uint32 id;
	uint32 map;
	uint32 zone;
	uint32 area;
	uint32 quest;
	int32 npcgo;
	float x;
	float y;
	float z;
	float orientation;
	uint8 ntype;
	int16 MinLevel;
};
typedef tbb::concurrent_vector<Z_QuestNpcGo*> QuestNpcGOQueue;
typedef tbb::concurrent_unordered_map<uint32, QuestNpcGOQueue*> QuestNpcGOMap;
typedef tbb::concurrent_unordered_map<uint32, TerrainInfo*> UpdateTerrainInfoMap;

void PlayerContext::calculateParallelZone_quest_npcgo_all_map(){

	FILE* log_file;
	log_file = fopen("e:/data.txt", "w");


	//SqlStatementID updatenpcgo;
	//SqlStatement stmt = WorldDatabase.CreateStatement(updatenpcgo, "UPDATE z_quest_npcgo_all_map SET zone = ?, area = ? WHERE id = ?");

											//        0   1    2          3            4       5           6     7     8       9
	QueryResult* result = WorldDatabase.Query("SELECT id,map,position_x,position_y,position_z,orientation,quest,npcgo,ntype,MinLevel FROM z_quest_npcgo_all_map order by map");
	
	QuestNpcGOMap questNpcGOMap;
	UpdateTerrainInfoMap updateTerrainInfoMap;
	QuestNpcGOQueue* questNpcGOQueue =nullptr;

	uint32 oldmap = 99999999;//标志变量，order by map，每张map改变一次
	
	do
	{
		Field* fields = result->Fetch();
		Z_QuestNpcGo* questNpcGO = new Z_QuestNpcGo();
		questNpcGO->id = fields[0].GetUInt32();
		questNpcGO->map = fields[1].GetUInt32();
		questNpcGO->x = fields[2].GetFloat();
		questNpcGO->y = fields[3].GetFloat();
		questNpcGO->z = fields[4].GetFloat();
		questNpcGO->orientation = fields[5].GetFloat();
		questNpcGO->quest = fields[6].GetUInt32();
		questNpcGO->npcgo = fields[7].GetInt32();
		questNpcGO->ntype = fields[8].GetUInt8();
		questNpcGO->MinLevel = fields[9].GetInt16();
		
		if (questNpcGO->MinLevel < 0)
			questNpcGO->MinLevel = 0;

		if (oldmap != questNpcGO->map){
			oldmap = questNpcGO->map;

			TerrainInfo* terrain = sTerrainMgr.LoadTerrain(questNpcGO->map);
			updateTerrainInfoMap[questNpcGO->map] = terrain;

			questNpcGOQueue = new QuestNpcGOQueue();
			questNpcGOMap[questNpcGO->map] = questNpcGOQueue;

		}
		questNpcGOQueue->push_back(questNpcGO);

	} while (result->NextRow());
	delete result;
	
	sLog.outError("questNpcGOMap size:%u", questNpcGOMap.size());
	for (auto itr = questNpcGOMap.begin(); itr != questNpcGOMap.end(); itr++){
			if (itr->second->size() == 0)
				continue;
			TerrainInfo* terrain = updateTerrainInfoMap[itr->first];
			sLog.outError("map:%u ,size:%u", itr->first, itr->second->size());
			tbb::parallel_for_each(itr->second->begin(), itr->second->end(), [&](Z_QuestNpcGo *v) {
				fprintf(log_file, "%u\t%u\t%u\t%u\t%u\t%d\t%f\t%f\t%f\t%f\t%u\t%u\n", v->id, v->map, terrain->GetZoneId(v->x, v->y, v->z), terrain->GetAreaId(v->x, v->y, v->z), v->quest, v->npcgo, v->x, v->y, v->z, v->orientation, v->ntype, v->MinLevel);
				//SELECT * from z_id_zone_area INTO OUTFILE 'e:/data.txt';
				//LOAD DATA INFILE 'e:/data.txt' INTO TABLE mangos.z_id_zone_area;
				//stmt.PExecute(terrain->GetZoneId(v->x, v->y, v->z), terrain->GetAreaId(v->x, v->y, v->z), v->quest);
				//sLog.outError("UPDATE z_quest_npcgo_all_map SET zone = u%, area = u% WHERE id = u% order by map;", terrain->GetZoneId(questNpcGO->x, questNpcGO->y, questNpcGO->z), terrain->GetAreaId(questNpcGO->x, questNpcGO->y, questNpcGO->z), questNpcGO->quest);
				delete v;
			});
			delete itr->second;
		}

	
	fclose(log_file);
}
struct Z_GameObject{
	uint32 guid;
	uint32 id;                                              // entry in gamobject_template
	uint16 map;
	uint8 spawnMask;
	uint16 phaseMask;
	float x;
	float y;
	float z;
	float orientation;
	float rotation0;
	float rotation1;
	float rotation2;
	float rotation3;
	int32  spawntimesecs;
	uint32 animprogress;
	uint8 state;
	
	uint16 zone;
	uint16 area;
};
typedef tbb::concurrent_vector<Z_GameObject*> Z_GameObjectQueue;
typedef tbb::concurrent_unordered_map<uint32, Z_GameObjectQueue*> Z_GameObjectMap;
void PlayerContext::calculateParallelZone_GameObject(){

	FILE* log_file;
	log_file = fopen("e:/GameObject.txt", "w");


	//												   0   1    2      3         4       5           6        7           8          9        10        11         12       13             14         15    16   17
	QueryResult* result = WorldDatabase.Query("SELECT guid,id,map,spawnMask,phaseMask,position_x,position_y,position_z,orientation,rotation0,rotation1,rotation2,rotation3,spawntimesecs,animprogress,state,zone,area FROM gameobject order by map");

	Z_GameObjectMap gameObjectMap;
	UpdateTerrainInfoMap updateTerrainInfoMap;
	Z_GameObjectQueue* gameObjectQueue = nullptr;

	uint32 oldmap = 99999999;//标志变量，order by map，每张map改变一次

	do
	{
		Field* fields = result->Fetch();
		Z_GameObject* gameObject = new Z_GameObject();
		gameObject->guid = fields[0].GetUInt32();
		gameObject->id = fields[1].GetUInt32();
		gameObject->map = fields[2].GetUInt16();
		gameObject->spawnMask = fields[3].GetUInt8();
		gameObject->phaseMask = fields[4].GetUInt16();
		gameObject->x = fields[5].GetFloat();
		gameObject->y = fields[6].GetFloat();
		gameObject->z = fields[7].GetFloat();
		gameObject->orientation = fields[8].GetFloat();
		gameObject->rotation0 = fields[9].GetFloat();
		gameObject->rotation1 = fields[10].GetFloat();
		gameObject->rotation2 = fields[11].GetFloat();
		gameObject->rotation3 = fields[12].GetFloat();
		gameObject->spawntimesecs = fields[13].GetInt32();
		gameObject->animprogress = fields[14].GetUInt32();
		gameObject->state = fields[15].GetUInt8();
		gameObject->zone = fields[16].GetUInt16();
		gameObject->area = fields[17].GetUInt16();

		if (oldmap != gameObject->map){
			oldmap = gameObject->map;

			TerrainInfo* terrain = sTerrainMgr.LoadTerrain(gameObject->map);
			updateTerrainInfoMap[gameObject->map] = terrain;

			gameObjectQueue = new Z_GameObjectQueue();
			gameObjectMap[gameObject->map] = gameObjectQueue;

		}
		gameObjectQueue->push_back(gameObject);

	} while (result->NextRow());

	delete result;

	sLog.outError("gameObjectMap size:%u", gameObjectMap.size());
	for (auto itr = gameObjectMap.begin(); itr != gameObjectMap.end(); itr++){

		if (itr->second->size() == 0)
			continue;
		
		TerrainInfo* terrain = updateTerrainInfoMap[itr->first];
		sLog.outError("map:%u ,size:%u", itr->first, itr->second->size());
		
		tbb::parallel_for_each(itr->second->begin(), itr->second->end(), [&](Z_GameObject *v) {
			fprintf(log_file, "%u\t%u\t%u\t%u\t%u\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%d\t%u\t%u\t%u\t%u\n", v->guid, v->id, v->map, v->spawnMask, v->phaseMask, v->x, v->y, v->z, v->orientation, v->rotation0, v->rotation1, v->rotation2, v->rotation3, v->spawntimesecs, v->animprogress, v->state, terrain->GetZoneId(v->x, v->y, v->z), terrain->GetAreaId(v->x, v->y, v->z));
			delete v;
		});
		delete itr->second;
	}


	fclose(log_file);
}

struct Z_Creature{
	uint32 guid;
	uint32 id;                                              // entry in creature_template
	uint16 map;
	uint8 spawnMask;
	uint16 phaseMask;
	uint32 modelid;                                // overrides any model defined in creature_template
	int32 equipmentId;
	float x;
	float y;
	float z;
	float orientation;
	uint32 spawntimesecs;
	float spawndist;
	uint32 currentwaypoint;
	uint32 curhealth;
	uint32 curmana;
	bool  DeathState;
	uint8 movementType;
	uint16 zone;
	uint16 area;
};
typedef tbb::concurrent_vector<Z_Creature*> Z_CreatureQueue;
typedef tbb::concurrent_unordered_map<uint32, Z_CreatureQueue*> Z_CreatureMap;
void PlayerContext::calculateParallelZone_Creature(){

	FILE* log_file;
	log_file = fopen("e:/Creature.txt", "w");


	//												   0   1    2      3         4       5           6        7           8          9        10             11         12          13             14       15     16             17        18  19
	QueryResult* result = WorldDatabase.Query("SELECT guid,id,map,spawnMask,phaseMask,modelid,equipment_id,position_x,position_y,position_z,orientation,spawntimesecs,spawndist,currentwaypoint,curhealth,curmana,DeathState,MovementType,zone,area FROM creature order by map");

	Z_CreatureMap creatureMap;
	UpdateTerrainInfoMap updateTerrainInfoMap;
	Z_CreatureQueue* creaturetQueue = nullptr;

	uint32 oldmap = 99999999;//标志变量，order by map，每张map改变一次

	do
	{
		Field* fields = result->Fetch();
		Z_Creature* creature = new Z_Creature();
		creature->guid = fields[0].GetUInt32();
		creature->id = fields[1].GetUInt32();
		creature->map = fields[2].GetUInt16();
		creature->spawnMask = fields[3].GetUInt8();
		creature->phaseMask = fields[4].GetUInt16();
		creature->modelid = fields[5].GetUInt32();
		creature->equipmentId = fields[6].GetInt32();

		creature->x = fields[7].GetFloat();
		creature->y = fields[8].GetFloat();
		creature->z = fields[9].GetFloat();
		creature->orientation = fields[10].GetFloat();

		creature->spawntimesecs = fields[11].GetUInt32();
		creature->spawndist = fields[12].GetFloat();

		creature->currentwaypoint = fields[13].GetUInt32();
		creature->curhealth = fields[14].GetUInt32();
		creature->curmana = fields[15].GetUInt32();

		creature->DeathState = fields[16].GetBool();
		creature->movementType = fields[17].GetUInt8();
		creature->zone = fields[18].GetUInt16();
		creature->area = fields[19].GetUInt16();


		if (oldmap != creature->map){
			oldmap = creature->map;

			TerrainInfo* terrain = sTerrainMgr.LoadTerrain(creature->map);
			updateTerrainInfoMap[creature->map] = terrain;

			creaturetQueue = new Z_CreatureQueue();
			creatureMap[creature->map] = creaturetQueue;

		}
		creaturetQueue->push_back(creature);

	} while (result->NextRow());
	delete result;

	sLog.outError("creatureMap size:%u", creatureMap.size());
	for (auto itr = creatureMap.begin(); itr != creatureMap.end(); itr++){

		if (itr->second->size() == 0)
			continue;
		
		TerrainInfo* terrain = updateTerrainInfoMap[itr->first];
		
		sLog.outError("map:%u ,size:%u", itr->first, itr->second->size());
		tbb::parallel_for_each(itr->second->begin(), itr->second->end(), [&](Z_Creature *v) {
			fprintf(log_file, "%u\t%u\t%u\t%u\t%u\t%u\t%d\t%f\t%f\t%f\t%f\t%u\t%f\t%u\t%u\t%u\t%u\t%u\t%u\t%u\n", v->guid, v->id, v->map, v->spawnMask, v->phaseMask, v->modelid, v->equipmentId, v->x, v->y, v->z, v->orientation, v->spawntimesecs, v->spawndist, v->currentwaypoint, v->curhealth, v->curmana, v->DeathState, v->movementType, terrain->GetZoneId(v->x, v->y, v->z), terrain->GetAreaId(v->x, v->y, v->z));
			delete v;
		});
		delete itr->second;
	}


	fclose(log_file);
}
void PlayerContext::calculateZone_quest_npcgo_all_map(){

	SqlStatementID updatenpcgo;
	SqlStatement stmt = WorldDatabase.CreateStatement(updatenpcgo, "UPDATE z_quest_npcgo_all_map SET zone = ?, area = ? WHERE id = ?");

	TerrainInfo* terrain = nullptr;
	Field* fields;
	uint32 id, map;
	float x, y, z;

	uint32 oldmap = -1;										//        0   1    2          3            4
	QueryResult* result = WorldDatabase.Query("SELECT id,map,position_x,position_y,position_z FROM z_quest_npcgo_all_map where zone=0  order by map");
	do
	{
		fields = result->Fetch();
		id = fields[0].GetUInt32();
		map = fields[1].GetUInt32();
		x = fields[2].GetFloat();
		y = fields[3].GetFloat();
		z = fields[4].GetFloat();

		if (oldmap != map){
			terrain = sTerrainMgr.LoadTerrain(map);
			oldmap = map;
		}
		//if (!MapManager::IsValidMapCoord(map, x, y))
		//	continue;

		stmt.PExecute(terrain->GetZoneId(x, y, z), terrain->GetAreaId(x, y, z), id);
		

	} while (result->NextRow());

	delete result;
}
void PlayerContext::calculateZone_quest_poi_points(){

	SqlStatementID updatenpcgo;
	SqlStatement stmt = WorldDatabase.CreateStatement(updatenpcgo, "UPDATE quest_poi_points SET zone = ?, area = ? WHERE prid = ?");

	TerrainInfo* terrain = nullptr;
	uint32 oldmap = -1;
	Field* fields;
	uint32 id, map;
	float x, y, z;
			
									//        0     1    2   3
	QueryResult* result = WorldDatabase.Query("SELECT A.prid,B.mapid,A.x,A.y FROM quest_poi_points A LEFT JOIN quest_poi B ON A.questId=B.questId AND A.poiId=B.poiId   where zone=0 order by map");
	do
	{
		fields = result->Fetch();
		id = fields[0].GetUInt32();
		map = fields[1].GetUInt32();
		x = fields[2].GetFloat();
		y = fields[3].GetFloat();

		if (oldmap != map){
			terrain = sTerrainMgr.LoadTerrain(map);
			oldmap = map;
		}
	
		//if (!MapManager::IsValidMapCoord(map, x, y))
		//	continue;

		z = terrain->GetWaterOrGroundLevel(x, y, MAX_HEIGHT);

		if (z != -200000.0f)//VMAP_INVALID_HEIGHT_VALUE
			stmt.PExecute(terrain->GetZoneId(x, y, z), terrain->GetAreaId(x, y, z), id);

	} while (result->NextRow());

	delete result;
}

inline std::string * PlayerContext::getGameMapsName(uint32 map){
	return  sObjectMgr.getGameMapsName(map);
}
inline std::string * PlayerContext::getGameAreaName(uint32 area){
	return  sObjectMgr.getGameAreaName(area);
}

tbb::concurrent_unordered_map<uint32, GameMap*> & PlayerContext::getGameMaps(){
	return  sObjectMgr.getGameMaps();
}
GameMap * PlayerContext::getGameMap(uint32 map){
	return  sObjectMgr.getGameMap(map);
}
GameZone * PlayerContext::getGameZone(uint32 zone){
	return  sObjectMgr.getGameZone(zone);
}
GameArea * PlayerContext::getGameArea(uint32 area){
	return  sObjectMgr.getGameArea(area);
}

Quest const* PlayerContext::findQuest(uint32 questid){
	return sObjectMgr.GetQuestTemplate(questid);
}
Quest const* PlayerContext::findAuxQuest(){
	if (aux_questid == -1)
		return nullptr;
	return sObjectMgr.GetQuestTemplate(aux_questid);
}
//返回true结束，false可继续
bool PlayerContext::recommendQuestByQuestList(tbb::concurrent_unordered_set<QuestNpcGO*>* questlist, uint32 num){

	Quest const * quest;
	uint32 curLevel = mPlayer->getLevel();

	for (auto itr = questlist->begin();  itr != questlist->end(); ++itr)
	{
		if (recommentQuestList.size() >= num)
			return true;

		quest = sObjectMgr.GetQuestTemplate((*itr)->quest);


		if (quest->GetRequiredRaces() > 0)
		if ((quest->GetRequiredRaces()&mPlayer->getRace()) == 0)/*大多任务是种族不符，优先排除*/
			continue;

		int zoneOrSort = quest->GetZoneOrSort();
		//                 季节性              悼念日             新年                 仲夏                    复活节                感恩节              情人节     战场
		if (zoneOrSort == -22 || zoneOrSort == -41 || zoneOrSort == -366 || zoneOrSort == -369 || zoneOrSort == -374 || zoneOrSort == -375 || zoneOrSort == -376 || zoneOrSort == -25
			//草药学						钓鱼                       锻造                 炼金            制皮                 工程学                 藏宝图            竞标赛
			|| zoneOrSort == -24 || zoneOrSort == -101 || zoneOrSort == -121 || zoneOrSort == -181 || zoneOrSort == -182 || zoneOrSort == -201 || zoneOrSort == -221 || zoneOrSort == -241
			//             裁缝              烹饪                急救                暗月马戏T0装备            铭文                珠宝加工          美酒节                    特殊
			|| zoneOrSort == -246 || zoneOrSort == -304 || zoneOrSort == -324 || zoneOrSort == -364 || zoneOrSort == -371 || zoneOrSort == -373 || zoneOrSort == -370 || zoneOrSort == -284
			//		天灾入侵            安其拉战争
			|| zoneOrSort == -368 || zoneOrSort == -365)
			continue;

		//if (zoneOrSort > 0 && zoneOrSort != this->GetZoneId())
		//continue;由于zoneOrSort数据不准确，略过

		if (quest->IsDailyOrWeekly() || quest->IsRepeatable() || quest->IsMonthly())/*季节任务，日常任务和可重复任务，跳过*/
			continue;

		if (quest->GetMinLevel() > curLevel)
			continue;
		


		if (mPlayer->SatisfyQuestStatus(quest, false) && mPlayer->SatisfyQuestExclusiveGroup(quest, false) &&
			mPlayer->SatisfyQuestClass(quest, false) &&
			mPlayer->SatisfyQuestSkill(quest, false) && mPlayer->SatisfyQuestReputation(quest, false) &&
			mPlayer->SatisfyQuestPreviousQuest(quest, false) && mPlayer->SatisfyQuestTimed(quest, false) &&
			mPlayer->SatisfyQuestNextChain(quest, false) && mPlayer->SatisfyQuestPrevChain(quest, false) && quest->IsActive())
		{
			int32 npcgo = (*itr)->npcgo;
			if (npcgo == 0)/*无可用的任务开始者*/
				continue;

			if (npcgo > 0)
			{
				CreatureData* creature = sObjectMgr.findCreatureDataByEntry(npcgo);
				if (creature != nullptr)
				{
					if (creature->faction == nullptr)
						recommentQuestList.push_back(*itr);
					else if (!creature->faction->IsHostileTo(*mPlayer->getFactionTemplateEntry()))//非敌视阵营
						recommentQuestList.push_back(*itr);
				}
			}
			else if (npcgo < 0){
				GameObjectData* gameobject = sObjectMgr.findGameObjectDataByEntry(-npcgo);
				if (gameobject != nullptr)
				{
					if (gameobject->faction == nullptr)
						recommentQuestList.push_back(*itr);
					else if (!gameobject->faction->IsHostileTo(*mPlayer->getFactionTemplateEntry()))//非敌视阵营			
						recommentQuestList.push_back(*itr);
				}
			}
		}
	}

	return recommentQuestList.size() >= num;
}
tbb::concurrent_vector<QuestNpcGO*>& PlayerContext::recommendQuestZone(int32 zone, uint32 num){
	recommentQuestList.clear();

	GameZone* gameZone = sObjectMgr.getGameZone(zone);
	if (gameZone == nullptr)
		return recommentQuestList;

	for (auto itr = gameZone->arealist->begin(); itr != gameZone->arealist->end(); itr++)
	{
		tbb::concurrent_unordered_set<QuestNpcGO*>* qlist = (*itr)->questlist;
		if (recommendQuestByQuestList(qlist, num))
			return recommentQuestList;
	}

	return recommentQuestList;
}
tbb::concurrent_vector<QuestNpcGO*>& PlayerContext::recommendQuestArea(int32 area, uint32 num){

	recommentQuestList.clear();

	GameArea * gamearea = sObjectMgr.getGameArea(area);
	if (gamearea == nullptr)
		return recommentQuestList;

	recommendQuestByQuestList(gamearea->questlist, num);
	return recommentQuestList;
}




Quest const* Player::GetQuest(uint32 quest_id){
	return sObjectMgr.GetQuestTemplate(quest_id);
}
void PlayerContext::GetQuestTitleLocale(uint32 quest_id, std::string * title){
	Quest const* pQuest = sObjectMgr.GetQuestTemplate(quest_id);
	int loc_idx = mPlayer->GetSession()->GetSessionDbLocaleIndex();
	*title = pQuest->GetTitle();
	sObjectMgr.GetQuestLocaleStrings(pQuest->GetQuestId(), loc_idx, title);
}

void PlayerContext::GetCreatureOrGOTitleLocale(int32 entry, const char  ** name){
	if (entry > 0)//creature
		sObjectMgr.GetCreatureLocaleStrings(entry, mPlayer->GetSession()->GetSessionDbLocaleIndex(), name);
	else if (entry < 0)//gameobject
		sObjectMgr.GetGameObjectLocaleStrings(0 - entry, mPlayer->GetSession()->GetSessionDbLocaleIndex(), name);
	else
		*name = "";
}
CreatureInfo const* PlayerContext::findCreatureInfoByEntry(uint32 entry){ return sObjectMgr.GetCreatureTemplate(entry); }
CreatureData* PlayerContext::findCreatureDataByPOI(uint64 mapxy){ return sObjectMgr.findCreatureDataByPOI(mapxy); }
GameObjectData* PlayerContext::findGameObjectDataByPOI(uint64 mapxy){ return sObjectMgr.findGameObjectDataByPOI(mapxy); }
CreatureData* PlayerContext::findCreatureDataByEntry(uint32 entry){ return sObjectMgr.findCreatureDataByEntry(entry); }
GameObjectData* PlayerContext::findGameObjectDataByEntry(uint32 entry){ return sObjectMgr.findGameObjectDataByEntry(entry); }
CreatureData* PlayerContext::findQuestStarterCreature(uint32 quest_id){ return sObjectMgr.findQuestStarterCreature(quest_id); }
GameObjectData* PlayerContext::findQuestStarterGameObject(uint32 quest_id){ return sObjectMgr.findQuestStarterGameObject(quest_id); }

std::string & PlayerContext::getSpellName(uint32 spellid){
	return MercenaryUtil::getSpellName(spellid);
}

inline int32 PlayerContext::findQuestStarterCreatureOrGO(uint32 questid){
	return sObjectMgr.findQuestStarterCreatureOrGO(questid);
}
inline void PlayerContext::findQuestInvolvedCreatureOrGO(uint32 questid, std::vector<int32> &result){
	return sObjectMgr.findQuestInvolvedCreatureOrGO(questid, result);
}
void PlayerContext::loadQuestAux(uint32 questid){
	
	questNpcGOVec->clear();

	tbb::concurrent_vector<QuestNpcGO*> * tmp;

	tmp=sObjectMgr.GetQuestNpcGOVector(questid);//加载questNpcGOVec
	
	std::set<int32> itemEntrySet;//再准备一个item排重的set

	if (tmp)
	for (auto it = tmp->begin(); it != tmp->end(); it++)
	{
		if (itemEntrySet.find((*it)->npcgo) == itemEntrySet.end())
		{
			itemEntrySet.insert((*it)->npcgo);
			if ((*it)->ntype == 2 && (*it)->npcgo < 0)//如果是游戏对象暴物品
			{
				GameObjectInfo const* info = sObjectMgr.GetGameObjectInfo(0 - (*it)->npcgo);
				if (info == nullptr)
					continue;

				switch (info->type)
				{
					case GAMEOBJECT_TYPE_CHEST:
						if(info->chest.questId == questid) //游戏对象宝箱，必须和本任务有关。
							questNpcGOVec->push_back(*it);
						break;
					case GAMEOBJECT_TYPE_GENERIC:
						if (info->_generic.questID == questid) //游戏对象宝箱，必须和本任务有关。
							questNpcGOVec->push_back(*it);
						break;
					case GAMEOBJECT_TYPE_GOOBER:
						if (info->goober.questId == questid) //游戏对象宝箱，必须和本任务有关。
							questNpcGOVec->push_back(*it);
						break;
					default:
						questNpcGOVec->push_back(*it);
				}
			}
			else if ((*it)->ntype == 2 && (*it)->npcgo > 0){//如果是怪物暴物品，怪物不允许超过玩家等级10级。
				CreatureInfo const* info = sObjectMgr.GetCreatureTemplate((*it)->npcgo);

				if (info == nullptr)
					continue;
				
				if (info->NpcFlags  & UNIT_NPC_FLAG_VENDOR) //是商人，那么不能是敌视阵营的
				{
					switch (mPlayer->GetTeam())
					{
						case HORDE:
							if (FactionTemplateEntry const* factionTemplate = sFactionTemplateStore.LookupEntry(info->FactionHorde))
							{
								if (!factionTemplate->IsHostileTo(*mPlayer->getFactionTemplateEntry()))
									questNpcGOVec->push_back(*it);
							}
						break;
						case ALLIANCE:
							if (FactionTemplateEntry const* factionTemplate = sFactionTemplateStore.LookupEntry(info->FactionAlliance))
							{
								if (!factionTemplate->IsHostileTo(*mPlayer->getFactionTemplateEntry()))
									questNpcGOVec->push_back(*it);
							}
						break;
					}
					
				}
				else if (info->MinLevel<=mPlayer->getLevel()+10)
					questNpcGOVec->push_back(*it);
			}
			else
				questNpcGOVec->push_back(*it);
		}
	}
	
//////////////////////////////////////////////////////////////

	std::set<uint64> uniqueMapXYSet;//准备一个排重的set
	std::set<int32> uniqueEntrySet;//再准备一个排重的set

	for (auto it = questNpcGOVec->begin(); it != questNpcGOVec->end(); it++)
	{
		if (uniqueMapXYSet.find((*it)->mapxy) == uniqueMapXYSet.end())
			uniqueMapXYSet.insert((*it)->mapxy);
		if (uniqueEntrySet.find((*it)->npcgo) == uniqueEntrySet.end())
			uniqueEntrySet.insert((*it)->npcgo);
	}
	uint32 map = mPlayer->GetMapId();
	uint32 zone = mPlayer->GetZoneId();
	uint32 area = mPlayer->GetAreaId();

	std::vector<QuestPOIPoint *> temp;
	sObjectMgr.loadQuestPOIVector(&temp, questid);//加载questPOIVec

	questPOIVec->clear();

	for (auto it2 = temp.begin(); it2 != temp.end(); it2++)
	{
		if (uniqueMapXYSet.find((*it2)->mapxy) == uniqueMapXYSet.end())//对mapxy排重
		{
			uniqueMapXYSet.insert((*it2)->mapxy);
			if ((*it2)->npcgo == 0 || uniqueEntrySet.find((*it2)->npcgo) == uniqueEntrySet.end())//对npcgo排重
			{
				uniqueEntrySet.insert((*it2)->npcgo);
				questPOIVec->push_back(*it2);
			}
		}
	}
	return;
}

void PlayerContext::deletePOIFromDB(uint32 questId,QuestPOIPoint * point){	
	
	QuestPOIVector * POI = sObjectMgr.GetQuestPOIVector(questId);
	if (POI == nullptr)
		return;

	SqlStatementID delpoi;
	SqlStatement stmt = WorldDatabase.CreateStatement(delpoi, "DELETE FROM quest_poi_points WHERE prid = ?");
	stmt.PExecute(point->prid);

	ChatHandler(mPlayer).PSendSysMessage("delete quest_poi_points sucess!prid=%u", point->prid);
	sObjectMgr.LoadQuestPOI();
	loadQuestAux(questId);//重新load
}

void PlayerContext::addSelectedToPOI(uint32 questId, WorldObject * target)
{
	uint32 map = target->GetMapId();
	uint32 area = target->GetAreaId();
	uint32 zone = target->GetZoneId();
	

	float _x = target->GetPositionX();
	float _y = target->GetPositionY();
	int x = (_x > 0) ? int16(0.5 + _x) : int16(_x - 0.5);
	int y = (_y > 0) ? int16(0.5 + _y) : int16(_y - 0.5);

	QuestPOIVector * POIV = sObjectMgr.GetQuestPOIVector(questId);
	QuestPOI* poi=nullptr;

	int max_poi_id = -1;
	if (POIV != nullptr)
	for (auto it = POIV->begin(); it != POIV->end(); it++)
	{
		if (it->MapId == target->GetMapId() && it->MapAreaId == area)
			poi = &*it;

		if (int(it->PoiId) > max_poi_id)
			max_poi_id = it->PoiId;//取得当前最大的poi_id，如果map和area没有匹配的，将以最大poi_id+1
	}
	if (poi == nullptr){
		uint32 new_poi_id = max_poi_id + 1;
		SqlStatementID insertpoi;
		SqlStatement stmt = WorldDatabase.CreateStatement(insertpoi, "INSERT into quest_poi(questId,poiId,objIndex,mapId,mapAreaId,floorId,unk3,unk4) values (?,?,0,?,?,0,0,1)");
		stmt.PExecute(questId, new_poi_id , map, area);
		ChatHandler(mPlayer).PSendSysMessage("add quest_poi sucess!questid=%u,PoiId=%u", questId, new_poi_id);

		poi = new QuestPOI(new_poi_id, 0, map, area, 0, 0, 1);
		sObjectMgr.addQuestPOIVector(questId, poi);

	}
	SqlStatementID insertpoint;
	SqlStatement stmt = WorldDatabase.CreateStatement(insertpoint, "INSERT into quest_poi_points(questId,poiId,x,y,zone,area) values (?,?,?,?,?,?)");
	stmt.PExecute(questId, poi->PoiId, x, y, zone, area);



	QueryResult* result = WorldDatabase.Query("SELECT max(prid) FROM quest_poi_points");
	Field* fields = result->Fetch();
	uint32 prid = fields[0].GetUInt32(); //用于显示
	delete result;

	ChatHandler(mPlayer).PSendSysMessage("add quest_poi_points sucess!prid=%u", prid);
	sObjectMgr.LoadQuestPOI();
	loadQuestAux(questId);//重新load
}
void PlayerContext::moveFast(uint32 mapid, uint32 zone, uint32 area, CreatureData* data){
	moveFast(mapid, zone, area, data->posX, data->posY, data->posZ, data->orientation);
}
void PlayerContext::moveFast(uint32 mapid, uint32 zone, uint32 area, GameObjectData* data){
	moveFast(mapid, zone, area, data->posX, data->posY, data->posZ+2.0f, data->orientation);//游戏对象需太高避免掉下
}

void PlayerContext::moveFast(QuestNpcGO const * questNpcGO){
	if (questNpcGO->npcgo > 0)
	{
		if (CreatureData* data = findCreatureDataByPOI(questNpcGO->mapxy))
			moveFast(questNpcGO->map, questNpcGO->zone, questNpcGO->area,data);
	}
	else if (questNpcGO->npcgo < 0)
	{
		if (GameObjectData* data = findGameObjectDataByPOI(questNpcGO->mapxy))
			moveFast(questNpcGO->map, questNpcGO->zone, questNpcGO->area, data);
	}else
		ChatHandler(mPlayer).SendSysMessage(-2800678);//该点没有地理信息。
}
void PlayerContext::moveFast(QuestPOIPoint * point){

	if (point->npcgo > 0)
	{
		if (CreatureData* data = findCreatureDataByPOI(point->mapxy))
			moveFast(point->map, point->zone, point->area,data);
	}
	else if (point->npcgo < 0)
	{
		if (GameObjectData* data =findGameObjectDataByPOI(point->mapxy))
			moveFast(point->map, point->zone, point->area, data);
	}
	else
	{
		if (point->groundZ == 0){
			TerrainInfo const* map = sTerrainMgr.LoadTerrain(point->map);	
			point->groundZ = map->GetWaterOrGroundLevel(point->x, point->y, MAX_HEIGHT);
	
			if (point->groundZ == -200000.0f)//VMAP_INVALID_HEIGHT_VALUE
				point->groundZ = 0;
		}
		//ChatHandler(mPlayer).HandleGoHelper(mPlayer, point->map, point->x, point->y);
		moveFast(point->map, point->zone, point->area, point->x, point->y, point->groundZ, 0);
	}
}

void PlayerContext::changeCamera(QuestNpcGO const * questNpcGO){
	if (questNpcGO->npcgo > 0)
	{
		if (CreatureData* data = findCreatureDataByPOI(questNpcGO->mapxy))
			changeCamera(data->mapid, data->posX, data->posY, data->posZ, 0.0f-data->orientation, 15000, 45.0f);
	}
	else if (questNpcGO->npcgo < 0)
	{
		if (GameObjectData* data = findGameObjectDataByPOI(questNpcGO->mapxy))
			changeCamera(data->mapid, data->posX, data->posY, data->posZ, 0.0f-data->orientation, 15000, 45.0f);
	}
	else
		ChatHandler(mPlayer).SendSysMessage(-2800678);//该点没有地理信息。
	
}
void PlayerContext::changeCamera(QuestPOIPoint * point){
	if (point->npcgo>0)
	if (CreatureData* data = findCreatureDataByPOI(point->mapxy))
	{
		changeCamera(data->mapid, data->posX, data->posY, data->posZ - 2.0f, 0.0f-data->orientation, 15000, 45.0f);//切换镜头，镜头适当降低2.0f
		return;
	}

	if (point->npcgo<0)
	if (GameObjectData* data = findGameObjectDataByPOI(point->mapxy))
	{
		changeCamera(data->mapid, data->posX, data->posY, data->posZ - 2.0f, 0.0f-data->orientation, 15000, 45.0f);//切换镜头，镜头适当降低2.0f
		return;
	}
	if (point->groundZ == 0)
	{
		TerrainInfo const* map = sTerrainMgr.LoadTerrain(point->map);
		point->groundZ = map->GetWaterOrGroundLevel(point->x, point->y, MAX_HEIGHT);
		if (point->groundZ == -200000.0f)//VMAP_INVALID_HEIGHT_VALUE
			point->groundZ = 0;
	}

	changeCamera(point->map, point->x, point->y, point->groundZ, 0.0f, 15000, 45.0f);//切换镜头

}
tbb::concurrent_unordered_set<uint32> & PlayerContext::GetRaceSetByClass(uint32 charClass){
	return sObjectMgr.GetRaceSetByClass(charClass);
}

void PlayerContext::changeCamera(WorldObject* target, int32 duration, float radius, float orientation){

	DynamicObject* dynObj = new DynamicObject;

	// set radius to 0: spell not expected to work as persistent aura  43975 切換視角 	
	if (!dynObj->Create(target->GetMap()->GenerateLocalLowGuid(HIGHGUID_DYNAMICOBJECT), mPlayer,
		43975, EFFECT_INDEX_0, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(), duration, radius, DYNAMIC_OBJECT_FARSIGHT_FOCUS, orientation))
	{
		delete dynObj;
		return;
	}

	mPlayer->AddDynObject(dynObj);
	target->GetMap()->Add(dynObj);

	mPlayer->GetCamera().SetView(dynObj);
}
void PlayerContext::changeCamera(uint32 mapid, float x, float y, float z, float orientation, int32 duration, float radius){

	DynamicObject* dynObj = new DynamicObject;

	// set radius to 0: spell not expected to work as persistent aura  43975 切換視角 	
	if (!dynObj->Create(mPlayer->GetMap()->GenerateLocalLowGuid(HIGHGUID_DYNAMICOBJECT), mPlayer,
		43975, EFFECT_INDEX_0, x, y, z, duration, radius, DYNAMIC_OBJECT_FARSIGHT_FOCUS, orientation))
	{
		delete dynObj;
		return;
	}

	mPlayer->AddDynObject(dynObj);
	mPlayer->GetMap()->Add(dynObj);

	mPlayer->GetCamera().SetView(dynObj);
}

bool PlayerContext::checkPositiveSpell(uint32 spellid){
	return IsPositiveSpell(spellid);
}

void PlayerContext::ClearMercenary(){
	if(Pet *pet=mPlayer->GetPet())
	if (pet->isMercenary()){
		mPlayer->UnsummonPetTemporaryIfAny();
	}
}

const char* PlayerContext::getQuestType(uint32 quest){
	if (const Quest * questInfo=sObjectMgr.GetQuestTemplate(quest)){
		switch (questInfo->GetType()){
		case 1:return mPlayer->GetMangosString(-2800570);
		case 41:return mPlayer->GetMangosString(-2800571);
		case 62:return mPlayer->GetMangosString(-2800572);
		case 81:return mPlayer->GetMangosString(-2800573);
		case 82:return mPlayer->GetMangosString(-2800578);
			case 84:return mPlayer->GetMangosString(-2800574);
			case 85:return mPlayer->GetMangosString(-2800575);
			case 88:return mPlayer->GetMangosString(-2800576);
			case 89:return mPlayer->GetMangosString(-2800577);
			default:return "";
		}
	}
	return "";
}