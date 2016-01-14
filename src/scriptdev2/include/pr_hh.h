//TEXT_SOURCE_CUSTOM_START=-20000, TEXT_SOURCE_CUSTOM_END=-30000

//#define PR_HH(chrText)  pr_hh(chrText)
#ifndef __PR_LANGUAGE_H

#define __PR_LANGUAGE_H

enum PrStrings
{
	GO_TELE_TO_DALARAN_CRYSTAL_FAILED = -2800001,
	GOSSIP_ITEM_MOONSTONE = -2800002,
	//D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\examples\example_gossip_codebox.cpp
	GOSSIP_ITEM_codebox_1 = -2800003,	//"A quiz: what's your name?"
	GOSSIP_ITEM_codebox_2 = -2800004,	//"I'm not interested"
	//D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\kalimdor\thousand_needles.cpp
	GOSSIP_ITEM_QUEST = -2800006, //"Please tell me the Phrase.."
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\outland\black_temple\black_temple.cpp
	GOSSIP_OLUM1 = -2800008, //"Teleport me to the other Ashtongue Deathsworn"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\eastern_kingdoms\stormwind_city.cpp
	GOSSIP_ITEM_KAT_1 = -2800009,	//"Pardon the intrusion, Lady Prestor, but Highlord Bolvar suggested that I seek your advice."
	GOSSIP_ITEM_KAT_2 = -2800010,	//"My apologies, Lady Prestor."
	GOSSIP_ITEM_KAT_3 = -2800011,	//"Begging your pardon, Lady Prestor. That was not my intent."
	GOSSIP_ITEM_KAT_4 = -2800012,	//"Thank you for your time, Lady Prestor."
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\world\npcs_special.cpp
	GOSSIP_ITEM_TRICK_OR_TREAT = -2800013, //"Trick or Treat!"
	GOSSIP_ITEM_WHAT_TO_DO = -2800014, //      "What can I do at an Inn?"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\world\npc_professions.cpp
	TALK_MUST_UNLEARN_WEAPON = -2800015,		//    "You must forget your weapon type specialty before I can help you. Go to Everlook in Winterspring and seek help there."
	TALK_HAMMER_LEARN = -2800016,		//           "Ah, a seasoned veteran you once were. I know you are capable, you merely need to ask and I shall teach you the way of the hammersmith."
	TALK_AXE_LEARN = -2800017,		//              "Ah, a seasoned veteran you once were. I know you are capable, you merely need to ask and I shall teach you the way of the axesmith."
	TALK_SWORD_LEARN = -2800018,		//            "Ah, a seasoned veteran you once were. I know you are capable, you merely need to ask and I shall teach you the way of the swordsmith."
	TALK_HAMMER_UNLEARN = -2800019,		//        "Forgetting your Hammersmithing skill is not something to do lightly. If you choose to abandon it you will forget all recipes that require Hammersmithing to create!"
	TALK_AXE_UNLEARN = -2800020,		//            "Forgetting your Axesmithing skill is not something to do lightly. If you choose to abandon it you will forget all recipes that require Axesmithing to create!"
	TALK_SWORD_UNLEARN = -2800021,		//          "Forgetting your Swordsmithing skill is not something to do lightly. If you choose to abandon it you will forget all recipes that require Swordsmithing to create!"
	GOSSIP_WEAPON_LEARN = -2800022,		//         "Please teach me how to become a Weaponsmith"
	GOSSIP_WEAPON_UNLEARN = -2800023,		//       "I wish to unlearn the art of Weaponsmithing"
	GOSSIP_ARMOR_LEARN = -2800024,		//          "Please teach me how to become a Armorsmith"
	GOSSIP_ARMOR_UNLEARN = -2800025,		//        "I wish to unlearn the art of Armorsmithing"
	GOSSIP_UNLEARN_SMITH_SPEC = -2800026,		//   "I wish to unlearn my blacksmith specialty"
	BOX_UNLEARN_ARMORORWEAPON = -2800027,		//   "Do you really want to unlearn your blacksmith specialty and lose all associated recipes? \n Cost: "
	GOSSIP_LEARN_HAMMER = -2800028,		//         "Please teach me how to become a Hammersmith, Lilith"
	GOSSIP_UNLEARN_HAMMER = -2800029,		//       "I wish to unlearn Hammersmithing"
	GOSSIP_LEARN_AXE = -2800030,		//            "Please teach me how to become a Axesmith, Kilram"
	GOSSIP_UNLEARN_AXE = -2800031,		//          "I wish to unlearn Axesmithing"
	GOSSIP_LEARN_SWORD = -2800032,		//          "Please teach me how to become a Swordsmith, Seril"
	GOSSIP_UNLEARN_SWORD = -2800033,		//        "I wish to unlearn Swordsmithing"
	BOX_UNLEARN_WEAPON_SPEC = -2800034,		//     "Do you really want to unlearn your weaponsmith specialty and lose all associated recipes? \n Cost: "
	GOSSIP_LEARN_DRAGON = -2800035,		//         "I am absolutely certain that i want to learn dragonscale leatherworking"
	GOSSIP_UNLEARN_DRAGON = -2800036,		//       "I wish to unlearn Dragonscale Leatherworking"
	GOSSIP_LEARN_ELEMENTAL = -2800037,		//      "I am absolutely certain that i want to learn elemental leatherworking"
	GOSSIP_UNLEARN_ELEMENTAL = -2800038,		//    "I wish to unlearn Elemental Leatherworking"
	GOSSIP_LEARN_TRIBAL = -2800039,		//         "I am absolutely certain that i want to learn tribal leatherworking"
	GOSSIP_UNLEARN_TRIBAL = -2800040,		//       "I wish to unlearn Tribal Leatherworking"
	BOX_UNLEARN_LEATHER_SPEC = -2800041,		//    "Do you really want to unlearn your leatherworking specialty and lose all associated recipes? \n Cost: "
	GOSSIP_LEARN_GOBLIN = -2800042,		//         "I am absolutely certain that i want to learn Goblin engineering"
	GOSSIP_LEARN_GNOMISH = -2800043,		//        "I am absolutely certain that i want to learn Gnomish engineering"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\eastern_kingdoms\blasted_lands.cpp
	GOSSIP_ITEM_FALLEN = -2800044,		// "Continue..."
	GOSSIP_ITEM_FALLEN1 = -2800045,		// "What could be worse than death?"
	GOSSIP_ITEM_FALLEN2 = -2800046,		// "Subordinates?"
	GOSSIP_ITEM_FALLEN3 = -2800047,		// "What are the stones of binding?"
	GOSSIP_ITEM_FALLEN4 = -2800048,		// "You can count on me, Hero"
	GOSSIP_ITEM_FALLEN5 = -2800049,		// "I shall"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\kalimdor\tanaris.cpp
	GOSSIP_ITEM_NORGANNON_1 = -2800050,		//     "What function do you serve?"
	GOSSIP_ITEM_NORGANNON_2 = -2800051,		//     "What are the Plates of Uldum?"
	GOSSIP_ITEM_NORGANNON_3 = -2800052,		//     "Where are the Plates of Uldum?"
	GOSSIP_ITEM_NORGANNON_4 = -2800053,		//     "Excuse me? We've been \"reschedueled for visitations\"? What does that mean?!"
	GOSSIP_ITEM_NORGANNON_5 = -2800054,		//     "So, what's inside Uldum?"
	GOSSIP_ITEM_NORGANNON_6 = -2800055,		//     "I will return when i have the Plates of Uldum."
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\examples\example_escort.cpp
	GOSSIP_ITEM_escor_1 = -2800056,		//   "Click to Test Escort(Attack, Run)"
	GOSSIP_ITEM_escor_2 = -2800057,		//   "Click to Test Escort(NoAttack, Walk)"
	GOSSIP_ITEM_escor_3 = -2800058,		//   "Click to Test Escort(NoAttack, Run)"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\kalimdor\felwood.cpp
	GOSSIP_ITEM_RELEASE = -2800059,		//     "I want to release the corrupted saber to Winna."
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\outland\hellfire_peninsula.cpp
	GOSSIP_ITEM_ATTUNE = -2800060,		//          "Yes, Scryer. You may possess me."
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\include\sc_gossip.h
	GOSSIP_TEXT_BROWSE_GOODS = -2800061,		//        "I'd like to browse your goods."
	GOSSIP_TEXT_TRAIN = -2800062,		//               "Train me!"
	GOSSIP_TEXT_BANK = -2800063,		//                "The bank"
	GOSSIP_TEXT_IRONFORGE_BANK = -2800064,		//      "The bank of Ironforge"
	GOSSIP_TEXT_STORMWIND_BANK = -2800065,		//      "The bank of Stormwind"
	GOSSIP_TEXT_WINDRIDER = -2800066,		//           "The wind rider master"
	GOSSIP_TEXT_GRYPHON = -2800067,		//             "The gryphon master"
	GOSSIP_TEXT_BATHANDLER = -2800068,		//          "The bat handler"
	GOSSIP_TEXT_HIPPOGRYPH = -2800069,		//          "The hippogryph master"
	GOSSIP_TEXT_ZEPPLINMASTER = -2800070,		//       "The zeppelin master"
	GOSSIP_TEXT_DEEPRUNTRAM = -2800071,		//         "The Deeprun Tram"
	GOSSIP_TEXT_FERRY = -2800072,		//               "The Rut'theran Ferry"
	GOSSIP_TEXT_FLIGHTMASTER = -2800073,		//        "The flight master"
	GOSSIP_TEXT_AUCTIONHOUSE = -2800074,		//        "The auction house"
	GOSSIP_TEXT_GUILDMASTER = -2800075,		//         "The guild master"
	GOSSIP_TEXT_INN = -2800076,		//                 "The inn"
	GOSSIP_TEXT_MAILBOX = -2800077,		//             "The mailbox"
	GOSSIP_TEXT_STABLEMASTER = -2800078,		//        "The stable master"
	GOSSIP_TEXT_WEAPONMASTER = -2800079,		//        "The weapon master"
	GOSSIP_TEXT_OFFICERS = -2800080,		//            "The officers' lounge"
	GOSSIP_TEXT_BATTLEMASTER = -2800081,		//        "The battlemaster"
	GOSSIP_TEXT_BARBER = -2800082,		//	"Barber"
	GOSSIP_TEXT_CLASSTRAINER = -2800083,		//        "A class trainer"
	GOSSIP_TEXT_PROFTRAINER = -2800084,		//         "A profession trainer"
	GOSSIP_TEXT_LEXICON = -2800085,		//             "Lexicon of Power"
	GOSSIP_TEXT_ALTERACVALLEY = -2800086,		//       "Alterac Valley"
	GOSSIP_TEXT_ARATHIBASIN = -2800087,		//         "Arathi Basin"
	GOSSIP_TEXT_WARSONGULCH = -2800088,		//    "Warsong Gulch"
	GOSSIP_TEXT_ARENA = -2800089,		//  "Arena"
	GOSSIP_TEXT_EYEOFTHESTORM = -2800090,		//  "Eye of The Storm"
	GOSSIP_TEXT_STRANDOFANCIENT = -2800091,		//   "Strand of the Ancients"
	GOSSIP_TEXT_DEATH_KNIGHT = -2800092,		//  "Death Knight"
	GOSSIP_TEXT_DRUID = -2800093,		// "Druid"
	GOSSIP_TEXT_HUNTER = -2800094,		// "Hunter"
	GOSSIP_TEXT_PRIEST = -2800095,		// "Priest"
	GOSSIP_TEXT_ROGUE = -2800096,		// "Rogue"
	GOSSIP_TEXT_WARRIOR = -2800097,		//  "Warrior"
	GOSSIP_TEXT_PALADIN = -2800098,		//   "Paladin"
	GOSSIP_TEXT_SHAMAN = -2800099,		//  "Shaman"
	GOSSIP_TEXT_MAGE = -2800100,		//   "Mage"
	GOSSIP_TEXT_WARLOCK = -2800101,		//            "Warlock"
	GOSSIP_TEXT_ALCHEMY = -2800102,		//  "Alchemy"
	GOSSIP_TEXT_BLACKSMITHING = -2800103,		// "Blacksmithing"
	GOSSIP_TEXT_COOKING = -2800104,		//   "Cooking"
	GOSSIP_TEXT_ENCHANTING = -2800105,		//    "Enchanting"
	GOSSIP_TEXT_ENGINEERING = -2800106,		//  "Engineering"
	GOSSIP_TEXT_FIRSTAID = -2800107,		// "First Aid"
	GOSSIP_TEXT_HERBALISM = -2800108,		//      "Herbalism"
	GOSSIP_TEXT_LEATHERWORKING = -2800109,		//    "Leatherworking"
	GOSSIP_TEXT_TAILORING = -2800110,		//    "Tailoring"
	GOSSIP_TEXT_MINING = -2800111,		//        "Mining"
	GOSSIP_TEXT_FISHING = -2800112,		//   "Fishing"
	GOSSIP_TEXT_SKINNING = -2800113,		//  "Skinning"
	GOSSIP_TEXT_JEWELCRAFTING = -2800114,		//  "Jewelcrafting"
	GOSSIP_TEXT_INSCRIPTION = -2800115,		//  "Inscription"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\examples\example_creature.cpp
	GOSSIP_ITEM = -2800116,		//    "I'm looking for a fight"
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\eastern_kingdoms\blackrock_depths\blackrock_depths.cpp
	GOSSIP_ITEM_KHARAN_1 = -2800119,		//   "I need to know where the princess are, Kharan!"
	GOSSIP_ITEM_KHARAN_2 = -2800120,		// "All is not lost, Kharan!"
	GOSSIP_ITEM_KHARAN_3 = -2800121,		// "Gor'shak is my friend, you can trust me."
	GOSSIP_ITEM_KHARAN_4 = -2800122,		// "Not enough, you need to tell me more."
	GOSSIP_ITEM_KHARAN_5 = -2800123,		//  "So what happened?"
	GOSSIP_ITEM_KHARAN_6 = -2800124,		// "Continue..."
	GOSSIP_ITEM_KHARAN_7 = -2800125,		// "So you suspect that someone on the inside was involved? That they were tipped off?"
	GOSSIP_ITEM_KHARAN_8 = -2800126,		//  "Continue with your story please."
	GOSSIP_ITEM_KHARAN_9 = -2800127,		//  "Indeed."
	GOSSIP_ITEM_KHARAN_10 = -2800128,		//  "The door is open, Kharan. You are a free man."
	//D:\workspace.github\wow\cmangos\mangos - wotlk\src\scriptdev2\scripts\eastern_kingdoms\burning_steppes.cpp
	ADD_GOSSIP_BURNING_STEPS1 = -2800129,	// "Official business, John. I need some information about Marshal Windsor. Tell me about the last time you saw him."
	ADD_GOSSIP_BURNING_STEPS2 = -2800130,	//  "So what did you do?"
	ADD_GOSSIP_BURNING_STEPS3 = -2800131,	// "Start making sense, dwarf. I don't want to have anything to do with your cracker, your pappy, or any sort of 'discreditin'."
	ADD_GOSSIP_BURNING_STEPS4 = -2800132,	//  "Ironfoe?"
	ADD_GOSSIP_BURNING_STEPS5 = -2800133,	// "Interesting... continue, John."
	ADD_GOSSIP_BURNING_STEPS6 = -2800134,	// "So that's how Windsor died..."
	ADD_GOSSIP_BURNING_STEPS7 = -2800135,	//  "So how did he die?"
	ADD_GOSSIP_BURNING_STEPS8 = -2800136,	// "Ok, so where the hell is he? Wait a minute! Are you drunk?"
	ADD_GOSSIP_BURNING_STEPS9 = -2800137,	// "WHY is he in Blackrock Depths?"
	ADD_GOSSIP_BURNING_STEPS10 = -2800138,	// "300? So the Dark Irons killed him and dragged him into the Depths?"
	ADD_GOSSIP_BURNING_STEPS11 = -2800139,	// "Ahh... Ironfoe."
	ADD_GOSSIP_BURNING_STEPS12 = -2800140,	// "Thanks, Ragged John. Your story was very uplifting and informative."
	//D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\eastern_kingdoms\loch_modan.cpp
	ADD_GOSSIP_ITEM_LOCH_MODAN1 = -2800141,	// "Open the gate please, i need to get to Searing Gorge"
	ADD_GOSSIP_ITEM_LOCH_MODAN2 = -2800142,	// "But i need to get there, now open the gate!"
	ADD_GOSSIP_ITEM_LOCH_MODAN3 = -2800143,	//  "Ok, so what is this other way?"
	ADD_GOSSIP_ITEM_LOCH_MODAN4 = -2800144,	// "Doesn't matter, i'm invulnerable."
	ADD_GOSSIP_ITEM_LOCH_MODAN5 = -2800145,	// "Yes..."
	ADD_GOSSIP_ITEM_LOCH_MODAN6 = -2800146,	//  "Ok, i'll try to remember that."
	ADD_GOSSIP_ITEM_LOCH_MODAN7 = -2800147,	//  "A key? Ok!"

	// D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\kalimdor\felwood.cpp
	GOSSIP_ITEM_RELEASE_felwood = -2800148,	//   "I want to release the corrupted saber to Winna."
	//D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\kalimdor\orgrimmar.cpp
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR1 = -2800150, //"Please share your wisdom with me, Warchief."
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR2 = -2800151, //"What discoveries?"
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR3 = -2800152, // "Usurper?"
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR4 = -2800153, // "With all due respect, Warchief - why not allow them to be destroyed? Does this not strengthen our position?"
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR5 = -2800154, //"I... I did not think of it that way, Warchief."
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR6 = -2800155, //"I live only to serve, Warchief! My life is empty and meaningless without your guidance."
	ADD_GOSSIP_ITEM_LOCH_ORGRIMMAR7 = -2800156, //"Of course, Warchief!"

	//D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\kalimdor\the_barrens.cpp
	ADD_GOSSIP_ITEM_THE_BARRENS1 = -2800157,  //"Examine corpse in detail..."

	//D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\eastern_kingdoms\blasted_lands.cpp
	ADD_GOSSIP_ITEM_blasted_lands1 = -2800158, //	Why are you here ?
	ADD_GOSSIP_ITEM_blasted_lands2 = -2800159, //	Continue story... ?
	ADD_GOSSIP_ITEM_blasted_lands3 = -2800160, //    Why are you here ?

	//  D:\workspace.github\wow\cmangos\mangos-wotlk\src\scriptdev2\scripts\kalimdor\azshara.cpp
	ADD_GOSSIP_ITEM_azshara1 = -2800161, //    Can you help me?
	ADD_GOSSIP_ITEM_azshara2 = -2800162, //    Tell me your story
	ADD_GOSSIP_ITEM_azshara3 = -2800163, //    Please continue
	ADD_GOSSIP_ITEM_azshara4 = -2800164, //    I do not understand
	ADD_GOSSIP_ITEM_azshara5 = -2800165, //    Indeed
	ADD_GOSSIP_ITEM_azshara6 = -2800166, //    I will do this with or your help, Loramus
	ADD_GOSSIP_ITEM_azshara7 = -2800167 //    Yes
	
};

#endif

/*
precompiled.h添加
#include "pr_hh.h"

程序使用
#include "precompiled.h"
pPlayer->GetSession()->SendNotification(GO_TELE_TO_DALARAN_CRYSTAL_FAILED);

数据库脚本：
delete from custom_texts where entry<=-2800001 and entry>=2800002; 

insert into custom_texts(entry, content_default, content_loc4, content_loc5)values
(-2800001, 'This teleport crystal cannot be used until the teleport crystal in Dalaran has been used at least once.', '这个传送水晶只能在达拉然首次使用过之后才能开启。', '这个传送水晶只能在达拉然首次使用过之后才能开启。');

insert into custom_texts(entry, content_default, content_loc4, content_loc5)values
(-2800002, 'Hand over the Southfury moonstone and I\'ll let you go.', '交出月亮石我就放了你。', '交出月亮石我就放了你。');

*/

