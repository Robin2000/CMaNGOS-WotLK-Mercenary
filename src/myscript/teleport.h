#ifndef __PR_TELEPORT_GOSSIP_H
#define __PR_TELEPORT_GOSSIP_H

#include "Player.h"

bool hearthstone_prepare_transport(Player* pPlayer, Item* pItem);
bool hearthstone_prepare_transport2(Player* pPlayer, Item* pItem, uint32 uiAction);
bool hearthstone_transport(Player* pPlayer, uint32 mapid, double x, double y, double z, double o);
bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction);

#endif