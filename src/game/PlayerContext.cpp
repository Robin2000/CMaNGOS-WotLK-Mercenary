#include <hash_map>
#include "ObjectMgr.h"
#include "Player.h"
#include "SpellMgr.h"
#include "GridMap.h"
#include "MercenaryMgr.h"


PlayerContext::PlayerContext(Player* player) :mPlayer(player), gamePointMgr(player){

}
PlayerContext::~PlayerContext() {}

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

void PlayerContext::calculateZoneArea(){

	TerrainInfo* terrain = nullptr;
	uint32 oldmap = -1;
											//        0   1    2          3            4
	QueryResult* result = WorldDatabase.Query("SELECT id,map,position_x,position_y,position_z FROM z_quest_npcgo_stater order by map");
	do
	{
		Field* fields = result->Fetch();
		uint32 id = fields[0].GetUInt32();
		uint32 map = fields[1].GetUInt32();
		float x = fields[2].GetFloat();
		float y = fields[3].GetFloat();
		float z = fields[4].GetFloat();
		if (oldmap != map)
		{
			terrain = sTerrainMgr.LoadTerrain(map);
			oldmap = map;
		}
		uint32 zone = terrain->GetZoneId(x, y, z);
		uint32 area = terrain->GetAreaId(x, y, z);
		WorldDatabase.PExecute("UPDATE z_quest_npcgo_stater SET zone = %u, area = %u WHERE id = %u", zone, area, id);

	} while (result->NextRow());

	delete result;
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
bool PlayerContext::recommendQuestByQuestList(tbb::concurrent_vector<uint32>* questlist, uint32 num){

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
		tbb::concurrent_vector<uint32>* qlist = (*itr)->questlist;
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

inline std::string & PlayerContext::getSpellName(uint32 idx){
	return sObjectMgr.getSpellName(idx);
}

inline int32 PlayerContext::findQuestStarterCreatureOrGO(uint32 questid){
	return sObjectMgr.findQuestStarterCreatureOrGO(questid);
}
inline void PlayerContext::findQuestInvolvedCreatureOrGO(uint32 questid, std::vector<int32> &result){
	return sObjectMgr.findQuestInvolvedCreatureOrGO(questid, result);
}
tbb::concurrent_vector<WorldLocation> PlayerContext::getQuestPOI(uint32 questid){

	questPOIVec.clear();

	QuestPOIVector const* POI = sObjectMgr.GetQuestPOIVector(questid);
	if (POI == nullptr)
		return questPOIVec;
	int count = 0;
	for (QuestPOIVector::const_iterator itr = POI->begin(); count<10 && itr != POI->end(); ++itr)
	{
		for (tbb::concurrent_vector<QuestPOIPoint>::const_iterator itr2 = itr->points.begin(); count<10 && itr2 != itr->points.end(); ++itr2, count++)
		{
			WorldLocation loc;
			loc.mapid = itr->MapId;
			loc.coord_x = itr2->x;
			loc.coord_y = itr2->y;
			questPOIVec.push_back(loc);
		}

	}

	return questPOIVec;
}