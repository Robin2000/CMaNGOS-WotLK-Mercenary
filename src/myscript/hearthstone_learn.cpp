#include "precompiled.h"
#include "hearthstone_learn.h"
/*
delete from custom_texts where entry>=-2800279 and entry<=-2800260;

insert into custom_texts(entry, content_default)values(-2800230,'ѧϰרҵ����');
insert into custom_texts(entry, content_default)values(-2800231,'ѧϰ��ְҵȫ�����ܣ�-500ԭ������');

insert into custom_texts(entry, content_default)values(-2800260,'����ҵ���ܡ����죨-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800261,'����ҵ���ܡ��鱦��-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800262,'����ҵ���ܡ�����ѧ��-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800263,'����ҵ���ܡ���Ƥ��-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800264,'����ҵ���ܡ����ģ�-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800265,'����ҵ���ܡ���������-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800266,'����ҵ���ܡ��÷죨-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800267,'����ҵ���ܡ���ħ��-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800268,'������ܡ���⿣�-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800269,'����ҵ���ܡ��ɿ�-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800270,'����ҵ���ܡ���ҩѧ��-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800271,'����ҵ���ܡ���Ƥ��-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800272,'������ܡ����㣨-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800273,'������ܡ����ȣ�-500ԭ������');
insert into custom_texts(entry, content_default)values(-2800279,'ϵͳ��ʾ����ҵ�������ѧϰ2����');
*/

void hearthstone_prepareLearn(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535�ǲ����ڵ�menuid�����ݿ���Ŀǰ���Ϊ50101 �رղ��ǹؼ���Ԥ����Ż����㡣

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800230, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 114);  // ѧϰרҵ����
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800231, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 115);  // ѧϰְҵ����

	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
}
void hearthstone_prepareProfessional(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535�ǲ����ڵ�menuid�����ݿ���Ŀǰ���Ϊ50101 �رղ��ǹؼ���Ԥ����Ż����㡣
	
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800260, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 130);  // ����
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800261, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 131);  // �鱦
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800262, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 132);  // ����ѧ
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800263, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 133);  // ��Ƥ
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800264, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 134);  // ����
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800265, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 135);  // ������
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800266, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 136);  // �÷�
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800267, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 137);  // ��ħ
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800268, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 138);  // ���
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800269, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 139);  // �ɿ�
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800270, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 140);  // ҩѧ
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800271, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 141);  // ��Ƥ
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800272, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 142);  // ����
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800273, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 143);  // ����
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
}

bool hearthstone_learn_click(Player* pPlayer, Item* pItem, uint32 uiAction){

	switch (uiAction)
	{
	case GOSSIP_ACTION_INFO_DEF + 114:hearthstone_prepareProfessional(pPlayer, pItem); return true;
	case GOSSIP_ACTION_INFO_DEF + 115:learn_default_spell(pPlayer, pItem, uiAction); break;
	case GOSSIP_ACTION_INFO_DEF + 130:learn_professional(pPlayer, 164, 2); break;//����,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 131:learn_professional(pPlayer, 755, 2); break;//�鱦,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 132:learn_professional(pPlayer, 202, 2); break;//����ѧ,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 133:learn_professional(pPlayer, 165, 2); break;//��Ƥ,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 134:learn_professional(pPlayer, 773, 2); break;//����,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 135:learn_professional(pPlayer, 171, 2); break;//������,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 136:learn_professional(pPlayer, 197, 2); break;//�÷�,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 137:learn_professional(pPlayer, 333, 2); break;//��ħ,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 138:learn_professional(pPlayer, 185, 3); break;//���,��������������������ѧϰ
	case GOSSIP_ACTION_INFO_DEF + 139:learn_professional(pPlayer, 186, 2); break;//�ɿ�,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 140:learn_professional(pPlayer, 182, 2); break;//��ҩѧ,��ҵ����ֻ��ѧϰ����
	case GOSSIP_ACTION_INFO_DEF + 141:learn_professional(pPlayer, 393, 2); break;//��Ƥ,��ҵ����ֻ��ѧϰ���� 
	case GOSSIP_ACTION_INFO_DEF + 142:learn_professional(pPlayer, 356, 3); break; //����,��������������������ѧϰ
	case GOSSIP_ACTION_INFO_DEF + 143:learn_professional(pPlayer, 129, 3); break; //����,��������������������ѧϰ
	}

	pPlayer->gossipMenuType = -1;/*Ĭ��ֵ*/
	return true;
}
bool learn_professional(Player* pPlayer, uint32 skillid, uint8 maxnum)//.learnsk 165 300 // ��Ƥ  
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
				ChatHandler(pPlayer).SendSysMessage(-2800279);//ϵͳ��ʾ����ҵ�������ѧϰ2����
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