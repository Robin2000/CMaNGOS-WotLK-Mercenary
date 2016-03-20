/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#include "precompiled.h"
#include <vector>
#include "MercenaryMgr.h"
#include "PetAI.h"
#include "Group.h"
#include "ScriptMgr.h"
#include "mercenary_bot.h"
#include "mercenary_gossip.h"
#include "mercenaryAI.hpp"
class ObjectAccessor;
//#include "ScriptedGossip.h"

/*#ifndef MANGOS
class mercenary_world_load : public WorldScript
{
public:
    mercenary_world_load() : WorldScript("mercenary_world_load") { }

    void OnStartup() override
    {
        sMercenaryMgr->LoadMercenaries();
    }
};

class mercenary_player : public PlayerScript
{
public:
    mercenary_player() : PlayerScript("mercenary_player") { }

    void OnSave(Player* player) override
    {
        Pet* pet = player->GetPet();
        if (!pet)
            return;

        sMercenaryMgr->OnSave(player, pet);
    }
};
#endif
*/
enum menu_ction_base{
	GOSSIP_ACTION_SPELL_DEF = 1000,
	GOSSIP_ACTION_RACE_DEF = 1001,
	GOSSIP_SEL_ROLE_DEF = 900,
	MAX_RACE_SEL=100
};



	void SendHireList(Player* player, Item* item)
	{
			if (Pet* pet = player->GetPet())
	{
		if (pet->GetEntry() == MERCENARY_DEFAULT_ENTRY)
		{
			player->GetSession()->SendNotification(player->GetMangosString(-2800634));//必须首先解散你的（雇佣兵）宠物
			return;
		}
	}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800097, 0, 106);//战士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800098, 0, 107);//圣骑士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800094, 0, 108);//猎人
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800096, 0, 109); //潜行者
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800092, 0, 110);//死亡骑士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800095, 0, 111);//牧师
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800099, 0, 112);//萨满
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800101, 0, 113);//术士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800093, 0, 114);//德鲁伊
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800100, 0, 115);//法师
		

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, GOSSIP_ACTION_INFO_DEF + 999);//取消
		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());

	}
	void SendToHello(Player* player,Item* item)
    {
		if (1 == 1)
		{
			SendHireList(player, item);/*直接弹出选单*/
			return;
		}
		Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());

        if (!mercenary)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800600, 0, 1);//我要雇佣帮手
        else
        {
            //if (mercenary->IsBeingCreated())
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800601, 0, 1);//继续召集雇佣兵
        }
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, GOSSIP_ACTION_INFO_DEF + 999);//取消

		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());

    }




	void SendFeatureList(Player* player, Item* item, bool races = false )
    {
        if (!races)
        {
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800605, 0, 15);//种族
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800606, 0, 98); // Send to SendHireList后退
        }
        else
		{
			player->context.gossipActionType = GOSSIP_ACTION_RACE_DEF;
			Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
			tbb::concurrent_unordered_set<uint32> & raceSet=player->context.GetRaceSetByClass(mercenary->GetType());
			for (auto it = raceSet.begin(); it != raceSet.end(); it++)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800607, 0, *it + MAX_RACE_SEL);//女
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800607, 0, *it );//男
			}


			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800606, 0, 97); // Send to SendFeatureList后退
        }


		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());


    }

	void SendRoleList(Player* player, Item* item, uint8 type)
    {
		std::vector<MercenaryRoleDef*>* roles = MercenaryUtil::findRoleVectorByClass(type);
		uint8 i = 0;
		for (auto itr = roles->begin(); itr != roles->end(); itr++, i++)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (*itr)->name, 0, GOSSIP_SEL_ROLE_DEF + i);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800606, 0, 96); // Back to all races & genders后退

		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());
    }

	void SendConfirmation(Player* player, Item* item)
    {

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800630, 0, 39);//确定吗？
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800631, 0, 40);//重新来
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800632, 0, 41);//暂时不招募雇佣兵了

		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());
    }

    void CreateMercenary(Player* player, Item* item, Mercenary* mercenary, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 classid)
    {
		if (!mercenary->Create(player, model, race, gender, classid, role))
        {
			player->GetSession()->SendNotification(player->GetMangosString(-2800185));//系统提示：本次操作失败。
            return;
        }

        player->CLOSE_GOSSIP_MENU();
        return;
    }



bool GossipHello_mercenary_npc_gossip(Player* player, Item* item)
{
	player->PlayerTalkClass->ClearMenus();
	if (player->isInCombat())
	{
		player->GetSession()->SendNotification(23);//"You are in combat."
		return false;
	}

	if (Pet* pet = player->GetPet())
	{
		if (pet->GetEntry() != MERCENARY_DEFAULT_ENTRY)
		{
			player->GetSession()->SendNotification(player->GetMangosString(-2800634));//必须首先解散你的（雇佣兵）宠物
			return false;
		}
	}

	SendToHello(player, item);

	return true;
}
uint32 getCharFemailModel(uint32 race){
	switch (race){
			case HUMAN:
				return HUMAN_FEMALE_MODEL;
			case ORC:
				return ORC_FEMALE_MODEL;
			case DWARF:
				return DWARF_FEMALE_MODEL;
			case NIGHTELF:
				return NIGHTELF_FEMALE_MODEL;
			case SCOURGE:
				return SCOURGE_FEMALE_MODEL;
			case TAUREN:
				return TAUREN_FEMALE_MODEL;
			case GNOME:
				return GNOME_FEMALE_MODEL;
			case TROLL:
				return TROLL_FEMALE_MODEL;
			case BLOODELF:
				return BLOODELF_FEMALE_MODEL;
			case DRAENEI:
				return DRAENEI_FEMALE_MODEL;
			default:
				return HUMAN_FEMALE_MODEL;
		}
	}
uint32 getCharMaleModel(uint32 race){
	switch (race){
		case HUMAN:
			return HUMAN_MALE_MODEL;
		case ORC:
			return ORC_MALE_MODEL;
		case DWARF:
			return DWARF_MALE_MODEL;
		case NIGHTELF:
			return NIGHTELF_MALE_MODEL;
		case SCOURGE:
			return SCOURGE_MALE_MODEL;
		case TAUREN:
			return TAUREN_MALE_MODEL;
		case GNOME:
			return GNOME_MALE_MODEL;
		case TROLL:
			return TROLL_MALE_MODEL;
		case BLOODELF:
			return BLOODELF_MALE_MODEL;
		case DRAENEI:
			return DRAENEI_MALE_MODEL;
	};
	return HUMAN_MALE_MODEL;
}
bool GossipSelect_mercenary_npc_gossip2(Player* player, Item* item, uint32 actions)
{
	//#ifndef MANGOS
	//        Mercenary* mercenary = GetMercenaryByOwner(player->GetGUID().GetCounter());
	//#else
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	//#endif

	if (!mercenary)
	{
		mercenary = new Mercenary;
		if (!mercenary->Create(player))/*创建并放入列表，下次直接获取不再创建*/
		{
			WorldSession* session = player->GetSession();
			session->SendNotification(-2800633);//未能招募成功!
			player->context.gossipMenuType = -1;/*还原默认值*/
			return false;
		}
	}
	if (player->context.gossipActionType = GOSSIP_ACTION_RACE_DEF)//选择种族
	{
		if (actions > MAX_RACE_SEL)
			mercenary->SetValues(getCharFemailModel(actions - MAX_RACE_SEL), actions - MAX_RACE_SEL, GENDER_FEMALE);
		else
			mercenary->SetValues(getCharMaleModel(actions), actions, GENDER_MALE);

		SendRoleList(player, item, mercenary->GetType());

		player->context.gossipActionType = -1;
		return true;
	}
	if (actions >= GOSSIP_SEL_ROLE_DEF&&actions <= GOSSIP_SEL_ROLE_DEF+5){//
		std::vector<MercenaryRoleDef*>* roles = MercenaryUtil::findRoleVectorByClass(mercenary->GetType());
		uint8 i = 0;
		for (auto itr = roles->begin(); itr != roles->end(); itr++, i++)
		if (GOSSIP_SEL_ROLE_DEF + i == actions)
		{
			mercenary->SetRole((*itr)->role);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			return true;
		}
	}

	switch (actions)
	{
		case 106:
			mercenary->SetType(MERCENARY_TYPE_WARRIOR);
			SendFeatureList(player, item);
			break;
		case 107:
			mercenary->SetType(MERCENARY_TYPE_PALADIN);
			SendFeatureList(player, item);
			break;
		case 108:
			mercenary->SetType(MERCENARY_TYPE_HUNTER);
			SendFeatureList(player, item);
			break;
		case 109:
			mercenary->SetType(MERCENARY_TYPE_ROGUE);
			SendFeatureList(player, item);
			break;
		case 110:
			mercenary->SetType(MERCENARY_TYPE_DK);
			SendFeatureList(player, item);
			break;
		case 111:
			mercenary->SetType(MERCENARY_TYPE_PRIEST);
			SendFeatureList(player, item);
			break;
		case 112:
			mercenary->SetType(MERCENARY_TYPE_SHAMAN);
			SendFeatureList(player, item);
			break;
		case 113:
			mercenary->SetType(MERCENARY_TYPE_WARLOCK);
			SendFeatureList(player, item);
			break;
		case 114:
			mercenary->SetType(MERCENARY_TYPE_DRUID);
			SendFeatureList(player, item);
			break;
		case 115:
			mercenary->SetType(MERCENARY_TYPE_MAGE);
			SendFeatureList(player, item);
			break;
		
		case 39:
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(),
				mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			player->context.gossipMenuType = -1;/*还原默认值*/
			break;

	}

	return true;
}
bool recallMercenary(Player* player, Item* item){
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	if (!mercenary)
	{
		ChatHandler(player).PSendSysMessage(player->GetMangosString(-2800698));//你还没有招募过雇佣兵。
		return false;
	}
	CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
	return true;
}
bool GossipSelect_mercenary_npc_gossip(Player* player, Item* item, uint32 /*sender*/, uint32 actions)
{
	player->PlayerTalkClass->ClearMenus();


	switch (actions)
	{
		case GOSSIP_ACTION_INFO_DEF + 998:/*唤醒雇佣兵*/
			recallMercenary(player, item);
			break;

		case GOSSIP_ACTION_INFO_DEF + 999:////取消
			player->CLOSE_GOSSIP_MENU();
			player->context.gossipMenuType = -1;/*还原默认值*/
			break;
	
		case 94: // Hire Mercenary and choose what's best for it (and you)【选择招募最好的雇佣兵】
			SendHireList(player, item);
			break;
		case 15:
			SendFeatureList(player, item, true);
			break;
	

		case 40:
			SendToHello(player, item);
			break;
		case 41:
			player->CLOSE_GOSSIP_MENU();
			player->context.gossipMenuType = -1;/*还原默认值*/
			break;
		case 96:
			SendFeatureList(player, item, true);
			break;
		case 97:
			SendFeatureList(player, item);
			break;
		case 98:
			SendHireList(player, item);
			break;
		case 99:
			SendToHello(player, item);
			break;
		default:
				GossipSelect_mercenary_npc_gossip2(player,item,actions);
	}

	return true;
}


CreatureAI* GetAI_mercenary_bot(Creature* creature)
{
	return new mercenary_bot_AI(creature);
}

void MercenarySetup()
{

    Script* s;

	s = new Script;
	s->Name = "mercenary_bot";
	s->GetAI = &GetAI_mercenary_bot;
	s->pGossipHello = &OnGossipHello_mercenary_bot;
	s->pGossipSelect = &OnGossipSelect_mercenary_bot;
	s->pGossipSelectWithCode = &OnGossipSelectWithCode_mercenary_bot;
    s->RegisterSelf();

}
