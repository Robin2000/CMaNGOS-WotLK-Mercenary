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
#define NPC_TEXT_INTRO  16777214 //在npc_text中定义。
#define HEARTHSTONE_CUSTOMTEXT_QUEST -2800169 //在custom_text中定义。
#define HEARTHSTONE_CUSTOMTEXT_NPC -2800170 //在custom_text中定义。
#define HEARTHSTONE_CUSTOMTEXT_SETHOME -2800171 //在custom_text中定义。
#define HEARTHSTONE_CUSTOMTEXT_GOHOME -2800172 //在custom_text中定义。


/*
insert into npc_text(ID,text0_0)values(16777214,'在线赚取积分，积分兑换服务。');
insert into custom_texts(entry, content_default)values(-2800169,'任务查询。');
insert into custom_texts(entry, content_default)values(-2800170,'NPC查询。');
insert into custom_texts(entry, content_default)values(-2800171,'将此地设置炉石绑定地点（-5积分）。');
insert into custom_texts(entry, content_default)values(-2800172,'传送我到炉石绑定地点。');
*/
bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	pPlayer->PrepareGossipMenu(pPlayer, 100000);//100000是不存在的menuid，数据库中目前最大为50101，
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, HEARTHSTONE_CUSTOMTEXT_QUEST, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, HEARTHSTONE_CUSTOMTEXT_NPC, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, HEARTHSTONE_CUSTOMTEXT_SETHOME, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, HEARTHSTONE_CUSTOMTEXT_GOHOME, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
	pPlayer->SEND_GOSSIP_MENU(NPC_TEXT_INTRO, pItem->GetObjectGuid()); //你好 $N。NPC_TEXT_INTRO在npc_text中定义。
	return true;
}

bool hearthstone_menu_click(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{
	pPlayer->CLOSE_GOSSIP_MENU();/*无条件关闭旧菜单*/

	if (uiAction == GOSSIP_ACTION_INFO_DEF + 4)
		pPlayer->TeleportToHomebind();
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 3)
	{

	}
	else
		pPlayer->TextEmote("Sorry");
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "dynamic", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_QUIT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		//pPlayer->SEND_GOSSIP_MENU(907, npc->GetObjectGuid());



	return true;
}

void AddSC_hearthstone_script()
{
	Script* pNewScript;
	pNewScript = new Script;
	pNewScript->Name = "pr_hearthstone";
	pNewScript->pItemUse = &hearthstone_click;
	pNewScript->pGossipSelectItem = &hearthstone_menu_click;
	pNewScript->RegisterSelf(false);

}
