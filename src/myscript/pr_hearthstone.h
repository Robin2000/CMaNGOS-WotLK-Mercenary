#ifndef __PR_HEARTHSTONE_H
#define __PR_HEARTHSTONE_H


bool levelup(Player* pPlayer, int level, int point);
void hearthstone_prepare_gamedirect(Player* pPlayer, Item* pItem);
void hearthstone_gamedirect(Player* pPlayer, Item* pItem, uint32 uiAction);
bool explorecheat(Player* pPlayer);
bool taxicheat(Player* pPlayer);
bool hearthstone_click2(Player* pPlayer, Item* pItem);

#endif