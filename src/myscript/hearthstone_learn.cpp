#include "precompiled.h"
#include "hearthstone_learn.h"
/*
delete from custom_texts where entry>=-2800279 and entry<=-2800260;

insert into custom_texts(entry, content_default)values(-2800230,'学习专业技能');
insert into custom_texts(entry, content_default)values(-2800231,'学习本职业全部技能（-500原力）。');

insert into custom_texts(entry, content_default)values(-2800260,'【商业技能】锻造（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800261,'【商业技能】珠宝（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800262,'【商业技能】工程学（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800263,'【商业技能】制皮（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800264,'【商业技能】铭文（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800265,'【商业技能】炼金术（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800266,'【商业技能】裁缝（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800267,'【商业技能】附魔（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800268,'【生活技能】烹饪（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800269,'【商业技能】采矿（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800270,'【商业技能】草药学（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800271,'【商业技能】剥皮（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800272,'【生活技能】钓鱼（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800273,'【生活技能】急救（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800279,'系统提示：商业技能最多学习2个。');
*/

void hearthstone_prepareLearn(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800230, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 114);  // 学习专业技能
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800231, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 115);  // 学习职业技能

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
void hearthstone_prepareProfessional(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800260, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 130);  // 锻造
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800261, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 131);  // 珠宝
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800262, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 132);  // 工程学
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800263, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 133);  // 制皮
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800264, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 134);  // 铭文
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800265, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 135);  // 炼金术
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800266, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 136);  // 裁缝
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800267, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 137);  // 附魔
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800268, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 138);  // 烹饪
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800269, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 139);  // 采矿
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800270, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 140);  // 药学
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800271, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 141);  // 剥皮
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800272, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 142);  // 钓鱼
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800273, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 143);  // 急救
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}

bool hearthstone_learn_click(Player* pPlayer, Item* pItem, uint32 uiAction){

	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 114:hearthstone_prepareProfessional(pPlayer, pItem); return true;
	case GOSSIP_ACTION_INFO_DEF + 115:learn_default_spell(pPlayer, pItem, uiAction); break;
	case GOSSIP_ACTION_INFO_DEF + 130:learn_professional(pPlayer, 164, 2); break;//锻造,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 131:learn_professional(pPlayer, 755, 2); break;//珠宝,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 132:learn_professional(pPlayer, 202, 2); break;//工程学,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 133:learn_professional(pPlayer, 165, 2); break;//制皮,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 134:learn_professional(pPlayer, 773, 2); break;//铭文,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 135:learn_professional(pPlayer, 171, 2); break;//炼金术,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 136:learn_professional(pPlayer, 197, 2); break;//裁缝,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 137:learn_professional(pPlayer, 333, 2); break;//附魔,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 138:learn_professional(pPlayer, 185, 3); break;//烹饪,生活技能有三个，可以随便学习
	case GOSSIP_ACTION_INFO_DEF + 139:learn_professional(pPlayer, 186, 2); break;//采矿,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 140:learn_professional(pPlayer, 182, 2); break;//草药学,商业技能只能学习二个
	case GOSSIP_ACTION_INFO_DEF + 141:learn_professional(pPlayer, 393, 2); break;//剥皮,商业技能只能学习二个 
	case GOSSIP_ACTION_INFO_DEF + 142:learn_professional(pPlayer, 356, 3); break; //钓鱼,生活技能有三个，可以随便学习
	case GOSSIP_ACTION_INFO_DEF + 143:learn_professional(pPlayer, 129, 3); break; //急救,生活技能有三个，可以随便学习
	}

	pPlayer->gossipMenuType = -1;/*默认值*/
	return true;
}
bool learn_professional(Player* pPlayer, uint32 skillid, uint8 maxnum)//.learnsk 165 300 // 制皮  
{
	if (!pPlayer->gamePointMgr.checkPoint(500))
		return false;

	if (pPlayer->HasSkill(skillid))
		return false;

	if (maxnum == 2){
		uint32 all[11] = { 164, 755, 202, 165, 773, 171, 197, 333, 186, 182, 393 };
		int count = 0;
		for (int i = 0; i < 11; i++)
		if (pPlayer->HasSkill(all[i]))
		{
			count++;
			if (count == 2)
			{
				ChatHandler(pPlayer).SendSysMessage(-2800279);//系统提示：商业技能最多学习2个。
				return false;
			}
		}
	}

	ChatHandler(pPlayer).HandleLearnSkillRecipesHelper(pPlayer, skillid);

	uint16 max = pPlayer->GetPureMaxSkillValue(skillid);

	pPlayer->SetSkill(skillid, max, max);

	pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);

	return true;
}