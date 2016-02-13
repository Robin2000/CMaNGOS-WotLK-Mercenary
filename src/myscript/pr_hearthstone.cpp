﻿/*
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

insert into gossip_menu(entry,text_id)values(21000,16777210);
insert into gossip_menu(entry,text_id)values(21001,16777210);
insert into gossip_menu(entry,text_id)values(21002,16777210);
insert into gossip_menu(entry,text_id)values(21003,16777210);
insert into gossip_menu(entry,text_id)values(21004,16777210);
insert into gossip_menu(entry,text_id)values(21005,16777210);
insert into gossip_menu(entry,text_id)values(21006,16777210);
insert into gossip_menu(entry,text_id)values(21007,16777210);
insert into gossip_menu(entry,text_id)values(21008,16777210);
insert into gossip_menu(entry,text_id)values(21009,16777210);
insert into gossip_menu(entry,text_id)values(21010,16777210);
insert into gossip_menu(entry,text_id)values(21011,16777210);
insert into gossip_menu(entry,text_id)values(21020,16777210);
insert into gossip_menu(entry,text_id)values(21021,16777210);
insert into gossip_menu(entry,text_id)values(21022,16777210);
insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21000,1,'|TInterface\\ICONS\\achievement_zone_zuldrak_05.blp:30|t |cff0000ff各大主城传送',1,21001,0),
(21000,2,'|TInterface\\ICONS\\Achievement_BG_winWSG.blp:30|t |cFFFF0000古拉巴什竞技场',13,0,0),
(21000,3,'|TInterface\\ICONS\\Achievement_Zone_EasternKingdoms_01.blp:30|t 东部王国地区',27,21002,0),
(21000,4,'|TInterface\\ICONS\\Achievement_Zone_Kalimdor_01.blp:30|t 卡利姆多地区',28,21003,0),
(21000,5,'|TInterface\\ICONS\\Achievement_Boss_PrinceKeleseth.blp:30|t 艾泽拉斯副本',3,21004,0),
(21000,6,'|TInterface\\ICONS\\Achievement_Zone_Blackrock_01.blp:30|t 艾泽拉斯团队副本',4,21005,0),
(21000,7,'|TInterface\\ICONS\\Achievement_Zone_Outland_01.blp:30|t 外域地区',5,21006,0),
(21000,8,'|TInterface\\ICONS\\Achievement_boss_murmur.blp:30|t 外域副本',6,21007,0),
(21000,9,'|TInterface\\ICONS\\Achievement_Zone_IsleOfQuelDanas.blp:30|t 外域团队副本',7,21008,0),
(21000,10,'|TInterface\\ICONS\\Achievement_Zone_Northrend_01.blp:30|t 诺森德地区',8,21009,0),
(21000,11,'|TInterface\\LFGFrame\\LFGIcon-TheForgeofSouls.blp:30|t 诺森德副本',9,21010,0),
(21000,12,'|TInterface\\ICONS\\Achievement_Zone_IceCrown_01.blp:30|t 诺森德团队副本',10,21011,0),

(21001,1,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_King_Varian_Wrynn.blp:30|t [联盟]暴风城',19,0,22),
(21001,2,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_King_Magni_Bronzebeard.blp:30|t [联盟]铁炉堡（主）',20,0,22),
(21001,3,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_Tyrande_Whisperwind.blp:30|t [联盟]达拉苏斯',21,0,22),
(21001,4,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_Prophet_Velen.blp:30|t [联盟]埃索达',22,0,22),
(21001,5,'|cFF000000|TInterface\\ICONS\\Achievement_Leader_ Thrall.blp:30|t [部落]奥格瑞玛（主城）',23,0,21),
(21001,6,'|cff000000|TInterface\\ICONS\\Achievement_Leader_Cairne Bloodhoof.blp:30|t [部落]雷霆崖',24,0,21),
(21001,7,'|cff000000|TInterface\\ICONS\\Achievement_Leader_Sylvanas.blp:30|t [部落]幽暗城',25,0,21),
(21001,8,'|cff000000|TInterface\\ICONS\\Achievement_Leader_Lorthemar_Theron .blp:30|t [部落]银月城',26,0,21),
(21001,9,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_Tanaris_01.blp:30|t [中立]加基森',200,0,0),
(21001,10,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_ElwynnForest.blp:30|t [中立]藏宝海湾',201,0,0),
(21001,11,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_EversongWoods.blp:30|t [中立]沙塔斯',202,0,0),
(21001,12,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_GrizzlyHills_01.blp:30|t [中立]达拉然',203,0,0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21002,1,'奥特兰克山脉',112,0,0),
(21002,2,'阿拉希高地',113,0,0),
(21002,3,'荒芜之地',1114,0,0),
(21002,4,'诅咒之地',115,0,0),
(21002,5,'燃烧平原',116,0,0),
(21002,6,'逆风小径',117,0,0),
(21002,7,'丹莫罗',1118,0,0),
(21002,8,'暮色森林',119,0,0),
(21002,9,'东瘟疫之地',120,0,0),
(21002,10,'艾尔文森林',121,0,0),
(21002,11,'永歌森林',122,0,0),
(21002,12,'幽魂之地',123,0,0),
(21002,13,'奥希尔斯布莱德丘陵',124,0,0),
(21002,14,'奎尔达纳斯之岛',125,0,0),
(21002,15,'下一页',121020, 21020,0),

(21020,1,'洛克莫丹',126,0,0),
(21020,2,'赤脊山脉',127,0,0),
(21020,3,'灼热峡谷',128,0,0),
(21020,4,'银松森林',129,0,0),
(21020,5,'荆棘谷',130,0,0),
(21020,6,'悲伤沼泽',131,0,0),
(21020,7,'辛特兰',132,0,0),
(21020,8,'提瑞斯法林地',133,0,0),
(21020,9,'西瘟疫之地',134,0,0),
(21020,10,'西部荒野',135,0,0),
(21020,11,'湿地',136,0,0),
(21020,12,'上一页',121002, 21002,0),


(21003,1,'灰谷',137, 0, 0),
(21003,2,'艾萨拉',138, 0, 0),
(21003,3,'秘蓝岛',139, 0, 0),
(21003,4,'秘血岛',140, 0, 0),
(21003,5,'黑海岸',141, 0, 0),
(21003,6,'凄凉之地',142, 0, 0),
(21003,7,'杜洛塔',143, 0, 0),
(21003,8,'尘泥沼泽',144, 0, 0),
(21003,9,'费伍德森林',145, 0, 0),
(21003,10,'菲拉斯',146, 0, 0),
(21003,11,'月光林地',147, 0, 0),
(21003,12,'莫高雷',148, 0, 0),
(21003,13,'希利苏斯',149, 0, 0),
(21003,14,'下一页',121021, 21021,0 ),

(21021,1,'石爪山脉',150, 0, 0),
(21021,2,'塔纳利斯',151, 0, 0),
(21021,3,'泰达希尔',152, 0, 0),
(21021,4,'贫瘠之地',153, 0, 0),
(21021,5,'千针石林',154, 0, 0),
(21021,6,'安戈洛环形山',155, 0, 0),
(21021,7,'冬泉谷',156, 0, 0),
(21021,8,'上一页',121003,21003, 0 ),

(21004,1,'黑暗深渊',29, 0,  0),
(21004,2,'黑石深渊',30, 0,  0),
(21004,3,'厄运之槌',31, 0,  0),
(21004,4,'诺莫瑞根',32, 0,  0),
(21004,5,'玛拉顿',33, 0,  0),
(21004,6,'怒焰裂谷',34, 0,  21),
(21004,7,'剃刀高地',35, 0,  0),
(21004,8,'剃刀沼泽',36, 0,  0),
(21004,9,'血色修道院',37, 0,  0),
(21004,10,'通灵学院',38, 0,  0),
(21004,11,'影牙城堡',39, 0,  0),
(21004,12,'斯坦索姆',40, 0,  0),
(21004,13,'沉没的神庙',41, 0,  0),
(21004,14,'死亡矿井',42, 0,  0),
(21004,15,'暴风城监狱',43, 0,  22),
(21004,16,'下一页',121022, 21022,  0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21022,1,'奥达曼',44, 0,  0),
(21022,2,'哀嚎洞穴',45, 0,  0),
(21022,3,'祖尔法拉克',46, 0,  0),
(21022,4,'上一页',121004, 21004,  0),


(21005,1,'黑翼之巢',47, 0,  0),
(21005,2,'熔火之心',48, 0,  0),
(21005,3,'奥妮克希亚的巢穴',49, 0,  0),
(21005,4,'安其拉废墟',50, 0,  0),
(21005,5,'安其拉神殿',51, 0,  0),
(21005,6,'祖尔格拉布',52, 0, 0),

(21006,1,'沙塔斯城',11, 0,  0),
(21006,2,'刀锋山脉',53, 0,  0),
(21006,3,'地狱火半岛',54, 0,  0),
(21006,4,'纳格兰',55, 0,  0),
(21006,5,'虚空风暴',56, 0, 0),
(21006,6,'影月谷',57, 0,  0),
(21006,7,'泰罗卡森林',58, 0,  0),
(21006,8,'赞加沼泽',59, 0,  0),


(21007,1,'|TInterface\\ICONS\\Achievement_boss_murmur.blp:30|t 奥金顿',60, 0,  0),
(21007,2,'|TInterface\\ICONS\\Achievement_Zone_Tanaris_01.blp:30|t 时光之穴',61, 0,  0),
(21007,3,'|TInterface\\ICONS\\Achievement_Boss_LadyVashj.blp:30|t 盘牙水库',62, 0,  0),
(21007,4,'|TInterface\\ICONS\\Achievement_Boss_KargathBladefist_01.blp:30|t 地狱火堡垒',63, 0,  0),
(21007,5,'|TInterface\\ICONS\\Achievement_Boss_PrinceKeleseth.blp:30|t 魔导师平台',64, 0,  0),
(21007,6,'|TInterface\\ICONS\\Achievement_Leader_ Thrall.blp:30|t 旧希尔斯布莱德丘陵',65, 0,  0),

(21008,1,'|TInterface\\ICONS\\achievement_boss_illidan.blp:30|t 黑暗神殿',66, 0,  0),
(21008,2,'|TInterface\\ICONS\\Achievement_Boss_Archimonde .blp:30|t 海加尔山',67, 0,  0),
(21008,3,'|TInterface\\ICONS\\achievement_boss_ladyvashj.blp:30|t 毒蛇神殿',68, 0,  0),
(21008,4,'|TInterface\\ICONS\\achievement_boss_gruulthedragonkiller.blp:30|t 格鲁尔巢穴',69, 0,  0),
(21008,5,'|TInterface\\ICONS\\achievement_boss_magtheridon.blp:30|t 玛瑟里顿巢穴',70, 0,  0),
(21008,6,'|TInterface\\ICONS\\achievement_boss_princemalchezaar_02.blp:30|t 卡拉赞',71, 0, 0),
(21008,7,'|TInterface\\ICONS\\Achievement_Boss_Kiljaedan.blp:30|t 太阳井高地',72, 0,  0),
(21008,8,'|TInterface\\ICONS\\achievement_character_bloodelf_male.blp:30|t 风暴要塞',73, 0,  0),
(21008,9,'|TInterface\\ICONS\\achievement_boss_zuljin.blp:30|t 祖阿曼',74, 0,  0),

(21009,1,'北风苔原',75, 0, 0),
(21009,2,'晶歌森林',76, 0, 0),
(21009,3,'龙骨荒野',77, 0, 0),
(21009,4,'灰熊丘陵',78, 0, 0),
(21009,5,'凛风峡湾',79, 0, 0),
(21009,6,'寒冰皇冠',80, 0, 0),
(21009,7,'索拉查盆地',81, 0, 0),
(21009,8,'风暴之巅',82, 0, 0),
(21009,9,'冬拥湖',83, 0, 0),
(21009,10,'祖达克',84, 0, 0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21010,1,'|TInterface\\ICONS\\achievement_dungeon_azjoluppercity_normal.blp:30|t 艾卓-尼鲁布',85, 0, 0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21010,2,'|TInterface\\ICONS\\achievement_dungeon_drak\'tharon_normal.blp:30|t 德拉克萨隆要塞',86, 0, 0),
(21010,3,'|TInterface\\ICONS\\achievement_dungeon_gundrak_normal.blp:30|t 古达克',87, 0, 0),
(21010,4,'|TInterface\\ICONS\\achievement_dungeon_cotstratholme_normal.blp:30|t 净化斯坦索姆',88, 0, 0),
(21010,5,'|TInterface\\ICONS\\achievement_dungeon_ulduar80_normal.blp:30|t 闪电大厅',89, 0, 0),
(21010,6,'|TInterface\\ICONS\\achievement_dungeon_ulduar77_normal.blp:30|t 岩石大厅',90, 0, 0),
(21010,7,'|TInterface\\ICONS\\achievement_dungeon_nexus70_normal.blp:30|t 魔枢',91, 0, 0),
(21010,8,'|TInterface\\ICONS\\achievement_dungeon_theviolethold_normal.blp:30|t 紫罗兰监狱',92, 0, 0),
(21010,9,'|TInterface\\ICONS\\achievement_dungeon_utgardekeep_normal.blp:30|t 乌特加德城堡',93, 0, 0),
(21010,10,'|TInterface\\ICONS\\achievement_dungeon_utgardepinnacle_normal.blp:30|t 乌特加德之巅',94, 0, 0),

(21011,1,'|TInterface\\ICONS\\Achievement_Zone_IceCrown_01.blp:30|t 冰冠堡垒',204, 0, 0),
(21011,2,'|TInterface\\LFGFrame\\LFGIcon-ArgentRaid.blp:30|t 十字军的试炼',205, 0, 0),
(21011,3,'|TInterface\\ICONS\\Achievement_Boss_KelThuzad_01.blp:30|t 纳克萨玛斯',95, 0, 0),
(21011,4,'|TInterface\\ICONS\\Achievement_Boss_Malygos_01.blp:30|t 永恒之眼',96, 0, 0),
(21011,5,'|TInterface\\ICONS\\Achievement_Reputation_WyrmrestTemple.blp:30|t 黑曜石圣殿',97, 0, 0),
(21011,6,'|TInterface\\ICONS\\achievement_dungeon_ulduarraid_archway_01.blp:30|t 奥杜尔',98, 0, 0),
(21011,7,'|TInterface\\ICONS\\Achievement_Dungeon_UlduarRaid_IceGiant_01.blp:30|t 阿尔卡冯的宝库',99, 0, 0);




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

bool hearthstone_menu_click(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{	
	pPlayer->PlayerTalkClass->ClearMenus();
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
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 8)//地图传送主菜单。
	{
		hearthstone_prepare_transport2(pPlayer, pItem, 21000);
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
	else if (uiAction < GOSSIP_ACTION_INFO_DEF){

		return hearthstone_transport_case(pPlayer, pItem,uiAction);
	}
	//pPlayer->HandleEmoteCommandHappy();
	return true;
}

bool hearthstone_prepare_transport2(Player* pPlayer, Item* pItem, uint32 menuid){
	pPlayer->PrepareGossipMenu(pPlayer, menuid);
	pPlayer->SEND_GOSSIP_MENU(65535, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}
bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction){

	if (uiAction == 1)return hearthstone_prepare_transport2(pPlayer, pItem, 21001);//主城
	else if (uiAction == 3)return hearthstone_prepare_transport2(pPlayer, pItem, 21004); //Azeroth Instances
	else if (uiAction == 121022)return hearthstone_prepare_transport2(pPlayer, pItem, 21022); //下一页
	else if (uiAction == 121004)return hearthstone_prepare_transport2(pPlayer, pItem, 21004); //上一页

	else if (uiAction == 4)return hearthstone_prepare_transport2(pPlayer, pItem, 21005); //Azeroth Raids21005
	else if (uiAction == 5)return hearthstone_prepare_transport2(pPlayer, pItem, 21006); //Outland Locations21006

	else if (uiAction == 6)return hearthstone_prepare_transport2(pPlayer, pItem, 21007); //Outland Instances21007
	else if (uiAction == 7)return hearthstone_prepare_transport2(pPlayer, pItem, 21008); //Outland Raids21008
	else if (uiAction == 8)return hearthstone_prepare_transport2(pPlayer, pItem, 21009); //Northrend Locations21009
	else if (uiAction == 9)return hearthstone_prepare_transport2(pPlayer, pItem, 21010); //Northrend Instances21010
	else if (uiAction == 10)return hearthstone_prepare_transport2(pPlayer, pItem, 21011); //Northrend Raids21011


	else if (uiAction == 11)return hearthstone_transport(pPlayer, 530, -1817.82, 5453.04, -12.42, 0);//Shattrath
	else if (uiAction == 13)return hearthstone_transport(pPlayer, 0, -13261.30, 164.45, 35.78, 0);//Gurubashi Arena
	else if (uiAction == 19)return hearthstone_transport(pPlayer, 0, -8913.23, 554.63, 93.79, 0);//Stormwind
	else if (uiAction == 20)return hearthstone_transport(pPlayer, 0, -4923.17, -956.568, 501.513, 0);//Ironforge
	else if (uiAction == 21)return hearthstone_transport(pPlayer, 1, 9945.49, 2609.89, 1316.26, 0);//Darnassus
	else if (uiAction == 22)return hearthstone_transport(pPlayer, 530, -4002.67, -11875.54, -0.71, 0);//Exodar
	else if (uiAction == 23)return hearthstone_transport(pPlayer, 1, 1502.71, -4415.41, 21.77, 0);//Orgimmar
	else if (uiAction == 24)return hearthstone_transport(pPlayer, 1, -1280.0, 125.0, 131.0, 0);//Thunderbluff
	else if (uiAction == 25)return hearthstone_transport(pPlayer, 0, 1831.26, 238.52, 60.52, 0);//Undercity
	else if (uiAction == 26)return hearthstone_transport(pPlayer, 530, 9398.75, -7277.41, 14.21, 0);//Silvermoon

	else if (uiAction == 27)return hearthstone_prepare_transport2(pPlayer, pItem, 21002);//Eastern Kingdoms21002
	else if (uiAction == 121020)return hearthstone_prepare_transport2(pPlayer, pItem, 21020);//下一页
	else if (uiAction == 121021)return hearthstone_prepare_transport2(pPlayer, pItem, 21021);//下一页

	else if (uiAction == 28)return hearthstone_prepare_transport2(pPlayer, pItem, 21003);//Kalimdor21003
	else if (uiAction == 121002)return hearthstone_prepare_transport2(pPlayer, pItem, 21002);//上一页
	else if (uiAction == 121003)return hearthstone_prepare_transport2(pPlayer, pItem, 21003);//上一页

	else if (uiAction == 29)return hearthstone_transport(pPlayer, 1, 4247.34, 744.05, -24.71, 0); //Blackfathom Deeps
	else if (uiAction == 30)return hearthstone_transport(pPlayer, 0, -7576.74, -1126.68, 262.26, 0);//Blackrock Depths
	else if (uiAction == 31)return hearthstone_transport(pPlayer, 1, -3879.52, 1095.26, 154.78, 0);//Dire Maul
	else if (uiAction == 32)return hearthstone_transport(pPlayer, 0, -5162.63, 923.21, 257.17, 0);//Gnomeregan
	else if (uiAction == 33)return hearthstone_transport(pPlayer, 1, -1412.73, 2816.92, 112.64, 0);//Maraudon
	else if (uiAction == 34)return hearthstone_transport(pPlayer, 1, 1814.17, -4401.13, -17.67, 0);//Ragefire Chasm
	else if (uiAction == 35)return hearthstone_transport(pPlayer, 1, -4378.32, -1949.14, 88.57, 0);//Razorfen Downs
	else if (uiAction == 36)return hearthstone_transport(pPlayer, 1, -4473.31, -1810.05, 86.11, 0);//Razorfen Kraul
	else if (uiAction == 37)return hearthstone_transport(pPlayer, 0, 2881.84, -816.23, 160.33, 0);//Scarlet Monastery
	else if (uiAction == 38)return hearthstone_transport(pPlayer, 0, 1229.45, -2576.66, 90.43, 0);//Scholomance
	else if (uiAction == 39)return hearthstone_transport(pPlayer, 0, -243.85, 1517.21, 76.23, 0);//Shadowfang Keep
	else if (uiAction == 40)return hearthstone_transport(pPlayer, 0, 3362.14, -3380.05, 144.78, 0);//Stratholme
	else if (uiAction == 41)return hearthstone_transport(pPlayer, 0, -10177.9, -3994.9, -111.239, 6.01885);//Sunken Temple
	else if (uiAction == 42)return hearthstone_transport(pPlayer, 0, -11209.6, 1666.54, 24.6974, 1.42053);//The Deadmines
	else if (uiAction == 43)return hearthstone_transport(pPlayer, 0, -8797.29, 826.67, 97.63, 0);//The Stockade
	else if (uiAction == 44)return hearthstone_transport(pPlayer, 0, -6072.23, -2955.94, 209.61, 0);//Uldaman
	else if (uiAction == 45)return hearthstone_transport(pPlayer, 1, -735.11, -2214.21, 16.83, 0);//Wailing Caverns
	else if (uiAction == 46)return hearthstone_transport(pPlayer, 1, -6825.69, -2882.77, 8.91, 0);//Zul'Farrak

	//Azeroth Raids
	else if (uiAction == 47)return hearthstone_transport(pPlayer, 469, -7666.11, -1101.53, 399.67, 0); //Blackwing Lair
	else if (uiAction == 48)return hearthstone_transport(pPlayer, 230, 1117.61, -457.36, -102.49, 0);//Molten Core
	else if (uiAction == 49)return hearthstone_transport(pPlayer, 1, -4697.81, -3720.44, 50.35, 0);//Onyxia's Lair
	else if (uiAction == 50)return hearthstone_transport(pPlayer, 1, -8380.47, 1480.84, 14.35, 0);//Ruins of Ahn'Qiraj
	else if (uiAction == 51)return hearthstone_transport(pPlayer, 1, -8258.27, 1962.73, 129.89, 0);//Temple of Ahn'Qiraj
	else if (uiAction == 52)return hearthstone_transport(pPlayer, 0, -11916.74, -1203.32, 92.28, 0);//Zul'Gurub

	//Outland Locations
	else if (uiAction == 53)return hearthstone_transport(pPlayer, 530, 2039.24, 6409.27, 134.30, 0);//Blade's Edge Mountains
	else if (uiAction == 54)return hearthstone_transport(pPlayer, 530, -247.37, 964.77, 84.33, 0);//Hellfire Peninsula
	else if (uiAction == 55)return hearthstone_transport(pPlayer, 530, -605.84, 8442.39, 60.76, 0);//Nagrand
	else if (uiAction == 56)return hearthstone_transport(pPlayer, 530, 3055.70, 3671.63, 142.44, 0);//Netherstorm
	else if (uiAction == 57)return hearthstone_transport(pPlayer, 530, -2859.75, 3184.24, 9.76, 0);//Shadowmoon Valley
	else if (uiAction == 58)return hearthstone_transport(pPlayer, 530, -1917.17, 4879.45, 2.10, 0);//Terokkar Forest
	else if (uiAction == 59)return hearthstone_transport(pPlayer, 530, -206.61, 5512.90, 21.58, 0);//Zangarmarsh

	//Outland Instances
	else if (uiAction == 60)return hearthstone_transport(pPlayer, 530, -3323.76, 4934.31, -100.21, 0);//Auchindoun
	else if (uiAction == 61)return hearthstone_transport(pPlayer, 1, -8187.16, -4704.91, 19.33, 0);//Caverns of Time
	else if (uiAction == 62)return hearthstone_transport(pPlayer, 530, 731.04, 6849.35, -66.62, 0);//Coilfang Reservoir
	else if (uiAction == 63)return hearthstone_transport(pPlayer, 530, -331.87, 3039.30, -16.66, 0);//Hellfire Citadel
	else if (uiAction == 64)return hearthstone_transport(pPlayer, 530, 12884.92, -7333.78, 65.48, 0);//Magisters' Terrace
	else if (uiAction == 65)return hearthstone_transport(pPlayer, 1, -8345.88, -4060.33, -208, 0);//Tempest Keep

	//Outland Raids
	else if (uiAction == 66)return hearthstone_transport(pPlayer, 530, -3638.16, 316.09, 35.40, 0);//Black Temple
	else if (uiAction == 67)return hearthstone_transport(pPlayer, 1, -8175.94, -4178.52, -166.74, 0);//Hyjal Summit
	else if (uiAction == 68)return hearthstone_transport(pPlayer, 530, 731.04, 6849.35, -66.62, 0);//Serpentshrine Cavern
	else if (uiAction == 69)return hearthstone_transport(pPlayer, 530, 3528.99, 5133.50, 1.31, 0);//Gruul's Lair
	else if (uiAction == 70)return hearthstone_transport(pPlayer, 530, -337.50, 3131.88, -102.92, 0);//Magtheridon's Lair
	else if (uiAction == 71)return hearthstone_transport(pPlayer, 0, -11119.22, -2010.73, 47.09, 0);//Karazhan
	else if (uiAction == 72)return hearthstone_transport(pPlayer, 530, 12560.79, -6774.58, 15.08, 0);//Sunwell Plateau
	else if (uiAction == 73)return hearthstone_transport(pPlayer, 530, 3088.25, 1388.17, 185.09, 0);//The Eye
	else if (uiAction == 74)return hearthstone_transport(pPlayer, 530, 6850.0, -7950.0, 170.0, 0);//Zul'Aman

	//Northrend Locations
	else if (uiAction == 75)return hearthstone_transport(pPlayer, 571, 2920.15, 4043.40, 1.82, 0);//Borean Tundra
	else if (uiAction == 76)return hearthstone_transport(pPlayer, 571, 5371.18, 109.11, 157.65, 0);//Crystalsong Forest
	else if (uiAction == 77)return hearthstone_transport(pPlayer, 571, 2729.59, 430.70, 66.98, 0);//Dragonblight
	else if (uiAction == 78)return hearthstone_transport(pPlayer, 571, 3587.20, -4545.12, 198.75, 0);//Grizzly Hills
	else if (uiAction == 79)return hearthstone_transport(pPlayer, 571, 154.39, -4896.33, 296.14, 0);//Howling Fjord
	else if (uiAction == 80)return hearthstone_transport(pPlayer, 571, 8406.89, 2703.79, 665.17, 0);//Icecrown
	else if (uiAction == 81)return hearthstone_transport(pPlayer, 571, 5569.49, 5762.99, -75.22, 0);//Sholazar Basin
	else if (uiAction == 82)return hearthstone_transport(pPlayer, 571, 6180.66, -1085.65, 415.54, 0);//The Storm Peaks
	else if (uiAction == 83)return hearthstone_transport(pPlayer, 571, 5044.03, 2847.23, 392.64, 0);//Wintergrasp
	else if (uiAction == 84)return hearthstone_transport(pPlayer, 571, 4700.09, -3306.54, 292.41, 0);//Zul'Drak

	//Northrend Instances
	else if (uiAction == 85)return hearthstone_transport(pPlayer, 571, 3738.93, 2164.14, 37.29, 0);//Azjol-Nerub
	else if (uiAction == 86)return hearthstone_transport(pPlayer, 571, 4772.13, -2035.85, 229.38, 0);//Drak'Tharon
	else if (uiAction == 87)return hearthstone_transport(pPlayer, 571, 6937.12, -4450.80, 450.90, 0);//Gundrak
	else if (uiAction == 88)return hearthstone_transport(pPlayer, 1, -8746.94, -4437.69, -199.98, 0);//The Culling of Stratholme
	else if (uiAction == 89)return hearthstone_transport(pPlayer, 571, 9171.01, -1375.94, 1099.55, 0);//The Halls of Lightning
	else if (uiAction == 90)return hearthstone_transport(pPlayer, 571, 8921.35, -988.56, 1039.37, 0);//The Halls of Stone
	else if (uiAction == 91)return hearthstone_transport(pPlayer, 571, 3897.42, 6985.33, 69.49, 0);//The Nexus
	else if (uiAction == 92)return hearthstone_transport(pPlayer, 571, 5695.19, 505.38, 652.68, 0);//The Violet Hold
	else if (uiAction == 93)return hearthstone_transport(pPlayer, 571, 1222.44, -4862.61, 41.24, 0);//Utgarde Keep
	else if (uiAction == 94)return hearthstone_transport(pPlayer, 571, 1251.10, -4856.31, 215.86, 0);//Utgarde Pinnacle

	// Northrend Raids
	else if (uiAction == 95)return hearthstone_transport(pPlayer, 571, 3669.77, -1275.48, 243.51, 0);//Naxxramas
	else if (uiAction == 96)return hearthstone_transport(pPlayer, 571, 3873.50, 6974.83, 152.04, 0);//The Eye of Eternity
	else if (uiAction == 97)return hearthstone_transport(pPlayer, 571, 3547.39, 267.95, -115.96, 0);//The Obsidian Sanctum
	else if (uiAction == 98)return hearthstone_transport(pPlayer, 571, 9330.53, -1115.40, 1245.14, 0);//Ulduar
	else if (uiAction == 99)return hearthstone_transport(pPlayer, 571, 5410.21, 2842.37, 418.67, 0);//Vault of Archavon

	//Eastern Kingdoms
	else if (uiAction == 112)return hearthstone_transport(pPlayer, 0, 353.79, -607.08, 150.76, 0);//Alterac Mountains
	else if (uiAction == 113)return hearthstone_transport(pPlayer, 0, -2269.78, -2501.06, 79.04, 0);//Arathi Highlands
	else if (uiAction == 114)return hearthstone_transport(pPlayer, 0, -6026.58, -3318.27, 260.64, 0);//Badlands
	else if (uiAction == 115)return hearthstone_transport(pPlayer, 0, -10797.67, -2994.29, 44.42, 0);//Blasted Lands
	else if (uiAction == 116)return hearthstone_transport(pPlayer, 0, -8357.72, -2537.49, 135.01, 0);//Burning Steppes
	else if (uiAction == 117)return hearthstone_transport(pPlayer, 0, -10460.22, -1699.33, 81.85, 0);//Deadwind Pass
	else if (uiAction == 118)return hearthstone_transport(pPlayer, 0, -6234.99, 341.24, 383.22, 0);//Dun Morogh
	else if (uiAction == 119)return hearthstone_transport(pPlayer, 0, -10068.30, -1501.07, 28.41, 0);//Duskwood
	else if (uiAction == 120)return hearthstone_transport(pPlayer, 0, 1924.70, -2653.54, 59.70, 0);//Eastern Plaguelands
	else if (uiAction == 121)return hearthstone_transport(pPlayer, 0, -8939.71, -131.22, 83.62, 0);//Elwynn Forest
	else if (uiAction == 122)return hearthstone_transport(pPlayer, 530, 10341.73, -6366.29, 34.31, 0);//Eversong Woods
	else if (uiAction == 123)return hearthstone_transport(pPlayer, 530, 7969.87, -6872.63, 58.66, 0);//Ghostlands
	else if (uiAction == 124)return hearthstone_transport(pPlayer, 0, -585.70, 612.18, 83.80, 0);//Hillsbrad Foothills
	else if (uiAction == 125)return hearthstone_transport(pPlayer, 530, 12916.81, -6867.82, 7.69, 0);//Isle of Quel'Danas
	else if (uiAction == 126)return hearthstone_transport(pPlayer, 0, -4702.59, -2698.61, 318.75, 0);//Loch Modan
	else if (uiAction == 127)return hearthstone_transport(pPlayer, 0, -9600.62, -2123.21, 66.23, 0);//Redridge Mountains
	else if (uiAction == 128)return hearthstone_transport(pPlayer, 0, -6897.73, -1821.58, 241.16, 0);//Searing Gorge
	else if (uiAction == 129)return hearthstone_transport(pPlayer, 0, 1499.57, 623.98, 47.01, 0);//Silverpine Forest
	else if (uiAction == 130)return hearthstone_transport(pPlayer, 0, -11355.90, -383.40, 65.14, 0);//Stranglethorn Vale
	else if (uiAction == 131)return hearthstone_transport(pPlayer, 0, -10552.60, -2355.25, 85.95, 0);//Swamp of Sorrows
	else if (uiAction == 132)return hearthstone_transport(pPlayer, 0, 92.63, -1942.31, 154.11, 0);//The Hinterlands
	else if (uiAction == 133)return hearthstone_transport(pPlayer, 0, 1676.13, 1669.37, 137.02, 0);//Tirisfal Glades
	else if (uiAction == 134)return hearthstone_transport(pPlayer, 0, 1635.57, -1068.50, 66.57, 0);//Western Plaguelands
	else if (uiAction == 135)return hearthstone_transport(pPlayer, 0, -9827.95, 865.80, 25.80, 0);//Westfall
	else if (uiAction == 136)return hearthstone_transport(pPlayer, 0, -4086.32, -2620.72, 43.55, 0);//Wetlands

	//Kalimdor
	if (uiAction == 137)return hearthstone_transport(pPlayer, 1, 3474.41, 853.47, 5.76, 0);//Ashenvale
	else if (uiAction == 138)return hearthstone_transport(pPlayer, 1, 2763.93, -3881.34, 92.52, 0);//Azshara
	else if (uiAction == 139)return hearthstone_transport(pPlayer, 530, -3972.72, -13914.99, 98.88, 0);//Azuremyst Isle
	else if (uiAction == 140)return hearthstone_transport(pPlayer, 530, -2721.67, -12208.90, 9.08, 0);//Bloodmyst Isle
	else if (uiAction == 141)return hearthstone_transport(pPlayer, 1, 4336.61, 173.83, 46.84, 0);//Darkshore
	else if (uiAction == 142)return hearthstone_transport(pPlayer, 1, 47.28, 1684.64, 93.55, 0);//Desolace
	else if (uiAction == 143)return hearthstone_transport(pPlayer, 1, -611.61, -4263.16, 38.95, 0);//Durotar
	else if (uiAction == 144)return hearthstone_transport(pPlayer, 1, -3682.58, -2556.93, 58.43, 0);//Dustwallow Marsh
	else if (uiAction == 145)return hearthstone_transport(pPlayer, 1, 3590.56, -1516.69, 169.98, 0);//Felwood
	else if (uiAction == 146)return hearthstone_transport(pPlayer, 1, -4300.02, -631.56, -9.35, 0);//Feralas
	else if (uiAction == 147)return hearthstone_transport(pPlayer, 1, 7999.68, -2670.19, 512.09, 0);//Moonglade
	else if (uiAction == 148)return hearthstone_transport(pPlayer, 1, -2931.49, -262.82, 53.25, 0);//Moonglade
	else if (uiAction == 149)return hearthstone_transport(pPlayer, 1, -6814.57, 833.77, 49.74, 0);//Silithus
	else if (uiAction == 150)return hearthstone_transport(pPlayer, 1, -225.34, -765.16, 6.4, 0);//Stonetalon Mountains
	else if (uiAction == 151)return hearthstone_transport(pPlayer, 1, -6999.47, -3707.94, 26.44, 0);//Tanaris
	else if (uiAction == 152)return hearthstone_transport(pPlayer, 1, 8754.06, 949.62, 25.99, 0);//Teldrassil
	else if (uiAction == 153)return hearthstone_transport(pPlayer, 1, -948.46, -3738.60, 5.98, 0);//The Barrens
	else if (uiAction == 154)return hearthstone_transport(pPlayer, 1, -4685.72, -1836.24, -44.04, 0);//Thousand Needles
	else if (uiAction == 155)return hearthstone_transport(pPlayer, 1, -6162.47, -1098.74, -208.99, 0);//Un'Goro Crater
	else if (uiAction == 156)return hearthstone_transport(pPlayer, 1, 6896.27, -2302.51, 586.69, 0);//Winterspring

	else if (uiAction == 200)return hearthstone_transport(pPlayer, 1, -7174, -3778, 9, 0);//加基森
	else if (uiAction == 201)return hearthstone_transport(pPlayer, 0, -14281.9, 552.564, 8.90422, 0.860144);//藏宝海湾
	else if (uiAction == 202)return hearthstone_transport(pPlayer, 530, -1887.62, 5359.09, -12.4279, 4.40435);//沙塔斯
	else if (uiAction == 203)return hearthstone_transport(pPlayer, 571, 5809.78, 432.913, 658.772, 1.20920);//达拉然
	else if (uiAction == 204)return hearthstone_transport(pPlayer, 571, 5855.22, 2102.03, 635.991, 3.57899);//冰冠堡垒
	else if (uiAction == 205)return hearthstone_transport(pPlayer, 571, 8515.61, 714.153, 558.248, 1.57753);//十字军的试炼

	return true;
}
bool hearthstone_transport(Player* pPlayer, uint32 mapid, double x, double y, double z, double o)
{
	if (!pPlayer->GetGamePointMgr().checkPoint(1))
		return false;
	pPlayer->TeleportTo(mapid, x, y, z, o);
	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 1);
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
