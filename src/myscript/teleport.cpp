#include "precompiled.h"
#include "teleport.h"
enum actionTransport{
	ACTION_SEL_TOP = 0,
	ACTION_SEL_MAP = 1,
	ACTION_SEL_INSTANCE = 2,
	NUM_PERPAGE = 15
};

bool hearthstone_prepare_transport(Player* pPlayer, Item* pItem){
	
	pPlayer->context.gossipActionType = ACTION_SEL_TOP;
	pPlayer->context.MAPSELPAGE = 0;

	pPlayer->PrepareGossipMenu(pPlayer, 65535);
	//tbb::concurrent_unordered_map<uint32, GameTransport*> & transportMap=pPlayer->context.getGameTransportMaps();
	//tbb::concurrent_unordered_map<uint32, GameMap*> & gameMap = pPlayer->context.getGameMaps();
	//tbb::concurrent_unordered_map<uint32, GameMap*> & gameMap = pPlayer->context.getGameMaps();

	for (auto it = pPlayer->context.getGameTransportMaps().begin(); it != pPlayer->context.getGameTransportMaps().end(); it++)
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, pPlayer->context.GetContinent(it->first,false), 0, GOSSIP_ACTION_INFO_DEF + it->first);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, pPlayer->context.GetContinent(it->first,true), 0, it->first); // 副本
	}

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(65535, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}

bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction){

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 995)//下一页
	{
		pPlayer->context.gossipActionType = ACTION_SEL_TOP;
		pPlayer->context.MAPSELPAGE += 1;
		hearthstone_transport_case(pPlayer, pItem, pPlayer->context.gossipActionOld);
		return true;
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 996)//上一页
	{
		pPlayer->context.gossipActionType = ACTION_SEL_TOP;
		pPlayer->context.MAPSELPAGE -= 1;
		hearthstone_transport_case(pPlayer, pItem, pPlayer->context.gossipActionOld);
		return true;
	}


	switch (pPlayer->context.gossipActionType)
	{
		case ACTION_SEL_TOP:
		{
			pPlayer->context.gossipActionOld = uiAction;
			if (uiAction >= GOSSIP_ACTION_INFO_DEF)
			{
				pPlayer->context.gossipActionType = ACTION_SEL_MAP;//地图选择

				tbb::concurrent_vector<GameMap*>* maps=pPlayer->context.getGameTransportMaps().find(uiAction - GOSSIP_ACTION_INFO_DEF)->second->maplist;
				
				uint32 start = pPlayer->context.MAPSELPAGE * NUM_PERPAGE;
				uint32 i = 0;
				for (auto it = maps->begin(); it != maps->end(); it++)
				{
					if ((*it)->level > pPlayer->getLevel())
						continue;

					if ((*it)->faction >0)
					{
						if (pPlayer->GetTeam() !=  (*it)->faction)
							continue;
					}
					

					i++;
					if (i < start)
						continue;
					if (i >= start + NUM_PERPAGE)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 995);//下一页
						break;
					}

					std::ostringstream os;
					os << *(pPlayer->context.getGameMapsName((*it)->zone));
					if ((*it)->level > 0)
						os << "(" << (*it)->level << pPlayer->GetMangosString(-2800703) << ")";

					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, os.str(), 0, (*it)->zone);

				}
				if (i == 0)
				{
					ChatHandler(pPlayer).SendSysMessage(-2800708);//没有适合等级的传送点。
					return true;
				}
				if (pPlayer->context.MAPSELPAGE>0)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 996);//上一页

			}
			else
			{
				pPlayer->context.gossipActionType = ACTION_SEL_INSTANCE;//副本选择
				tbb::concurrent_vector<GameInstance*>* instances = pPlayer->context.getGameTransportMaps().find(uiAction - GOSSIP_ACTION_INFO_DEF)->second->instancelist;
				
				uint32 start = pPlayer->context.MAPSELPAGE * NUM_PERPAGE; 
				uint32 i = 0;
				for (auto it = instances->begin(); it != instances->end(); it++)
				{
					if ((*it)->minlevel > pPlayer->getLevel())
						continue;
					
					if ((*it)->faction >0)
					{
						if (pPlayer->GetTeam() != (*it)->faction)
							continue;
					}

					i++;
					if (i < start)
						continue;
					if (i >= start + NUM_PERPAGE)
					{
						pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800599, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 995);//下一页
						break;
					}

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
				if (i == 0)
				{
					ChatHandler(pPlayer).SendSysMessage(-2800708);//没有适合等级的传送点。
					return true;
				}
				if (pPlayer->context.MAPSELPAGE>0)
					pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 996);//上一页
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