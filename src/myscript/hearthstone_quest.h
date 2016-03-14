#ifndef __PR_HEARTSTONE_QUEST_GOSSIP_H
#define __PR_HEARTSTONE_QUEST_GOSSIP_H

#include "Player.h"

void hearthstone_quest(Player* pPlayer, Item* pItem, uint32 questid);
void transportByCreatureOrGO(Player* pPlayer, Item* pItem, int idx);
void transportByPOI(Player* pPlayer, Item* pItem, int idx);
void transportByCreatureOrGO2(Player* pPlayer, Item* pItem, int32 creatureOrGOEntry);
void transportToStarter(Player* pPlayer, Item* pItem);
void transportToInvolved(Player* pPlayer, Item* pItem, int idx);

void hearthstone_prepare_quest_aux(Player* pPlayer, Item* pItem);

bool hearthstone_quest_click(Player* pPlayer, Item* pItem, uint32 uiAction);

/*任务推荐主菜单*/
void hearthstone_prepare_quest_area(Player* pPlayer, Item* pItem, int area);

//推荐zone下所有区域
void hearthstone_prepare_quest_zone(Player* pPlayer, Item* pItem, int zone);

#endif