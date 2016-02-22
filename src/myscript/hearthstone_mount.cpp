#include "precompiled.h"
#include "hearthstone_mount.h"

/*
insert into custom_texts(entry, content_default)values(-2800191,'马（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800192,'羊（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800193,'鹿（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800194,'鸡（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800195,'鸟（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800196,'龙（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800197,'凤（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800198,'狮鹫（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800199,'象（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800200,'熊（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800201,'虎（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800202,'豹（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800203,'狼（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800204,'兽（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800205,'毯（-1原力）。');
insert into custom_texts(entry, content_default)values(-2800206,'虫（-1原力）。');
*/
void hearthstone_prepare_mount(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	//pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535已添加到数据库,
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800191, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 91);  // 马
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800192, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 92);  // 羊
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800193, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 93);  // 鹿
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800194, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 94);  // 鸡
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800195, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 95);  // 鸟
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800196, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 96);  // 龙
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800197, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 97);  // 凤
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800198, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 98);  // 狮鹫
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800199, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);  // 象
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800200, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);  // 熊
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800201, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);  // 虎
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800202, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);  // 豹
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800203, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);  // 狼
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800204, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 104);  // 兽
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800205, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 105);  // 毯
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800206, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 106);  // 坦克
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777211, pItem->GetObjectGuid()); //利用原力临时随机召唤一只坐骑，忠诚度有限。
}
bool hearthstone_mount_click(Player* pPlayer, Item* pItem, uint32 uiAction){
	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 91:hearthstone_random_mount(pPlayer, maMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 92:hearthstone_random_mount(pPlayer, yangMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 93:hearthstone_random_mount(pPlayer, luMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 94:hearthstone_random_mount(pPlayer, jiMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 95:hearthstone_random_mount(pPlayer, niaoMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 96:hearthstone_random_mount(pPlayer, longMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 97:hearthstone_random_mount(pPlayer, fengMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 98:hearthstone_random_mount(pPlayer, shijiuMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 99:hearthstone_random_mount(pPlayer, xiangMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 100:hearthstone_random_mount(pPlayer, xiongMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 101:hearthstone_random_mount(pPlayer, huMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 102:hearthstone_random_mount(pPlayer, baoMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 103:hearthstone_random_mount(pPlayer, langMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 104:hearthstone_random_mount(pPlayer, shouMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 105:hearthstone_random_mount(pPlayer, tanMountSpell); break;
	case GOSSIP_ACTION_INFO_DEF + 106:hearthstone_random_mount(pPlayer, tankeMountSpell); break;
	}
	pPlayer->gossipMenuType = -1;/*还原默认值*/
	return true;
}
bool hearthstone_random_mount(Player* pPlayer, const int spellarray[])
{
	if (!pPlayer->gamePointMgr.checkPoint(1))
		return false;

	int size = sizeof(spellarray) / sizeof((int)1);
	int spell = spellarray[rand() % size];
	if (!pPlayer->AddMountSpellAura(spell))
		return false;


	pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 1);

	return true;
}