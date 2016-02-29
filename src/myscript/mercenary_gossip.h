#ifndef __PR_MERCENARY_GOSSIP_H
#define __PR_MERCENARY_GOSSIP_H

#include "Player.h"
bool GossipHello_mercenary_npc_gossip(Player* player, Item* item);
bool GossipSelect_mercenary_npc_gossip(Player* player, Item* item, uint32 /*sender*/, uint32 actions);
bool GossipSelect_mercenary_npc_gossip2(Player* player, Item* item, uint32 actions);
bool recallMercenary(Player* player, Item* item);

#endif