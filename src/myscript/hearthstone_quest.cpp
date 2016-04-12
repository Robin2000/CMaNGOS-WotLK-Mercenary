#include "precompiled.h"
#include "hearthstone_quest.h"

static uint32	FIRST_PAGE = 1;
enum gossip_quest_action{
	MAP_SEL_ACTION = 1,
	ZONE_SEL_ACTION =2,
	AREA_SEL_ACTION = 3,
	QUEST_SEL_ACTION = 4,
	NPCGO_SEL_ACTION = 5,
	NPCGO_SEL_CAMERA_NPCGO = 6,
	NPCGO_SEL_CAMERA_POI = 7
};

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
std::string *getMapName(Player* pPlayer){
	uint32 mapid = (pPlayer->context.MAPSEL == -1) ? pPlayer->GetMapId() : pPlayer->context.MAPSEL;
	return pPlayer->context.getGameMapsName(mapid);
}
std::string * getZoneName(Player* pPlayer){
	uint32 zoneid = (pPlayer->context.ZONESEL == -1) ? pPlayer->GetZoneId() : pPlayer->context.ZONESEL;
	return pPlayer->context.getGameAreaName(zoneid);
}
std::string * getAreaName(Player* pPlayer){
	uint32 areaid = (pPlayer->context.AREASEL == -1) ? pPlayer->GetAreaId() : pPlayer->context.AREASEL;
	return pPlayer->context.getGameAreaName(areaid);;
}
bool showMapMenu(Player* pPlayer, Item* pItem, uint32 curPage){
	pPlayer->context.gossipActionType = MAP_SEL_ACTION;
	pPlayer->context.MAPSELPAGE=curPage;
	tbb::concurrent_unordered_map<uint32, GameMap*>& maps = pPlayer->context.getGameMaps();

	uint32 pageStart = (curPage - 1) * 17;
	uint32 MAX_INDEX = pageStart + 17;
	uint32 i = 0;
	for (auto it = maps.begin(); i<MAX_INDEX&&it!=maps.end(); it++,i++)
	{
		if (i<pageStart)
			continue;
		std::string * name = pPlayer->context.getGameMapsName(it->first);
		if (name!=nullptr)
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, *name, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+it->first);//ѡ��map 100-299
	}

	if (curPage>1)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN,  649);//��һҳ

	if (maps.size()> MAX_INDEX)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN,  651);//��һҳ

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	return true;
}

bool showZoneMenu(Player* pPlayer, Item* pItem, uint32 curPage){

	pPlayer->context.ZONESELPAGE=curPage;
	tbb::concurrent_vector<GameZone*> * zonelist = getZonelist(pPlayer);
	if (zonelist == nullptr)
		return false;

	std::string* mapName = getMapName(pPlayer);
	
	uint32 pageStart = (curPage-1) * 17;
	uint32 MAX_INDEX = pageStart + 17;
	uint32 countAll = zonelist->size();

	pPlayer->context.gossipActionType = ZONE_SEL_ACTION;

	for (uint32 i = pageStart; i<MAX_INDEX&&i<countAll; i++)
	{
		GameArea * gameArea = pPlayer->context.getGameArea(zonelist->at(i)->id);
		std::string* areaName = pPlayer->context.getGameAreaName(zonelist->at(i)->id);
		if (mapName != nullptr&&areaName != nullptr)
		{
			std::ostringstream os;
			os << *mapName << "-" << *areaName << "(Lvl:" << gameArea->areaLevel << ")";
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1,os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + zonelist->at(i)->id);
		}
	}
	if (curPage>1)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN,  699);//��һҳ

	if (countAll> MAX_INDEX)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN,  701);//��һҳ

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	return true;
}

bool showAreaMenu(Player* pPlayer, Item* pItem, uint32 curPage){

	pPlayer->context.AREASELPAGE=curPage;
	tbb::concurrent_vector<GameArea*> * arealist = getArealist(pPlayer);
	if (arealist == nullptr)
		return false;

	pPlayer->context.gossipActionType=AREA_SEL_ACTION;

	std::string* zoneName = getZoneName(pPlayer);
	
	uint32 pageStart = (curPage - 1) * 16;
	uint32 MAX_INDEX = pageStart + 16;
	uint32 countAll = arealist->size();

	if (zoneName!=nullptr)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, *zoneName + "-" + pPlayer->GetMangosString(-2801000), GOSSIP_SENDER_MAIN, 2000);//ѡ��ȫ������

	for (uint32 i = pageStart; i<MAX_INDEX&&i<countAll; i++)
	{
		GameArea * gameArea=pPlayer->context.getGameArea(arealist->at(i)->id);
		std::string* areaName = pPlayer->context.getGameAreaName(arealist->at(i)->id);
		if (zoneName != nullptr&&areaName != nullptr)
		{
			std::ostringstream os;
			os << *zoneName << "-" << *areaName << "(Lvl:" << gameArea->areaLevel << ")";
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + arealist->at(i)->id);
		}
	}
	if (curPage>1)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN,  750);//��һҳ

	if (countAll> MAX_INDEX)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN,  751);//��һҳ

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	return true;
}
typedef tbb::concurrent_vector<QuestNpcGO*> RecommentQuestList;
bool hearthstone_quest_click(Player* pPlayer, Item* pItem, uint32 uiAction){	

	switch (uiAction)
	{
		case  649:/*map��һҳ*/
			return	showMapMenu(pPlayer, pItem, pPlayer->context.MAPSELPAGE - 1);
		case  650:/*map�˵�*/
			return showMapMenu(pPlayer, pItem, FIRST_PAGE);
		case  651:/*map��һҳ*/
			return	showMapMenu(pPlayer, pItem, pPlayer->context.MAPSELPAGE + 1);

		case  698:/*ѡ���Զ����*/
			pPlayer->context.MAPSEL = pPlayer->GetMapId();
			pPlayer->context.ZONESEL = pPlayer->GetZoneId();
			pPlayer->context.AREASEL = pPlayer->GetAreaId();
			return showZoneMenu(pPlayer, pItem, FIRST_PAGE);
		case  699:/*zone��һҳ*/
			return showZoneMenu(pPlayer, pItem, pPlayer->context.ZONESELPAGE - 1);
		case  700:/*zone�˵�*/
			return showZoneMenu(pPlayer, pItem, FIRST_PAGE);
		case  701:/*zone��һҳ*/
			return showZoneMenu(pPlayer, pItem, pPlayer->context.ZONESELPAGE + 1);
		case  600:/*ѡ��ǰ����*/
			pPlayer->context.MAPSEL = pPlayer->GetMapId();
			pPlayer->context.ZONESEL = pPlayer->GetZoneId();
			pPlayer->context.AREASEL = pPlayer->GetAreaId();
			hearthstone_prepare_quest_area(pPlayer, pItem, pPlayer->context.AREASEL);
			return true;
		case  748:	/*ѡ�񸽽�����*/
			pPlayer->context.MAPSEL = pPlayer->GetMapId();
			pPlayer->context.ZONESEL = pPlayer->GetZoneId();
			pPlayer->context.AREASEL = pPlayer->GetAreaId();
			return showAreaMenu(pPlayer, pItem, FIRST_PAGE);
		case  749:/*area��һҳ*/
			return showAreaMenu(pPlayer, pItem, pPlayer->context.AREASELPAGE - 1);
		case  750:/*area�˵�*/
			return showAreaMenu(pPlayer, pItem, FIRST_PAGE);
		case  751:/*area��һҳ*/
			return showAreaMenu(pPlayer, pItem, pPlayer->context.AREASELPAGE+ 1);
		case  2000:
			hearthstone_prepare_quest_zone(pPlayer, pItem, pPlayer->context.ZONESEL);/*�Ƽ�zone���������������*/
			return true;
	}
	switch (pPlayer->context.gossipActionType)/*ʹ��gossipActionType�����̬id�����⣬��̬id��Ҫ+GOSSIP_ACTION_INFO_DEF��������Ĺ̶�id���̶�id��С��GOSSIP_ACTION_INFO_DEF*/
	{
	case  MAP_SEL_ACTION:
		pPlayer->context.MAPSEL = uiAction - GOSSIP_ACTION_INFO_DEF;
		showZoneMenu(pPlayer, pItem, FIRST_PAGE);
		return true;
	case  ZONE_SEL_ACTION:
		pPlayer->context.ZONESEL = uiAction - GOSSIP_ACTION_INFO_DEF;
		showAreaMenu(pPlayer, pItem, FIRST_PAGE);
		return true;
	case  AREA_SEL_ACTION:
		pPlayer->context.AREASEL = uiAction - GOSSIP_ACTION_INFO_DEF;
		hearthstone_prepare_quest_area(pPlayer, pItem, pPlayer->context.AREASEL);
		return true;
	case  QUEST_SEL_ACTION:
		pPlayer->context.aux_questid = uiAction - GOSSIP_ACTION_INFO_DEF;
		hearthstone_quest(pPlayer, pItem, pPlayer->context.aux_questid);
		return true;
	case NPCGO_SEL_CAMERA_NPCGO://npcgo��ͷ+����ִ��
	{
		tbb::concurrent_vector<QuestNpcGO *> const* npcgoV = pPlayer->context.GetQuestNpcGOVector();
		if (npcgoV == nullptr)
			return true;
		QuestNpcGO const * questNpcGO = npcgoV->at(pPlayer->context.aux_npcgo_idx);
		switch (uiAction)
		{
			case GOSSIP_ACTION_INFO_DEF + 1111://��ʾĿ�곡�������رղ˵����ɷ����ۿ�
				pPlayer->context.changeCamera(questNpcGO);//�л���ͷ
				pPlayer->context.gossipActionType = NPCGO_SEL_ACTION;//������ʾ�˵�
				hearthstone_quest_click(pPlayer, pItem, GOSSIP_ACTION_INFO_DEF + 960 + pPlayer->context.aux_npcgo_idx);//������ʾ�˵�
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1112://�����ҵ���Ŀ��(-3��ԭ��)
				if (!pPlayer->context.gamePointMgr.checkPoint(3))
					return false;
				pPlayer->context.moveFast(questNpcGO);
				

				pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_AUX, 3);
				pPlayer->CLOSE_GOSSIP_MENU();/*�������رվɲ˵�*/
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1113://�����ҵ���Ŀ��(-1��ԭ��)
				if (!pPlayer->context.gamePointMgr.checkPoint(1))
					return false;

				pPlayer->context.moveFast(questNpcGO);

				pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_AUX, 1);
				pPlayer->CLOSE_GOSSIP_MENU();/*�������رվɲ˵�*/
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1114://��һҳ
				hearthstone_quest(pPlayer, pItem, pPlayer->context.aux_questid);
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1210:
			case GOSSIP_ACTION_INFO_DEF + 1211:
				pPlayer->context.gossipActionType = NPCGO_SEL_ACTION;
				pPlayer->context.disableFindPath = !pPlayer->context.disableFindPath;
				hearthstone_quest_click(pPlayer, pItem, pPlayer->context.gossipActionOld);
				return true;
			default:
				return true;
		}
	}case NPCGO_SEL_CAMERA_POI:  //��Ȥ�㾵ͷ+����ִ��
	{
		tbb::concurrent_vector<QuestPOIPoint *> * POI = pPlayer->context.GetQuestPOI();
		if (POI->size() <= pPlayer->context.aux_poi_idx)
			return false;

		QuestPOIPoint * point = POI->at(pPlayer->context.aux_poi_idx);
		
		switch (uiAction)
		{
			case GOSSIP_ACTION_INFO_DEF + 1111://��ʾĿ�곡�������رղ˵����ɷ����ۿ�
				pPlayer->context.changeCamera(point);

				pPlayer->context.gossipActionType = NPCGO_SEL_ACTION;//������ʾ�˵�
				hearthstone_quest_click(pPlayer, pItem, GOSSIP_ACTION_INFO_DEF + 980 + pPlayer->context.aux_poi_idx);//������ʾ�˵�
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1112://�����ҵ���Ŀ��(-3��ԭ��)
				if (!pPlayer->context.gamePointMgr.checkPoint(3))
					return false;

					pPlayer->context.moveFast(point);//z�߶�����10.0f���������

				pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_AUX, 3);
				pPlayer->CLOSE_GOSSIP_MENU();/*�������رվɲ˵�*/
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1113://�����ҵ���Ŀ��(-1��ԭ��)
				if (!pPlayer->context.gamePointMgr.checkPoint(1))
					return false;

					pPlayer->context.moveFast(point); //z�߶�����10.0f���������

				pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_QUEST_AUX, 1);
				pPlayer->CLOSE_GOSSIP_MENU();/*�������رվɲ˵�*/
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1114://��һҳ
				hearthstone_quest(pPlayer, pItem, pPlayer->context.aux_questid);
				return true;
			case GOSSIP_ACTION_INFO_DEF + 1118://GM delete this POI
				pPlayer->context.deletePOIFromDB(pPlayer->context.aux_questid,point);
				break;
			default:
				return true;
		}
	}
	case NPCGO_SEL_ACTION:
		if (uiAction == GOSSIP_ACTION_INFO_DEF + 1116)//add selected target to POI
		{
			ObjectGuid guid = pPlayer->GetSelectionGuid();
			if (!guid)
			{
				ChatHandler(pPlayer).SendSysMessage(200);
				return false;
			}

			WorldObject * target = pPlayer->GetMap()->GetWorldObject(guid);
			if (target == nullptr)
			{
				ChatHandler(pPlayer).SendSysMessage(200);
				return false;
			}

			pPlayer->context.addSelectedToPOI(pPlayer->context.aux_questid, target);
			return true;
		}
		else if (uiAction == GOSSIP_ACTION_INFO_DEF + 1117)//add me to POI
		{
			pPlayer->context.addSelectedToPOI(pPlayer->context.aux_questid, pPlayer);
			return true;
		}
		else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 960 && uiAction <= GOSSIP_ACTION_INFO_DEF + 979){//NPCGO��ͷ+���Ͳ˵�
			tbb::concurrent_vector<QuestNpcGO *> *  npcgoV = pPlayer->context.GetQuestNpcGOVector();
			if (npcgoV != nullptr)
			{
				pPlayer->context.aux_npcgo_idx=uiAction - GOSSIP_ACTION_INFO_DEF - 960;
				QuestNpcGO const * questNpcGO = npcgoV->at(pPlayer->context.aux_npcgo_idx);
				if (questNpcGO->zone == 0 && questNpcGO->area == 0)
				{
					ChatHandler(pPlayer).SendSysMessage(-2800185);//�õ�û�е�����Ϣ
					return false;
				}
				
				const char * name = "";
				pPlayer->context.GetCreatureOrGOTitleLocale(questNpcGO->npcgo, &name);

				pPlayer->context.gossipActionType = NPCGO_SEL_CAMERA_NPCGO;

				if (questNpcGO->map == pPlayer->GetMapId())
				{
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT,pPlayer->GetMangosString(-2800679) + std::string(name) ,GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1111);//��ʾĿ�곡����
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI,pPlayer->GetMangosString(-2800680) + std::string(name) ,GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1112);//�����ҵ���Ŀ��(-3��ԭ��)��
				}
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetMangosString(-2800681) +std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1113);//�����ҵ���Ŀ��(-1��ԭ��)��
				
				pPlayer->context.gossipActionOld = uiAction;//�����Ҫ�ظ�ʹ����ͬ�˵���ʹ�ô�uiAction

				if (pPlayer->context.disableFindPath)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800687, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1210);//����Ѱ·ģʽ
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800686, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1211);//�ر�Ѱ·ģʽ

			}
		}
		else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 980 && uiAction <= GOSSIP_ACTION_INFO_DEF + 999)//��Ȥ�㾵ͷ+���Ͳ˵�
		{
			pPlayer->context.aux_poi_idx = uiAction - GOSSIP_ACTION_INFO_DEF - 980;
			tbb::concurrent_vector<QuestPOIPoint *> * POI = pPlayer->context.GetQuestPOI();

			if (POI->size() > pPlayer->context.aux_poi_idx)
			{
				QuestPOIPoint * point = POI->at(pPlayer->context.aux_poi_idx);

				pPlayer->context.gossipActionType = NPCGO_SEL_CAMERA_POI;

				std::ostringstream os;
				getPOIName(pPlayer, point,point->map, point->zone, point->area, point->x, point->y, os);
				if (point->map == pPlayer->GetMapId())
				{
					std::string name = os.str();
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, pPlayer->GetMangosString(-2800679) + name, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1111);//��ʾĿ�곡����
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetMangosString(-2800680) + name, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1112);//�����ҵ���Ŀ��(-3��ԭ��)��
				}
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, pPlayer->GetMangosString(-2800681) + os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1113);//�����ҵ���Ŀ��(-1��ԭ��)��
				
				pPlayer->context.gossipActionOld = uiAction;//�����Ҫ�ظ�ʹ����ͬ�˵���ʹ�ô�uiAction

				if (pPlayer->context.disableFindPath)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800687, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1210);//����Ѱ·ģʽ
				else
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800686, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1211);//�ر�Ѱ·ģʽ

				if (pPlayer->isGameMaster())
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "delete this POI", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1118);

			}
			//transportByPOI(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 980);
		}

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1114);//��һҳ
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
		return true;
	}

	return true;
}

	/*���������Ƽ��ĵ����Ŀ*/
	void hearthstone_quest(Player* pPlayer, Item* pItem, uint32 questid)
	{
		pPlayer->context.gossipActionType = NPCGO_SEL_ACTION;

		pPlayer->PrepareGossipMenu(pPlayer, 65535);
		//Quest const * quest = pPlayer->GetQuest(questid);
		pPlayer->context.aux_questid = questid;

		//��ʼ��npcgo����Ȥ��
		pPlayer->context.loadQuestAux(questid);

		//�������QuestNpcGO
		tbb::concurrent_vector<QuestNpcGO *> * npcgoV = pPlayer->context.GetQuestNpcGOVector();

		int npcgocount = 0;
		int itemcount = 0;

		int i = 0;
		
		if (npcgoV != nullptr)
		for (auto itr = npcgoV->begin(); i<19&&itr != npcgoV->end(); ++itr,i++){

				const char * name = "";
				pPlayer->context.GetCreatureOrGOTitleLocale((*itr)->npcgo, &name);
				
				std::string* area = pPlayer->context.getGameAreaName((*itr)->area);
				
				switch ((*itr)->ntype)
				{
					 case 0://����ʼ
						 pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800587) + ((area==nullptr)?"":*area) +"-"+ std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 960 + i);//������ʼ��
						 npcgocount++;
						 break;
					 case 1://����Ŀ��
						 if (npcgocount<18)
							 pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800588) +((area == nullptr) ? "" : *area) + "-"+std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 960 + i);//������Ŀ�꡿
						 npcgocount++;
						 break;
					 case 2://������Ʒȡ����Դ
						 if (npcgocount < 18 && itemcount++ < 6)/*�������5��item��Դ��̫��Ҳû������*/
						 {
							 pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800589) + ((area == nullptr) ? "" : *area) + "-" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 960 + i);//��������Ʒ��
							 npcgocount++;
						 }
						 
						 break;
					 case 3://�������
						 pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, pPlayer->GetMangosString(-2800590) + ((area == nullptr) ? "" : *area) + "-" + std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 960 + i);//�����������
						 npcgocount++;
						 break;
				}
				
			}
	
			if (npcgocount <18)
			{
				//���������Ȥ��
				tbb::concurrent_vector<QuestPOIPoint *> * POI = pPlayer->context.GetQuestPOI();
				if (POI)
				{
					int size = POI->size();
					if (size > 0)
					{
						int maxPOI = 19 - npcgocount;
						for (int i = 0; i < size&&i < maxPOI; i++)
						{
							QuestPOIPoint * point = POI->at(i);
							std::ostringstream os;
							getPOIName(pPlayer, point, uint32(point->map), uint32(point->zone), uint32(point->area), int32(point->x), int32(point->y), os);

							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 980 + i);

						}
					}
				}
			}

			if (pPlayer->isGameMaster())
			{
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "add selected target to POI", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1116);
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "add me to POI", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1117);
			}


			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
			pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	}

	void getPOIName(Player * pPlayer, QuestPOIPoint * point , uint32 map, uint32 zone, uint32 area, int32 x, int32 y, std::ostringstream& os)
	{
		std::string *mapName = pPlayer->context.getGameMapsName(map);
		if (mapName != nullptr)
			os << *mapName;

		std::string *zoneName = pPlayer->context.getGameAreaName(zone);
		if (zoneName != nullptr)
			os << "-" << *zoneName;

		std::string *areaName = pPlayer->context.getGameAreaName(area);
		if (areaName != nullptr&&areaName != zoneName)
			os << *areaName;

		const char * name = "";
		if (point->npcgo!=0)
			pPlayer->context.GetCreatureOrGOTitleLocale(point->npcgo, &name);
		
		if (name == "")
			os << "[" << x << "," << y << "]";
		else
			os << "-" << name;

	}
	/*���������˵�*/
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
			ChatHandler(pPlayer).SendSysMessage(-2800593);//ϵͳ��ʾ��û�н����е�����
			return;
		}
		pPlayer->context.gossipActionType = QUEST_SEL_ACTION;
		for (QuestStatusMap::const_iterator it = readMap.begin(); it != readMap.end(); it++)
		{
			Quest const* quest=pPlayer->context.findQuest(it->first);
			if (quest == nullptr)
				continue;
			std::string  title = "";
			pPlayer->context.GetQuestTitleLocale(it->first, &title);
			std::ostringstream os;
			if (pPlayer->isGameMaster())
				os << it->first<<"." << pPlayer->context.getQuestType(it->first) << title;
			else
				os << pPlayer->context.getQuestType(it->first) << title;
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+it->first);//���ݿ������Ϊ26034�����Ը������Ϊ46034����uint32��Χ��
		}

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	}

	typedef tbb::concurrent_vector<QuestNpcGO*> QuestList;
	void hearthstone_prepare_quest_list(std::string* areaOrZone,Player * pPlayer, Item* pItem, QuestList& recommendResult){//��ʾ�Ƽ������б�
		pPlayer->PrepareGossipMenu(pPlayer, 65535);

		pPlayer->context.gossipActionType = QUEST_SEL_ACTION;
		for (int i = 0; i < recommendResult.size(); i++)
		{
			std::string  title = "";
			pPlayer->context.GetQuestTitleLocale(recommendResult.at(i)->quest, &title);
			std::ostringstream os;
			os << ((areaOrZone == nullptr) ? "" : *areaOrZone) << ":" << pPlayer->context.getQuestType(recommendResult.at(i)->quest) << title;
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, os.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + recommendResult.at(i)->quest);//���ݿ������Ϊ26034�����Ը������Ϊ46034����uint32��Χ��
		}

		if (pPlayer->context.ZONESEL != -1 && pPlayer->GetZoneId() != pPlayer->context.ZONESEL)
		{
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800597, GOSSIP_SENDER_MAIN,  600);//ѡ��ǰ����
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800594, GOSSIP_SENDER_MAIN,  748);//ѡ�񸽽�����
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800595, GOSSIP_SENDER_MAIN,  698);//ѡ���Զ����
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800596, GOSSIP_SENDER_MAIN,  650);//ȫ��ͼѡ��
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ

	}
	/*�����Ƽ����˵�*/
	void hearthstone_prepare_quest_area(Player* pPlayer, Item* pItem, int area){
		if (area == -1)
			area = pPlayer->GetAreaId();

		std::string* areaname = pPlayer->context.getGameAreaName(area);
		QuestList& recommendResult = pPlayer->context.recommendQuestArea(area, 15);
		if (recommendResult.size() == 0)
		{
			
			std::string msg = pPlayer->GetMangosString(-2800592) + ((areaname == nullptr) ? "" : *areaname);
			ChatHandler(pPlayer).SendSysMessage(msg.c_str());//ϵͳ��ʾ��������û�п����Ƽ�������
			//return;��Ȼ��Ҫ��ʾ�б�
		}

		hearthstone_prepare_quest_list(areaname,pPlayer, pItem, recommendResult);
	}
	//�Ƽ�zone����������
	void hearthstone_prepare_quest_zone(Player* pPlayer, Item* pItem, int zone){
		if (zone == -1)
			zone=pPlayer->GetZoneId();
		
		std::string* zonename = pPlayer->context.getGameAreaName(zone);
		QuestList& recommendResult = pPlayer->context.recommendQuestZone(zone, 15);
		if (recommendResult.size() == 0)
		{
			
			std::string msg = pPlayer->GetMangosString(-2800592) + ((zonename == nullptr) ? "" : *zonename);
			ChatHandler(pPlayer).SendSysMessage(msg.c_str());//ϵͳ��ʾ��������û�п����Ƽ�������
			//return;��Ȼ��Ҫ��ʾ�б�
		}
		hearthstone_prepare_quest_list(zonename,pPlayer, pItem, recommendResult);
	}