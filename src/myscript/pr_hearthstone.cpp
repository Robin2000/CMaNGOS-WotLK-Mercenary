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
insert into npc_text(ID,text0_0)values(16777212,'设置返回点成功,原力与你同在！');
insert into npc_text(ID,text0_0)values(16777213,'在线即可累积原力。');
insert into npc_text(ID,text0_0)values(16777214,'原力与你同在！更多信息请移步交流论坛：http://mangos.mybbs.us。');

insert into custom_texts(entry, content_default)values(-2800173,'当前原力值：%d');
insert into custom_texts(entry, content_default)values(-2800174,'设置返回点。（-3原力）');
insert into custom_texts(entry, content_default)values(-2800175,'前往返回点。（-2原力）');
insert into custom_texts(entry, content_default)values(-2800176,'回家。');
insert into custom_texts(entry, content_default)values(-2800177,'系统提示：原力不足，无法这样做。');
insert into custom_texts(entry, content_default)values(-2800178,'系统提示：还没有设置返回点。');
insert into custom_texts(entry, content_default)values(-2800179,'系统提示：设置返回点成功。');
insert into custom_texts(entry, content_default)values(-2800180,'系统提示：禁止在副本中设置返回点。');
insert into custom_texts(entry, content_default)values(-2800181,'返回上级菜单。');

insert into gossip_menu(entry,text_id) values(65535,16777213);
*/
bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	//pPlayer->GetMotionMaster()->MovePoint(50000, pPlayer->GetPositionX() + 10, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), true);
	pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem);
	char * title=new char[1024];
	sprintf(title, pPlayer->GetMangosString(-2800173), pPlayer->GetGamePointMgr().getGamePoint()); // 当前原力值：%d
	
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101，
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 当前原力值：%d
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800174, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);//设置返回点。（-3原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800175, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);//前往返回点。（-2原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800176, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);//回家。
	pPlayer->SEND_GOSSIP_MENU(16777213, pItem->GetObjectGuid()); //在线即可累积原力。

	return true;
}

bool hearthstone_menu_click(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{	
	if (pPlayer->isInCombat())
		ChatHandler(pPlayer).SendSysMessage(23);//23 系统提示：在战斗中无法这样做。
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) //当前原力值：%d
	{
		pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535已添加到数据库
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 返回上级菜单。
		pPlayer->SEND_GOSSIP_MENU(16777214, pItem->GetObjectGuid()); //'原力与你同在！更多信息请移步交流论坛：http://mangos.mybbs.us。'
		return true;
	}else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2){//设置返回点。（-3原力）
		pPlayer->GetGamePointMgr().setReturnPoint(3);
		if (pPlayer->GetGamePointMgr().setReturnPoint(3))
		{
			//pPlayer->PlayerTalkClass->SendPointOfInterest(pPlayer->GetPositionX(), pPlayer->GetPositionY(), 7, 39, 0, "ReturnPoint");
			//pPlayer->SEND_GOSSIP_MENU(16777212, pItem->GetObjectGuid());//设置返回点成功,原力与你同在！
		}
		//return true;
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF +3)//前往返回地点。（-2原力）
	{
		pPlayer->GetGamePointMgr().useReturnPoint(2);
	}else if (uiAction == GOSSIP_ACTION_INFO_DEF + 4)//回家。
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
