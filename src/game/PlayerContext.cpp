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


void DelayedAction::Update(uint32 update_diff){

	if (timeout)
		return;

	timelimit -= update_diff;
	DETAIL_LOG("time: %d", timelimit);
	if (timelimit < 0)
	{
		timeout = true;
		run();
	}
}
PlayerContext::PlayerContext(Player* player) :mPlayer(player), gamePointMgr(player), delayActionQueue(0), eventPlugin(player){

}
PlayerContext::~PlayerContext(){
	DelayedAction *action;
	while (delayActionQueue.pop(action))
	{
		delete action;
	}
}
void PlayerContext::Update(uint32 update_diff, uint32 time){
	
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

											//        0   1    2          3            4       5     6     7     8
	QueryResult* result = WorldDatabase.Query("SELECT id,map,position_x,position_y,position_z,quest,npcgo,ntype,MinLevel FROM z_quest_npcgo_all_map order by map");
	
	QuestNpcGOMap questNpcGOMap;
	UpdateTerrainInfoMap updateTerrainInfoMap;
	QuestNpcGOQueue* questNpcGOQueue =nullptr;

	uint32 oldmap = -1;//标志变量，order by map，每张map改变一次
	
	do
	{
		Field* fields = result->Fetch();
		Z_QuestNpcGo* questNpcGO = new Z_QuestNpcGo();
		questNpcGO->id = fields[0].GetUInt32();//注意，这里使用quest字段保存id，是为了方便，实际使用时应看作id
		questNpcGO->map = fields[1].GetUInt32();
		questNpcGO->x = fields[2].GetFloat();
		questNpcGO->y = fields[3].GetFloat();
		questNpcGO->z = fields[4].GetFloat();
		questNpcGO->quest = fields[5].GetUInt32();
		questNpcGO->npcgo = fields[6].GetInt32();
		questNpcGO->ntype = fields[7].GetUInt8();
		questNpcGO->MinLevel = fields[8].GetInt16();
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
	
	sLog.outError("map0:size:%u", questNpcGOMap[0]->size());

	sLog.outError("questNpcGOMap size:%u", questNpcGOMap.size());
	for (QuestNpcGOMap::iterator itr = questNpcGOMap.begin(); itr != questNpcGOMap.end(); itr++){
			if (itr->second->size() == 0)
				continue;
			TerrainInfo* terrain = updateTerrainInfoMap[itr->first];
			sLog.outError("map:%u ,size:%u", itr->first, itr->second->size());
			tbb::parallel_for_each(itr->second->begin(), itr->second->end(), [&](Z_QuestNpcGo *v) {
				fprintf(log_file, "%u\t%u\t%u\t%u\t%u\t%d\t%f\t%f\t%f\t%u\t%u\n", v->id, v->map, terrain->GetZoneId(v->x, v->y, v->z), terrain->GetAreaId(v->x, v->y, v->z), v->quest, v->npcgo, v->x, v->y, v->z, v->ntype, v->MinLevel);
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
bool PlayerContext::recommendQuestByQuestList(tbb::concurrent_unordered_set<uint32>* questlist, uint32 num){

	Quest const * quest;
	uint32 curLevel = mPlayer->getLevel();

	for (auto itr = questlist->begin();  itr != questlist->end(); ++itr)
	{
		if (recommentQuestList.size() >= num)
			return true;

		quest = sObjectMgr.GetQuestTemplate(*itr);

		if (quest->GetRequiredRaces() == 0)/*大多任务是种族不符，优先排除*/
			continue;
		if ((quest->GetRequiredRaces()&mPlayer->getRace()) == 0)
			continue;

		int zoneOrSort = quest->GetZoneOrSort();
		//                 季节性              悼念日             新年                 仲夏                    复活节                感恩节              情人节     战场
		if (zoneOrSort == -22 || zoneOrSort == -41 || zoneOrSort == -366 || zoneOrSort == -369 || zoneOrSort == -374 || zoneOrSort == -375 || zoneOrSort == -376 || zoneOrSort == -25
			//草药学						钓鱼                       锻造                 炼金            制皮                 工程学                 藏宝图            竞标赛
			|| zoneOrSort == -24 || zoneOrSort == -101 || zoneOrSort == -121 || zoneOrSort == -181 || zoneOrSort == -182 || zoneOrSort == -201 || zoneOrSort == -221 || zoneOrSort == -241
			//             裁缝              烹饪                急救                    暗月马戏            铭文                珠宝加工          美酒节                    特殊
			|| zoneOrSort == -246 || zoneOrSort == -304 || zoneOrSort == -324 || zoneOrSort == -364 || zoneOrSort == -371 || zoneOrSort == -373 || zoneOrSort == -370 || zoneOrSort == -284
			//		天灾入侵            安其拉战争
			|| zoneOrSort == -368 || zoneOrSort == -365)
			continue;

		//if (zoneOrSort > 0 && zoneOrSort != this->GetZoneId())
		//continue;

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
			int32 npcgo = sObjectMgr.GetQuestStarterNpcGOId(quest->GetQuestId());
			if (npcgo == 0)/*无可用的任务开始者*/
				continue;

			if (npcgo > 0)
			{
				CreatureData* creature = sObjectMgr.findCreatureDataByEntry(npcgo);
				if (creature != nullptr)
				{
					if (creature->faction == nullptr)
						recommentQuestList.push_back(quest);
					else if (!creature->faction->IsHostileTo(*mPlayer->getFactionTemplateEntry()))//非敌视阵营
						recommentQuestList.push_back(quest);
				}
			}
			else if (npcgo < 0){
				GameObjectData* gameobject = sObjectMgr.findGameObjectDataByEntry(-npcgo);
				if (gameobject != nullptr)
				{
					if (gameobject->faction == nullptr)
						recommentQuestList.push_back(quest);
					else if (!gameobject->faction->IsHostileTo(*mPlayer->getFactionTemplateEntry()))//非敌视阵营			
						recommentQuestList.push_back(quest);
				}
			}
		}
	}

	return recommentQuestList.size() >= num;
}
tbb::concurrent_vector<Quest const*>& PlayerContext::recommendQuestZone(int32 zone, uint32 num){
	recommentQuestList.clear();

	GameZone* gameZone = sObjectMgr.getGameZone(zone);
	if (gameZone == nullptr)
		return recommentQuestList;

	for (auto itr = gameZone->arealist->begin(); itr != gameZone->arealist->end(); itr++)
	{
		tbb::concurrent_unordered_set<uint32>* qlist = (*itr)->questlist;
		if (recommendQuestByQuestList(qlist, num))
			return recommentQuestList;
	}

	return recommentQuestList;
}
tbb::concurrent_vector<Quest const*>& PlayerContext::recommendQuestArea(int32 area, uint32 num){

	recommentQuestList.clear();

	GameArea * gamearea = sObjectMgr.getGameArea(area);
	if (gamearea == nullptr)
		return recommentQuestList;

	recommendQuestByQuestList(gamearea->questlist, num);
	return recommentQuestList;
}


tbb::concurrent_vector<QuestNpcGO> * PlayerContext::GetQuestNpcGOVector(uint32 questid){
	return sObjectMgr.GetQuestNpcGOVector(questid);
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
		*name = "error";
}

CreatureData* PlayerContext::findCreatureDataByPOI(uint32 mapid, float x, float y){ return sObjectMgr.findCreatureDataByPOI(mapid, x, y); }
GameObjectData* PlayerContext::findGameObjectDataByPOI(uint32 mapid, float x, float y){ return sObjectMgr.findGameObjectDataByPOI(mapid, x, y); }
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
QuestPOIVector const*  PlayerContext::loadQuestPOI(uint32 questid){
	questPOIVec = sObjectMgr.GetQuestPOIVector(questid);
	return questPOIVec;
}

tbb::concurrent_unordered_set<uint32> & PlayerContext::GetRaceSetByClass(uint32 charClass){
	return sObjectMgr.GetRaceSetByClass(charClass);
}

