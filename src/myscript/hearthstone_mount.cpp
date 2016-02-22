#include "precompiled.h"
#include "hearthstone_mount.h"

/*
insert into custom_texts(entry, content_default)values(-2800191,'��-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800192,'��-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800193,'¹��-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800194,'����-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800195,'��-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800196,'����-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800197,'�-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800198,'ʨ�գ�-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800199,'��-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800200,'�ܣ�-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800201,'����-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800202,'����-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800203,'�ǣ�-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800204,'�ޣ�-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800205,'̺��-1ԭ������');
insert into custom_texts(entry, content_default)values(-2800206,'�棨-1ԭ������');
*/
void hearthstone_prepare_mount(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535�ǲ����ڵ�menuid�����ݿ���Ŀǰ���Ϊ50101 �رղ��ǹؼ���Ԥ����Ż����㡣
	//pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535����ӵ����ݿ�,
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800191, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 91);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800192, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 92);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800193, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 93);  // ¹
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800194, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 94);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800195, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 95);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800196, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 96);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800197, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 97);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800198, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 98);  // ʨ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800199, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 99);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800200, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800201, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800202, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800203, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800204, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 104);  // ��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800205, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 105);  // ̺
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800206, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 106);  // ̹��
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	pPlayer->SEND_GOSSIP_MENU(16777211, pItem->GetObjectGuid()); //����ԭ����ʱ����ٻ�һֻ����ҳ϶����ޡ�
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
	pPlayer->gossipMenuType = -1;/*��ԭĬ��ֵ*/
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