#ifndef PR_SPELL_PLUGIN_H
#define PR_SPELL_PLUGIN_H
#include "PlayerContext.h"

class MANGOS_DLL_SPEC PrSpellPlugin{
public:
	bool spell_handler_deal(Spell * spell, Unit* caster);
	void update(uint32 difftime);
};

#endif