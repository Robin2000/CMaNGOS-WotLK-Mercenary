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
/*
insert into npc_text(ID,text0_0)values(16777209,'利用原力传送。');
insert into npc_text(ID,text0_0)values(16777210,'利用原力直达游戏目标。');
insert into npc_text(ID,text0_0)values(16777211,'利用原力临时随机召唤一只坐骑，忠诚度有限。');
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

insert into custom_texts(entry, content_default)values(-2800190,'原力骑乘。');

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

insert into custom_texts(entry, content_default)values(-2800210,'小技巧。');//

insert into custom_texts(entry, content_default)values(-2800220,'游戏直达。');

insert into custom_texts(entry, content_default)values(-2800221,'秒升30级（-10原力）。');
insert into custom_texts(entry, content_default)values(-2800222,'秒升55级（-50原力）。');
insert into custom_texts(entry, content_default)values(-2800223,'秒升70级（-200原力）。');
insert into custom_texts(entry, content_default)values(-2800224,'秒升80级（-500原力）。');

insert into custom_texts(entry, content_default)values(-2800230,'秒升专业。');
insert into custom_texts(entry, content_default)values(-2800231,'秒升法术（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800232,'秒升声望（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800233,'秒开地图（-500原力）。');
insert into custom_texts(entry, content_default)values(-2800234,'秒开鸟点（-500原力）。');

insert into custom_texts(entry, content_default)values(-2800240,'T1-T8套装（-500原力）。');

delete from custom_texts where entry>=-2800279 and entry<=-2800260;

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


insert into custom_texts(entry, content_default)values(-2800300,'地图传送。');

insert into custom_texts(entry, content_default)values(-2800301,'|TInterface\\ICONS\\achievement_zone_zuldrak_05.blp:30|t |cff0000ff各大主城传送。');
insert into custom_texts(entry, content_default)values(-2800302,'|TInterface\\ICONS\\Achievement_BG_winWSG.blp:30|t |cFFFF0000古拉巴什竞技场"。');
insert into custom_texts(entry, content_default)values(-2800303,'|TInterface\\ICONS\\Achievement_Zone_EasternKingdoms_01.blp:30|t 东部王国地区。');
insert into custom_texts(entry, content_default)values(-2800304,'|TInterface\\ICONS\\Achievement_Zone_Kalimdor_01.blp:30|t 卡利姆多地区。');
insert into custom_texts(entry, content_default)values(-2800305,'|TInterface\\ICONS\\Achievement_Boss_PrinceKeleseth.blp:30|t 艾泽拉斯副本。');
insert into custom_texts(entry, content_default)values(-2800306,'|TInterface\\ICONS\\Achievement_Zone_Blackrock_01.blp:30|t 艾泽拉斯团队副本。');
insert into custom_texts(entry, content_default)values(-2800307,'|TInterface\\ICONS\\Achievement_Zone_Outland_01.blp:30|t 外域地区。');
insert into custom_texts(entry, content_default)values(-2800308,'|TInterface\\ICONS\\Achievement_boss_murmur.blp:30|t 外域副本。');
insert into custom_texts(entry, content_default)values(-2800309,'|TInterface\\ICONS\\Achievement_Zone_IsleOfQuelDanas.blp:30|t 外域团队副本。');
insert into custom_texts(entry, content_default)values(-2800310,'|TInterface\\ICONS\\Achievement_Zone_Northrend_01.blp:30|t 诺森德地区。');
insert into custom_texts(entry, content_default)values(-2800311,'|TInterface\\LFGFrame\\LFGIcon-TheForgeofSouls.blp:30|t 诺森德副本。');
insert into custom_texts(entry, content_default)values(-2800312,'|TInterface\\ICONS\\Achievement_Zone_IceCrown_01.blp:30|t 诺森德团队副本。');

insert into custom_texts(entry, content_default)values(-2800321, '|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_King_Varian_Wrynn.blp:30|t [联盟]暴风城。');
insert into custom_texts(entry, content_default)values(-2800322, '|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_King_Magni_Bronzebeard.blp:30|t [联盟]铁炉堡（主城）。');
insert into custom_texts(entry, content_default)values(-2800323, '|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_Tyrande_Whisperwind.blp:30|t [联盟]达拉苏斯。');
insert into custom_texts(entry, content_default)values(-2800324, '|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_Prophet_Velen.blp:30|t [联盟]埃索达');
insert into custom_texts(entry, content_default)values(-2800325, '|cFF990066|TInterface\\ICONS\\Achievement_Zone_Tanaris_01.blp:30|t [中立]加基森。');
insert into custom_texts(entry, content_default)values(-2800326, '|cFF990066|TInterface\\ICONS\\Achievement_Zone_ElwynnForest.blp:30|t [中立]藏宝海湾。');
insert into custom_texts(entry, content_default)values(-2800327, '|cFF990066|TInterface\\ICONS\\Achievement_Zone_EversongWoods.blp:30|t [中立]沙塔斯。');
insert into custom_texts(entry, content_default)values(-2800328, '|cFF990066|TInterface\\ICONS\\Achievement_Zone_GrizzlyHills_01.blp:30|t [中立]达拉然。');

insert into custom_texts(entry, content_default)values(-2800331, '|cFF000000|TInterface\\ICONS\\Achievement_Leader_ Thrall.blp:30|t [部落]奥格瑞玛（主城）。');
insert into custom_texts(entry, content_default)values(-2800332, '|cff000000|TInterface\\ICONS\\Achievement_Leader_Cairne Bloodhoof.blp:30|t [部落]雷霆崖。');
insert into custom_texts(entry, content_default)values(-2800333, '|cff000000|TInterface\\ICONS\\Achievement_Leader_Sylvanas.blp:30|t [部落]幽暗城。');
insert into custom_texts(entry, content_default)values(-2800334, '|cff000000|TInterface\\ICONS\\Achievement_Leader_Lorthemar_Theron .blp:30|t [部落]银月城');

insert into gossip_menu(entry,text_id) values(65535,16777213);
*/
bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	//pPlayer->GetMotionMaster()->MovePoint(50000, pPlayer->GetPositionX() + 10, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), true);
	
	char * title=new char[1024];
	sprintf(title, pPlayer->GetMangosString(-2800173), pPlayer->GetGamePointMgr().getGamePoint()); // 当前原力值：%d

	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 当前原力值：%d
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800190, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);//原力骑乘。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800220, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);//游戏直达。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800300, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 8);//地图传送。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800210, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);//发送游戏小技巧。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800174, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);//设置返回点。（-3原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800175, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);//前往返回点。（-2原力）
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800176, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);//回家。
	pPlayer->SEND_GOSSIP_MENU(16777213, pItem->GetObjectGuid()); //在线即可累积原力。

	pPlayer->SendEquipError(EQUIP_ERR_NONE, pItem);
	return true;
}
void hearthstone_prepare_transport(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800301, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 200);  // 各大主城传送
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800302, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 201);  // 古拉巴什竞技场
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800303, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 202);  // 东部王国地区
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800304, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 203);  // 卡利姆多地区
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800305, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 204);  // 艾泽拉斯副本
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800306, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 205);  // 艾泽拉斯团队副本
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800307, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 206);  // 外域地区
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800308, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 207);  // 外域副本
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800309, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 208);  // 外域团队副本
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800310, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 209);  // 诺森德地区
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800311, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 210);  // 诺森德副本
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800312, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 211);  // 诺森德团队副本
	pPlayer->SEND_GOSSIP_MENU(16777209, pItem->GetObjectGuid()); //利用原力传送
}
void hearthstone_prepare_transport2(Player* pPlayer, Item* pItem, uint32 uiAction){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	if (uiAction >= GOSSIP_ACTION_INFO_DEF + 200){// 各大主城传送
		uint8 race=pPlayer->getRace();
		if (race == 1 || race == 3 || race == 4 || race == 7 || race == 11) {//联盟传送
			pPlayer->ADD_GOSSIP_ITEM(2, -2800321, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 230);//[联盟]暴风城
			pPlayer->ADD_GOSSIP_ITEM(2, -2800322, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 231);//[联盟]铁炉堡（主城）
			pPlayer->ADD_GOSSIP_ITEM(2, -2800323, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 232);//[联盟]达拉苏斯
			pPlayer->ADD_GOSSIP_ITEM(2, -2800324, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 233);//[联盟]埃索达
			pPlayer->ADD_GOSSIP_ITEM(2, -2800325, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 234);//[中立]加基森
			pPlayer->ADD_GOSSIP_ITEM(2, -2800326, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 235);//[联盟]暴风城
			pPlayer->ADD_GOSSIP_ITEM(2, -2800327, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 236);//[联盟]暴风城
			pPlayer->ADD_GOSSIP_ITEM(2, -2800328, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 237);//[联盟]暴风城
		}
		//Horde Cities
		else if (race == 2 | race == 5 | race == 6 | race == 8 | race == 10){
			pPlayer->ADD_GOSSIP_ITEM(2, -2800331, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 240);//[部落]奥格瑞玛（主城）
			pPlayer->ADD_GOSSIP_ITEM(2, -2800332, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 241);//[部落]雷霆崖
			pPlayer->ADD_GOSSIP_ITEM(2, -2800333, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 242);//[部落]幽暗城
			pPlayer->ADD_GOSSIP_ITEM(2, -2800334, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 243);//[部落]银月城
			pPlayer->ADD_GOSSIP_ITEM(2, -2800325, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 244);//[中立]加基森
			pPlayer->ADD_GOSSIP_ITEM(2, -2800326, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 245);//[中立]藏宝海湾
			pPlayer->ADD_GOSSIP_ITEM(2, -2800327, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 246);//[中立]沙塔斯
			pPlayer->ADD_GOSSIP_ITEM(2, -2800328, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 247);//[中立]达拉然
		}
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 201)// 古拉巴什竞技场
		hearthstone_transport(pPlayer,0, -13261.30, 164.45, 35.78, 0);
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 202){ // 东部王国地区
	player:GossipMenuAddItem(2, "奥特兰克山脉", 0, 112)
	player : GossipMenuAddItem(2, "阿拉希高地", 0, 113)
		 player : GossipMenuAddItem(2, "荒芜之地", 0, 114)
			  player : GossipMenuAddItem(2, "诅咒之地", 0, 115)
				   player : GossipMenuAddItem(2, "燃烧平原", 0, 116)
						player : GossipMenuAddItem(2, "逆风小径", 0, 117)
							 player : GossipMenuAddItem(2, "丹莫罗", 0, 118)
								  player : GossipMenuAddItem(2, "暮色森林", 0, 119)
									   player : GossipMenuAddItem(2, "东瘟疫之地", 0, 120)
											player : GossipMenuAddItem(2, "艾尔文森林", 0, 121)
												 player : GossipMenuAddItem(2, "永歌森林", 0, 122)
													  player : GossipMenuAddItem(2, "幽魂之地", 0, 123)
														   player : GossipMenuAddItem(2, "希尔斯布莱德丘陵", 0, 124)
																player : GossipMenuAddItem(2, "奎尔达纳斯之岛", 0, 125)
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 203){  // 卡利姆多地区
	}
	pPlayer->SEND_GOSSIP_MENU(16777209, pItem->GetObjectGuid()); //利用原力传送
}
bool hearthstone_transport(Player* pPlayer, uint32 mapid, float x, float y, float z, float orientation){
	if (!pPlayer->GetGamePointMgr().checkPoint(1))
		return false;
	pPlayer->TeleportTo(mapid, x, y, z, orientation);
	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 1);
	return true;
}
bool hearthstone_menu_click(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{	

	pPlayer->CLOSE_GOSSIP_MENU();/*无条件关闭旧菜单*/

	if (pPlayer->isInCombat())
		ChatHandler(pPlayer).SendSysMessage(23);//23 系统提示：在战斗中无法这样做。
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 1) //当前原力值：%d
	{
		pPlayer->SEND_GOSSIP_MENU(16777214, pItem->GetObjectGuid()); //'原力与你同在！更多信息请移步交流论坛：http://mangos.mybbs.us。'
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 2){//设置返回点。（-3原力）
		hearthstone_prepare_mount(pPlayer, pItem);
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 3){//游戏直达
		hearthstone_prepare_gamedirect(pPlayer, pItem);
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 4){//设置返回点。（-3原力）
		pPlayer->sendSplitMsg(pPlayer->GetGamePointMgr().nextGameTip());
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 5){//设置返回点。（-3原力）
		pPlayer->GetGamePointMgr().setReturnPoint(3);
		if (pPlayer->GetGamePointMgr().setReturnPoint(3))
		{
			//pPlayer->PlayerTalkClass->SendPointOfInterest(pPlayer->GetPositionX(), pPlayer->GetPositionY(), 7, 39, 0, "ReturnPoint");
			//pPlayer->SEND_GOSSIP_MENU(16777212, pItem->GetObjectGuid());//设置返回点成功,原力与你同在！
		}
		//return true;
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF +6)//前往返回地点。（-2原力）
	{
		pPlayer->GetGamePointMgr().useReturnPoint(2);
	}else if (uiAction == GOSSIP_ACTION_INFO_DEF + 7)//回家。
	{
		pPlayer->TeleportToHomebind();
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 8)//地图传送。
	{
		hearthstone_prepare_transport(pPlayer, pItem);
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 91 && uiAction <= GOSSIP_ACTION_INFO_DEF + 106)
	{
		hearthstone_mount(pPlayer, pItem, uiAction);
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 110 && uiAction <= GOSSIP_ACTION_INFO_DEF + 129)
	{
		hearthstone_gamedirect(pPlayer, pItem, uiAction);
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 130 && uiAction <= GOSSIP_ACTION_INFO_DEF + 149)
	{
		hearthstone_learn_professional(pPlayer, pItem, uiAction);
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 115)
	{
		learn_default_spell(pPlayer, pItem, uiAction);
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 200 && uiAction <= GOSSIP_ACTION_INFO_DEF + 220)//地图传送2级菜单。
	{
		hearthstone_prepare_transport2(pPlayer, pItem, uiAction);
	}
	//pPlayer->HandleEmoteCommandHappy();
	return true;
}
bool learn_default_spell(Player* pPlayer, Item* pItem, uint32 uiAction){
	if (!pPlayer->GetGamePointMgr().checkPoint(500))
		return false;
	pPlayer->learnDefaultSpells();
	pPlayer->learnQuestRewardedSpells();
	ChatHandler chatHandler(pPlayer);
	chatHandler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_DEFAULT_AND_QUEST, chatHandler.GetNameLink(pPlayer).c_str());
	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);
	return true;
}
void hearthstone_prepare_gamedirect(Player* pPlayer, Item* pItem){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800221, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 110);  // 秒升30级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800222, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 111);  // 秒升55级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800223, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 112);  // 秒升70级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800224, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 113);  // 秒升80级
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800230, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 114);  // 秒升专业
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800231, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 115);  // 秒升法术
	//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800232, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 116);  // 秒升声望
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800233, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 117);  // 秒开地图
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800234, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 118);  // 秒开鸟点
	//pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800240, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 119);  // T1-T6套装
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
void prepareProfessional(Player* pPlayer, Item* pItem){
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
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}

void hearthstone_learn_professional(Player* pPlayer, Item* pItem, uint32 uiAction){
	switch (uiAction)
	{
		case GOSSIP_ACTION_INFO_DEF + 130:learn_professional(pPlayer, 164,2); break;// 锻造,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 131:learn_professional(pPlayer, 755,2); break;//珠宝,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 132:learn_professional(pPlayer, 202,2); break;//工程学,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 133:learn_professional(pPlayer, 165, 2); break;//制皮,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 134:learn_professional(pPlayer, 773, 2); break;//铭文,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 135:learn_professional(pPlayer, 171, 2); break;//炼金术,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 136:learn_professional(pPlayer, 197, 2); break;//裁缝,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 137:learn_professional(pPlayer, 333, 2); break;//附魔,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 138:learn_professional(pPlayer, 185,3); break;//烹饪,生活技能有三个，可以随便学习
		case GOSSIP_ACTION_INFO_DEF + 139:learn_professional(pPlayer, 186, 2); break;//采矿,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 140:learn_professional(pPlayer, 182, 2); break;//草药学,商业技能只能学习二个
		case GOSSIP_ACTION_INFO_DEF + 141:learn_professional(pPlayer, 393, 2); break;//剥皮,商业技能只能学习二个 
		case GOSSIP_ACTION_INFO_DEF + 142:learn_professional(pPlayer, 356,3); break; //钓鱼,生活技能有三个，可以随便学习
		case GOSSIP_ACTION_INFO_DEF + 143:learn_professional(pPlayer, 129,3); break; //急救,生活技能有三个，可以随便学习
	}
}
bool learn_professional(Player* pPlayer, uint32 skillid,uint8 maxnum)//.learnsk 165 300 // 制皮  
{
	if (!pPlayer->GetGamePointMgr().checkPoint(500))
		return false;

	if (pPlayer->HasSkill(skillid))
		return false;

	if (maxnum == 2){
		uint32 all[11] = { 164,	755,202,165,773,171,197,333,186,182,393 };
		int count = 0;
		for (int i = 0; i < 11;i++)
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
	
	uint16 max=pPlayer->GetPureMaxSkillValue(skillid);

	pPlayer->SetSkill(skillid, max, max);

	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);

	return true;
}
void hearthstone_gamedirect(Player* pPlayer, Item* pItem, uint32 uiAction){
	switch (uiAction)
	{
		case GOSSIP_ACTION_INFO_DEF + 110:levelup(pPlayer, 30,10); break;
		case GOSSIP_ACTION_INFO_DEF + 111:levelup(pPlayer, 55,50); break;
		case GOSSIP_ACTION_INFO_DEF + 112:levelup(pPlayer, 70,200); break;
		case GOSSIP_ACTION_INFO_DEF + 113:levelup(pPlayer, 80,500); break;
		case GOSSIP_ACTION_INFO_DEF + 114:prepareProfessional(pPlayer, pItem); break;
		//case GOSSIP_ACTION_INFO_DEF + 115:learnSpell(pPlayer, maMountSpell); break;
		//case GOSSIP_ACTION_INFO_DEF + 116:gamedirect(pPlayer, maMountSpell); break;
		case GOSSIP_ACTION_INFO_DEF + 117:explorecheat(pPlayer); break;
		case GOSSIP_ACTION_INFO_DEF + 118:taxicheat(pPlayer); break;
		//case GOSSIP_ACTION_INFO_DEF + 119:itemset(pPlayer, maMountSpell); break;
	}
}
//explorecheat
bool explorecheat(Player* pPlayer){
	if (!pPlayer->GetGamePointMgr().checkPoint(500))
		return false;

	for (uint8 i = 0; i < PLAYER_EXPLORED_ZONES_SIZE; ++i)
		pPlayer->SetFlag(PLAYER_EXPLORED_ZONES_1 + i, 0xFFFFFFFF);

	ChatHandler chatHandler(pPlayer);
	chatHandler.PSendSysMessage(LANG_YOURS_EXPLORE_SET_ALL, chatHandler.GetNameLink().c_str());

	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);
	return true;
}
//taxicheat
bool taxicheat(Player* pPlayer){
	if (!pPlayer->GetGamePointMgr().checkPoint(500))
		return false;

	pPlayer->SetTaxiCheater(true);
	ChatHandler chatHandler(pPlayer);
	chatHandler.PSendSysMessage(LANG_YOURS_TAXIS_ADDED, chatHandler.GetNameLink().c_str());

	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);
	return true;
}
bool levelup(Player* pPlayer, int level, int point)
{
	if (!pPlayer->GetGamePointMgr().checkPoint(point))
		return false;

	if (level - pPlayer->getLevel() <= 0)
		return false;

	pPlayer->GiveLevel(level);
	pPlayer->InitTalentForLevel();
	pPlayer->SetUInt32Value(PLAYER_XP, 0);

	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, point);

	return true;
}
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

	pPlayer->SEND_GOSSIP_MENU(16777211, pItem->GetObjectGuid()); //利用原力临时随机召唤一只坐骑，忠诚度有限。
}
void hearthstone_mount(Player* pPlayer, Item* pItem,uint32 uiAction){
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
}
bool hearthstone_random_mount(Player* pPlayer,const int spellarray[])
{
	if (!pPlayer->GetGamePointMgr().checkPoint(1))
		return false;

	int size = sizeof(spellarray) / sizeof((int)1);
	int spell = spellarray[rand() % size];
	if (!pPlayer->AddMountSpellAura(spell))
		return false;
	
	
	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 1);

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
