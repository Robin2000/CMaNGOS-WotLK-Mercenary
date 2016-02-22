#ifndef __PR_HEARTSTONE_STORE_GOSSIP_H
#define __PR_HEARTSTONE_STORE_GOSSIP_H

#include "Player.h"

bool hearthstone_store_click(Player* pPlayer, Item* pItem, uint32 uiAction);
void hearthstone_prepare_itemset(Player* pPlayer, Item* pItem, uint32 uiAction);
bool additemset(Player* pPlayer, uint32 itemset);
void hearthstone_prepare_store(Player* pPlayer, Item* pItem);


#endif