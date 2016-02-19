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

insert into custom_texts(entry, content_default)values(-2800220,'成就直达。');

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


insert into custom_texts(entry, content_default)values(-2800300,'地图传送（-1原力）。');

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
(21002,15,'下一页',255, 0,21020),

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
(21020,12,'上一页',254, 0,21002),


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
(21003,14,'下一页',253, 0,21021 ),

(21021,1,'石爪山脉',150, 0, 0),
(21021,2,'塔纳利斯',151, 0, 0),
(21021,3,'泰达希尔',152, 0, 0),
(21021,4,'贫瘠之地',153, 0, 0),
(21021,5,'千针石林',154, 0, 0),
(21021,6,'安戈洛环形山',155, 0, 0),
(21021,7,'冬泉谷',156, 0, 0),
(21021,8,'上一页',252,0, 21003 ),

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
(21004,16,'下一页',251, 0,  21022);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21022,1,'奥达曼',44, 0,  0),
(21022,2,'哀嚎洞穴',45, 0,  0),
(21022,3,'祖尔法拉克',46, 0,  0),
(21022,4,'上一页',250, 0,  21004),


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

delete from custom_texts where entry>=-2800547 and entry<=-2800400;

insert into custom_texts(entry, content_default)values(-2800400,  'T1力量(60级');
insert into custom_texts(entry, content_default)values(-2800401,  'T2愤怒(60级');
insert into custom_texts(entry, content_default)values(-2800402,  'T3无畏(60级');
insert into custom_texts(entry, content_default)values(-2800403,  'T4战神(70级防御');
insert into custom_texts(entry, content_default)values(-2800404,  'T4战神(70级战斗');
insert into custom_texts(entry, content_default)values(-2800405,  'T5摧毁者(70级防御');
insert into custom_texts(entry, content_default)values(-2800406,  'T5摧毁者(70级战斗');
insert into custom_texts(entry, content_default)values(-2800407,  'T6冲击(70级战斗');
insert into custom_texts(entry, content_default)values(-2800408,  'T6冲击(70级防御');
insert into custom_texts(entry, content_default)values(-2800409,  'T7套装(80级防御');
insert into custom_texts(entry, content_default)values(-2800410,  'T7套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800411,  'T8套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800412,  'T8套装(80级防御');

insert into custom_texts(entry, content_default)values(-2800420,  'T1秩序之源(60级');
insert into custom_texts(entry, content_default)values(-2800421,  'T2审判(60级');
insert into custom_texts(entry, content_default)values(-2800422,  'T3救赎(60级');
insert into custom_texts(entry, content_default)values(-2800423,  'T4庇护(70级治疗');
insert into custom_texts(entry, content_default)values(-2800424,  'T4庇护(70级防御');
insert into custom_texts(entry, content_default)values(-2800425,  'T4庇护(70级惩戒');
insert into custom_texts(entry, content_default)values(-2800426,  'T5晶铸(70级治疗');
insert into custom_texts(entry, content_default)values(-2800427,  'T5晶铸(70级防御');
insert into custom_texts(entry, content_default)values(-2800428,  'T5晶铸(70级惩戒');
insert into custom_texts(entry, content_default)values(-2800429,  'T6光明使者(70级防御');
insert into custom_texts(entry, content_default)values(-2800430,  'T6光明使者(惩戒');
insert into custom_texts(entry, content_default)values(-2800431,  'T6光明使者(治疗');
insert into custom_texts(entry, content_default)values(-2800432,  'T7套装（80级惩戒');
insert into custom_texts(entry, content_default)values(-2800433,  'T7套装（80级治疗');
insert into custom_texts(entry, content_default)values(-2800434,  'T7套装（80级防御');
insert into custom_texts(entry, content_default)values(-2800435,  'T8套装（80级惩戒');
insert into custom_texts(entry, content_default)values(-2800436,  'T8套装（80级防御');
insert into custom_texts(entry, content_default)values(-2800437,  'T8套装(80级防御');


insert into custom_texts(entry, content_default)values(-2800440,  'T1巨兽之王(60级');
insert into custom_texts(entry, content_default)values(-2800441,  'T2驭龙者(60级');
insert into custom_texts(entry, content_default)values(-2800442,  'T3地穴追猎者(60级');
insert into custom_texts(entry, content_default)values(-2800443,  'T4恶魔追猎者(70级');
insert into custom_texts(entry, content_default)values(-2800444,  'T5裂缝行者(70级');
insert into custom_texts(entry, content_default)values(-2800445,  'T6戈隆追猎者(70级');
insert into custom_texts(entry, content_default)values(-2800446,  'T7套装(80级');
insert into custom_texts(entry, content_default)values(-2800447,  'T8套装(80级');

insert into custom_texts(entry, content_default)values(-2800450,  'T1夜幕杀手(60级');
insert into custom_texts(entry, content_default)values(-2800451,  'T2血牙(60级');
insert into custom_texts(entry, content_default)values(-2800452,  'T3骨镰(60级');
insert into custom_texts(entry, content_default)values(-2800453,  'T4虚空之刃(70级');
insert into custom_texts(entry, content_default)values(-2800454,  'T5死神传承(70级');
insert into custom_texts(entry, content_default)values(-2800455,  'T6刺杀者(70级');
insert into custom_texts(entry, content_default)values(-2800456,  'T7套装 (80级');
insert into custom_texts(entry, content_default)values(-2800457,  'T8套装(80级');

insert into custom_texts(entry, content_default)values(-2800460,  'T1预言(60级');
insert into custom_texts(entry, content_default)values(-2800461,  'T2卓越(60级');
insert into custom_texts(entry, content_default)values(-2800462,  'T3信仰(60级');
insert into custom_texts(entry, content_default)values(-2800463,  'T4化身(70级治疗');
insert into custom_texts(entry, content_default)values(-2800464,  'T4化身(70级战斗');
insert into custom_texts(entry, content_default)values(-2800465,  'T5幻身(70级治疗');
insert into custom_texts(entry, content_default)values(-2800466,  'T5幻身(70级战斗');
insert into custom_texts(entry, content_default)values(-2800467,  'T6赦免(70级战斗');
insert into custom_texts(entry, content_default)values(-2800468,  'T6赦免(70级治疗');
insert into custom_texts(entry, content_default)values(-2800469,  'T7套装(80级治疗');
insert into custom_texts(entry, content_default)values(-2800470,  'T7套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800471,  'T8套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800472,  'T8套装(80级治疗');


insert into custom_texts(entry, content_default)values(-2800480,  'T7套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800481,  'T7套装(80级防御');
insert into custom_texts(entry, content_default)values(-2800482,  'T8套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800483,  'T8套装(80级防御');

insert into custom_texts(entry, content_default)values(-2800490,  'T1大地之怒(60级');
insert into custom_texts(entry, content_default)values(-2800491,  'T2无尽风暴(60级');
insert into custom_texts(entry, content_default)values(-2800492,  'T3碎地者(60级');
insert into custom_texts(entry, content_default)values(-2800493,  'T4飓风(70级治疗');
insert into custom_texts(entry, content_default)values(-2800494,  'T4飓风(70级战斗');
insert into custom_texts(entry, content_default)values(-2800495,  'T4飓风(70级战斗');

insert into custom_texts(entry, content_default)values(-2800496,  'T5裂地(70级治疗');
insert into custom_texts(entry, content_default)values(-2800497,  'T5裂地(70级战斗');
insert into custom_texts(entry, content_default)values(-2800498,  'T6裂地(70级战斗');

insert into custom_texts(entry, content_default)values(-2800499,  'T6破天(70级战斗');
insert into custom_texts(entry, content_default)values(-2800500,  'T6破天(70级治疗');
insert into custom_texts(entry, content_default)values(-2800501,  'T6破天(70级治疗');

insert into custom_texts(entry, content_default)values(-2800502,  'T7套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800503,  'T7套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800504,  'T7套装(80级治疗');

insert into custom_texts(entry, content_default)values(-2800505,  'T8套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800506,  'T8套装(80级战斗');
insert into custom_texts(entry, content_default)values(-2800507,  'T8套装(80级治疗');


insert into custom_texts(entry, content_default)values(-2800510,  'T1奥术师(60级');
insert into custom_texts(entry, content_default)values(-2800511,  'T2灵风(60级');
insert into custom_texts(entry, content_default)values(-2800512,  'T3霜火(60级');
insert into custom_texts(entry, content_default)values(-2800513,  'T4占卜者(70级');
insert into custom_texts(entry, content_default)values(-2800514,  'T5提里斯法(70级');
insert into custom_texts(entry, content_default)values(-2800515,  'T6风暴(70级');
insert into custom_texts(entry, content_default)values(-2800516,  'T7套装(80级');
insert into custom_texts(entry, content_default)values(-2800517,  'T8套装(80级');

insert into custom_texts(entry, content_default)values(-2800520,  'T1恶魔之心 (60级');
insert into custom_texts(entry, content_default)values(-2800521,  'T2复仇(60级');
insert into custom_texts(entry, content_default)values(-2800522,  'T3瘟疫之心(60级');
insert into custom_texts(entry, content_default)values(-2800523,  'T4虚空之心(70级');
insert into custom_texts(entry, content_default)values(-2800524,  'T5堕落(70级');
insert into custom_texts(entry, content_default)values(-2800525,  'T6凶星(70级');
insert into custom_texts(entry, content_default)values(-2800526,  'T7套装(80级');
insert into custom_texts(entry, content_default)values(-2800527,  'T8套装(80级');

insert into custom_texts(entry, content_default)values(-2800530,  'T1塞纳里奥(60级');
insert into custom_texts(entry, content_default)values(-2800531,  'T2怒风(60级');
insert into custom_texts(entry, content_default)values(-2800532,  'T3梦游者(60级');
insert into custom_texts(entry, content_default)values(-2800533,  'T4玛洛尼(70级治疗');
insert into custom_texts(entry, content_default)values(-2800534,  'T4玛洛尼(70级平衡');
insert into custom_texts(entry, content_default)values(-2800535,  'T4玛洛尼(70级野性');
insert into custom_texts(entry, content_default)values(-2800536,  'T5诺达希尔(70级野性');
insert into custom_texts(entry, content_default)values(-2800537,  'T5诺达希尔(70级治疗');
insert into custom_texts(entry, content_default)values(-2800538,  'T5诺达希尔(70级平衡');
insert into custom_texts(entry, content_default)values(-2800539,  'T6雷霆之心(70级野性');
insert into custom_texts(entry, content_default)values(-2800540,  'T6雷霆之心(70级平衡');
insert into custom_texts(entry, content_default)values(-2800541,  'T6雷霆之心(70级治疗');
insert into custom_texts(entry, content_default)values(-2800542,  'T7套装(80级野性');
insert into custom_texts(entry, content_default)values(-2800543,  'T7套装(80级治疗');
insert into custom_texts(entry, content_default)values(-2800544,  'T7套装(80级平衡');
insert into custom_texts(entry, content_default)values(-2800545,  'T8套装(80级平衡');
insert into custom_texts(entry, content_default)values(-2800546,  'T8套装(80级野性');
insert into custom_texts(entry, content_default)values(-2800547,  'T8套装(80级治疗');


insert into gossip_menu(entry,text_id) values(65535,16777213);
*/
bool hearthstone_click(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
	return hearthstone_click2(pPlayer, pItem);
}
bool hearthstone_click2(Player* pPlayer, Item* pItem)
{
	//pPlayer->GetMotionMaster()->MovePoint(50000, pPlayer->GetPositionX() + 10, pPlayer->GetPositionY(), pPlayer->GetPositionZ(), true);

	char * title = new char[1024];
	sprintf(title, pPlayer->GetMangosString(-2800173), pPlayer->GetGamePointMgr().getGamePoint()); // 当前原力值：%d

	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);  // 当前原力值：%d
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TAXI, -2800169, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 9);//任务辅助。
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
	//pPlayer->PlayerTalkClass->ClearMenus();
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
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 9)//任务辅助。
	{
		hearthstone_prepare_quest(pPlayer, pItem);
	}
	else if (uiAction == GOSSIP_ACTION_INFO_DEF + 999)
	{
		hearthstone_click2(pPlayer, pItem);//返回主菜单
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
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 180 && uiAction <= GOSSIP_ACTION_INFO_DEF + 330)
	{
		hearthstone_itemset(pPlayer, pItem, uiAction);
	}
	else if (uiAction < GOSSIP_ACTION_INFO_DEF){

		return hearthstone_transport_case(pPlayer, pItem,uiAction);
	}
	else if (uiAction>GOSSIP_ACTION_INFO_DEF + 1000)
	{
		hearthstone_quest(pPlayer, pItem,uiAction - GOSSIP_ACTION_INFO_DEF - 1000);
	}
	else if (uiAction >= GOSSIP_ACTION_INFO_DEF + 980 && uiAction < GOSSIP_ACTION_INFO_DEF + 990)
	{
		transportByCreatureOrGO(pPlayer, pItem, uiAction - GOSSIP_ACTION_INFO_DEF - 980);
	}
	//pPlayer->HandleEmoteCommandHappy();
	return true;
}
void transportByCreatureOrGO(Player* pPlayer, Item* pItem, int idx){
	Quest const * quest = pPlayer->GetHearthstoneQuest();
	int32 creature=quest->ReqCreatureOrGOId[idx];
	if (creature>0)
	{
		CreatureData& data=pPlayer->findCreatureDataByEntry(creature);
		pPlayer->TeleportTo(data.mapid, data.posX, data.posY, data.posZ, 0);
	}
	else if (creature < 0)
	{
		uint32 gameobject = -creature;
		GameObjectData& data = pPlayer->findGameObjectDataByEntry(gameobject);
		pPlayer->TeleportTo(data.mapid, data.posX, data.posY, data.posZ, 0);
	}
}
void hearthstone_quest(Player* pPlayer, Item* pItem, uint32 questid)
{
	pPlayer->PrepareGossipMenu(pPlayer, 65535);
	Quest const * quest = pPlayer->GetQuest(questid);
	pPlayer->SetHearthstoneQuest(quest);

	for (int i = 0; i < QUEST_OBJECTIVES_COUNT; ++i)
	{
		if (quest->ReqCreatureOrGOId[i] != 0)
		{
			const char * name = "";
			pPlayer->GetCreatureOrGOTitleLocale(quest->ReqCreatureOrGOId[i],&name);
			pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK,"->"+std::string(name), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 980 + i);
		}
	}


	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
/*动态生成任务列表*/
void hearthstone_prepare_quest(Player* pPlayer, Item* pItem){

	pPlayer->PrepareGossipMenu(pPlayer, 65535);
	
	QuestStatusMap readMap;
	QuestStatusMap& map = pPlayer->getQuestStatusMap();
	
	uint8 count = 0;
	for (QuestStatusMap::const_iterator it = map.begin(); it != map.end() && count <= 20; it++, count++)
	{
		if (it->second.m_status == QUEST_STATUS_INCOMPLETE || (it->second.m_status == QUEST_STATUS_COMPLETE && !it->second.m_rewarded))
			readMap[it->first] = it->second;
	}

	for (QuestStatusMap::const_iterator it = readMap.begin(); it != readMap.end(); it++)
	{
		std::string  title = "";
		pPlayer->GetQuestTitleLocale(it->first, &title);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, title, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1000 + it->first);//数据库中最大为26034，所以该项最大为46034，在uint32范围内
	}


	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, -2800181, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
bool hearthstone_prepare_transport2(Player* pPlayer, Item* pItem, uint32 menuid){
	pPlayer->PrepareGossipMenu(pPlayer, menuid);
	pPlayer->SEND_GOSSIP_MENU(65535, pItem->GetObjectGuid()); //利用原力直达游戏目标。
	return true;
}
bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction){

	if (uiAction == 1)return hearthstone_prepare_transport2(pPlayer, pItem, 21001);//主城
	else if (uiAction == 3)return hearthstone_prepare_transport2(pPlayer, pItem, 21004); //Azeroth Instances
	else if (uiAction == 251)return hearthstone_prepare_transport2(pPlayer, pItem, 21022); //下一页
	else if (uiAction == 250)return hearthstone_prepare_transport2(pPlayer, pItem, 21004); //上一页

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
	else if (uiAction == 255)return hearthstone_prepare_transport2(pPlayer, pItem, 21020);//下一页
	else if (uiAction == 253)return hearthstone_prepare_transport2(pPlayer, pItem, 21021);//下一页

	else if (uiAction == 28)return hearthstone_prepare_transport2(pPlayer, pItem, 21003);//Kalimdor21003
	else if (uiAction == 254)return hearthstone_prepare_transport2(pPlayer, pItem, 21002);//上一页
	else if (uiAction == 252)return hearthstone_prepare_transport2(pPlayer, pItem, 21003);//上一页

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
	
	ChatHandler chatHandler(pPlayer);
	chatHandler.HandleLearnAllMySpellsCommand((char*)"");
	chatHandler.HandleLearnAllMyTalentsCommand((char*)"");

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
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800240, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 119);  // T1-T8套装
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
		case GOSSIP_ACTION_INFO_DEF + 115:learn_default_spell(pPlayer, pItem, uiAction); break;
		//case GOSSIP_ACTION_INFO_DEF + 116:gamedirect(pPlayer, maMountSpell); break;
		case GOSSIP_ACTION_INFO_DEF + 117:explorecheat(pPlayer); break;
		case GOSSIP_ACTION_INFO_DEF + 118:taxicheat(pPlayer); break;
		case GOSSIP_ACTION_INFO_DEF + 119:hearthstone_prepare_itemset(pPlayer, pItem, uiAction); break;
	}
}
void hearthstone_prepare_itemset(Player* pPlayer, Item* pItem, uint32 uiAction){
	pPlayer->PrepareGossipMenu(pPlayer, 65535);//65535是不存在的menuid，数据库中目前最大为50101 关闭不是关键，预处理才会清零。
	switch (pPlayer->getClass())
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
	case 6://死亡骑士
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800480, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 260);  // T7套装 （80级战斗）792  
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800481, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 261);  // T7套装 （80级防御）793 
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800482, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 262);  // T8套装（80级战斗）834
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800483, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 263);  // T8套装（80级防御）835 
		//板甲
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
	case 8://法师
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800510, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 290);  // T1奥术师 (60级) 201   
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800511, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 291);  // T2灵风(60级)  210    
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800512, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 292);  // T3霜火(60级) 526   
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800513, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 293);  // T4占卜者（70级）648   
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800514, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 294);  // T5提里斯法（70级） 649 
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800515, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 295);  // T6风暴（70级）671   
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800516, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 296);  // T7套装 （80级）803  
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800517, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 297);  // T8套装（80级）836
	case 9://术士
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800520, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 300);  // T1恶魔之心 (60级) 203    
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800521, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 301);  // T2复仇(60级)  212     
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800522, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 302);  // T3瘟疫之心(60级) 529    
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800523, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 303);  // T4虚空之心（70级）645    
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800524, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 304);  // T5堕落（70级） 646  
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800525, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 305);  // T6凶星（70级）670    
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800526, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 306);  // T7套装（80级）802  
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, -2800527, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 307);  // T8套装（80级）837
	case 10://德鲁伊
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
	}
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。

}
bool hearthstone_itemset(Player* pPlayer, Item* pItem, uint32 uiAction){
	switch(uiAction){
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
	if (!pPlayer->GetGamePointMgr().checkPoint(500))
		return false;

	ChatHandler(pPlayer).addItemSet(itemset);
	switch (pPlayer->getClass()){
	case 1:pPlayer->learnSpell(750,true); break;//战士 学习板甲.learn 750
	case 2:pPlayer->learnSpell(750, true); break;//圣骑士 // 学习板甲.learn 750
	case 3:pPlayer->learnSpell(8737, true); break;//猎人 //锁甲
	case 6:pPlayer->learnSpell(750, true); break;//死亡骑士 //板甲
	case 7:pPlayer->learnSpell(8737, true); break;//萨满祭司 学习锁甲.learn 8737
	}


	pPlayer->GetGamePointMgr().comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_MOUNT, 500);
	return true;
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
