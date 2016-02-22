#include "precompiled.h"
#include "hearthstone_quest.h"

bool hearthstone_quest_click(Player* pPlayer, Item* pItem, uint32 uiAction){
	
	if (uiAction > GOSSIP_ACTION_INFO_DEF + 1000)
	{
		hearthstone_quest(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 1000);
		return true;
	}
	if (uiAction == GOSSIP_ACTION_INFO_DEF + 970)
		transportToStarter(pPlayer, pItem);
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 971 && uiAction <= GOSSIP_ACTION_INFO_DEF + 975)
		transportToInvolved(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 971);
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 980 && uiAction < GOSSIP_ACTION_INFO_DEF + 985)
		transportByCreatureOrGO(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 980);
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 985 && uiAction < GOSSIP_ACTION_INFO_DEF + 990)
		transportByPOI(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 985);

	pPlayer->gossipMenuType = -1;/*还原默认值*/
	return true;
}

	void transportByPOI(Player* pPlayer, Item* pItem, int idx){
		if (!pPlayer->gamePointMgr.checkPoint(1))
			return;
		tbb::concurrent_vector<WorldLocation> &POI = pPlayer->getQuestPOI();
		WorldLocation const loc = POI.at(idx);
		ChatHandler(pPlayer).HandleGoHelper(pPlayer, loc.mapid, loc.coord_x, loc.coord_y);
		pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_POI, 1);
	}
	void transportToInvolved(Player* pPlayer, Item* pItem, int idx){
		std::vector<int32> result;
		const Quest * quest = pPlayer->hearthstoneQuest;
		pPlayer->findQuestInvolvedCreatureOrGO(quest->GetQuestId(), result);
		transportByCreatureOrGO2(pPlayer, pItem, result.at(idx));
	}
	void transportToStarter(Player* pPlayer, Item* pItem){
		const Quest * quest = pPlayer->hearthstoneQuest;
		int32 creatureOrGO = pPlayer->findQuestStarterCreatureOrGO(quest->GetQuestId());
		transportByCreatureOrGO2(pPlayer, pItem, creatureOrGO);
	}
	void transportByCreatureOrGO2(Player* pPlayer, Item* pItem, int32 creatureOrGOEntry){
		if (!pPlayer->gamePointMgr.checkPoint(1))
			return;

		if (creatureOrGOEntry>0)
		{
			CreatureData& data = pPlayer->findCreatureDataByEntry(creatureOrGOEntry);
			pPlayer->TeleportTo(data.mapid, data.posX, data.posY, data.posZ, 0);
		}
		else if (creatureOrGOEntry < 0)
		{
			uint32 gameobject = 0 - creatureOrGOEntry;
			GameObjectData& data = pPlayer->findGameObjectDataByEntry(gameobject);
			pPlayer->TeleportTo(data.mapid, data.posX, data.posY, data.posZ, 0);
		}
		pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_TARGET, 1);
	}
	void transportByCreatureOrGO(Player* pPlayer, Item* pItem, int idx){

		Quest const * quest = pPlayer->hearthstoneQuest;
		int32 creature = quest->ReqCreatureOrGOId[idx];
		transportByCreatureOrGO2(pPlayer, pItem, creature);
	}
	void hearthstone_quest(Player* pPlayer, Item* pItem, uint32 questid)
	{
		pPlayer->PrepareGossipMenu(pPlayer, 65535);
		Quest const * quest = pPlayer->GetQuest(questid);
		pPlayer->hearthstoneQuest = quest;

		//下面添加任务开始：最多1个
		int32 creatureOrGO = pPlayer->findQuestStarterCreatureOrGO(questid);
		if (creatureOrGO != 0)
		{
			const char * name = "";
			pPlayer->GetCreatureOrGOTitleLocale(creatureOrGO, &name);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "S=" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 970);//970作为任务的开始
		}
		//下面添加任务关联：最多4个
		std::vector<int32> result;
		pPlayer->findQuestInvolvedCreatureOrGO(questid, result);
		int count = 0;
		for (std::vector<int32>::iterator iter = result.begin(); count<5 && iter != result.end(); iter++, count++)
		{
			const char * name = "";
			pPlayer->GetCreatureOrGOTitleLocale(*iter, &name);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "R=" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 971 + count);//971--975作为任务的相关
		}

		//下面添加要杀死的npc或者游戏对象
		for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
		{
			if (quest->ReqCreatureOrGOId[i] != 0)
			{
				CreatureData& data = pPlayer->findCreatureDataByEntry(quest->ReqCreatureOrGOId[i]);
				if (data.id == 0)/*生物不在地图上，可能不是这个阶段的phrase*/
					continue;
				const char * name = "";
				pPlayer->GetCreatureOrGOTitleLocale(quest->ReqCreatureOrGOId[i], &name);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, "O=" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 980 + i);//0-3 最大为983
			}
		}
		//下面添加兴趣点

		count = 0;
		tbb::concurrent_vector<WorldLocation> &POI = pPlayer->getQuestPOI(questid);
		for (tbb::concurrent_vector<WorldLocation>::const_iterator itr = POI.begin(); itr != POI.end(); ++itr, count++)
		{
			char * title = new char[250];
			sprintf(title, "%s(%d,%d)", pPlayer->getGameMaps(uint32(itr->mapid)).c_str(), int32(itr->coord_x), int32(itr->coord_y)); // 坐标(x,y)：%f,%f
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 985 + count);//返回主菜单

		}

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	}

	/*动态生成任务列表*/
	void hearthstone_prepare_quest(Player* pPlayer, Item* pItem){

		pPlayer->PrepareGossipMenu(pPlayer, 65535);

		QuestStatusMap readMap;
		QuestStatusMap& map = pPlayer->getQuestStatusMap();

		uint8 count = 0;
		for (QuestStatusMap::const_iterator it = map.begin(); it != map.end() && count <= 20; it++, count++)
		{
			if (it->second.m_status == QUEST_STATUS_INCOMPLETE || (it->second.m_status == QUEST_STATUS_COMPLETE && !it->second.m_rewarded))
				readMap[it->first] = it->second;
		}

		for (QuestStatusMap::const_iterator it = readMap.begin(); it != readMap.end(); it++)
		{
			std::string  title = "";
			pPlayer->GetQuestTitleLocale(it->first, &title);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1000 + it->first);//数据库中最大为26034，所以该项最大为46034，在uint32范围内
		}


		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	}
