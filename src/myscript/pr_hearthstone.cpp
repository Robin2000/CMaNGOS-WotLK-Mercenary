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
#include "pr_hearthstone.h"
#include "Chat.h"
#include "Language.h"
#include "ObjectMgr.h"
#include "hearthstone_store.h"
#include "mercenary_gossip.h"
#include "teleport.h"
#include "hearthstone_quest.h"
#include "hearthstone_learn.h"
#include "hearthstone_mount.h"
#include "gmtools.h"
#include "Mercenary.h"
enum actionMorph{
	ACTION_PLAYER_MORTH = 0,
	ACTION_PET_MORTH = 1
};
/*
insert into npc_text(ID,text0_0)values(16777210,'利用原力直达游戏目标。');
insert into npc_text(ID,text0_0)values(16777211,'利用原力临时随机召唤一只坐骑，忠诚度有限。');
insert into npc_text(ID,text0_0)values(16777212,'设置返回点成功,原力与你同在！');
insert into npc_text(ID,text0_0)values(16777213,'在线即可累积原力。');
insert into npc_text(ID,text0_0)values(16777214,'原力与你同在！更多信息请移步网站：http://51.neocities.org/。');

insert into custom_texts(entry, content_default)values(-2800169,'任务辅助');

insert into custom_texts(entry, content_default)values(-2800173,'当前原力值：%d');
insert into custom_texts(entry, content_default)values(-2800174,'设置返回点。（-3原力）');
insert into custom_texts(entry, content_default)values(-2800175,'前往返回点。（-2原力）');
insert into custom_texts(entry, content_default)values(-2800176,'回家。');
insert into custom_texts(entry, content_default)values(-2800177,'系统提示：原力不足，无法这样做。');
insert into custom_texts(entry, content_default)values(-2800178,'系统提示：还没有设置返回点。');
insert into custom_texts(entry, content_default)values(-2800179,'系统提示：设置返回点成功。');
insert into custom_texts(entry, content_default)values(-2800180,'系统提示：禁止在副本中设置返回点。');
insert into custom_texts(entry, content_default)values(-2800181,'返回主菜单。');
insert into custom_texts(entry, content_default)values(-2800182,'原力商店。');
insert into custom_texts(entry, content_default)values(-2800183,'雇佣兵招募。');

insert into custom_texts(entry, content_default)values(-2800190,'原力骑乘。');
insert into custom_texts(entry, content_default)values(-2800210,'小技巧。');//
insert into custom_texts(entry, content_default)values(-2800220,'成就直达。');
insert into custom_texts(entry, content_default)values(-2800184,'技能学习。');

insert into custom_texts(entry, content_default)values(-2800221,'秒升30级（-10原力）。');
insert into custom_texts(entry, content_default)values(-2800222,'秒升55级（-50原力）。');
insert into custom_texts(entry, content_default)values(-2800223,'秒升70级（-200原力）。');
insert into custom_texts(entry, content_default)values(-2800224,'秒升80级（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800232,'秒升声望（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800233,'秒开地图（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800234,'全开飞行点（-2原力）。');

insert into custom_texts(entry, content_default)values(-2800240,'T1-T8套装（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800300,'地图传送（-1原力）。');

insert into gossip_menu(entry,text_id) values(65535,16777213);
*/
bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	if (pPlayer->context.heart_stone_cooldown > 0)
	{
		ChatHandler(pPlayer).SendSysMessage(pPlayer->GetMangosString(-2800684));//23 系统提示：炉石冷却中。
		return true;
	}
	if (pPlayer->isInCombat())
	{
		ChatHandler(pPlayer).SendSysMessage(23);//23 系统提示：在战斗中无法这样做。
		pPlayer->CombatStopWithPets(true);
		pPlayer->context.heart_stone_cooldown = 6000;//设置6秒冷却
		return true;
	}
	pPlayer->context.checkFirstGuideQuest();
	hearthstone_click2(pPlayer, pItem);
	return true;
}
bool hearthstone_click2(Player* pPlayer, Item* pItem)
{
	//pPlayer->GetMotionMaster()->MovePoint(50000, pPlayer->GetPositionX() + 10, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), true);

	char * title = new char[1024];
	sprintf(title, pPlayer->GetMangosString(-2800173), pPlayer->context.gamePointMgr.getGamePoint()); // 当前原力值：%d

	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 当前原力值：%d
	
	for (auto it = pPlayer->getQuestStatusMap().begin(); it != pPlayer->getQuestStatusMap().end();it++)
	if (it->second.m_status == QUEST_STATUS_INCOMPLETE || (it->second.m_status == QUEST_STATUS_COMPLETE && !it->second.m_rewarded))
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800169, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);//任务辅助。
		break;
	}

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800167, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 14);//任务推荐。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800190, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);//原力骑乘。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, -2800182, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 10);//原力商店。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800183, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);//雇佣兵招募。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800699, 0, GOSSIP_ACTION_INFO_DEF + 998);//唤醒雇佣兵
	
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800689, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);//角色幻化。

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, -2800184, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 13);//技能学习
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, -2800220, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);//成就直升。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800300, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);//地图传送。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, -2800210, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);//发送游戏小技巧。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800174, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);//设置返回点。（-3原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800175, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);//前往返回点。（-2原力）
	if (pPlayer->isGameMaster())
	{
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, "TOOLS", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 20);//更新数据库表。

		if (pPlayer->GetSession()->GetSessionDbcLocale() != LOCALE_enUS)
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, "English", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);//中英文切换。
		else
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800683, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 21);//中英文切换。
	}

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800176, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);//回家。

	pPlayer->SEND_GOSSIP_MENU(16777213, pItem->GetObjectGuid()); //在线即可累积原力。

	pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem);
	pPlayer->context.gossipMenuType = -1;//出主菜单，就将玩家子菜单标志置为空
	pPlayer->context.gossipActionType = -1;
	return true;
}
bool excuteMorph2(Player* pPlayer, bool morphPet, uint32 display)
{
	if (morphPet)
	{
		if (Pet * pet = pPlayer->GetPet())
		if (pet->isMercenary())
		{
			pet->SetDisplayId(display);
			pet->SetNativeDisplayId(display);
			pPlayer->context.GetMercenary()->SetDisplay(display);
			pPlayer->context.GetMercenary()->SendMirrorImagePacket(pet);
			return true;
		}
		ChatHandler(pPlayer).SendSysMessage(-2800698);//你还没有招募雇佣兵。
		return false;
	}
	pPlayer->context.displayid = display;
	pPlayer->SetDisplayId(display);
	return true;
}
void excuteMorph(Player* pPlayer, bool morphPet, uint32 displayid){
	
	pPlayer->context.gossipActionType = -1;//还原

	if (displayid == 0)
	{
		ObjectGuid obj = pPlayer->GetSelectionGuid();
		if (!obj)
		{
			ChatHandler(pPlayer).SendSysMessage(-2800690);
			return;
		}

		Creature * creature = pPlayer->GetMap()->GetCreature(obj);
		if (!creature)
			return;
		
		if(excuteMorph2(pPlayer, morphPet, creature->GetDisplayId()))
			pPlayer->context.addMorphHistory(creature->GetEntry(), creature->GetDisplayId());//添加到历史
	}
	else
	{
		excuteMorph2(pPlayer, morphPet, displayid);
	}

}
void DeMorphMercenary(Player* pPlayer, Pet * pet){
	
	Mercenary * mercenary=pPlayer->context.GetMercenary();
	uint32 model = mercenary->GetGender() == GENDER_FEMALE ? mercenary->getCharFemalModel(mercenary->GetRace()) : mercenary->getCharMaleModel(mercenary->GetRace());

	pet->SetDisplayId(model);
	pet->SetNativeDisplayId(model);
	pPlayer->context.GetMercenary()->SetDisplay(model);
	pPlayer->context.GetMercenary()->SendMirrorImagePacket(pet);
}
void showMorphMenu(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800695, GOSSIP_SENDER_MAIN, 0);  // 易容为选择对象
	
	int count = 0;
	for (auto it = pPlayer->context.creaturehistory.begin(); it != pPlayer->context.creaturehistory.end(); it++, count++)
	{
		const char * name = "";
		pPlayer->context.GetCreatureOrGOTitleLocale(*it, &name);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, name, GOSSIP_SENDER_MAIN, pPlayer->context.displayhistory.at(count));// 易容为历史对象
	}
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800598, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);//上一页
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
void showHH(Player* pPlayer, Item* pItem)
{
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800691, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 22);  // 角色易容(-2原力)
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800692, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 23);  // 取消角色易容
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800693, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 24);  // 雇佣兵易容
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800694, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 25);  // 取消雇佣兵易容

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
bool hearthstone_menu_click(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{	
	//pPlayer->PlayerTalkClass->ClearMenus();
	pPlayer->CLOSE_GOSSIP_MENU();/*无条件关闭旧菜单*/

	if (pPlayer->isInCombat())
	{
		ChatHandler(pPlayer).SendSysMessage(23);//23 系统提示：在战斗中无法这样做。
		return true;
	}
	if (uiAction == GOSSIP_ACTION_INFO_DEF + 999)
	{
		hearthstone_click2(pPlayer, pItem);//返回主菜单，无论哪个菜单子类，通用
		return true;
	}

	//////////////////////////////////////////雇佣兵招募
	if (pPlayer->context.gossipMenuType == -1&&uiAction == GOSSIP_ACTION_INFO_DEF + 12){
		pPlayer->context.gossipMenuType = 1;
		GossipHello_mercenary_npc_gossip(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 998)///召唤雇佣兵
	{
		recallMercenary(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 1)
		return GossipSelect_mercenary_npc_gossip(pPlayer, pItem,NULL , uiAction);

	//////////////////////////////////////////地图传送。
	if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 8)
	{
		pPlayer->context.gossipMenuType = 2;//地图传送。
		hearthstone_prepare_transport(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 2)
	{
		hearthstone_transport_case(pPlayer, pItem, uiAction);
		return true;
	}
	
	//////////////////////////////////////////原力商店
	if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 10)//原力商店
	{
		pPlayer->context.gossipMenuType = 3;//原力商店
		hearthstone_prepare_store(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 3)
	{
		hearthstone_store_click(pPlayer, pItem, uiAction);
		return true;
	}

	//////////////////////////////////////////任务辅助
	if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 9)//任务辅助。
	{
		pPlayer->context.gossipMenuType = 4;//任务辅助。
		pPlayer->context.gossipActionType = -1;
		hearthstone_prepare_quest_aux(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 4)
	{
		hearthstone_quest_click(pPlayer, pItem, uiAction);
		return true;
	}

	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 14)//任务推荐
	{
		pPlayer->context.gossipMenuType = 5;//任务推荐。
		pPlayer->context.gossipActionType = -1;
		hearthstone_prepare_quest_area(pPlayer, pItem, pPlayer->GetAreaId());
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 5)
	{
		hearthstone_quest_click(pPlayer, pItem, uiAction);
		return true;
	}

	//////////////////////////////////////////技能学习
	if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 13)//技能学习。
	{
		pPlayer->context.gossipMenuType = 6;//技能学习。
		hearthstone_prepareLearn(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 6)
	{
		hearthstone_learn_click(pPlayer, pItem, uiAction);
		return true;
	}

	//////////////////////////////////////////原力骑乘
	if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 2)//原力骑乘
	{
		pPlayer->context.gossipMenuType = 7;//原力骑乘
		hearthstone_prepare_mount(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 7)
	{
		hearthstone_mount_click(pPlayer, pItem, uiAction);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 20){
		pPlayer->context.gossipMenuType = 8;//GM_TOOLS
		hearthstone_prepare_gmtools(pPlayer, pItem, uiAction);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 21){
		if (pPlayer->GetSession()->GetSessionDbcLocale() == LOCALE_zhCN)
			pPlayer->GetSession()->SetSessionDbcLocale(LOCALE_enUS);
		else
			pPlayer->GetSession()->SetSessionDbcLocale(LOCALE_zhCN);
		pPlayer->UpdateForQuestWorldObjects();

		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 11){
		showHH(pPlayer,pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 22){
		pPlayer->context.gossipActionType = ACTION_PLAYER_MORTH;
		showMorphMenu(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 23){
		pPlayer->DeMorph(); 
		pPlayer->context.displayid = 0;
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 24){
		pPlayer->context.gossipActionType = ACTION_PET_MORTH;
		showMorphMenu(pPlayer, pItem);
		return true;
	}
	else if (pPlayer->context.gossipMenuType == -1 && uiAction == GOSSIP_ACTION_INFO_DEF + 25){
		if (Pet * pet = pPlayer->GetPet())
		if (pet->isMercenary())
		{
			DeMorphMercenary(pPlayer, pet);
			return true;
		}
		ChatHandler(pPlayer).SendSysMessage(-2800698);//你还没有招募雇佣兵。
		return true;
	}
	else if (pPlayer->context.gossipMenuType == 8)
	{
		hearthstone_click_gmtools(pPlayer, pItem, uiAction);
		return true;
	}

	//////////////////////////////////////////其它
	pPlayer->context.gossipMenuType = -1;/*默认值*/

	if (pPlayer->context.gossipActionType == ACTION_PLAYER_MORTH)
	{
		excuteMorph(pPlayer, false, uiAction);
		return true;
	}
	else if (pPlayer->context.gossipActionType == ACTION_PET_MORTH){
		excuteMorph(pPlayer, true, uiAction);
		return true;
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) //当前原力值：%d
	{
		pPlayer->SEND_GOSSIP_MENU(16777214, pItem->GetObjectGuid()); //'原力与你同在！更多信息请移步：http://51.neocities.org。'
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2){//设置返回点。（-3原力）
		hearthstone_prepare_mount(pPlayer, pItem);
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 3){//游戏直达
		hearthstone_prepare_gamedirect(pPlayer, pItem);
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 4){//设置返回点。（-3原力）
		pPlayer->sendSplitMsg(pPlayer->context.gamePointMgr.nextGameTip());
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 5){//设置返回点。（-3原力）
		pPlayer->context.gamePointMgr.setReturnPoint(3);
		if (pPlayer->context.gamePointMgr.setReturnPoint(3))
		{
			//pPlayer->PlayerTalkClass->SendPointOfInterest(pPlayer->GetPositionX(), pPlayer->GetPositionY(), 7, 39, 0, "ReturnPoint");
			//pPlayer->SEND_GOSSIP_MENU(16777212, pItem->GetObjectGuid());//设置返回点成功,原力与你同在！
		}
		//return true;
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF +6)//前往返回地点。（-2原力）
	{
		pPlayer->context.gamePointMgr.useReturnPoint(2);
	}else if (uiAction == GOSSIP_ACTION_INFO_DEF + 7)//回家。
	{
		pPlayer->TeleportToHomebind();
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 110 && uiAction <= GOSSIP_ACTION_INFO_DEF + 129)
	{
		hearthstone_gamedirect(pPlayer, pItem, uiAction);
	}


	//pPlayer->HandleEmoteCommandHappy();
	return true;
}


bool learn_default_spell(Player* pPlayer, Item* pItem, uint32 uiAction){
	if (!pPlayer->context.gamePointMgr.checkPoint(500))
		return false;
	
	ChatHandler chatHandler(pPlayer);
	chatHandler.HandleLearnAllMySpellsCommand((char*)"");
	chatHandler.HandleLearnAllMyTalentsCommand((char*)"");

	pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);
	return true;
}
void hearthstone_prepare_gamedirect(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800221, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 110);  // 秒升30级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800222, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 111);  // 秒升55级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800223, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 112);  // 秒升70级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800224, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 113);  // 秒升80级
	//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800232, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 116);  // 秒升声望
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800233, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 117);  // 全开地图
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800234, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 118);  // 全开飞行点
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}

void hearthstone_gamedirect(Player* pPlayer, Item* pItem, uint32 uiAction){
	switch (uiAction)
	{
		case GOSSIP_ACTION_INFO_DEF + 110:levelup(pPlayer, 30,10); break;
		case GOSSIP_ACTION_INFO_DEF + 111:levelup(pPlayer, 55,50); break;
		case GOSSIP_ACTION_INFO_DEF + 112:levelup(pPlayer, 70,200); break;
		case GOSSIP_ACTION_INFO_DEF + 113:levelup(pPlayer, 80,500); break;
		//case GOSSIP_ACTION_INFO_DEF + 116:gamedirect(pPlayer, maMountSpell); break;
		case GOSSIP_ACTION_INFO_DEF + 117:explorecheat(pPlayer); break;
		case GOSSIP_ACTION_INFO_DEF + 118:taxicheat(pPlayer); break;
	}
}

//explorecheat
bool explorecheat(Player* pPlayer){
	if (!pPlayer->context.gamePointMgr.checkPoint(3))
		return false;

	for (uint8 i = 0; i < PLAYER_EXPLORED_ZONES_SIZE; ++i)
		pPlayer->SetFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);

	ChatHandler chatHandler(pPlayer);
	chatHandler.PSendSysMessage(LANG_YOURS_EXPLORE_SET_ALL, chatHandler.GetNameLink().c_str());

	pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);
	return true;
}
//taxicheat
bool taxicheat(Player* pPlayer){
	if (!pPlayer->context.gamePointMgr.checkPoint(2))
		return false;

	pPlayer->SetTaxiCheater(true);
	ChatHandler chatHandler(pPlayer);
	chatHandler.PSendSysMessage(LANG_YOURS_TAXIS_ADDED, chatHandler.GetNameLink().c_str());

	pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_TAXICHEAT, 2);
	return true;
}
bool levelup(Player* pPlayer, int level, int point)
{
	if (!pPlayer->context.gamePointMgr.checkPoint(point))
		return false;

	if (level - pPlayer->getLevel() <= 0)
		return false;

	pPlayer->GiveLevel(level);
	pPlayer->InitTalentForLevel();
	pPlayer->SetUInt32Value(PLAYER_XP, 0);

	pPlayer->context.gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_LEVELUP, point);

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
