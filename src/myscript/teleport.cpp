#include "precompiled.h"
#include "teleport.h"
/*


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
(21000,1,'|TInterface\\ICONS\\achievement_zone_zuldrak_05.blp:30|t |cff0000ff�������Ǵ���',1,21001,0),
(21000,2,'|TInterface\\ICONS\\Achievement_BG_winWSG.blp:30|t |cFFFF0000������ʲ������',13,0,0),
(21000,3,'|TInterface\\ICONS\\Achievement_Zone_EasternKingdoms_01.blp:30|t ������������',27,21002,0),
(21000,4,'|TInterface\\ICONS\\Achievement_Zone_Kalimdor_01.blp:30|t ����ķ�����',28,21003,0),
(21000,5,'|TInterface\\ICONS\\Achievement_Boss_PrinceKeleseth.blp:30|t ������˹����',3,21004,0),
(21000,6,'|TInterface\\ICONS\\Achievement_Zone_Blackrock_01.blp:30|t ������˹�ŶӸ���',4,21005,0),
(21000,7,'|TInterface\\ICONS\\Achievement_Zone_Outland_01.blp:30|t �������',5,21006,0),
(21000,8,'|TInterface\\ICONS\\Achievement_boss_murmur.blp:30|t ���򸱱�',6,21007,0),
(21000,9,'|TInterface\\ICONS\\Achievement_Zone_IsleOfQuelDanas.blp:30|t �����ŶӸ���',7,21008,0),
(21000,10,'|TInterface\\ICONS\\Achievement_Zone_Northrend_01.blp:30|t ŵɭ�µ���',8,21009,0),
(21000,11,'|TInterface\\LFGFrame\\LFGIcon-TheForgeofSouls.blp:30|t ŵɭ�¸���',9,21010,0),
(21000,12,'|TInterface\\ICONS\\Achievement_Zone_IceCrown_01.blp:30|t ŵɭ���ŶӸ���',10,21011,0),

(21001,1,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_King_Varian_Wrynn.blp:30|t [����]�����',19,0,22),
(21001,2,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_King_Magni_Bronzebeard.blp:30|t [����]��¯��������',20,0,22),
(21001,3,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_Tyrande_Whisperwind.blp:30|t [����]������˹',21,0,22),
(21001,4,'|cFF0000CC|TInterface\\ICONS\\Achievement_Leader_Prophet_Velen.blp:30|t [����]������',22,0,22),
(21001,5,'|cFF000000|TInterface\\ICONS\\Achievement_Leader_ Thrall.blp:30|t [����]�¸����꣨���ǣ�',23,0,21),
(21001,6,'|cff000000|TInterface\\ICONS\\Achievement_Leader_Cairne Bloodhoof.blp:30|t [����]������',24,0,21),
(21001,7,'|cff000000|TInterface\\ICONS\\Achievement_Leader_Sylvanas.blp:30|t [����]�İ���',25,0,21),
(21001,8,'|cff000000|TInterface\\ICONS\\Achievement_Leader_Lorthemar_Theron .blp:30|t [����]���³�',26,0,21),
(21001,9,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_Tanaris_01.blp:30|t [����]�ӻ�ɭ',200,0,0),
(21001,10,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_ElwynnForest.blp:30|t [����]�ر�����',201,0,0),
(21001,11,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_EversongWoods.blp:30|t [����]ɳ��˹',202,0,0),
(21001,12,'|cFF990066|TInterface\\ICONS\\Achievement_Zone_GrizzlyHills_01.blp:30|t [����]����Ȼ',203,0,0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21002,1,'��������ɽ��',112,0,0),
(21002,2,'����ϣ�ߵ�',113,0,0),
(21002,3,'����֮��',1114,0,0),
(21002,4,'����֮��',115,0,0),
(21002,5,'ȼ��ƽԭ',116,0,0),
(21002,6,'���С��',117,0,0),
(21002,7,'��Ī��',1118,0,0),
(21002,8,'ĺɫɭ��',119,0,0),
(21002,9,'������֮��',120,0,0),
(21002,10,'������ɭ��',121,0,0),
(21002,11,'����ɭ��',122,0,0),
(21002,12,'�Ļ�֮��',123,0,0),
(21002,13,'��ϣ��˹����������',124,0,0),
(21002,14,'��������˹֮��',125,0,0),
(21002,15,'��һҳ',255, 0,21020),

(21020,1,'���Ī��',126,0,0),
(21020,2,'�༹ɽ��',127,0,0),
(21020,3,'����Ͽ��',128,0,0),
(21020,4,'����ɭ��',129,0,0),
(21020,5,'������',130,0,0),
(21020,6,'��������',131,0,0),
(21020,7,'������',132,0,0),
(21020,8,'����˹���ֵ�',133,0,0),
(21020,9,'������֮��',134,0,0),
(21020,10,'������Ұ',135,0,0),
(21020,11,'ʪ��',136,0,0),
(21020,12,'��һҳ',254, 0,21002),


(21003,1,'�ҹ�',137, 0, 0),
(21003,2,'������',138, 0, 0),
(21003,3,'������',139, 0, 0),
(21003,4,'��Ѫ��',140, 0, 0),
(21003,5,'�ں���',141, 0, 0),
(21003,6,'����֮��',142, 0, 0),
(21003,7,'������',143, 0, 0),
(21003,8,'��������',144, 0, 0),
(21003,9,'�����ɭ��',145, 0, 0),
(21003,10,'����˹',146, 0, 0),
(21003,11,'�¹��ֵ�',147, 0, 0),
(21003,12,'Ī����',148, 0, 0),
(21003,13,'ϣ����˹',149, 0, 0),
(21003,14,'��һҳ',253, 0,21021 ),

(21021,1,'ʯצɽ��',150, 0, 0),
(21021,2,'������˹',151, 0, 0),
(21021,3,'̩��ϣ��',152, 0, 0),
(21021,4,'ƶ�֮��',153, 0, 0),
(21021,5,'ǧ��ʯ��',154, 0, 0),
(21021,6,'�����廷��ɽ',155, 0, 0),
(21021,7,'��Ȫ��',156, 0, 0),
(21021,8,'��һҳ',252,0, 21003 ),

(21004,1,'�ڰ���Ԩ',29, 0,  0),
(21004,2,'��ʯ��Ԩ',30, 0,  0),
(21004,3,'����֮�',31, 0,  0),
(21004,4,'ŵĪ���',32, 0,  0),
(21004,5,'������',33, 0,  0),
(21004,6,'ŭ���ѹ�',34, 0,  21),
(21004,7,'�굶�ߵ�',35, 0,  0),
(21004,8,'�굶����',36, 0,  0),
(21004,9,'Ѫɫ�޵�Ժ',37, 0,  0),
(21004,10,'ͨ��ѧԺ',38, 0,  0),
(21004,11,'Ӱ���Ǳ�',39, 0,  0),
(21004,12,'˹̹��ķ',40, 0,  0),
(21004,13,'��û������',41, 0,  0),
(21004,14,'������',42, 0,  0),
(21004,15,'����Ǽ���',43, 0,  22),
(21004,16,'��һҳ',251, 0,  21022);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21022,1,'�´���',44, 0,  0),
(21022,2,'������Ѩ',45, 0,  0),
(21022,3,'���������',46, 0,  0),
(21022,4,'��һҳ',250, 0,  21004),


(21005,1,'����֮��',47, 0,  0),
(21005,2,'�ۻ�֮��',48, 0,  0),
(21005,3,'���ݿ�ϣ�ǵĳ�Ѩ',49, 0,  0),
(21005,4,'����������',50, 0,  0),
(21005,5,'���������',51, 0,  0),
(21005,6,'���������',52, 0, 0),

(21006,1,'ɳ��˹��',11, 0,  0),
(21006,2,'����ɽ��',53, 0,  0),
(21006,3,'������뵺',54, 0,  0),
(21006,4,'�ɸ���',55, 0,  0),
(21006,5,'��շ籩',56, 0, 0),
(21006,6,'Ӱ�¹�',57, 0,  0),
(21006,7,'̩�޿�ɭ��',58, 0,  0),
(21006,8,'�޼�����',59, 0,  0),


(21007,1,'|TInterface\\ICONS\\Achievement_boss_murmur.blp:30|t �½��',60, 0,  0),
(21007,2,'|TInterface\\ICONS\\Achievement_Zone_Tanaris_01.blp:30|t ʱ��֮Ѩ',61, 0,  0),
(21007,3,'|TInterface\\ICONS\\Achievement_Boss_LadyVashj.blp:30|t ����ˮ��',62, 0,  0),
(21007,4,'|TInterface\\ICONS\\Achievement_Boss_KargathBladefist_01.blp:30|t ��������',63, 0,  0),
(21007,5,'|TInterface\\ICONS\\Achievement_Boss_PrinceKeleseth.blp:30|t ħ��ʦƽ̨',64, 0,  0),
(21007,6,'|TInterface\\ICONS\\Achievement_Leader_ Thrall.blp:30|t ��ϣ��˹����������',65, 0,  0),

(21008,1,'|TInterface\\ICONS\\achievement_boss_illidan.blp:30|t �ڰ����',66, 0,  0),
(21008,2,'|TInterface\\ICONS\\Achievement_Boss_Archimonde .blp:30|t ���Ӷ�ɽ',67, 0,  0),
(21008,3,'|TInterface\\ICONS\\achievement_boss_ladyvashj.blp:30|t �������',68, 0,  0),
(21008,4,'|TInterface\\ICONS\\achievement_boss_gruulthedragonkiller.blp:30|t ��³����Ѩ',69, 0,  0),
(21008,5,'|TInterface\\ICONS\\achievement_boss_magtheridon.blp:30|t ��ɪ��ٳ�Ѩ',70, 0,  0),
(21008,6,'|TInterface\\ICONS\\achievement_boss_princemalchezaar_02.blp:30|t ������',71, 0, 0),
(21008,7,'|TInterface\\ICONS\\Achievement_Boss_Kiljaedan.blp:30|t ̫�����ߵ�',72, 0,  0),
(21008,8,'|TInterface\\ICONS\\achievement_character_bloodelf_male.blp:30|t �籩Ҫ��',73, 0,  0),
(21008,9,'|TInterface\\ICONS\\achievement_boss_zuljin.blp:30|t �氢��',74, 0,  0),

(21009,1,'����̦ԭ',75, 0, 0),
(21009,2,'����ɭ��',76, 0, 0),
(21009,3,'���ǻ�Ұ',77, 0, 0),
(21009,4,'��������',78, 0, 0),
(21009,5,'�ݷ�Ͽ��',79, 0, 0),
(21009,6,'�����ʹ�',80, 0, 0),
(21009,7,'���������',81, 0, 0),
(21009,8,'�籩֮��',82, 0, 0),
(21009,9,'��ӵ��',83, 0, 0),
(21009,10,'����',84, 0, 0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21010,1,'|TInterface\\ICONS\\achievement_dungeon_azjoluppercity_normal.blp:30|t ��׿-��³��',85, 0, 0);

insert into gossip_menu_option
(menu_id,id,option_text,option_id,action_menu_id,condition_id)value
(21010,2,'|TInterface\\ICONS\\achievement_dungeon_drak\'tharon_normal.blp:30|t ��������¡Ҫ��',86, 0, 0),
(21010,3,'|TInterface\\ICONS\\achievement_dungeon_gundrak_normal.blp:30|t �Ŵ��',87, 0, 0),
(21010,4,'|TInterface\\ICONS\\achievement_dungeon_cotstratholme_normal.blp:30|t ����˹̹��ķ',88, 0, 0),
(21010,5,'|TInterface\\ICONS\\achievement_dungeon_ulduar80_normal.blp:30|t �������',89, 0, 0),
(21010,6,'|TInterface\\ICONS\\achievement_dungeon_ulduar77_normal.blp:30|t ��ʯ����',90, 0, 0),
(21010,7,'|TInterface\\ICONS\\achievement_dungeon_nexus70_normal.blp:30|t ħ��',91, 0, 0),
(21010,8,'|TInterface\\ICONS\\achievement_dungeon_theviolethold_normal.blp:30|t ����������',92, 0, 0),
(21010,9,'|TInterface\\ICONS\\achievement_dungeon_utgardekeep_normal.blp:30|t ���ؼӵ³Ǳ�',93, 0, 0),
(21010,10,'|TInterface\\ICONS\\achievement_dungeon_utgardepinnacle_normal.blp:30|t ���ؼӵ�֮��',94, 0, 0),

(21011,1,'|TInterface\\ICONS\\Achievement_Zone_IceCrown_01.blp:30|t ���ڱ���',204, 0, 0),
(21011,2,'|TInterface\\LFGFrame\\LFGIcon-ArgentRaid.blp:30|t ʮ�־�������',205, 0, 0),
(21011,3,'|TInterface\\ICONS\\Achievement_Boss_KelThuzad_01.blp:30|t �ɿ�����˹',95, 0, 0),
(21011,4,'|TInterface\\ICONS\\Achievement_Boss_Malygos_01.blp:30|t ����֮��',96, 0, 0),
(21011,5,'|TInterface\\ICONS\\Achievement_Reputation_WyrmrestTemple.blp:30|t ����ʯʥ��',97, 0, 0),
(21011,6,'|TInterface\\ICONS\\achievement_dungeon_ulduarraid_archway_01.blp:30|t �¶Ŷ�',98, 0, 0),
(21011,7,'|TInterface\\ICONS\\Achievement_Dungeon_UlduarRaid_IceGiant_01.blp:30|t ��������ı���',99, 0, 0);

*/
bool hearthstone_prepare_transport2(Player* pPlayer, Item* pItem, uint32 menuid){
	pPlayer->PrepareGossipMenu(pPlayer, menuid);
	pPlayer->SEND_GOSSIP_MENU(65535, pItem->GetObjectGuid()); //����ԭ��ֱ����ϷĿ�ꡣ
	return true;
}
bool hearthstone_transport_case(Player* pPlayer, Item* pItem, uint32 uiAction){

	if (uiAction == 1)return hearthstone_prepare_transport2(pPlayer, pItem, 21001);//����
	else if (uiAction == 3)return hearthstone_prepare_transport2(pPlayer, pItem, 21004); //Azeroth Instances
	else if (uiAction == 251)return hearthstone_prepare_transport2(pPlayer, pItem, 21022); //��һҳ
	else if (uiAction == 250)return hearthstone_prepare_transport2(pPlayer, pItem, 21004); //��һҳ

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
	else if (uiAction == 255)return hearthstone_prepare_transport2(pPlayer, pItem, 21020);//��һҳ
	else if (uiAction == 253)return hearthstone_prepare_transport2(pPlayer, pItem, 21021);//��һҳ

	else if (uiAction == 28)return hearthstone_prepare_transport2(pPlayer, pItem, 21003);//Kalimdor21003
	else if (uiAction == 254)return hearthstone_prepare_transport2(pPlayer, pItem, 21002);//��һҳ
	else if (uiAction == 252)return hearthstone_prepare_transport2(pPlayer, pItem, 21003);//��һҳ

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

	else if (uiAction == 200)return hearthstone_transport(pPlayer, 1, -7174, -3778, 9, 0);//�ӻ�ɭ
	else if (uiAction == 201)return hearthstone_transport(pPlayer, 0, -14281.9, 552.564, 8.90422, 0.860144);//�ر�����
	else if (uiAction == 202)return hearthstone_transport(pPlayer, 530, -1887.62, 5359.09, -12.4279, 4.40435);//ɳ��˹
	else if (uiAction == 203)return hearthstone_transport(pPlayer, 571, 5809.78, 432.913, 658.772, 1.20920);//����Ȼ
	else if (uiAction == 204)return hearthstone_transport(pPlayer, 571, 5855.22, 2102.03, 635.991, 3.57899);//���ڱ���
	else if (uiAction == 205)return hearthstone_transport(pPlayer, 571, 8515.61, 714.153, 558.248, 1.57753);//ʮ�־�������
	
	return true;
}
bool hearthstone_transport(Player* pPlayer, uint32 mapid, double x, double y, double z, double o)
{
	if (!pPlayer->gamePointMgr.checkPoint(1))
	{
		pPlayer->gossipMenuType = -1;/*��ԭĬ��ֵ*/
		return false;
	}
	pPlayer->TeleportTo(mapid, x, y, z, o);
	pPlayer->gamePointMgr.comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_TELE, 1);
	pPlayer->gossipMenuType = -1;/*��ԭĬ��ֵ*/
	return true;
}