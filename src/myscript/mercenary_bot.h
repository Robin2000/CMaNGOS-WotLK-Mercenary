#ifndef __PR_MERCENARY_BOT_H
#define __PR_MERCENARY_BOT_H

#include "precompiled.h"
struct MercenarySpell;
bool OnGossipHello_mercenary_bot(Player* player, Creature* creature);
bool OnGossipSelect_mercenary_bot(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions);
bool OnGossipSelectWithCode_mercenary_bot(Player* player, Creature* creature, uint32 sender, uint32 actions, const char* code);
void addUnLearnSpellMenu(Player* player, Mercenary* mercenary, Creature* creature);
void addLearnSpellMenu(Player* player, Mercenary* mercenary, Creature* creature);
#endif