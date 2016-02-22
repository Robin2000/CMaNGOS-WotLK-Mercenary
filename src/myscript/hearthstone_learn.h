#ifndef __PR_HEARTSTONE_LEARN_GOSSIP_H
#define __PR_HEARTSTONE_LEARN_GOSSIP_H

#include "Player.h"

void hearthstone_prepareLearn(Player* pPlayer, Item* pItem);
bool hearthstone_learn_click(Player* pPlayer, Item* pItem, uint32 uiAction);
bool learn_professional(Player* pPlayer, uint32 skillid, uint8 maxnum);
bool learn_default_spell(Player* pPlayer, Item* pItem, uint32 uiAction);


#endif