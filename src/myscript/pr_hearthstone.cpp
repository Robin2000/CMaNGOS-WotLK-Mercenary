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
#include <boost/format.hpp>
#include "GamePointMgr.h"


/*

insert into npc_text(ID,text0_0)values(16777213,'在线即可累积原力。');
insert into npc_text(ID,text0_0)values(16777214,'原力与你同在！更多信息请移步交流论坛：http://mangos.mybbs.us。');

insert into custom_texts(entry, content_default)values(-2800173,'当前原力值：%d');
insert into custom_texts(entry, content_default)values(-2800174,'设置回城地点。（-3原力）');
insert into custom_texts(entry, content_default)values(-2800175,'返回回城地点。（-2原力）');
insert into custom_texts(entry, content_default)values(-2800176,'返回炉石绑定地点。');
insert into custom_texts(entry, content_default)values(-2800177,'系统提示：原力不足，无法这样做。');
insert into custom_texts(entry, content_default)values(-2800178,'系统提示：还没有设置回城点。');
insert into custom_texts(entry, content_default)values(-2800179,'系统提示：设置回城点成功。');
insert into custom_texts(entry, content_default)values(-2800180,'系统提示：无法在副本中设置回城点。');
insert into custom_texts(entry, content_default)values(-2800181,'返回上级菜单。');
*/
bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	//pPlayer->GetMotionMaster()->MovePoint(50000, pPlayer->GetPositionX() + 10, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), true);
	pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem);
	char * title=new char[1024];
	sprintf(title, pPlayer->GetMangosString(-2800173), pPlayer->GetGamePointMgr().getGamePoint()); // 当前原力值：%d
	
	pPlayer->PrepareGossipMenu(pPlayer, 100000);//100000是不存在的menuid，数据库中目前最大为50101，
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 当前原力值：%d
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800174, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);//设置回城地点。（-3原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800175, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);//返回回城地点。（-2原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800176, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);//返回炉石绑定地点。
	pPlayer->SEND_GOSSIP_MENU(16777213, pItem->GetObjectGuid()); //在线即可累积原力。

	return true;
}

bool hearthstone_menu_click(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{	
	if (pPlayer->isInCombat())
		ChatHandler(pPlayer).SendSysMessage(23);//23 系统提示：在战斗中无法这样做。
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) //当前原力值：%d
	{
		pPlayer->PrepareGossipMenu(pPlayer, 100000);//100000是不存在的menuid，数据库中目前最大为50101，
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 返回上级菜单。
		pPlayer->SEND_GOSSIP_MENU(16777214, pItem->GetObjectGuid()); //'原力与你同在！更多信息请移步交流论坛：http://mangos.mybbs.us。'
	}else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2){//设置回城点。（-3原力）
		if(pPlayer->GetGamePointMgr().setReturnPoint(3))
			pPlayer->PlayerTalkClass->SendPointOfInterest(pPlayer->GetPositionX(), pPlayer->GetPositionY(), 7, 583, 0, "test");
		//pPlayer->PlayerTalkClass->SendPointOfInterest(target->GetPositionX(), target->GetPositionY(), Poi_Icon(icon), flags, 30, "Test POI");

	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF +3)//返回回城地点。（-2原力）
	{
		pPlayer->GetGamePointMgr().useReturnPoint(2);
	}else if (uiAction == GOSSIP_ACTION_INFO_DEF + 4)//传送我到炉石绑定地点。
	{
		pPlayer->TeleportToHomebind();
	}
	pPlayer->CLOSE_GOSSIP_MENU();/*无条件关闭旧菜单*/
	//pPlayer->HandleEmoteCommandHappy();
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
