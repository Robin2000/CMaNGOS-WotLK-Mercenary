#ifndef PR_SPELL_PLUGIN_H
#define PR_SPELL_PLUGIN_H
#include "PlayerContext.h"

class MANGOS_DLL_SPEC PrSpellPlugin{
public:
	PrSpellPlugin(Player * _player) :player(_player){}
	bool spell_handler_deal(Spell * spell);
	void spell_handler_cancel(uint32 spellid);
	void update(uint32 difftime);
	Player * player;
};

#endif