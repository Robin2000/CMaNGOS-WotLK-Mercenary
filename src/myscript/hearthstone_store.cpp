#include "precompiled.h"
#include "hearthstone_store.h"
#include "MercenaryMgr.h"

/*
delete from custom_texts where entry>=-2800547 and entry<=-2800400;

insert into custom_texts(entry, content_default)values(-2800400,  'T1����(սʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800401,  'T2��ŭ(սʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800402,  'T3��η(սʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800403,  'T4ս��(սʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800404,  'T4ս��(սʿ70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800405,  'T5�ݻ���(սʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800406,  'T5�ݻ���(սʿ70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800407,  'T6���(սʿ70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800408,  'T6���(սʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800409,  'T7��װ(սʿ80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800410,  'T7��װ(սʿ80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800411,  'T8��װ(սʿ80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800412,  'T8��װ(սʿ80������,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800420,  'T1����֮Դ(ʥ��ʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800421,  'T2����(ʥ��ʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800422,  'T3����(ʥ��ʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800423,  'T4�ӻ�(ʥ��ʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800424,  'T4�ӻ�(ʥ��ʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800425,  'T4�ӻ�(ʥ��ʿ70���ͽ�,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800426,  'T5����(ʥ��ʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800427,  'T5����(ʥ��ʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800428,  'T5����(ʥ��ʿ70���ͽ�,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800429,  'T6����ʹ��(ʥ��ʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800430,  'T6����ʹ��(ʥ��ʿ70���ͽ�,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800431,  'T6����ʹ��(ʥ��ʿ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800432,  'T7��װ��ʥ��ʿ80���ͽ�,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800433,  'T7��װ��ʥ��ʿ80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800434,  'T7��װ��ʥ��ʿ80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800435,  'T8��װ��ʥ��ʿ80���ͽ�,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800436,  'T8��װ��ʥ��ʿ80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800437,  'T8��װ(ʥ��ʿ80������,-500ԭ��)');


insert into custom_texts(entry, content_default)values(-2800440,  'T1����֮��(����60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800441,  'T2Ԧ����(����60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800442,  'T3��Ѩ׷����(����60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800443,  'T4��ħ׷����(����70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800444,  'T5�ѷ�����(����70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800445,  'T6��¡׷����(����70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800446,  'T7��װ(����80��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800447,  'T8��װ(����80��,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800450,  'T1ҹĻɱ��(Ǳ����60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800451,  'T2Ѫ��(Ǳ����60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800452,  'T3����(Ǳ����60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800453,  'T4���֮��(Ǳ����70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800454,  'T5���񴫳�(Ǳ����70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800455,  'T6��ɱ��(Ǳ����70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800456,  'T7��װ (Ǳ����80��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800457,  'T8��װ(Ǳ����80��,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800460,  'T1Ԥ��(��ʦ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800461,  'T2׿Խ(��ʦ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800462,  'T3����(��ʦ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800463,  'T4����(��ʦ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800464,  'T4����(��ʦ70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800465,  'T5����(��ʦ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800466,  'T5����(��ʦ70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800467,  'T6����(��ʦ70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800468,  'T6����(��ʦ70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800469,  'T7��װ(��ʦ80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800470,  'T7��װ(��ʦ80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800471,  'T8��װ(��ʦ80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800472,  'T8��װ(��ʦ80������,-500ԭ��)');


insert into custom_texts(entry, content_default)values(-2800480,  'T7��װ(������ʿ80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800481,  'T7��װ(������ʿ80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800482,  'T8��װ(������ʿ80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800483,  'T8��װ(������ʿ80������,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800490,  'T1���֮ŭ(������˾60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800491,  'T2�޾��籩(������˾60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800492,  'T3�����(������˾60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800493,  'T4쫷�(������˾70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800494,  'T4쫷�(������˾70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800495,  'T4쫷�(������˾70��ս��,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800496,  'T5�ѵ�(������˾70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800497,  'T5�ѵ�(������˾70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800498,  'T6�ѵ�(������˾70��ս��,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800499,  'T6����(������˾70��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800500,  'T6����(������˾70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800501,  'T6����(������˾70������,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800502,  'T7��װ(������˾80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800503,  'T7��װ(������˾80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800504,  'T7��װ(������˾80������,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800505,  'T8��װ(������˾80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800506,  'T8��װ(������˾80��ս��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800507,  'T8��װ(������˾80������,-500ԭ��)');


insert into custom_texts(entry, content_default)values(-2800510,  'T1����ʦ(��ʦ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800511,  'T2���(��ʦ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800512,  'T3˪��(��ʦ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800513,  'T4ռ����(��ʦ70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800514,  'T5����˹��(��ʦ70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800515,  'T6�籩(��ʦ70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800516,  'T7��װ(��ʦ80��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800517,  'T8��װ(��ʦ80��,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800520,  'T1��ħ֮�� (��ʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800521,  'T2����(��ʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800522,  'T3����֮��(��ʿ60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800523,  'T4���֮��(��ʿ70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800524,  'T5����(��ʿ70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800525,  'T6����(��ʿ70��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800526,  'T7��װ(��ʿ80��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800527,  'T8��װ(��ʿ80��,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800530,  'T1�������(��³��60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800531,  'T2ŭ��(��³��60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800532,  'T3������(��³��60��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800533,  'T4������(��³��70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800534,  'T4������(��³��70��ƽ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800535,  'T4������(��³��70��Ұ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800536,  'T5ŵ��ϣ��(��³��70��Ұ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800537,  'T5ŵ��ϣ��(��³��70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800538,  'T5ŵ��ϣ��(��³��70��ƽ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800539,  'T6����֮��(��³��70��Ұ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800540,  'T6����֮��(��³��70��ƽ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800541,  'T6����֮��(��³��70������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800542,  'T7��װ(��³��80��Ұ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800543,  'T7��װ(��³��80������,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800544,  'T7��װ(��³��80��ƽ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800545,  'T8��װ(��³��80��ƽ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800546,  'T8��װ(��³��80��Ұ��,-500ԭ��)');
insert into custom_texts(entry, content_default)values(-2800547,  'T8��װ(��³��80������,-500ԭ��)');

insert into custom_texts(entry, content_default)values(-2800550,  '36�񱳰�(-5ԭ��)');
*/
	/*ԭ���̵�*/
	void hearthstone_prepare_store(Player* pPlayer, Item* pItem){
		pPlayer->PrepareGossipMenu(pPlayer, 65535);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, -2800550, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);  // "36�񱳰�(-5ԭ��)"
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "�ռ�����(-500ԭ��)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);  // �ռ�����(-500ԭ��)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, -2800240, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 119);//T1-T8��װ
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	}

	void add_itemsetByClass(Player* pPlayer, Item* pItem, uint8 playerOrPetClass){
		switch (playerOrPetClass)
		{
		case 1://սʿ
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800400, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 180);  // T1����(60��) 209 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800401, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 181);  // T2��ŭ(60��)  218 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800402, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 182);  // T3��η(60��) 523 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800403, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 183);  // T4ս��(70������) 654 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800404, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 184);  // T4ս��(70��ս��) 655 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800405, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 185);  // T5�ݻ��ߣ�70��������656 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800406, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 186);  // T5�ݻ��ߣ�70��ս����657 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800407, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 187);  // T6���(70��ս����672 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800408, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 188);  // T6���(70��������673
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800409, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 189);  // T7��װ(80������) 787
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800410, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 190);  // T7��װ(80��ս����788
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800411, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 191);  // T8��װ(80��ս���� 830
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800412, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 192);  // T8��װ(80������) 831
			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800418, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 195);  // ѧϰ���.learn 750
			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800419, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1959);  // ѧϰ����.learn 8737
			break;
		case 2://ʥ��ʿ
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800420, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 200);  // T1����֮Դ(60��) 208
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800421, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 201);  // T2����(60��)  217 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800422, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 202);  // T3����(60��) 528
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800423, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 203);  // T4�ӻ���70�����ƣ�624 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800424, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 204);  // T4�ӻ���70��������625
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800425, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 205);  // T4�ӻ���70���ͽ䣩626  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800426, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 206);  // T5������70�����ƣ�627 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800427, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 207);  // T5������70��������628 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800428, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 208);  // T5������70���ͽ䣩 629 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800429, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 209);  // T6����ʹ�ߣ�70�������� 679 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800430, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 210);  // T6����ʹ�ߣ�70�ͽ䣩680 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800431, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 211);  // T6����ʹ�ߣ�70���ƣ�681 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800432, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 212);  // T7��װ��80���ͽ䣩 789
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800433, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 213);  // T7��װ��80�����ƣ� 790 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800434, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 214);  // T7��װ��80�������� 791
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800435, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 215);  // T8��װ��80���ͽ䣩 820
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800436, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 216);  // T8��װ��80�������� 821

			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800418, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 195);  // ѧϰ���.learn 750
			break;
		case 3://����
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800440, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 220);  // T1����֮��(60��) 206 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800441, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 221);  // T2Ԧ����(60��)  215  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800442, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 222);  // T3��Ѩ׷����(60��) 530 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800443, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 223);  // T4��ħ׷���ߣ�70����651  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800444, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 224);  // T5�ѷ����ߣ�70����652 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800445, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 225);  // T6��¡׷���ߣ�70����669   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800446, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 226);  // T7��װ ��80����794 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800447, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 227);  // T8��װ��80����838 
			//����
			break;
		case 4://Ǳ����
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800450, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 230);  // T1ҹĻɱ��(60��) 204  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800451, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 231);  // T2Ѫ��(60��)  213   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800452, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 232);  // T3����(60��) 524  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800453, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 233);  // T4���֮�У�70����621  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800454, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 234);  // T5���񴫳У�70���� 622
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800455, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 235);  // T6��ɱ�ߣ�70����668  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800456, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 236);  // T7��װ ��80����801  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800457, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 237);  // T8��װ��80����826
			break;
		case 5://��ʦ
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800460, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 240);  // T1Ԥ��(60��) 202  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800461, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 241);  // T2׿Խ(60��)  211    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800462, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 242);  // T3����(60��) 525   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800463, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 243);  // T4����70�����ƣ�663  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800464, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 244);  // T4����70��ս����664   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800465, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 245);  // T5����70�����ƣ� 665 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800466, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 246);  // T5����70��ս���� 666  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800467, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 247);  // T6���⣨70��ս����674   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800468, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 248);  // T6���⣨70�����ƣ�675   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800469, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 249);  // T7��װ ��80�����ƣ�804  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800470, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 250);  // T7��װ ��80��ս����805
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800471, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 251);  // T8��װ��80��ս����832
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800472, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 252);  // T8��װ��80�����ƣ�833
			break;
		case 6://������ʿ
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800480, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 260);  // T7��װ ��80��ս����792  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800481, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 261);  // T7��װ ��80��������793 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800482, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 262);  // T8��װ��80��ս����834
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800483, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 263);  // T8��װ��80��������835 
			//���
			break;
		case 7://������˾
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800490, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 264);  // T1���֮ŭ(60��) 207  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800492, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 265);  // T2�޾��籩(60��)  216     
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800493, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 266);  // T3�����(60��) 527    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800493, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 267);  // T4쫷磨70�����ƣ�631   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800494, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 268);  // T4쫷磨70��ս����632    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800495, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 269);  // T4쫷磨70��ս����633    

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800496, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 270);  // T5�ѵأ�70�����ƣ� 634  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800497, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 271);  // T5�ѵأ�70��ս���� 635   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800498, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 272);  // T6�ѵأ�70��ս����636    

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800499, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 273);  // T6���죨70��ս����682    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800500, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 274);  // T6���죨70�����ƣ�683   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800501, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 275);  // T6���죨70�����ƣ�684   

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800502, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 276);  // T7��װ ��80��ս����795  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800503, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 277);  // T7��װ ��80��ս����796  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800504, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 278);  // T7��װ ��80�����ƣ�797 

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800505, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 279);  // T8��װ��80��ս����823
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800506, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 280);  // T8��װ��80��ս����824
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800507, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 281);  // T8��װ��80�����ƣ�825

			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800418, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 195);  // ѧϰ����.learn 8737
			break;
		case 8://��ʦ
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800510, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 290);  // T1����ʦ (60��) 201   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800511, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 291);  // T2���(60��)  210    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800512, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 292);  // T3˪��(60��) 526   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800513, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 293);  // T4ռ���ߣ�70����648   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800514, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 294);  // T5����˹����70���� 649 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800515, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 295);  // T6�籩��70����671   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800516, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 296);  // T7��װ ��80����803  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800517, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 297);  // T8��װ��80����836
			break;
		case 9://��ʿ
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800520, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 300);  // T1��ħ֮�� (60��) 203    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800521, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 301);  // T2����(60��)  212     
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800522, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 302);  // T3����֮��(60��) 529    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800523, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 303);  // T4���֮�ģ�70����645    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800524, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 304);  // T5���䣨70���� 646  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800525, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 305);  // T6���ǣ�70����670    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800526, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 306);  // T7��װ��80����802  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800527, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 307);  // T8��װ��80����837
			break;
		case 11://��³��
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800530, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);  // T1�������(60��) 205
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800531, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 311);  // T2ŭ��(60��)  214  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800532, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 312);  // T3������(60��) 521 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800533, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 313);  // T4�����ᣨ70�����ƣ�638  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800534, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 314);  // T4�����ᣨ70��ƽ�⣩639 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800535, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 315);  // T4�����ᣨ70��Ұ�ԣ�640   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800536, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 316);  // T5ŵ��ϣ����70��Ұ�ԣ�641  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800537, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 317);  // T5ŵ��ϣ����70�����ƣ�642  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800538, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 318);  // T5ŵ��ϣ����70��ƽ�⣩ 643  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800539, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 319);  // T6����֮�ģ�70��Ұ�ԣ� 676  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800540, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 320);  // T6����֮�ģ�70��ƽ�⣩677  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800541, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 321);  // T6����֮�ģ�70�����ƣ�678  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800542, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 322);  // T7��װ��80��Ұ�ԣ�798
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800543, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 323);  // T7��װ��80�����ƣ� 799
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800544, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 324);  // T7��װ��80��ƽ�⣩ 800
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800545, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 325);  // T8��װ��80��ƽ�⣩ 827
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800546, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 326);  // T8��װ��80��Ұ�ԣ� 828
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800547, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 327);  // T8��װ��80�����ƣ� 829
			break;
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//�������˵�
	}
	void hearthstone_prepare_itemset(Player* pPlayer, Item* pItem, uint32 uiAction){
		pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535�ǲ����ڵ�menuid�����ݿ���Ŀǰ���Ϊ50101 �رղ��ǹؼ���Ԥ����Ż����㡣
		uint8 playerOrPetClass = pPlayer->getClass();
		add_itemsetByClass(pPlayer, pItem, playerOrPetClass);

		Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(pPlayer->GetGUIDLow());

		if (mercenary)
		{
			if (mercenary->GetType() != playerOrPetClass)
				add_itemsetByClass(pPlayer, pItem, mercenary->GetType());
		}

		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ

	}
	bool hearthstone_store_click(Player* pPlayer, Item* pItem, uint32 uiAction){

		if (uiAction == GOSSIP_ACTION_INFO_DEF + 11){//36���ӱ���
				if (!pPlayer->gamePointMgr.checkPoint(5)) 	return false;
				ChatHandler(pPlayer).HandleAddItemCommand("23162");
				pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_STORE_POCKET, 5);
				pPlayer->gossipMenuType = -1;/*��ԭĬ��ֵ*/
				return true;
		}
		if (uiAction == GOSSIP_ACTION_INFO_DEF + 119){//T1-T8
			hearthstone_prepare_itemset( pPlayer, pItem, uiAction);
			return true;
		}

		switch (uiAction){
		case GOSSIP_ACTION_INFO_DEF + 180:return additemset(pPlayer, 209);  // T1����(60��) 209 
		case GOSSIP_ACTION_INFO_DEF + 181:return additemset(pPlayer, 218);  // T2��ŭ(60��)  218 
		case GOSSIP_ACTION_INFO_DEF + 182:return additemset(pPlayer, 523);  // T3��η(60��) 523 
		case GOSSIP_ACTION_INFO_DEF + 183:return additemset(pPlayer, 654);  // T4ս��(70������) 654 
		case GOSSIP_ACTION_INFO_DEF + 184:return additemset(pPlayer, 655);  // T4ս��(70��ս��) 655 
		case GOSSIP_ACTION_INFO_DEF + 185:return additemset(pPlayer, 656);  // T5�ݻ��ߣ�70��������656 
		case GOSSIP_ACTION_INFO_DEF + 186:return additemset(pPlayer, 657);  // T5�ݻ��ߣ�70��ս����657 
		case GOSSIP_ACTION_INFO_DEF + 187:return additemset(pPlayer, 672);  // T6���(70��ս����672 
		case GOSSIP_ACTION_INFO_DEF + 188:return additemset(pPlayer, 673);  // T6���(70��������673
		case GOSSIP_ACTION_INFO_DEF + 189:return additemset(pPlayer, 787);  // T7��װ(80������) 787
		case GOSSIP_ACTION_INFO_DEF + 190:return additemset(pPlayer, 788);  // T7��װ(80��ս����788
		case GOSSIP_ACTION_INFO_DEF + 191:return additemset(pPlayer, 830);  // T8��װ(80��ս���� 830
		case GOSSIP_ACTION_INFO_DEF + 192:return additemset(pPlayer, 831);  // T8��װ(80������) 831

		case GOSSIP_ACTION_INFO_DEF + 200:return additemset(pPlayer, 208);  // T1����֮Դ(60��) 208
		case GOSSIP_ACTION_INFO_DEF + 201:return additemset(pPlayer, 217);  // T2����(60��)  217 
		case GOSSIP_ACTION_INFO_DEF + 202:return additemset(pPlayer, 528);  // T3����(60��) 528
		case GOSSIP_ACTION_INFO_DEF + 203:return additemset(pPlayer, 624);  // T4�ӻ���70�����ƣ�624 
		case GOSSIP_ACTION_INFO_DEF + 204:return additemset(pPlayer, 625);  // T4�ӻ���70��������625
		case GOSSIP_ACTION_INFO_DEF + 205:return additemset(pPlayer, 626);  // T4�ӻ���70���ͽ䣩626  
		case GOSSIP_ACTION_INFO_DEF + 206:return additemset(pPlayer, 627);  // T5������70�����ƣ�627 
		case GOSSIP_ACTION_INFO_DEF + 207:return additemset(pPlayer, 628);  // T5������70��������628 
		case GOSSIP_ACTION_INFO_DEF + 208:return additemset(pPlayer, 629);  // T5������70���ͽ䣩 629 
		case GOSSIP_ACTION_INFO_DEF + 209:return additemset(pPlayer, 679);  // T6����ʹ�ߣ�70��������679  
		case GOSSIP_ACTION_INFO_DEF + 210:return additemset(pPlayer, 680);  // T6����ʹ�ߣ��ͽ䣩680 
		case GOSSIP_ACTION_INFO_DEF + 211:return additemset(pPlayer, 681);  // T6����ʹ�ߣ����ƣ�681 
		case GOSSIP_ACTION_INFO_DEF + 212:return additemset(pPlayer, 789);  // T7��װ 789
		case GOSSIP_ACTION_INFO_DEF + 213:return additemset(pPlayer, 790);  // T7��װ 790 
		case GOSSIP_ACTION_INFO_DEF + 214:return additemset(pPlayer, 791);  // T7��װ 791
		case GOSSIP_ACTION_INFO_DEF + 215:return additemset(pPlayer, 820);  // T8��װ 820
		case GOSSIP_ACTION_INFO_DEF + 216:return additemset(pPlayer, 821);  // T8��װ 821
		case GOSSIP_ACTION_INFO_DEF + 217:return additemset(pPlayer, 823);  // T8��װ 823

		case GOSSIP_ACTION_INFO_DEF + 220:return additemset(pPlayer, 206);  // T1����֮��(60��) 206 
		case GOSSIP_ACTION_INFO_DEF + 221:return additemset(pPlayer, 215);  // T2Ԧ����(60��)  215  
		case GOSSIP_ACTION_INFO_DEF + 222:return additemset(pPlayer, 530);  // T3��Ѩ׷����(60��) 530 
		case GOSSIP_ACTION_INFO_DEF + 223:return additemset(pPlayer, 651);  // T4��ħ׷���ߣ�70����651  
		case GOSSIP_ACTION_INFO_DEF + 224:return additemset(pPlayer, 652);  // T5�ѷ����ߣ�70����652 
		case GOSSIP_ACTION_INFO_DEF + 225:return additemset(pPlayer, 669);  // T6��¡׷���ߣ�70����669   
		case GOSSIP_ACTION_INFO_DEF + 226:return additemset(pPlayer, 794);  // T7��װ ��80����794 
		case GOSSIP_ACTION_INFO_DEF + 227:return additemset(pPlayer, 838);  // T8��װ��80����838 

		case GOSSIP_ACTION_INFO_DEF + 230:return additemset(pPlayer, 204);  // T1ҹĻɱ��(60��) 204  
		case GOSSIP_ACTION_INFO_DEF + 231:return additemset(pPlayer, 213);  // T2Ѫ��(60��)  213   
		case GOSSIP_ACTION_INFO_DEF + 232:return additemset(pPlayer, 524);  // T3����(60��) 524  
		case GOSSIP_ACTION_INFO_DEF + 233:return additemset(pPlayer, 621);  // T4���֮�У�70����621  
		case GOSSIP_ACTION_INFO_DEF + 234:return additemset(pPlayer, 622);  // T5���񴫳У�70���� 622
		case GOSSIP_ACTION_INFO_DEF + 235:return additemset(pPlayer, 668);  // T6��ɱ�ߣ�70����668  
		case GOSSIP_ACTION_INFO_DEF + 236:return additemset(pPlayer, 801);  // T7��װ ��80����801  
		case GOSSIP_ACTION_INFO_DEF + 237:return additemset(pPlayer, 826);  // T8��װ��80����826
		case GOSSIP_ACTION_INFO_DEF + 240:return additemset(pPlayer, 202);  // T1Ԥ��(60��) 202  
		case GOSSIP_ACTION_INFO_DEF + 241:return additemset(pPlayer, 211);  // T2׿Խ(60��)  211    
		case GOSSIP_ACTION_INFO_DEF + 242:return additemset(pPlayer, 525);  // T3����(60��) 525   
		case GOSSIP_ACTION_INFO_DEF + 243:return additemset(pPlayer, 663);  // T4����70�����ƣ�663  
		case GOSSIP_ACTION_INFO_DEF + 244:return additemset(pPlayer, 664);  // T4����70��ս����664   
		case GOSSIP_ACTION_INFO_DEF + 245:return additemset(pPlayer, 665);  // T5����70�����ƣ� 665 
		case GOSSIP_ACTION_INFO_DEF + 246:return additemset(pPlayer, 666);  // T5����70��ս���� 666  
		case GOSSIP_ACTION_INFO_DEF + 247:return additemset(pPlayer, 674);  // T6���⣨70��ս����674   
		case GOSSIP_ACTION_INFO_DEF + 248:return additemset(pPlayer, 675);  // T6���⣨70�����ƣ�675   
		case GOSSIP_ACTION_INFO_DEF + 249:return additemset(pPlayer, 804);  // T7��װ ��80����804  
		case GOSSIP_ACTION_INFO_DEF + 250:return additemset(pPlayer, 805);  // T7��װ ��80����805
		case GOSSIP_ACTION_INFO_DEF + 251:return additemset(pPlayer, 832);  // T8��װ��80����832
		case GOSSIP_ACTION_INFO_DEF + 252:return additemset(pPlayer, 833);  // T8��װ��80����833
		case GOSSIP_ACTION_INFO_DEF + 260:return additemset(pPlayer, 792);  // T7��װ ��80����792  
		case GOSSIP_ACTION_INFO_DEF + 261:return additemset(pPlayer, 793);  // T7��װ ��80����793 
		case GOSSIP_ACTION_INFO_DEF + 262:return additemset(pPlayer, 834);  // T8��װ��80����834
		case GOSSIP_ACTION_INFO_DEF + 263:return additemset(pPlayer, 835);  // T8��װ��80����835 
		case GOSSIP_ACTION_INFO_DEF + 264:return additemset(pPlayer, 207);  // T1���֮ŭ(60��) 207  
		case GOSSIP_ACTION_INFO_DEF + 265:return additemset(pPlayer, 216);  // T2�޾��籩(60��)  216     
		case GOSSIP_ACTION_INFO_DEF + 266:return additemset(pPlayer, 527);  // T3�����(60��) 527    
		case GOSSIP_ACTION_INFO_DEF + 267:return additemset(pPlayer, 631);  // T4쫷磨70�����ƣ�631   
		case GOSSIP_ACTION_INFO_DEF + 268:return additemset(pPlayer, 632);  // T4쫷磨70��ս����632    
		case GOSSIP_ACTION_INFO_DEF + 269:return additemset(pPlayer, 633);  // T4쫷磨70��ս����633    

		case GOSSIP_ACTION_INFO_DEF + 270:return additemset(pPlayer, 634);  // T5�ѵأ�70�����ƣ� 634  
		case GOSSIP_ACTION_INFO_DEF + 271:return additemset(pPlayer, 635);  // T5�ѵأ�70��ս���� 635   
		case GOSSIP_ACTION_INFO_DEF + 272:return additemset(pPlayer, 636);  // T6�ѵأ�70��ս����636    

		case GOSSIP_ACTION_INFO_DEF + 273:return additemset(pPlayer, 682);  // T6���죨70�����ƣ�682    
		case GOSSIP_ACTION_INFO_DEF + 274:return additemset(pPlayer, 683);  // T6���죨70�����ƣ�683   
		case GOSSIP_ACTION_INFO_DEF + 275:return additemset(pPlayer, 684);  // T6���죨70�����ƣ�684   

		case GOSSIP_ACTION_INFO_DEF + 276:return additemset(pPlayer, 795);  // T7��װ ��80����795  
		case GOSSIP_ACTION_INFO_DEF + 277:return additemset(pPlayer, 796);  // T7��װ ��80����796  
		case GOSSIP_ACTION_INFO_DEF + 278:return additemset(pPlayer, 797);  // T7��װ ��80����797 

		case GOSSIP_ACTION_INFO_DEF + 279:return additemset(pPlayer, 823);  // T8��װ��80����823
		case GOSSIP_ACTION_INFO_DEF + 280:return additemset(pPlayer, 824);  // T8��װ��80����824
		case GOSSIP_ACTION_INFO_DEF + 281:return additemset(pPlayer, 825);  // T8��װ��80����825

		case GOSSIP_ACTION_INFO_DEF + 290:return additemset(pPlayer, 201);  // T1����ʦ (60��) 201   
		case GOSSIP_ACTION_INFO_DEF + 291:return additemset(pPlayer, 210);  // T2���(60��)  210    
		case GOSSIP_ACTION_INFO_DEF + 292:return additemset(pPlayer, 526);  // T3˪��(60��) 526   
		case GOSSIP_ACTION_INFO_DEF + 293:return additemset(pPlayer, 648);  // T4ռ���ߣ�70����648   
		case GOSSIP_ACTION_INFO_DEF + 294:return additemset(pPlayer, 649);  // T5����˹����70���� 649 
		case GOSSIP_ACTION_INFO_DEF + 295:return additemset(pPlayer, 671);  // T6�籩��70����671   
		case GOSSIP_ACTION_INFO_DEF + 296:return additemset(pPlayer, 803);  // T7��װ ��80����803  
		case GOSSIP_ACTION_INFO_DEF + 297:return additemset(pPlayer, 836);  // T8��װ��80����836

		case GOSSIP_ACTION_INFO_DEF + 300:return additemset(pPlayer, 203);  // T1��ħ֮�� (60��) 203    
		case GOSSIP_ACTION_INFO_DEF + 301:return additemset(pPlayer, 212);  // T2����(60��)  212     
		case GOSSIP_ACTION_INFO_DEF + 302:return additemset(pPlayer, 529);  // T3����֮��(60��) 529    
		case GOSSIP_ACTION_INFO_DEF + 303:return additemset(pPlayer, 645);  // T4���֮�ģ�70����645    
		case GOSSIP_ACTION_INFO_DEF + 304:return additemset(pPlayer, 646);  // T5���䣨70���� 646  
		case GOSSIP_ACTION_INFO_DEF + 305:return additemset(pPlayer, 670);  // T6���ǣ�70����670    
		case GOSSIP_ACTION_INFO_DEF + 306:return additemset(pPlayer, 802);  // T7��װ��80����802  
		case GOSSIP_ACTION_INFO_DEF + 307:return additemset(pPlayer, 837);  // T8��װ��80����837

		case GOSSIP_ACTION_INFO_DEF + 310:return additemset(pPlayer, 205);  // T1�������(60��) 205
		case GOSSIP_ACTION_INFO_DEF + 311:return additemset(pPlayer, 214);  // T2ŭ��(60��)  214  
		case GOSSIP_ACTION_INFO_DEF + 312:return additemset(pPlayer, 521);  // T3������(60��) 521 
		case GOSSIP_ACTION_INFO_DEF + 313:return additemset(pPlayer, 638);  // T4�����ᣨ70�����ƣ�638  
		case GOSSIP_ACTION_INFO_DEF + 314:return additemset(pPlayer, 639);  // T4�����ᣨ70��ƽ�⣩639 
		case GOSSIP_ACTION_INFO_DEF + 315:return additemset(pPlayer, 640);  // T4�����ᣨ70��Ұ�ԣ�640   
		case GOSSIP_ACTION_INFO_DEF + 316:return additemset(pPlayer, 641);  // T5ŵ��ϣ����70��Ұ�ԣ�641  
		case GOSSIP_ACTION_INFO_DEF + 317:return additemset(pPlayer, 642);  // T5ŵ��ϣ����70�����ƣ�642  
		case GOSSIP_ACTION_INFO_DEF + 318:return additemset(pPlayer, 643);  // T5ŵ��ϣ����70��ƽ�⣩ 643  
		case GOSSIP_ACTION_INFO_DEF + 319:return additemset(pPlayer, 676);  // T6����֮�ģ�70��Ұ�ԣ� 676  
		case GOSSIP_ACTION_INFO_DEF + 320:return additemset(pPlayer, 677);  // T6����֮�ģ�70��ƽ�⣩677  
		case GOSSIP_ACTION_INFO_DEF + 321:return additemset(pPlayer, 678);  // T6����֮�ģ�70�����ƣ�678  
		case GOSSIP_ACTION_INFO_DEF + 322:return additemset(pPlayer, 798);  // T7��װ��80����798
		case GOSSIP_ACTION_INFO_DEF + 323:return additemset(pPlayer, 799);  // T7��װ��80���� 799
		case GOSSIP_ACTION_INFO_DEF + 324:return additemset(pPlayer, 800);  // T7��װ��80���� 800
		case GOSSIP_ACTION_INFO_DEF + 325:return additemset(pPlayer, 827);  // T8��װ��80���� 827
		case GOSSIP_ACTION_INFO_DEF + 326:return additemset(pPlayer, 828);  // T8��װ��80���� 828
		case GOSSIP_ACTION_INFO_DEF + 327:return additemset(pPlayer, 829);  // T8��װ��80���� 829
		};
		return false;
	}

	bool additemset(Player* pPlayer, uint32 itemset){
		if (!pPlayer->gamePointMgr.checkPoint(500))
		{
			pPlayer->gossipMenuType = -1;/*��ԭĬ��ֵ*/
			return false;
		}
		ChatHandler(pPlayer).addItemSet(itemset);
		switch (pPlayer->getClass()){
		case 1:pPlayer->learnSpell(750, true); break;//սʿ ѧϰ���.learn 750
		case 2:pPlayer->learnSpell(750, true); break;//ʥ��ʿ // ѧϰ���.learn 750
		case 3:pPlayer->learnSpell(8737, true); break;//���� //����
		case 6:pPlayer->learnSpell(750, true); break;//������ʿ //���
		case 7:pPlayer->learnSpell(8737, true); break;//������˾ ѧϰ����.learn 8737
		}
		Pet * pet = pPlayer->GetPet();
		if (pet){
			switch (pet->getClass()){
			case 1:pet->learnSpell(750); break;//սʿ ѧϰ���.learn 750
			case 2:pet->learnSpell(750); break;//ʥ��ʿ // ѧϰ���.learn 750
			case 3:pet->learnSpell(8737); break;//���� //����
			case 6:pet->learnSpell(750); break;//������ʿ //���
			case 7:pet->learnSpell(8737); break;//������˾ ѧϰ����.learn 8737
			}
		}

		pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_ITEMSET, 500);

		pPlayer->gossipMenuType = -1;/*��ԭĬ��ֵ*/
		return true;
	}