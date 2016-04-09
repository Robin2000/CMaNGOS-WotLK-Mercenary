
#include "precompiled.h"
#include <vector>
#include "MercenaryMgr.h"
#include "PetAI.h"
#include "Group.h"
#include "ScriptMgr.h"
#include "mercenary_bot.h"
#include "mercenary_gossip.h"
#include "mercenaryAI.hpp"
#include "Mercenary.h"
class ObjectAccessor;
//#include "ScriptedGossip.h"

enum menu_ction_base{
	GOSSIP_ACTION_SPELL_DEF = 1000,
	GOSSIP_ACTION_RACE_DEF = 1001,
	GOSSIP_ACTION_CLASS_DEF = 102,
	GOSSIP_ACTION_ROLE_DEF = 103,
	MAX_RACE_SEL = 100
};

int getRaceMalStringID(uint8 race)
{
	switch (race){
	case HUMAN:
		return -2800615;
	case ORC:
		return -2800619;
	case DWARF:
		return -2800611;
	case NIGHTELF:
		return -2800617;
	case SCOURGE:
		return -2800674;
	case TAUREN:
		return -2800621;
	case GNOME:
		return -2800613;
	case TROLL:
		return -2800623;
	case BLOODELF:
		return -2800607;
	case DRAENEI:
		return -2800609;
	default:
		return 0;
	}
}
int getRaceFemalStringID(uint8 race)
{
	switch (race){
	case HUMAN:
		return -2800616;
	case ORC:
		return -2800620;
	case DWARF:
		return -2800612;
	case NIGHTELF:
		return -2800618;
	case SCOURGE:
		return -2800675;
	case TAUREN:
		return -2800622;
	case GNOME:
		return -2800614;
	case TROLL:
		return -2800624;
	case BLOODELF:
		return -2800608;
	case DRAENEI:
		return -2800610;
	default:
		return 0;
	}
}

	void SendHireList(Player* player, Item* item)
	{
		if (Pet* pet = player->GetPet())
		{
			if (pet->isMercenary())
			{
				player->GetSession()->SendNotification(player->GetMangosString(-2800634));//�������Ƚ�ɢ��ģ���Ӷ��������
				return;
			}
		}
		player->context.gossipActionType = GOSSIP_ACTION_CLASS_DEF;

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800097, 0, MERCENARY_TYPE_WARRIOR);//սʿ
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800098, 0, MERCENARY_TYPE_PALADIN);//ʥ��ʿ
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800094, 0, MERCENARY_TYPE_HUNTER);//����
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800096, 0, MERCENARY_TYPE_ROGUE); //Ǳ����
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800095, 0, MERCENARY_TYPE_PRIEST);//��ʦ
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800092, 0, MERCENARY_TYPE_DK);//������ʿ
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800099, 0, MERCENARY_TYPE_SHAMAN);//����
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800100, 0, MERCENARY_TYPE_MAGE);//��ʦ
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800101, 0, MERCENARY_TYPE_WARLOCK);//��ʿ
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800093, 0, MERCENARY_TYPE_DRUID);//��³��

		

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, GOSSIP_ACTION_INFO_DEF + 999);//ȡ��
		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());

	}


	void SendFeatureList(Player* player, Item* item, bool races = false )
    {
 
			player->context.gossipActionType = GOSSIP_ACTION_RACE_DEF;
			Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
			tbb::concurrent_unordered_set<uint32> & raceSet=player->context.GetRaceSetByClass(mercenary->GetType());
			for (auto it = raceSet.begin(); it != raceSet.end(); it++)
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getRaceFemalStringID(*it), 0, *it + MAX_RACE_SEL);//Ů
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getRaceMalStringID(*it), 0, *it);//��
			}


			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800606, 0, GOSSIP_ACTION_INFO_DEF +97 ); // Send to SendFeatureList����
 

		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());


    }

	void SendRoleList(Player* player, Item* item, uint8 type)
    {
		player->context.gossipActionType = GOSSIP_ACTION_ROLE_DEF;
		std::vector<MercenaryRoleDef*>* roles = MercenaryUtil::findRoleVectorByClass(type);
		uint8 i = 0;
		for (auto itr = roles->begin(); itr != roles->end(); itr++, i++)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, (*itr)->name, 0, i);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800606, 0, GOSSIP_ACTION_INFO_DEF + 96); // Back to all races & genders����

		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());
    }


    void CreateMercenary(Player* player, Item* item, Mercenary* mercenary, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 classid)
    {
		if (!mercenary->Create(player, model, race, gender, classid, role))
        {
			player->GetSession()->SendNotification(player->GetMangosString(-2800185));//ϵͳ��ʾ�����β���ʧ�ܡ�
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
		if (!pet->isMercenary())
		{
			player->GetSession()->SendNotification(player->GetMangosString(-2800634));//�������Ƚ�ɢ��ĳ���
			return false;
		}
	}

	SendHireList(player, item);

	return true;
}

bool recallMercenary(Player* player, Item* item){
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	if (!mercenary)
	{
		ChatHandler(player).PSendSysMessage(player->GetMangosString(-2800698));//�㻹û����ļ����Ӷ����
		return false;
	}
	player->ResummonPetTemporaryUnSummonedIfAny();//���ȳ���
	CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
	return true;
}
bool GossipSelect_mercenary_npc_gossip(Player* player, Item* item, uint32 /*sender*/, uint32 actions)
{
	player->PlayerTalkClass->ClearMenus();
	switch (actions)
	{
		case GOSSIP_ACTION_INFO_DEF + 998:
			recallMercenary(player, item);
			return true;
	}

	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());

	if (!mercenary)
	{
		mercenary = new Mercenary;
		if (!mercenary->Create(player))/*�����������б��´�ֱ�ӻ�ȡ���ٴ���*/
		{
			WorldSession* session = player->GetSession();
			session->SendNotification(-2800633);//δ����ļ�ɹ�!
			player->context.gossipMenuType = -1;/*��ԭĬ��ֵ*/
			return false;
		}		
	}

	switch (actions)
	{
		case GOSSIP_ACTION_INFO_DEF + 96://���˵�����ѡ��
				SendFeatureList(player, item);
				return true;
		case GOSSIP_ACTION_INFO_DEF + 97:
				SendHireList(player, item);//���˵�ְҵѡ��
				return true;
	}

	if (player->context.gossipActionType == GOSSIP_ACTION_CLASS_DEF)//ѡ��ְҵ
	{
		mercenary->SetType(actions);
		SendFeatureList(player, item);
		return true;
	}

	if (player->context.gossipActionType == GOSSIP_ACTION_RACE_DEF)//ѡ������
	{
		if (actions > MAX_RACE_SEL)
			mercenary->SetValues(mercenary->getCharFemalModel(actions - MAX_RACE_SEL), actions - MAX_RACE_SEL, GENDER_FEMALE);
		else
			mercenary->SetValues(mercenary->getCharMaleModel(actions), actions, GENDER_MALE);

		SendRoleList(player, item, mercenary->GetType());
		return true;
	}
	if (player->context.gossipActionType == GOSSIP_ACTION_ROLE_DEF){//ѡ���ɫ
		std::vector<MercenaryRoleDef*>* roles = MercenaryUtil::findRoleVectorByClass(mercenary->GetType());
		uint8 i = 0;
		for (auto itr = roles->begin(); itr != roles->end(); itr++, i++)
		if (i == actions)
		{
			mercenary->SetRole((*itr)->role);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			break;
		}
		return true;
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
