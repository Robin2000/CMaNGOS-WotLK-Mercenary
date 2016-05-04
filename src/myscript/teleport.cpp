#include "precompiled.h"
#include "teleport.h"
enum actionTransport{
	ACTION_SEL_TOP = 0,
	ACTION_SEL_MAP = 1,
	ACTION_SEL_INSTANCE = 2
};

bool hearthstone_prepare_transport(Player* pPlayer, Item* pItem){
	
	pPlayer->context.gossipActionType = ACTION_SEL_TOP;

	pPlayer->PrepareGossipMenu(pPlayer, 65535);
	//tbb::concurrent_unordered_map<uint32, GameTransport*> & transportMap=pPlayer->context.getGameTransportMaps();
	//tbb::concurrent_unordered_map<uint32, GameMap*> & gameMap = pPlayer->context.getGameMaps();
	//tbb::concurrent_unordered_map<uint32, GameMap*> & gameMap = pPlayer->context.getGameMaps();
	
	uint32 key[] = { 6000, 6001, 6002, 6003 };//               卡里姆多，东部王国，外域，诺森德
	uint32 map[] = { 1, 0, 530, 571 };		  //               卡里姆多，东部王国，外域，诺森德

	for (uint16 i = 0; i < 4; i++)
	{
		std::string* title = pPlayer->context.getGameMapsName(key[i]);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, *title, 0, GOSSIP_ACTION_INFO_DEF + map[i]);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, *title + pPlayer->GetMangosString(-2800701), 0, map[i]); // 副本
	}

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(65535, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}
bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction){

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 998)
	{
		hearthstone_prepare_transport(pPlayer, pItem);
		return true;
	}
	pPlayer->PrepareGossipMenu(pPlayer, 65535);

	switch (pPlayer->context.gossipActionType)
	{
		case ACTION_SEL_TOP:
		{
		
			tbb::concurrent_unordered_map<uint32, GameTransport*> & transportMap = pPlayer->context.getGameTransportMaps();

			if (uiAction >= GOSSIP_ACTION_INFO_DEF)
			{
				pPlayer->context.gossipActionType = ACTION_SEL_MAP;//地图选择
				uint32 map = uiAction-GOSSIP_ACTION_INFO_DEF;
				tbb::concurrent_vector<GameMap*>*  maplist = transportMap.find(map)->second->maplist;
				for (auto it = maplist->begin(); it != maplist->end(); it++)
				{
					if ((*it)->level > pPlayer->getLevel())
						continue;

					if ((*it)->faction >0)
					{
						if (pPlayer->context.IsHostileTo((*it)->faction))
						continue;
					}

					std::ostringstream os;
					os << *(pPlayer->context.getGameMapsName((*it)->zone));
					if ((*it)->level > 0)
						os << "(" << (*it)->level << pPlayer->GetMangosString(-2800703) << ")";

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, os.str(), 0, (*it)->zone);
				}
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 998);//上一页
			}
			else
			{
				pPlayer->context.gossipActionType = ACTION_SEL_INSTANCE;//副本选择
				uint32 map = uiAction;
				tbb::concurrent_vector<GameInstance*>*  instancelist = transportMap.find(map)->second->instancelist;
				for (auto it = instancelist->begin(); it != instancelist->end(); it++)
				{
					if ((*it)->minlevel > pPlayer->getLevel())
						continue;
					
					std::ostringstream os;
					os << (*it)->name << "(" << (*it)->minlevel << "-" << (*it)->maxlevel << pPlayer->GetMangosString(-2800703) << "," << (*it)->maxplayer << pPlayer->GetMangosString(-2800702) ;
					switch ((*it)->instancetype)
					{
					case 1:
						os << pPlayer->GetMangosString(-2800704);
						break;
					case 2:
						os << pPlayer->GetMangosString(-2800705);
						break;
					case 3:
						os << pPlayer->GetMangosString(-2800706);
						break;
					case 4:
						os << pPlayer->GetMangosString(-2800707);
						break;
					}
					os << ")";
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, os.str(), 0, (*it)->area);
				}
				
				pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 998);//上一页
			}
			break;
		}
		case ACTION_SEL_MAP:
		{
			tbb::concurrent_unordered_map<uint32, GameMap*> & gameMap = pPlayer->context.getGameMaps();
			auto it = gameMap.find(uiAction);
			hearthstone_transport(pPlayer, it->second->map, it->second->x, it->second->y, it->second->z, it->second->o);
			break;
		}
		case ACTION_SEL_INSTANCE:
		{
			tbb::concurrent_unordered_map<uint32, GameInstance*> & instanceMap = pPlayer->context.getGameInstanceMap();
			auto it = instanceMap.find(uiAction);
			hearthstone_transport(pPlayer, it->second->map, it->second->x, it->second->y, it->second->z, it->second->o);
			break;
		}
	}

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(65535, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}

bool hearthstone_transport(Player* pPlayer, uint32 mapid, double x, double y, double z, double o)
{
	if (!pPlayer->context.gamePointMgr.checkPoint(1))
		return false;
	
	pPlayer->TeleportTo(mapid, x, y, z, o);
	pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_TELE, 1);
	return true;
}