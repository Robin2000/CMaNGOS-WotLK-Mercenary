/*
* 通过炉石实现召唤
* 表：scripted_areatrigger|scripted_event_id|creature_template.ScriptName|gameobject_template.ScriptName|item_template.ScriptName|instance_template.ScriptName|world_template.ScriptName
* 物品: entry=6948 name=Hearthstone ScriptName=pr_hearthstone
* SQL:
update  item_template set ScriptName='pr_hearthstone' where entry=6948;
*/

/* ScriptData
SDName: Example_Misc
SD%Complete: 100
SDComment: Item, Areatrigger and other small code examples
SDCategory: Script Examples
EndScriptData */

#include "precompiled.h"

#define NPC_BIRD 100000


bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	//uint32 id, float x, float y, float z, float ang, TempSummonType spwtype, uint32 despwtime, bool asActiveObject
	Creature* npc = pPlayer->SummonCreature(NPC_BIRD, pPlayer->GetPositionX() + 1, pPlayer->GetPositionY() + 1, pPlayer->GetPositionZ() + 2, 0, TEMPSUMMON_TIMED_OOC_OR_DEAD_DESPAWN, 300000);

	npc->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
	//npc->SetName("temp");

	pPlayer->SetFacingToObject(npc);

	//npc->HandleEmote(EMOTE_ONESHOT_WAVE);

	return true;
}




void AddSC_hearthstone_script()
{
	Script* pNewScript;
	pNewScript = new Script;
	pNewScript->Name = "pr_hearthstone";
	pNewScript->pItemUse = &hearthstone_click;
	pNewScript->RegisterSelf(false);

}
