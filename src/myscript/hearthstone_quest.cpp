#include "precompiled.h"
#include "hearthstone_quest.h"

static uint32	FIRST_PAGE = 1;

std::string getQuestType(Player* pPlayer,uint32 type){
	switch (type){
		case 1:return pPlayer->GetMangosString(-2800570);
		case 41:return pPlayer->GetMangosString(-2800571);
		case 62:return pPlayer->GetMangosString(-2800572);
		case 81:return pPlayer->GetMangosString(-2800573);
		case 82:return pPlayer->GetMangosString(-2800578);
		case 84:return pPlayer->GetMangosString(-2800574);
		case 85:return pPlayer->GetMangosString(-2800575);
		case 88:return pPlayer->GetMangosString(-2800576);
		case 89:return pPlayer->GetMangosString(-2800577);
		default:return "";
	}
}

tbb::concurrent_vector<GameZone*> * getZonelist(Player* pPlayer){
	uint32 mapid = (pPlayer->context.MAPSEL == -1) ? pPlayer->GetMapId() : pPlayer->context.MAPSEL;
	GameMap * gameMap=pPlayer->context.getGameMap(mapid);
	if (gameMap == nullptr)
		return nullptr;
	return gameMap->zonelist;
}
tbb::concurrent_vector<GameArea*> * getArealist(Player* pPlayer){
	uint32 zoneid = (pPlayer->context.ZONESEL == -1) ? pPlayer->GetZoneId() : pPlayer->context.ZONESEL;
	GameZone * gameZone=pPlayer->context.getGameZone(zoneid);
	if (gameZone == nullptr)
		return nullptr;
	return gameZone->arealist;
}
std::string &getMapName(Player* pPlayer){
	uint32 mapid = (pPlayer->context.MAPSEL == -1) ? pPlayer->GetMapId() : pPlayer->context.MAPSEL;
	GameMap *gameMap=pPlayer->context.getGameMap(mapid);
	if (gameMap == nullptr)
		return std::string("");
	return gameMap->name;
}
std::string & getZoneName(Player* pPlayer){
	uint32 zoneid = (pPlayer->context.ZONESEL == -1) ? pPlayer->GetZoneId() : pPlayer->context.ZONESEL;
	GameZone * gameZone = pPlayer->context.getGameZone(zoneid);
	if (gameZone == nullptr)
		return std::string("");
	return gameZone->name;
}
std::string & getAreaName(Player* pPlayer){
	uint32 areaid = (pPlayer->context.AREASEL == -1) ? pPlayer->GetAreaId() : pPlayer->context.AREASEL;
	GameArea * gameArea=pPlayer->context.getGameArea(areaid);
	if (gameArea == nullptr)
		return std::string("");
	return gameArea->name;
}
bool showMapMenu(Player* pPlayer, Item* pItem, uint32 curPage){
	
	pPlayer->context.MAPSELPAGE=curPage;
	tbb::concurrent_unordered_map<uint32, GameMap*>& maps = pPlayer->context.getGameMaps();

	uint32 pageStart = (curPage - 1) * 17;
	uint32 MAX_INDEX = pageStart + 17;
	uint32 countAll = maps.size();

	for (uint32 i = pageStart; i<MAX_INDEX&&i<countAll; i++)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, maps.at(i)->name, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100 + i);//选择map 100-299

	if (curPage>1)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 649);//上一页

	if (countAll> MAX_INDEX)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800594, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 651);//下一页

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}

bool showZoneMenu(Player* pPlayer, Item* pItem, uint32 curPage){

	pPlayer->context.ZONESELPAGE=curPage;
	tbb::concurrent_vector<GameZone*> * zonelist = getZonelist(pPlayer);
	if (zonelist == nullptr)
		return false;
	std::string& mapName = getMapName(pPlayer);
	
	uint32 pageStart = (curPage-1) * 17;
	uint32 MAX_INDEX = pageStart + 17;
	uint32 countAll = zonelist->size();

	for (uint32 i = pageStart; i<MAX_INDEX&&i<countAll; i++)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, mapName + "-" + zonelist->at(i)->name, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 300 + i);//选择zone 300-499

	if (curPage>1)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 699);//上一页

	if (countAll> MAX_INDEX)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 701);//下一页

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}

bool showAreaMenu(Player* pPlayer, Item* pItem, uint32 curPage){

	pPlayer->context.AREASELPAGE=curPage;
	tbb::concurrent_vector<GameArea*> * arealist = getArealist(pPlayer);
	if (arealist == nullptr)
		return;

	std::string& zoneName = getZoneName(pPlayer);
	
	uint32 pageStart = (curPage - 1) * 16;
	uint32 MAX_INDEX = pageStart + 16;
	uint32 countAll = arealist->size();

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, zoneName + "-" + pPlayer->GetMangosString(-2801000), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2000);//选择area 500-599

	for (uint32 i = pageStart; i<MAX_INDEX&&i<countAll; i++)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, zoneName + "-" + arealist->at(i)->name, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 500 + i);//选择area 500-599

	if (curPage>1)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 750);//上一页

	if (countAll> MAX_INDEX)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 751);//下一页

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}
typedef tbb::concurrent_vector<Quest const*> RecommentQuestList;
bool hearthstone_quest_click(Player* pPlayer, Item* pItem, uint32 uiAction){
	
	switch (uiAction)
	{
		case GOSSIP_ACTION_INFO_DEF + 649:/*map上一页*/
			return	showMapMenu(pPlayer, pItem, pPlayer->context.MAPSELPAGE - 1);
		case GOSSIP_ACTION_INFO_DEF + 650:/*map菜单*/
			return showMapMenu(pPlayer, pItem, FIRST_PAGE);
		case GOSSIP_ACTION_INFO_DEF + 651:/*map下一页*/
			return	showMapMenu(pPlayer, pItem, pPlayer->context.MAPSELPAGE + 1);

		case GOSSIP_ACTION_INFO_DEF + 698:/*选择更远地区*/
			pPlayer->context.MAPSEL = pPlayer->GetMapId();
			pPlayer->context.ZONESEL = pPlayer->GetZoneId();
			pPlayer->context.AREASEL = pPlayer->GetAreaId();
			return showZoneMenu(pPlayer, pItem, FIRST_PAGE);
		case GOSSIP_ACTION_INFO_DEF + 699:/*zone上一页*/
			return showZoneMenu(pPlayer, pItem, pPlayer->context.ZONESELPAGE - 1);
		case GOSSIP_ACTION_INFO_DEF + 700:/*zone菜单*/
			return showZoneMenu(pPlayer, pItem, FIRST_PAGE);
		case GOSSIP_ACTION_INFO_DEF + 701:/*zone下一页*/
			return showZoneMenu(pPlayer, pItem, pPlayer->context.ZONESELPAGE + 1);
		case GOSSIP_ACTION_INFO_DEF + 600:/*选择当前地区*/
			pPlayer->context.MAPSEL = pPlayer->GetMapId();
			pPlayer->context.ZONESEL = pPlayer->GetZoneId();
			pPlayer->context.AREASEL = pPlayer->GetAreaId();
			hearthstone_prepare_quest_area(pPlayer, pItem, pPlayer->context.AREASEL);
			return true;
		case GOSSIP_ACTION_INFO_DEF + 800:/*生成地图区域数据【开发工具】*/
			pPlayer->context.calculateZoneArea();
			return true;
		case GOSSIP_ACTION_INFO_DEF + 748:	/*选择附近区域*/
			pPlayer->context.MAPSEL = pPlayer->GetMapId();
			pPlayer->context.ZONESEL = pPlayer->GetZoneId();
			pPlayer->context.AREASEL = pPlayer->GetAreaId();
			return showAreaMenu(pPlayer, pItem, FIRST_PAGE);
		case GOSSIP_ACTION_INFO_DEF + 749:/*area上一页*/
			return showAreaMenu(pPlayer, pItem, pPlayer->context.AREASELPAGE - 1);
		case GOSSIP_ACTION_INFO_DEF + 750:/*area菜单*/
			return showAreaMenu(pPlayer, pItem, FIRST_PAGE);
		case GOSSIP_ACTION_INFO_DEF + 751:/*area下一页*/
			return showAreaMenu(pPlayer, pItem, pPlayer->context.AREASELPAGE+ 1);
		case GOSSIP_ACTION_INFO_DEF + 2000:
				hearthstone_prepare_quest_zone(pPlayer, pItem, pPlayer->context.ZONESEL);
			return true;
	}

	if (uiAction >= GOSSIP_ACTION_INFO_DEF + 900 && uiAction <= GOSSIP_ACTION_INFO_DEF + 920)/*任务推荐*/
	{
		int recommendResultIndex = uiAction - GOSSIP_ACTION_INFO_DEF - 900;
		tbb::concurrent_vector<Quest const*>& v = pPlayer->context.recommentQuestList;
		if (v.size() > recommendResultIndex)
		{
			uint32 questid = v.at(recommendResultIndex)->GetQuestId();
			pPlayer->context.aux_questid = questid;
			hearthstone_quest(pPlayer, pItem, questid);
		}
	}
	else if (uiAction >=GOSSIP_ACTION_INFO_DEF + 100 && uiAction <= GOSSIP_ACTION_INFO_DEF + 299)/*map选择*/
	{
		tbb::concurrent_unordered_map<uint32, GameMap*>& maps = pPlayer->context.getGameMaps();
		uint32 count = uiAction - GOSSIP_ACTION_INFO_DEF - 100;
		pPlayer->context.MAPSEL=maps.at(count)->id;
		showZoneMenu(pPlayer, pItem, FIRST_PAGE);
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 300 && uiAction <= GOSSIP_ACTION_INFO_DEF + 499)/*zone选择*/
	{
		tbb::concurrent_vector<GameZone*> * zonelist = getZonelist(pPlayer);
		if (zonelist == nullptr)
			return false;
		uint32 count = uiAction - GOSSIP_ACTION_INFO_DEF - 300;
		pPlayer->context.ZONESEL=zonelist->at(count)->id;
		showAreaMenu(pPlayer, pItem, FIRST_PAGE);
	}
	
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 500 && uiAction < GOSSIP_ACTION_INFO_DEF + 649)/*area选择*/
	{
		tbb::concurrent_vector<GameArea*> * arealist = getArealist(pPlayer);
		if (arealist == nullptr)
			return false;

		uint32 count = uiAction - GOSSIP_ACTION_INFO_DEF - 500;
		if (arealist->size() <= count)
			return false;
		pPlayer->context.AREASEL=arealist->at(count)->id;
		hearthstone_prepare_quest_area(pPlayer, pItem, arealist->at(count)->id);

	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 970 && uiAction <= GOSSIP_ACTION_INFO_DEF + 990){
		tbb::concurrent_vector<QuestNpcGO> const* npcgoV = pPlayer->context.GetQuestNpcGOVector(pPlayer->context.aux_questid);
		if (npcgoV != nullptr)
			transportByCreatureOrGO2(pPlayer, pItem, npcgoV->at(uiAction - GOSSIP_ACTION_INFO_DEF - 970).npcgo);
	}
	/*else if (uiAction == GOSSIP_ACTION_INFO_DEF + 970)
		transportToStarter(pPlayer, pItem);
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 971 && uiAction <= GOSSIP_ACTION_INFO_DEF + 975)
		transportToInvolved(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 971);
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 980 && uiAction < GOSSIP_ACTION_INFO_DEF + 985)
		transportByCreatureOrGO(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 980);
	*/
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 985 && uiAction <= GOSSIP_ACTION_INFO_DEF + 995)//兴趣点传送
		transportByPOI(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 985);
	else if (uiAction > GOSSIP_ACTION_INFO_DEF + 1000)
	{
		hearthstone_quest(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 1000);//任务辅助条目点击
		return true;
	}

	return true;
}

	void transportByPOI(Player* pPlayer, Item* pItem, int idx){
		if (!pPlayer->context.gamePointMgr.checkPoint(1))
			return;
		tbb::concurrent_vector<WorldLocation> &POI = pPlayer->context.getQuestPOI();
		WorldLocation const loc = POI.at(idx);

		CreatureData* creature = pPlayer->context.findCreatureDataByPOI(loc.mapid, loc.coord_x, loc.coord_y);
		if (creature != nullptr)
		{
			pPlayer->TeleportTo(creature->mapid, creature->posX, creature->posY, creature->posZ, 0);
			pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_POI, 1);
			return;
		}
		GameObjectData* gameobject = pPlayer->context.findGameObjectDataByPOI(loc.mapid, loc.coord_x, loc.coord_y);
		if (gameobject != nullptr)
		{
			pPlayer->TeleportTo(gameobject->mapid, gameobject->posX, gameobject->posY, gameobject->posZ, 0);
			pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_POI, 1);
			return;
		}
		
		ChatHandler(pPlayer).HandleGoHelper(pPlayer, loc.mapid, loc.coord_x, loc.coord_y);
		pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_POI, 1);
	}
	void transportToInvolved(Player* pPlayer, Item* pItem, int idx){
		std::vector<int32> result;
		pPlayer->context.findQuestInvolvedCreatureOrGO(pPlayer->context.aux_questid, result);
		transportByCreatureOrGO2(pPlayer, pItem, result.at(idx));
	}
	void transportToStarter(Player* pPlayer, Item* pItem){
		int32 creatureOrGO = pPlayer->context.findQuestStarterCreatureOrGO(pPlayer->context.aux_questid);
		transportByCreatureOrGO2(pPlayer, pItem, creatureOrGO);
	}
	void transportByCreatureOrGO2(Player* pPlayer, Item* pItem, int32 creatureOrGOEntry){
		if (!pPlayer->context.gamePointMgr.checkPoint(1))
			return;

		if (creatureOrGOEntry>0)
		{
			CreatureData* data = pPlayer->context.findCreatureDataByEntry(creatureOrGOEntry);
			if (data == nullptr)
			{
				ChatHandler(pPlayer).SendSysMessage(-2800185);//系统提示：本次操作失败。
				return;
			}
			pPlayer->TeleportTo(data->mapid, data->posX, data->posY, data->posZ, 0);
		}
		else if (creatureOrGOEntry < 0)
		{
			uint32 gameobject = 0 - creatureOrGOEntry;
			GameObjectData* data = pPlayer->context.findGameObjectDataByEntry(gameobject);
			if (data == nullptr)
			{
				ChatHandler(pPlayer).SendSysMessage(-2800185);//系统提示：本次操作失败。
				return;
			}
			pPlayer->TeleportTo(data->mapid, data->posX, data->posY, data->posZ, 0);
		}
		pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_TARGET, 1);
	}
	void transportByCreatureOrGO(Player* pPlayer, Item* pItem, int idx){

		Quest const* quest = pPlayer->context.findAuxQuest();
		int32 creature = quest->ReqCreatureOrGOId[idx];
		transportByCreatureOrGO2(pPlayer, pItem, creature);
	}
	/*任务辅助或推荐的点击条目*/
	void hearthstone_quest(Player* pPlayer, Item* pItem, uint32 questid)
	{
		pPlayer->PrepareGossipMenu(pPlayer, 65535);
		//Quest const * quest = pPlayer->GetQuest(questid);
		pPlayer->context.aux_questid = questid;

		//下面添加QuestNpcGO
		tbb::concurrent_vector<QuestNpcGO> * npcgoV =pPlayer->context.GetQuestNpcGOVector(questid);
		if (npcgoV != nullptr)
		{
			int i = 0;
			for (auto itr = npcgoV->begin(); i<19&&itr != npcgoV->end(); ++itr,i++){
				const char * name = "";
				pPlayer->context.GetCreatureOrGOTitleLocale(itr->npcgo, &name);
				
				switch (itr->ntype)
				{
				 case 0:
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800587) + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 970 + i);//【任务开始】
					 break;
				 case 1:
					 if (i<18)
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800588) + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 970 + i);//【任务目标】
					 break;
				 case 2:
					 if (i<18)
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800589) + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 970 + i);//【任务物品】
					 break;
				 case 3:
					 pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800590) + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 970 + i);//【任务结束】
					 break;
				}
				
			}
		}else
		{
				//下面添加兴趣点
				int count = 0;
				tbb::concurrent_vector<WorldLocation> &POI = pPlayer->context.getQuestPOI(questid);
				for (tbb::concurrent_vector<WorldLocation>::const_iterator itr = POI.begin(); count<10&&itr != POI.end(); ++itr, count++)
				{
					CreatureData* creature = pPlayer->context.findCreatureDataByPOI(itr->mapid, itr->coord_x, itr->coord_y);
					if (creature != nullptr)
					{
						const char * name = "";
						pPlayer->context.GetCreatureOrGOTitleLocale(creature->id, &name);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "P=" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 985 + count);//
						continue;
					}
					GameObjectData* gameobject = pPlayer->context.findGameObjectDataByPOI(itr->mapid, itr->coord_x, itr->coord_y);
					if (gameobject != nullptr)
					{
						const char * name = "";
						pPlayer->context.GetCreatureOrGOTitleLocale(0 - gameobject->id, &name);
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "P=" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 985 + count);//
						continue;
					}

					
					GameMap *gameMap = pPlayer->context.getGameMap(uint32(itr->mapid));
					if (gameMap != nullptr)
					{
						std::ostringstream os;
						os << gameMap->name << "(" << int32(itr->coord_x) << "," << int32(itr->coord_y) << ")";
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 985 + count);//
					}
				}
		}
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "GenQuestMapZoneArea", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 800);//产生地图和区域数据
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	}


	/*任务辅助主菜单*/
	void hearthstone_prepare_quest_aux(Player* pPlayer, Item* pItem){

		pPlayer->PrepareGossipMenu(pPlayer, 65535);

		QuestStatusMap readMap;
		QuestStatusMap& map = pPlayer->getQuestStatusMap();

		uint32 count = 0;
		for (QuestStatusMap::const_iterator it = map.begin(); it != map.end() && count <= 20; it++)
		{
			if (it->second.m_status == QUEST_STATUS_INCOMPLETE || (it->second.m_status == QUEST_STATUS_COMPLETE && !it->second.m_rewarded))
			{
				readMap[it->first] = it->second;
				count++;
			}
		}
		if (readMap.size() == 0)
		{
			ChatHandler(pPlayer).SendSysMessage(-2800593);//系统提示：没有进行中的任务。
			return;
		}
		for (QuestStatusMap::const_iterator it = readMap.begin(); it != readMap.end(); it++)
		{
			std::string  title = "";
			pPlayer->context.GetQuestTitleLocale(it->first, &title);
			std::ostringstream os;
			os << it->first << "." << title;
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1000 + it->first);//数据库中最大为26034，所以该项最大为46034，在uint32范围内
		}


		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	}

	typedef tbb::concurrent_vector<Quest const*> QuestList;
	void hearthstone_prepare_quest_list(Player* pPlayer, Item* pItem, QuestList& recommendResult){
		pPlayer->PrepareGossipMenu(pPlayer, 65535);
		std::string areaname = getAreaName(pPlayer);
		if (recommendResult.size() == 0)
		{
			std::string msg = pPlayer->GetMangosString(-2800592) + areaname;
			ChatHandler(pPlayer).SendSysMessage(msg.c_str());//系统提示：此区域已经没有可以推荐的任务。
		}
		for (int i = 0; i < recommendResult.size(); i++)
		{
			std::string  title = "";
			pPlayer->context.GetQuestTitleLocale(recommendResult.at(i)->GetQuestId(), &title);
			std::ostringstream os;
			os << areaname << "." << getQuestType(pPlayer, recommendResult.at(i)->GetType()) << title;
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 900 + i);//数据库中最大为26034，所以该项最大为46034，在uint32范围内
		}

		if (pPlayer->context.ZONESEL != -1 && pPlayer->GetZoneId() != pPlayer->context.ZONESEL)
		{
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800597, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 600);//选择当前地区
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800594, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 748);//选择附近区域
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800595, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 698);//选择更远地区
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800596, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 650);//全地图选择
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。

	}
	/*任务推荐主菜单*/
	void hearthstone_prepare_quest_area(Player* pPlayer, Item* pItem, int area){
		if (area == -1)
			area = pPlayer->GetAreaId();

		QuestList& recommendResult = pPlayer->context.recommendQuestArea(area, 15);
		hearthstone_prepare_quest_list(pPlayer, pItem, recommendResult);
	}
	//推荐zone下所有区域
	void hearthstone_prepare_quest_zone(Player* pPlayer, Item* pItem, int zone){
		if (zone == -1)
			zone=pPlayer->GetZoneId();
		
		QuestList& recommendResult = pPlayer->context.recommendQuestZone(zone, 15);
		hearthstone_prepare_quest_list(pPlayer, pItem, recommendResult);
	}