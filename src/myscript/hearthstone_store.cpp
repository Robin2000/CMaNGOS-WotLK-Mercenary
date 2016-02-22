#include "precompiled.h"
#include "hearthstone_store.h"
#include "MercenaryMgr.h"

/*
delete from custom_texts where entry>=-2800547 and entry<=-2800400;

insert into custom_texts(entry, content_default)values(-2800400,  'T1力量(战士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800401,  'T2愤怒(战士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800402,  'T3无畏(战士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800403,  'T4战神(战士70级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800404,  'T4战神(战士70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800405,  'T5摧毁者(战士70级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800406,  'T5摧毁者(战士70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800407,  'T6冲击(战士70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800408,  'T6冲击(战士70级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800409,  'T7套装(战士80级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800410,  'T7套装(战士80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800411,  'T8套装(战士80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800412,  'T8套装(战士80级防御,-500原力)');

insert into custom_texts(entry, content_default)values(-2800420,  'T1秩序之源(圣骑士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800421,  'T2审判(圣骑士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800422,  'T3救赎(圣骑士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800423,  'T4庇护(圣骑士70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800424,  'T4庇护(圣骑士70级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800425,  'T4庇护(圣骑士70级惩戒,-500原力)');
insert into custom_texts(entry, content_default)values(-2800426,  'T5晶铸(圣骑士70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800427,  'T5晶铸(圣骑士70级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800428,  'T5晶铸(圣骑士70级惩戒,-500原力)');
insert into custom_texts(entry, content_default)values(-2800429,  'T6光明使者(圣骑士70级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800430,  'T6光明使者(圣骑士70级惩戒,-500原力)');
insert into custom_texts(entry, content_default)values(-2800431,  'T6光明使者(圣骑士70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800432,  'T7套装（圣骑士80级惩戒,-500原力)');
insert into custom_texts(entry, content_default)values(-2800433,  'T7套装（圣骑士80级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800434,  'T7套装（圣骑士80级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800435,  'T8套装（圣骑士80级惩戒,-500原力)');
insert into custom_texts(entry, content_default)values(-2800436,  'T8套装（圣骑士80级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800437,  'T8套装(圣骑士80级防御,-500原力)');


insert into custom_texts(entry, content_default)values(-2800440,  'T1巨兽之王(猎人60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800441,  'T2驭龙者(猎人60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800442,  'T3地穴追猎者(猎人60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800443,  'T4恶魔追猎者(猎人70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800444,  'T5裂缝行者(猎人70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800445,  'T6戈隆追猎者(猎人70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800446,  'T7套装(猎人80级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800447,  'T8套装(猎人80级,-500原力)');

insert into custom_texts(entry, content_default)values(-2800450,  'T1夜幕杀手(潜行者60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800451,  'T2血牙(潜行者60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800452,  'T3骨镰(潜行者60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800453,  'T4虚空之刃(潜行者70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800454,  'T5死神传承(潜行者70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800455,  'T6刺杀者(潜行者70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800456,  'T7套装 (潜行者80级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800457,  'T8套装(潜行者80级,-500原力)');

insert into custom_texts(entry, content_default)values(-2800460,  'T1预言(牧师60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800461,  'T2卓越(牧师60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800462,  'T3信仰(牧师60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800463,  'T4化身(牧师70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800464,  'T4化身(牧师70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800465,  'T5幻身(牧师70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800466,  'T5幻身(牧师70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800467,  'T6赦免(牧师70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800468,  'T6赦免(牧师70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800469,  'T7套装(牧师80级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800470,  'T7套装(牧师80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800471,  'T8套装(牧师80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800472,  'T8套装(牧师80级治疗,-500原力)');


insert into custom_texts(entry, content_default)values(-2800480,  'T7套装(死亡骑士80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800481,  'T7套装(死亡骑士80级防御,-500原力)');
insert into custom_texts(entry, content_default)values(-2800482,  'T8套装(死亡骑士80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800483,  'T8套装(死亡骑士80级防御,-500原力)');

insert into custom_texts(entry, content_default)values(-2800490,  'T1大地之怒(萨满祭司60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800491,  'T2无尽风暴(萨满祭司60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800492,  'T3碎地者(萨满祭司60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800493,  'T4飓风(萨满祭司70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800494,  'T4飓风(萨满祭司70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800495,  'T4飓风(萨满祭司70级战斗,-500原力)');

insert into custom_texts(entry, content_default)values(-2800496,  'T5裂地(萨满祭司70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800497,  'T5裂地(萨满祭司70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800498,  'T6裂地(萨满祭司70级战斗,-500原力)');

insert into custom_texts(entry, content_default)values(-2800499,  'T6破天(萨满祭司70级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800500,  'T6破天(萨满祭司70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800501,  'T6破天(萨满祭司70级治疗,-500原力)');

insert into custom_texts(entry, content_default)values(-2800502,  'T7套装(萨满祭司80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800503,  'T7套装(萨满祭司80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800504,  'T7套装(萨满祭司80级治疗,-500原力)');

insert into custom_texts(entry, content_default)values(-2800505,  'T8套装(萨满祭司80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800506,  'T8套装(萨满祭司80级战斗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800507,  'T8套装(萨满祭司80级治疗,-500原力)');


insert into custom_texts(entry, content_default)values(-2800510,  'T1奥术师(法师60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800511,  'T2灵风(法师60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800512,  'T3霜火(法师60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800513,  'T4占卜者(法师70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800514,  'T5提里斯法(法师70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800515,  'T6风暴(法师70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800516,  'T7套装(法师80级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800517,  'T8套装(法师80级,-500原力)');

insert into custom_texts(entry, content_default)values(-2800520,  'T1恶魔之心 (术士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800521,  'T2复仇(术士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800522,  'T3瘟疫之心(术士60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800523,  'T4虚空之心(术士70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800524,  'T5堕落(术士70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800525,  'T6凶星(术士70级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800526,  'T7套装(术士80级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800527,  'T8套装(术士80级,-500原力)');

insert into custom_texts(entry, content_default)values(-2800530,  'T1塞纳里奥(德鲁伊60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800531,  'T2怒风(德鲁伊60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800532,  'T3梦游者(德鲁伊60级,-500原力)');
insert into custom_texts(entry, content_default)values(-2800533,  'T4玛洛尼(德鲁伊70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800534,  'T4玛洛尼(德鲁伊70级平衡,-500原力)');
insert into custom_texts(entry, content_default)values(-2800535,  'T4玛洛尼(德鲁伊70级野性,-500原力)');
insert into custom_texts(entry, content_default)values(-2800536,  'T5诺达希尔(德鲁伊70级野性,-500原力)');
insert into custom_texts(entry, content_default)values(-2800537,  'T5诺达希尔(德鲁伊70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800538,  'T5诺达希尔(德鲁伊70级平衡,-500原力)');
insert into custom_texts(entry, content_default)values(-2800539,  'T6雷霆之心(德鲁伊70级野性,-500原力)');
insert into custom_texts(entry, content_default)values(-2800540,  'T6雷霆之心(德鲁伊70级平衡,-500原力)');
insert into custom_texts(entry, content_default)values(-2800541,  'T6雷霆之心(德鲁伊70级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800542,  'T7套装(德鲁伊80级野性,-500原力)');
insert into custom_texts(entry, content_default)values(-2800543,  'T7套装(德鲁伊80级治疗,-500原力)');
insert into custom_texts(entry, content_default)values(-2800544,  'T7套装(德鲁伊80级平衡,-500原力)');
insert into custom_texts(entry, content_default)values(-2800545,  'T8套装(德鲁伊80级平衡,-500原力)');
insert into custom_texts(entry, content_default)values(-2800546,  'T8套装(德鲁伊80级野性,-500原力)');
insert into custom_texts(entry, content_default)values(-2800547,  'T8套装(德鲁伊80级治疗,-500原力)');

insert into custom_texts(entry, content_default)values(-2800550,  '36格背包(-5原力)');
*/
	/*原力商店*/
	void hearthstone_prepare_store(Player* pPlayer, Item* pItem){
		pPlayer->PrepareGossipMenu(pPlayer, 65535);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, -2800550, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 11);  // "36格背包(-5原力)"
		//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "终极武器(-500原力)", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 12);  // 终极武器(-500原力)
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, -2800240, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 119);//T1-T8套装
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	}

	void add_itemsetByClass(Player* pPlayer, Item* pItem, uint8 playerOrPetClass){
		switch (playerOrPetClass)
		{
		case 1://战士
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800400, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 180);  // T1力量(60级) 209 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800401, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 181);  // T2愤怒(60级)  218 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800402, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 182);  // T3无畏(60级) 523 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800403, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 183);  // T4战神(70级防御) 654 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800404, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 184);  // T4战神(70级战斗) 655 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800405, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 185);  // T5摧毁者（70级防御）656 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800406, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 186);  // T5摧毁者（70级战斗）657 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800407, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 187);  // T6冲击(70级战斗）672 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800408, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 188);  // T6冲击(70级防御）673
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800409, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 189);  // T7套装(80级防御) 787
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800410, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 190);  // T7套装(80级战斗）788
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800411, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 191);  // T8套装(80级战斗） 830
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800412, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 192);  // T8套装(80级防御) 831
			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800418, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 195);  // 学习板甲.learn 750
			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800419, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1959);  // 学习锁甲.learn 8737
			break;
		case 2://圣骑士
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800420, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 200);  // T1秩序之源(60级) 208
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800421, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 201);  // T2审判(60级)  217 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800422, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 202);  // T3救赎(60级) 528
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800423, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 203);  // T4庇护（70级治疗）624 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800424, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 204);  // T4庇护（70级防御）625
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800425, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 205);  // T4庇护（70级惩戒）626  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800426, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 206);  // T5晶铸（70级治疗）627 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800427, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 207);  // T5晶铸（70级防御）628 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800428, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 208);  // T5晶铸（70级惩戒） 629 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800429, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 209);  // T6光明使者（70级防御） 679 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800430, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 210);  // T6光明使者（70惩戒）680 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800431, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 211);  // T6光明使者（70治疗）681 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800432, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 212);  // T7套装（80级惩戒） 789
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800433, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 213);  // T7套装（80级治疗） 790 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800434, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 214);  // T7套装（80级防御） 791
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800435, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 215);  // T8套装（80级惩戒） 820
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800436, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 216);  // T8套装（80级防御） 821

			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800418, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 195);  // 学习板甲.learn 750
			break;
		case 3://猎人
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800440, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 220);  // T1巨兽之王(60级) 206 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800441, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 221);  // T2驭龙者(60级)  215  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800442, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 222);  // T3地穴追猎者(60级) 530 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800443, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 223);  // T4恶魔追猎者（70级）651  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800444, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 224);  // T5裂缝行者（70级）652 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800445, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 225);  // T6戈隆追猎者（70级）669   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800446, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 226);  // T7套装 （80级）794 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800447, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 227);  // T8套装（80级）838 
			//锁甲
			break;
		case 4://潜行者
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800450, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 230);  // T1夜幕杀手(60级) 204  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800451, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 231);  // T2血牙(60级)  213   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800452, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 232);  // T3骨镰(60级) 524  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800453, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 233);  // T4虚空之刃（70级）621  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800454, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 234);  // T5死神传承（70级） 622
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800455, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 235);  // T6刺杀者（70级）668  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800456, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 236);  // T7套装 （80级）801  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800457, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 237);  // T8套装（80级）826
			break;
		case 5://牧师
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800460, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 240);  // T1预言(60级) 202  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800461, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 241);  // T2卓越(60级)  211    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800462, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 242);  // T3信仰(60级) 525   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800463, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 243);  // T4化身（70级治疗）663  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800464, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 244);  // T4化身（70级战斗）664   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800465, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 245);  // T5幻身（70级治疗） 665 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800466, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 246);  // T5幻身（70级战斗） 666  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800467, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 247);  // T6赦免（70级战斗）674   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800468, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 248);  // T6赦免（70级治疗）675   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800469, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 249);  // T7套装 （80级治疗）804  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800470, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 250);  // T7套装 （80级战斗）805
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800471, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 251);  // T8套装（80级战斗）832
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800472, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 252);  // T8套装（80级治疗）833
			break;
		case 6://死亡骑士
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800480, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 260);  // T7套装 （80级战斗）792  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800481, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 261);  // T7套装 （80级防御）793 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800482, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 262);  // T8套装（80级战斗）834
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800483, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 263);  // T8套装（80级防御）835 
			//板甲
			break;
		case 7://萨满祭司
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800490, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 264);  // T1大地之怒(60级) 207  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800492, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 265);  // T2无尽风暴(60级)  216     
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800493, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 266);  // T3碎地者(60级) 527    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800493, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 267);  // T4飓风（70级治疗）631   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800494, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 268);  // T4飓风（70级战斗）632    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800495, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 269);  // T4飓风（70级战斗）633    

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800496, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 270);  // T5裂地（70级治疗） 634  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800497, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 271);  // T5裂地（70级战斗） 635   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800498, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 272);  // T6裂地（70级战斗）636    

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800499, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 273);  // T6破天（70级战斗）682    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800500, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 274);  // T6破天（70级治疗）683   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800501, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 275);  // T6破天（70级治疗）684   

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800502, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 276);  // T7套装 （80级战斗）795  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800503, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 277);  // T7套装 （80级战斗）796  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800504, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 278);  // T7套装 （80级治疗）797 

			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800505, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 279);  // T8套装（80级战斗）823
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800506, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 280);  // T8套装（80级战斗）824
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800507, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 281);  // T8套装（80级治疗）825

			//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800418, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 195);  // 学习锁甲.learn 8737
			break;
		case 8://法师
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800510, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 290);  // T1奥术师 (60级) 201   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800511, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 291);  // T2灵风(60级)  210    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800512, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 292);  // T3霜火(60级) 526   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800513, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 293);  // T4占卜者（70级）648   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800514, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 294);  // T5提里斯法（70级） 649 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800515, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 295);  // T6风暴（70级）671   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800516, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 296);  // T7套装 （80级）803  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800517, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 297);  // T8套装（80级）836
			break;
		case 9://术士
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800520, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 300);  // T1恶魔之心 (60级) 203    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800521, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 301);  // T2复仇(60级)  212     
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800522, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 302);  // T3瘟疫之心(60级) 529    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800523, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 303);  // T4虚空之心（70级）645    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800524, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 304);  // T5堕落（70级） 646  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800525, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 305);  // T6凶星（70级）670    
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800526, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 306);  // T7套装（80级）802  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800527, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 307);  // T8套装（80级）837
			break;
		case 11://德鲁伊
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800530, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 310);  // T1塞纳里奥(60级) 205
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800531, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 311);  // T2怒风(60级)  214  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800532, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 312);  // T3梦游者(60级) 521 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800533, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 313);  // T4玛洛尼（70级治疗）638  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800534, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 314);  // T4玛洛尼（70级平衡）639 
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800535, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 315);  // T4玛洛尼（70级野性）640   
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800536, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 316);  // T5诺达希尔（70级野性）641  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800537, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 317);  // T5诺达希尔（70级治疗）642  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800538, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 318);  // T5诺达希尔（70级平衡） 643  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800539, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 319);  // T6雷霆之心（70级野性） 676  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800540, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 320);  // T6雷霆之心（70级平衡）677  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800541, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 321);  // T6雷霆之心（70级治疗）678  
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800542, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 322);  // T7套装（80级野性）798
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800543, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 323);  // T7套装（80级治疗） 799
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800544, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 324);  // T7套装（80级平衡） 800
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800545, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 325);  // T8套装（80级平衡） 827
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800546, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 326);  // T8套装（80级野性） 828
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800547, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 327);  // T8套装（80级治疗） 829
			break;
		}
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	}
	void hearthstone_prepare_itemset(Player* pPlayer, Item* pItem, uint32 uiAction){
		pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
		uint8 playerOrPetClass = pPlayer->getClass();
		add_itemsetByClass(pPlayer, pItem, playerOrPetClass);

		Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(pPlayer->GetGUIDLow());

		if (mercenary)
		{
			if (mercenary->GetType() != playerOrPetClass)
				add_itemsetByClass(pPlayer, pItem, mercenary->GetType());
		}

		pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。

	}
	bool hearthstone_store_click(Player* pPlayer, Item* pItem, uint32 uiAction){

		if (uiAction == GOSSIP_ACTION_INFO_DEF + 11){//36格子背包
				if (!pPlayer->gamePointMgr.checkPoint(5)) 	return false;
				ChatHandler(pPlayer).HandleAddItemCommand("23162");
				pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_STORE_POCKET, 5);
				pPlayer->gossipMenuType = -1;/*还原默认值*/
				return true;
		}
		if (uiAction == GOSSIP_ACTION_INFO_DEF + 119){//T1-T8
			hearthstone_prepare_itemset( pPlayer, pItem, uiAction);
			return true;
		}

		switch (uiAction){
		case GOSSIP_ACTION_INFO_DEF + 180:return additemset(pPlayer, 209);  // T1力量(60级) 209 
		case GOSSIP_ACTION_INFO_DEF + 181:return additemset(pPlayer, 218);  // T2愤怒(60级)  218 
		case GOSSIP_ACTION_INFO_DEF + 182:return additemset(pPlayer, 523);  // T3无畏(60级) 523 
		case GOSSIP_ACTION_INFO_DEF + 183:return additemset(pPlayer, 654);  // T4战神(70级防御) 654 
		case GOSSIP_ACTION_INFO_DEF + 184:return additemset(pPlayer, 655);  // T4战神(70级战斗) 655 
		case GOSSIP_ACTION_INFO_DEF + 185:return additemset(pPlayer, 656);  // T5摧毁者（70级防御）656 
		case GOSSIP_ACTION_INFO_DEF + 186:return additemset(pPlayer, 657);  // T5摧毁者（70级战斗）657 
		case GOSSIP_ACTION_INFO_DEF + 187:return additemset(pPlayer, 672);  // T6冲击(70级战斗）672 
		case GOSSIP_ACTION_INFO_DEF + 188:return additemset(pPlayer, 673);  // T6冲击(70级防御）673
		case GOSSIP_ACTION_INFO_DEF + 189:return additemset(pPlayer, 787);  // T7套装(80级防御) 787
		case GOSSIP_ACTION_INFO_DEF + 190:return additemset(pPlayer, 788);  // T7套装(80级战斗）788
		case GOSSIP_ACTION_INFO_DEF + 191:return additemset(pPlayer, 830);  // T8套装(80级战斗） 830
		case GOSSIP_ACTION_INFO_DEF + 192:return additemset(pPlayer, 831);  // T8套装(80级防御) 831

		case GOSSIP_ACTION_INFO_DEF + 200:return additemset(pPlayer, 208);  // T1秩序之源(60级) 208
		case GOSSIP_ACTION_INFO_DEF + 201:return additemset(pPlayer, 217);  // T2审判(60级)  217 
		case GOSSIP_ACTION_INFO_DEF + 202:return additemset(pPlayer, 528);  // T3救赎(60级) 528
		case GOSSIP_ACTION_INFO_DEF + 203:return additemset(pPlayer, 624);  // T4庇护（70级治疗）624 
		case GOSSIP_ACTION_INFO_DEF + 204:return additemset(pPlayer, 625);  // T4庇护（70级防御）625
		case GOSSIP_ACTION_INFO_DEF + 205:return additemset(pPlayer, 626);  // T4庇护（70级惩戒）626  
		case GOSSIP_ACTION_INFO_DEF + 206:return additemset(pPlayer, 627);  // T5晶铸（70级治疗）627 
		case GOSSIP_ACTION_INFO_DEF + 207:return additemset(pPlayer, 628);  // T5晶铸（70级防御）628 
		case GOSSIP_ACTION_INFO_DEF + 208:return additemset(pPlayer, 629);  // T5晶铸（70级惩戒） 629 
		case GOSSIP_ACTION_INFO_DEF + 209:return additemset(pPlayer, 679);  // T6光明使者（70级防御）679  
		case GOSSIP_ACTION_INFO_DEF + 210:return additemset(pPlayer, 680);  // T6光明使者（惩戒）680 
		case GOSSIP_ACTION_INFO_DEF + 211:return additemset(pPlayer, 681);  // T6光明使者（治疗）681 
		case GOSSIP_ACTION_INFO_DEF + 212:return additemset(pPlayer, 789);  // T7套装 789
		case GOSSIP_ACTION_INFO_DEF + 213:return additemset(pPlayer, 790);  // T7套装 790 
		case GOSSIP_ACTION_INFO_DEF + 214:return additemset(pPlayer, 791);  // T7套装 791
		case GOSSIP_ACTION_INFO_DEF + 215:return additemset(pPlayer, 820);  // T8套装 820
		case GOSSIP_ACTION_INFO_DEF + 216:return additemset(pPlayer, 821);  // T8套装 821
		case GOSSIP_ACTION_INFO_DEF + 217:return additemset(pPlayer, 823);  // T8套装 823

		case GOSSIP_ACTION_INFO_DEF + 220:return additemset(pPlayer, 206);  // T1巨兽之王(60级) 206 
		case GOSSIP_ACTION_INFO_DEF + 221:return additemset(pPlayer, 215);  // T2驭龙者(60级)  215  
		case GOSSIP_ACTION_INFO_DEF + 222:return additemset(pPlayer, 530);  // T3地穴追猎者(60级) 530 
		case GOSSIP_ACTION_INFO_DEF + 223:return additemset(pPlayer, 651);  // T4恶魔追猎者（70级）651  
		case GOSSIP_ACTION_INFO_DEF + 224:return additemset(pPlayer, 652);  // T5裂缝行者（70级）652 
		case GOSSIP_ACTION_INFO_DEF + 225:return additemset(pPlayer, 669);  // T6戈隆追猎者（70级）669   
		case GOSSIP_ACTION_INFO_DEF + 226:return additemset(pPlayer, 794);  // T7套装 （80级）794 
		case GOSSIP_ACTION_INFO_DEF + 227:return additemset(pPlayer, 838);  // T8套装（80级）838 

		case GOSSIP_ACTION_INFO_DEF + 230:return additemset(pPlayer, 204);  // T1夜幕杀手(60级) 204  
		case GOSSIP_ACTION_INFO_DEF + 231:return additemset(pPlayer, 213);  // T2血牙(60级)  213   
		case GOSSIP_ACTION_INFO_DEF + 232:return additemset(pPlayer, 524);  // T3骨镰(60级) 524  
		case GOSSIP_ACTION_INFO_DEF + 233:return additemset(pPlayer, 621);  // T4虚空之刃（70级）621  
		case GOSSIP_ACTION_INFO_DEF + 234:return additemset(pPlayer, 622);  // T5死神传承（70级） 622
		case GOSSIP_ACTION_INFO_DEF + 235:return additemset(pPlayer, 668);  // T6刺杀者（70级）668  
		case GOSSIP_ACTION_INFO_DEF + 236:return additemset(pPlayer, 801);  // T7套装 （80级）801  
		case GOSSIP_ACTION_INFO_DEF + 237:return additemset(pPlayer, 826);  // T8套装（80级）826
		case GOSSIP_ACTION_INFO_DEF + 240:return additemset(pPlayer, 202);  // T1预言(60级) 202  
		case GOSSIP_ACTION_INFO_DEF + 241:return additemset(pPlayer, 211);  // T2卓越(60级)  211    
		case GOSSIP_ACTION_INFO_DEF + 242:return additemset(pPlayer, 525);  // T3信仰(60级) 525   
		case GOSSIP_ACTION_INFO_DEF + 243:return additemset(pPlayer, 663);  // T4化身（70级治疗）663  
		case GOSSIP_ACTION_INFO_DEF + 244:return additemset(pPlayer, 664);  // T4化身（70级战斗）664   
		case GOSSIP_ACTION_INFO_DEF + 245:return additemset(pPlayer, 665);  // T5幻身（70级治疗） 665 
		case GOSSIP_ACTION_INFO_DEF + 246:return additemset(pPlayer, 666);  // T5幻身（70级战斗） 666  
		case GOSSIP_ACTION_INFO_DEF + 247:return additemset(pPlayer, 674);  // T6赦免（70级战斗）674   
		case GOSSIP_ACTION_INFO_DEF + 248:return additemset(pPlayer, 675);  // T6赦免（70级治疗）675   
		case GOSSIP_ACTION_INFO_DEF + 249:return additemset(pPlayer, 804);  // T7套装 （80级）804  
		case GOSSIP_ACTION_INFO_DEF + 250:return additemset(pPlayer, 805);  // T7套装 （80级）805
		case GOSSIP_ACTION_INFO_DEF + 251:return additemset(pPlayer, 832);  // T8套装（80级）832
		case GOSSIP_ACTION_INFO_DEF + 252:return additemset(pPlayer, 833);  // T8套装（80级）833
		case GOSSIP_ACTION_INFO_DEF + 260:return additemset(pPlayer, 792);  // T7套装 （80级）792  
		case GOSSIP_ACTION_INFO_DEF + 261:return additemset(pPlayer, 793);  // T7套装 （80级）793 
		case GOSSIP_ACTION_INFO_DEF + 262:return additemset(pPlayer, 834);  // T8套装（80级）834
		case GOSSIP_ACTION_INFO_DEF + 263:return additemset(pPlayer, 835);  // T8套装（80级）835 
		case GOSSIP_ACTION_INFO_DEF + 264:return additemset(pPlayer, 207);  // T1大地之怒(60级) 207  
		case GOSSIP_ACTION_INFO_DEF + 265:return additemset(pPlayer, 216);  // T2无尽风暴(60级)  216     
		case GOSSIP_ACTION_INFO_DEF + 266:return additemset(pPlayer, 527);  // T3碎地者(60级) 527    
		case GOSSIP_ACTION_INFO_DEF + 267:return additemset(pPlayer, 631);  // T4飓风（70级治疗）631   
		case GOSSIP_ACTION_INFO_DEF + 268:return additemset(pPlayer, 632);  // T4飓风（70级战斗）632    
		case GOSSIP_ACTION_INFO_DEF + 269:return additemset(pPlayer, 633);  // T4飓风（70级战斗）633    

		case GOSSIP_ACTION_INFO_DEF + 270:return additemset(pPlayer, 634);  // T5裂地（70级治疗） 634  
		case GOSSIP_ACTION_INFO_DEF + 271:return additemset(pPlayer, 635);  // T5裂地（70级战斗） 635   
		case GOSSIP_ACTION_INFO_DEF + 272:return additemset(pPlayer, 636);  // T6裂地（70级战斗）636    

		case GOSSIP_ACTION_INFO_DEF + 273:return additemset(pPlayer, 682);  // T6破天（70级治疗）682    
		case GOSSIP_ACTION_INFO_DEF + 274:return additemset(pPlayer, 683);  // T6破天（70级治疗）683   
		case GOSSIP_ACTION_INFO_DEF + 275:return additemset(pPlayer, 684);  // T6破天（70级治疗）684   

		case GOSSIP_ACTION_INFO_DEF + 276:return additemset(pPlayer, 795);  // T7套装 （80级）795  
		case GOSSIP_ACTION_INFO_DEF + 277:return additemset(pPlayer, 796);  // T7套装 （80级）796  
		case GOSSIP_ACTION_INFO_DEF + 278:return additemset(pPlayer, 797);  // T7套装 （80级）797 

		case GOSSIP_ACTION_INFO_DEF + 279:return additemset(pPlayer, 823);  // T8套装（80级）823
		case GOSSIP_ACTION_INFO_DEF + 280:return additemset(pPlayer, 824);  // T8套装（80级）824
		case GOSSIP_ACTION_INFO_DEF + 281:return additemset(pPlayer, 825);  // T8套装（80级）825

		case GOSSIP_ACTION_INFO_DEF + 290:return additemset(pPlayer, 201);  // T1奥术师 (60级) 201   
		case GOSSIP_ACTION_INFO_DEF + 291:return additemset(pPlayer, 210);  // T2灵风(60级)  210    
		case GOSSIP_ACTION_INFO_DEF + 292:return additemset(pPlayer, 526);  // T3霜火(60级) 526   
		case GOSSIP_ACTION_INFO_DEF + 293:return additemset(pPlayer, 648);  // T4占卜者（70级）648   
		case GOSSIP_ACTION_INFO_DEF + 294:return additemset(pPlayer, 649);  // T5提里斯法（70级） 649 
		case GOSSIP_ACTION_INFO_DEF + 295:return additemset(pPlayer, 671);  // T6风暴（70级）671   
		case GOSSIP_ACTION_INFO_DEF + 296:return additemset(pPlayer, 803);  // T7套装 （80级）803  
		case GOSSIP_ACTION_INFO_DEF + 297:return additemset(pPlayer, 836);  // T8套装（80级）836

		case GOSSIP_ACTION_INFO_DEF + 300:return additemset(pPlayer, 203);  // T1恶魔之心 (60级) 203    
		case GOSSIP_ACTION_INFO_DEF + 301:return additemset(pPlayer, 212);  // T2复仇(60级)  212     
		case GOSSIP_ACTION_INFO_DEF + 302:return additemset(pPlayer, 529);  // T3瘟疫之心(60级) 529    
		case GOSSIP_ACTION_INFO_DEF + 303:return additemset(pPlayer, 645);  // T4虚空之心（70级）645    
		case GOSSIP_ACTION_INFO_DEF + 304:return additemset(pPlayer, 646);  // T5堕落（70级） 646  
		case GOSSIP_ACTION_INFO_DEF + 305:return additemset(pPlayer, 670);  // T6凶星（70级）670    
		case GOSSIP_ACTION_INFO_DEF + 306:return additemset(pPlayer, 802);  // T7套装（80级）802  
		case GOSSIP_ACTION_INFO_DEF + 307:return additemset(pPlayer, 837);  // T8套装（80级）837

		case GOSSIP_ACTION_INFO_DEF + 310:return additemset(pPlayer, 205);  // T1塞纳里奥(60级) 205
		case GOSSIP_ACTION_INFO_DEF + 311:return additemset(pPlayer, 214);  // T2怒风(60级)  214  
		case GOSSIP_ACTION_INFO_DEF + 312:return additemset(pPlayer, 521);  // T3梦游者(60级) 521 
		case GOSSIP_ACTION_INFO_DEF + 313:return additemset(pPlayer, 638);  // T4玛洛尼（70级治疗）638  
		case GOSSIP_ACTION_INFO_DEF + 314:return additemset(pPlayer, 639);  // T4玛洛尼（70级平衡）639 
		case GOSSIP_ACTION_INFO_DEF + 315:return additemset(pPlayer, 640);  // T4玛洛尼（70级野性）640   
		case GOSSIP_ACTION_INFO_DEF + 316:return additemset(pPlayer, 641);  // T5诺达希尔（70级野性）641  
		case GOSSIP_ACTION_INFO_DEF + 317:return additemset(pPlayer, 642);  // T5诺达希尔（70级治疗）642  
		case GOSSIP_ACTION_INFO_DEF + 318:return additemset(pPlayer, 643);  // T5诺达希尔（70级平衡） 643  
		case GOSSIP_ACTION_INFO_DEF + 319:return additemset(pPlayer, 676);  // T6雷霆之心（70级野性） 676  
		case GOSSIP_ACTION_INFO_DEF + 320:return additemset(pPlayer, 677);  // T6雷霆之心（70级平衡）677  
		case GOSSIP_ACTION_INFO_DEF + 321:return additemset(pPlayer, 678);  // T6雷霆之心（70级治疗）678  
		case GOSSIP_ACTION_INFO_DEF + 322:return additemset(pPlayer, 798);  // T7套装（80级）798
		case GOSSIP_ACTION_INFO_DEF + 323:return additemset(pPlayer, 799);  // T7套装（80级） 799
		case GOSSIP_ACTION_INFO_DEF + 324:return additemset(pPlayer, 800);  // T7套装（80级） 800
		case GOSSIP_ACTION_INFO_DEF + 325:return additemset(pPlayer, 827);  // T8套装（80级） 827
		case GOSSIP_ACTION_INFO_DEF + 326:return additemset(pPlayer, 828);  // T8套装（80级） 828
		case GOSSIP_ACTION_INFO_DEF + 327:return additemset(pPlayer, 829);  // T8套装（80级） 829
		};
		return false;
	}

	bool additemset(Player* pPlayer, uint32 itemset){
		if (!pPlayer->gamePointMgr.checkPoint(500))
		{
			pPlayer->gossipMenuType = -1;/*还原默认值*/
			return false;
		}
		ChatHandler(pPlayer).addItemSet(itemset);
		switch (pPlayer->getClass()){
		case 1:pPlayer->learnSpell(750, true); break;//战士 学习板甲.learn 750
		case 2:pPlayer->learnSpell(750, true); break;//圣骑士 // 学习板甲.learn 750
		case 3:pPlayer->learnSpell(8737, true); break;//猎人 //锁甲
		case 6:pPlayer->learnSpell(750, true); break;//死亡骑士 //板甲
		case 7:pPlayer->learnSpell(8737, true); break;//萨满祭司 学习锁甲.learn 8737
		}
		Pet * pet = pPlayer->GetPet();
		if (pet){
			switch (pet->getClass()){
			case 1:pet->learnSpell(750); break;//战士 学习板甲.learn 750
			case 2:pet->learnSpell(750); break;//圣骑士 // 学习板甲.learn 750
			case 3:pet->learnSpell(8737); break;//猎人 //锁甲
			case 6:pet->learnSpell(750); break;//死亡骑士 //板甲
			case 7:pet->learnSpell(8737); break;//萨满祭司 学习锁甲.learn 8737
			}
		}

		pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_ITEMSET, 500);

		pPlayer->gossipMenuType = -1;/*还原默认值*/
		return true;
	}