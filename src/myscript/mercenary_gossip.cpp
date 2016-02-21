/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#include "precompiled.h"
#include "MercenaryMgr.h"
#include "PetAI.h"
#include "Group.h"
#include "ScriptMgr.h"
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
struct mercenary_npc_gossip : public CreatureAI
{
public:
	mercenary_npc_gossip(Creature* pCreature) : CreatureAI(pCreature) {  }


    RandomMercenary static GetRandomMercenary()
    {
        RandomMercenary rndMerc[] =
        {
            { HUMAN_MALE_MODEL, RACE_HUMAN, GENDER_MALE },
            { HUMAN_FEMALE_MODEL, RACE_HUMAN, GENDER_FEMALE },
            { ORC_MALE_MODEL, RACE_ORC, GENDER_MALE },
            { ORC_FEMALE_MODEL, RACE_ORC, GENDER_FEMALE },
            { DWARF_MALE_MODEL, RACE_DWARF, GENDER_MALE },
            { DWARF_FEMALE_MODEL, RACE_DWARF, GENDER_FEMALE },
            { NIGHTELF_MALE_MODEL, RACE_NIGHTELF, GENDER_MALE },
            { NIGHTELF_FEMALE_MODEL, RACE_NIGHTELF, GENDER_FEMALE },
            { TAUREN_MALE_MODEL, RACE_TAUREN, GENDER_MALE },
            { TAUREN_FEMALE_MODEL, RACE_TAUREN, GENDER_FEMALE },
            { GNOME_MALE_MODEL, RACE_GNOME, GENDER_MALE },
            { GNOME_FEMALE_MODEL, RACE_GNOME, GENDER_FEMALE },
            { TROLL_MALE_MODEL, RACE_TROLL, GENDER_MALE },
            { TROLL_FEMALE_MODEL, RACE_TROLL, GENDER_FEMALE },
            { BLOODELF_MALE_MODEL, RACE_BLOODELF, GENDER_MALE },
            { BLOODELF_FEMALE_MODEL, RACE_BLOODELF, GENDER_FEMALE },
            { DRAENEI_MALE_MODEL, RACE_DRAENEI, GENDER_MALE },
            { DRAENEI_FEMALE_MODEL, RACE_DRAENEI, GENDER_FEMALE }
        };
		return rndMerc[MercenaryUtil::GetMercenaryRandom().Next(0, 18)];
    }

    RandomMercenaryTypeRole static GetRandomTypeAndRole()
    {
        RandomMercenaryTypeRole rndTypeRole[] =
        {
            { MERCENARY_TYPE_WARRIOR, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_WARRIOR, ROLE_TANK },
            { MERCENARY_TYPE_PALADIN, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_PALADIN, ROLE_TANK },
            { MERCENARY_TYPE_PALADIN, ROLE_HEALER },
            { MERCENARY_TYPE_PRIEST, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_PRIEST, ROLE_HEALER },
            { MERCENARY_TYPE_DK, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_DK, ROLE_TANK },
            { MERCENARY_TYPE_WARLOCK, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_MAGE, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_HUNTER, ROLE_MARKSMAN_DPS },
            { MERCENARY_TYPE_ROGUE, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_DRUID, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_DRUID, ROLE_TANK },
            { MERCENARY_TYPE_DRUID, ROLE_HEALER },
            { MERCENARY_TYPE_DRUID, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_SHAMAN, ROLE_MELEE_DPS },
            { MERCENARY_TYPE_SHAMAN, ROLE_CASTER_DPS },
            { MERCENARY_TYPE_SHAMAN, ROLE_HEALER }
        };
		return rndTypeRole[MercenaryUtil::GetMercenaryRandom().Next(0, 20)];
    }

    void static SendToHello(Player* player, Creature* creature)
    {
//#ifndef MANGOS
//        Mercenary* mercenary = GetMercenaryByOwner(player->GetGUID().GetCounter());
//#else
		Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
//#endif
        if (!mercenary)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I want a mercenary.", 0, 1);
        else
        {
            if (mercenary->IsBeingCreated())
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Continue creating your Mercenary", 0, 1);
        }
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 4);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendHireOrOptionalList(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hire random Mercenary", 0, 5);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hire Mercenary and choose what's best for it", 0, 94);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendHireList(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Warrior", 0, 6);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Paladin", 0, 7);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Hunter", 0, 8);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Rogue", 0, 9);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Death Knight", 0, 10);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Priest", 0, 11);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Shaman", 0, 12);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Warlock", 0, 13);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Druid", 0, 14);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 99);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendFeatureList(Player* player, Creature* creature, bool races = false)
    {
        if (!races)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Race", 0, 15);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- Back <-", 0, 98); // Send to SendHireList
        }
        else
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Blood Elf", 0, 16);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Blood Elf", 0, 17);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Draenei", 0, 18);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Draenei", 0, 19);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Dwarf", 0, 20);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Dwarf", 0, 21);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Gnome", 0, 22);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Gnome", 0, 23);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Human", 0, 24);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Human", 0, 25);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Night Elf", 0, 26);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Night Elf", 0, 27);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Orc", 0, 28);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Orc", 0, 29);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Tauren", 0, 30);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Tauren", 0, 31);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Male Troll", 0, 32);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Female Troll", 0, 33);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- Back <-", 0, 97); // Send to SendFeatureList
        }
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendRoleList(Player* player, Creature* creature, uint8 type)
    {
        bool isMelee = (type == MERCENARY_TYPE_DRUID || type == MERCENARY_TYPE_DK || type == MERCENARY_TYPE_WARRIOR
            || type == MERCENARY_TYPE_ROGUE || type == MERCENARY_TYPE_SHAMAN || type == MERCENARY_TYPE_PALADIN);
        bool isCaster = (type == MERCENARY_TYPE_DRUID || type == MERCENARY_TYPE_MAGE || type == MERCENARY_TYPE_WARLOCK
            || type == MERCENARY_TYPE_PRIEST || type == MERCENARY_TYPE_SHAMAN);
        bool isHealer = (type == MERCENARY_TYPE_DRUID || type == MERCENARY_TYPE_PRIEST || type == MERCENARY_TYPE_PALADIN
            || type == MERCENARY_TYPE_SHAMAN);
        bool isTank = (type == MERCENARY_TYPE_WARRIOR || type == MERCENARY_TYPE_DK || type == MERCENARY_TYPE_PALADIN
            || type == MERCENARY_TYPE_DRUID);
        bool isRanged = (type == MERCENARY_TYPE_HUNTER);

        if (isMelee)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Melee DPS", 0, 34);
        if (isCaster)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Caster DPS", 0, 35);
        if (isRanged)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Marksman DPS", 0, 36);
        if (isHealer)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Healer", 0, 37);
        if (isTank)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Tank", 0, 38);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "<- Back <-", 0, 96); // Back to all races & genders
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendConfirmation(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Confirm?", 0, 39);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Start over again", 0, 40);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I don't want to make a mercenary now!", 0, 41);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static CreateMercenary(Player* player, Creature* creature, Mercenary* mercenary, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type)
    {
        if (!mercenary->Create(player, model, race, gender, type, role))
        {
            player->GetSession()->SendNotification("Could not create your Mercenary!");
            SendToHello(player, creature);
            return;
        }

        player->CLOSE_GOSSIP_MENU();
        return;
    }

};


bool GossipHello_mercenary_npc_gossip(Player* player, Creature* creature)
{
	player->PlayerTalkClass->ClearMenus();
	if (player->isInCombat())
	{
		player->GetSession()->SendNotification("You are in combat.");
		return false;
	}

	if (Pet* pet = player->GetPet())
	{
		if (pet->GetEntry() != MERCENARY_DEFAULT_ENTRY)
		{
			player->GetSession()->SendNotification("You must dismiss your pet for a Mercenary.");
			return false;
		}
	}

	mercenary_npc_gossip::SendToHello(player, creature);
	return true;
}

bool GossipSelect_mercenary_npc_gossip(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions)
{
	player->PlayerTalkClass->ClearMenus();
	//#ifndef MANGOS
	//        Mercenary* mercenary = GetMercenaryByOwner(player->GetGUID().GetCounter());
	//#else
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	//#endif
	WorldSession* session = player->GetSession();
	if (!mercenary)
	{
		mercenary = new Mercenary;
		if (!mercenary->Create(player))
		{
			session->SendNotification("Failed to create Mercenary!");
			return false;
		}
	}

	switch (actions)
	{
	case 1: // Hire Mercenary
		mercenary_npc_gossip::SendHireOrOptionalList(player, creature);
		break;
	case 4:
		player->CLOSE_GOSSIP_MENU();
		break;
	case 5: // Hire random Mercenary
	{
				RandomMercenary randomMercenary = mercenary_npc_gossip::GetRandomMercenary();
				RandomMercenaryTypeRole randomClass = mercenary_npc_gossip::GetRandomTypeAndRole();
				mercenary->SetValues(randomMercenary.model, randomMercenary.race, randomMercenary.gender);
				mercenary->SetType(randomClass.type);
				mercenary->SetRole(randomClass.role);
				mercenary_npc_gossip::CreateMercenary(player, creature, mercenary, mercenary->GetDisplay(), mercenary->GetRace(),
					mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
				player->CLOSE_GOSSIP_MENU();
	}break;
	case 94: // Hire Mercenary and choose what's best for it (and you)
		mercenary_npc_gossip::SendHireList(player, creature);
		break;
	case 15:
		mercenary_npc_gossip::SendFeatureList(player, creature, true);
		break;
	case 6:
		mercenary->SetType(MERCENARY_TYPE_WARRIOR);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 7:
		mercenary->SetType(MERCENARY_TYPE_PALADIN);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 8:
		mercenary->SetType(MERCENARY_TYPE_HUNTER);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 9:
		mercenary->SetType(MERCENARY_TYPE_ROGUE);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 10:
		mercenary->SetType(MERCENARY_TYPE_DK);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 11:
		mercenary->SetType(MERCENARY_TYPE_PRIEST);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 12:
		mercenary->SetType(MERCENARY_TYPE_SHAMAN);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 13:
		mercenary->SetType(MERCENARY_TYPE_WARLOCK);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 14:
		mercenary->SetType(MERCENARY_TYPE_DRUID);
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 16:
		mercenary->SetValues(BLOODELF_MALE_MODEL, RACE_BLOODELF, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 17:
		mercenary->SetValues(BLOODELF_FEMALE_MODEL, RACE_BLOODELF, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 18:
		mercenary->SetValues(DRAENEI_MALE_MODEL, RACE_DRAENEI, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 19:
		mercenary->SetValues(DRAENEI_FEMALE_MODEL, RACE_DRAENEI, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 20:
		mercenary->SetValues(DWARF_MALE_MODEL, RACE_DWARF, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 21:
		mercenary->SetValues(DWARF_FEMALE_MODEL, RACE_DWARF, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 22:
		mercenary->SetValues(GNOME_MALE_MODEL, RACE_GNOME, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 23:
		mercenary->SetValues(GNOME_FEMALE_MODEL, RACE_GNOME, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 24:
		mercenary->SetValues(HUMAN_MALE_MODEL, RACE_HUMAN, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 25:
		mercenary->SetValues(HUMAN_FEMALE_MODEL, RACE_HUMAN, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 26:
		mercenary->SetValues(NIGHTELF_MALE_MODEL, RACE_NIGHTELF, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 27:
		mercenary->SetValues(NIGHTELF_FEMALE_MODEL, RACE_NIGHTELF, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 28:
		mercenary->SetValues(ORC_MALE_MODEL, RACE_ORC, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 29:
		mercenary->SetValues(ORC_FEMALE_MODEL, RACE_ORC, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 30:
		mercenary->SetValues(TAUREN_MALE_MODEL, RACE_TAUREN, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 31:
		mercenary->SetValues(TAUREN_FEMALE_MODEL, RACE_TAUREN, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 32:
		mercenary->SetValues(TROLL_MALE_MODEL, RACE_TROLL, GENDER_MALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 33:
		mercenary->SetValues(TROLL_FEMALE_MODEL, RACE_TROLL, GENDER_FEMALE);
		mercenary_npc_gossip::SendRoleList(player, creature, mercenary->GetType());
		break;
	case 34:
		mercenary->SetRole(ROLE_MELEE_DPS);
		mercenary_npc_gossip::SendConfirmation(player, creature);
		break;
	case 35:
		mercenary->SetRole(ROLE_CASTER_DPS);
		mercenary_npc_gossip::SendConfirmation(player, creature);
		break;
	case 36:
		mercenary->SetRole(ROLE_MARKSMAN_DPS);
		mercenary_npc_gossip::SendConfirmation(player, creature);
		break;
	case 37:
		mercenary->SetRole(ROLE_HEALER);
		mercenary_npc_gossip::SendConfirmation(player, creature);
		break;
	case 38:
		mercenary->SetRole(ROLE_TANK);
		mercenary_npc_gossip::SendConfirmation(player, creature);
		break;
	case 39:
		mercenary_npc_gossip::CreateMercenary(player, creature, mercenary, mercenary->GetDisplay(), mercenary->GetRace(),
			mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
		player->CLOSE_GOSSIP_MENU();
		break;
	case 40:
		mercenary_npc_gossip::SendToHello(player, creature);
		break;
	case 41:
		player->CLOSE_GOSSIP_MENU();
		break;
	case 96:
		mercenary_npc_gossip::SendFeatureList(player, creature, true);
		break;
	case 97:
		mercenary_npc_gossip::SendFeatureList(player, creature);
		break;
	case 98:
		mercenary_npc_gossip::SendHireList(player, creature);
		break;
	case 99:
		mercenary_npc_gossip::SendToHello(player, creature);
		break;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct mercenary_bot_AI : public PetAI
{
	mercenary_bot_AI(Creature* creature) : PetAI(creature) { Reset(); }

	uint32 talkTimer;

	void Reset() //ע�⣬�����и�overide�����ָ���û�и÷�����Reset��ʱ���ã����뿼��
	{
		talkTimer = urand(35000, 120000);
		lastMessage = "";

		//#ifndef MANGOS
		//            if (Unit* owner = me->GetOwner())
		//#else
		if (Unit* owner = m_creature->GetOwner())
			//#endif
		{
			//#ifndef MANGOS
			//                mercenary = GetMercenaryByOwner(owner->GetGUID().GetCounter());
			//#else
			mercenary = MercenaryUtil::GetMercenaryByOwner(owner->GetGUIDLow());
			//#endif
			if (mercenary)
			{
				for (auto it = MercenaryUtil::MercenarySpellsBegin(); it != MercenaryUtil::MercenarySpellsEnd(); ++it)
				{
					if (!it->isActive || !it->isDefaultAura)
						continue;

					if (it->type == mercenary->GetType() && it->role == mercenary->GetRole())
						//#ifdef MANGOS
					if (!m_creature->HasAura(it->spellId))
						m_creature->CastSpell(m_creature, it->spellId, true);
					//#else
					//                            if (!me->HasAura(it->spellId))
					//                                me->CastSpell(me, it->spellId, true);
					//#endif
				}
			}
			//#ifndef MANGOS  ����û�����MANGOS�Ĵ�����룿��������������������������������������������
			//                if (Unit* owner = me->GetOwner())
			//                    me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
			//#endif
		}
	}

//#ifndef MANGOS
//	void UpdateAI(uint32 diff) override
//#else
	void UpdateAI(const uint32 diff) override
//#endif
	{
//#ifdef MANGOS
		if (Unit* owner = m_creature->GetOwner())
		if (!m_creature->getVictim())
		if (m_creature->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW) && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
			m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
//#endif
		if (mercenary)
		{
			if (talkTimer <= diff)
			{
				std::vector<MercenaryTalking> tempVector = MercenaryUtil::MercenaryGetTalk(mercenary->GetType(), mercenary->GetRole());
				if (tempVector.size() > 0)
				{
					int rnd = MercenaryUtil::GetMercenaryRandom().Next(0, tempVector.size() - 1);
					int rnd2 = MercenaryUtil::GetMercenaryRandom().Next(0, 100);
					mercenaryTalk = tempVector[rnd];
					if (rnd2 <= 50 && mercenaryTalk.healthPercentageToTalk == 100 && lastMessage != mercenaryTalk.text)
					{
						//#ifndef MANGOS
						//                            me->Say(mercenaryTalk.text.c_str(), LANG_UNIVERSAL);
						//#else
						m_creature->MonsterSay(mercenaryTalk.text.c_str(), LANG_UNIVERSAL);
						//#endif
						lastMessage = mercenaryTalk.text;
					}
				}
				talkTimer = urand(35000, 120000);
			}
			else
				talkTimer -= diff;
		}
		DoMeleeAttackIfReady();
	}
private:
	Mercenary* mercenary;
	MercenaryTalking mercenaryTalk;
	std::string lastMessage;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct mercenary_bot : public CreatureAI
{
public:
	mercenary_bot(Creature* pCreature) : CreatureAI(pCreature) { }

    bool removingSpell;
    void static SendToHello(Player* player, Creature* creature, Mercenary* mercenary)
    {
//#ifndef MANGOS
//        if (mercenary->GetOwnerGUID() == player->GetGUID().GetCounter())
//#else
        if (mercenary->GetOwnerGUID() == player->GetGUIDLow())
//#endif
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Change Gear", 0, 1);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "View equipped Gear", 0, 2);
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Change Spells", 0, 3);
            //player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, "Change Name", 0, 37, "", 0, true);��֧�ָ���
        }
        else
            player->CLOSE_GOSSIP_MENU();
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 4);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendEquipGear(Player* player, Creature* creature, Mercenary* mercenary)
    {
        if (mercenary->GetEditSlot() != SLOT_EMPTY)
            mercenary->SetEditSlot(SLOT_EMPTY);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_HEAD) + MercenaryUtil::GetMercenarySlotName(SLOT_HEAD), 0, 14);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_SHOULDERS) + MercenaryUtil::GetMercenarySlotName(SLOT_SHOULDERS), 0, 13);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_HANDS) + MercenaryUtil::GetMercenarySlotName(SLOT_HANDS), 0, 12);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_FEET) + MercenaryUtil::GetMercenarySlotName(SLOT_FEET), 0, 11);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_LEGS) + MercenaryUtil::GetMercenarySlotName(SLOT_LEGS), 0, 10);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_CHEST) + MercenaryUtil::GetMercenarySlotName(SLOT_CHEST), 0, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_MAIN_HAND) + MercenaryUtil::GetMercenarySlotName(SLOT_MAIN_HAND), 0, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_OFF_HAND)+MercenaryUtil::GetMercenarySlotName(SLOT_OFF_HAND), 0, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_RANGED) + MercenaryUtil::GetMercenarySlotName(SLOT_RANGED), 0, 6);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind", 0, 36);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendItemList(Player* player, Creature* creature, Mercenary* mercenary, uint8 slot)
    {
        mercenary->SetEditSlot(slot);
        std::vector<uint32> tempVector = mercenary->GetEquippableItems(player, slot);
        for (auto itr = tempVector.begin(); itr != tempVector.end(); ++itr)
        {
            if (Item* item = player->GetItemByEntry(*itr))
            {
                std::ostringstream ss;
                if (mercenary->GetItemBySlot(slot) != item->GetEntry())
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(item->GetEntry()) + MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), player->GetSession()), 0, item->GetEntry());
                else
                {
					ss << MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), player->GetSession()) << " [|cff990000Already Equipped|r]";
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(item->GetEntry()) + ss.str().c_str(), 0, 36);
                }
            }
        }

        if (slot == SLOT_OFF_HAND && mercenary->HasWeapon(true))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Remove Off Hand Weapon", 0, 5);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back to Main Menu", 0, 36);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendSpellList(Player* player, Creature* creature, Mercenary* mercenary)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teach your Mercenary a new spell", 0, 39);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Make your Mercenary Unlearn a spell", 0, 40);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back to Main Menu", 0, 36);
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

};

bool OnGossipHello_mercenary_bot(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	if (player->isInCombat())
	{
		player->GetSession()->SendNotification("You are in combat.");
		return false;
	}

	//#ifndef MANGOS
	//        Mercenary* mercenary = GetMercenaryByOwner(player->GetGUID().GetCounter());
	//#else
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	//#endif
	if (!mercenary)
		return false;

	mercenary_bot::SendToHello(player, creature, mercenary);
	return true;
}

bool OnGossipSelect_mercenary_bot(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions) 
{
	player->PlayerTalkClass->ClearMenus();
	//#ifndef MANGOS
	//        Mercenary* mercenary = GetMercenaryByOwner(player->GetGUID().GetCounter());
	//#else
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	//#endif
	WorldSession* session = player->GetSession();
	if (mercenary)
	{
		if (mercenary->GetEditSlot() != SLOT_EMPTY)
		{
			std::vector<uint32> tempVector = mercenary->GetEquippableItems(player, mercenary->GetEditSlot());
			for (auto itr = tempVector.begin(); itr != tempVector.end(); ++itr)
			{
				if (Item* item = player->GetItemByEntry(*itr))
				{
					if (actions == item->GetEntry())
					{
						player->CLOSE_GOSSIP_MENU();
						if (!mercenary->CanEquipItem(player, item))
							return false;
						else
						{
							ChatHandler(session).PSendSysMessage("Successfully equipped %s to your Mercenary!", MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), session).c_str());
							mercenary->SetEditSlot(SLOT_EMPTY);
							return false;
						}
					}
				}
			}
		}

		for (auto it = MercenaryUtil::MercenarySpellsBegin(); it != MercenaryUtil::MercenarySpellsEnd(); ++it)
		{
			if (!it->isActive || it->isDefaultAura)
				continue;

			if (mercenary->GetType() == it->type && mercenary->GetRole() == it->role)
			{
				if (actions == it->spellId)
				{
					if (creature->removingSpell)
					{
						Pet* pet = (Pet*)creature;
						pet->removeSpell(it->spellId, false);
						creature->removingSpell = false;
					}
					else
						mercenary->LearnSpell(player, it->spellId);
					player->CLOSE_GOSSIP_MENU();
					return false;
				}
			}
		}
	}

	switch (actions)
	{
	case 1:
		mercenary_bot::SendEquipGear(player, creature, mercenary);
		break;
	case 2:
		for (auto itr = mercenary->GearBegin(); itr != mercenary->GearEnd(); ++itr)
		{
			//#ifndef MANGOS
			//                    const ItemTemplate* proto = sObjectMgr->GetItemTemplate(itr->itemId);
			//#else
			const ItemPrototype* proto = MercenaryUtil::GetItemPrototype(itr->itemId);
			//#endif
			if (proto)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(itr->itemId) + MercenaryUtil::GetMercenaryItemLink(itr->itemId, session), 0, 36);
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "<- Back <-", 0, 36);
		//#ifndef MANGOS
		//                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		//#else
		player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
		//#endif
		break;
	case 3:
		mercenary_bot::SendSpellList(player, creature, mercenary);
		break;
	case 4:
		player->CLOSE_GOSSIP_MENU();
		break;
	case 5:
		mercenary->RemoveOffHand(player->GetPet());
		player->CLOSE_GOSSIP_MENU();
		break;
	case 6: // Equip Ranged
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_RANGED);
		break;
	case 7: // Equip Off Hand
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_OFF_HAND);
		break;
	case 8: // Equip Main Hand
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_MAIN_HAND);
		break;
	case 9: // Equip Chest
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_CHEST);
		break;
	case 10: // Equip Legs
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_LEGS);
		break;
	case 11: // Equip Feet
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_FEET);
		break;
	case 12: // Equip Hands
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_HANDS);
		break;
	case 13: // Equip Shoulders
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_SHOULDERS);
		break;
	case 14: // Equip Head
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_HEAD);
		break;
	case 36:
		mercenary_bot::SendToHello(player, creature, mercenary);
		break;
	case 39:
	case 40:
		Pet* pet = (Pet*)creature;
		for (auto it = MercenaryUtil::MercenarySpellsBegin(); it != MercenaryUtil::MercenarySpellsEnd(); ++it)
		{
			if (!it->isActive || it->isDefaultAura)
				continue;

			if (mercenary->GetType() == it->type && mercenary->GetRole() == it->role)
			{
				if (actions == 39)
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySpellIcon(it->spellId, player->GetSession()), 0, it->spellId);
				else
				{
					if (pet->HasSpell(it->spellId))
					{
						player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySpellIcon(it->spellId, player->GetSession()) + " [Unlearn]", 0, it->spellId);
						creature->removingSpell = true;
					}
				}
			}
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Back to Main Menu", 0, 36);
		//#ifndef MANGOS
		//                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		//#else
		player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
		//#endif
		break;
	}

	return true;
}
/*
bool OnGossipSelectWithCode_mercenary_bot(Player* player, Creature* creature, uint32 sender, uint32 actions, const char* code)
{
	player->PlayerTalkClass->ClearMenus();
	if (actions == 37)
	{
		Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
		if (!mercenary)
			return false;

		Pet* pet = player->GetPet();
		if (!pet)
			return false;

		std::string name = code;
		WorldSession* session = player->GetSession();
		if (name.empty())
		{
			session->SendNotification("Name is empty! Failed to change your Mercenary's name.");
			return false;
		}

		if (name.length() <= 3)
		{
			session->SendNotification("Name must contain more than 3 characters. Failed to change your Mercenary's name.");
			return false;
		}

		if (name.length() > 21)
		{
			session->SendNotification("Name is too long. Failed to change your Mercenary's name.");
			return false;
		}

		for (uint8 i = 0; i < strlen(name.c_str()); ++i)
		{
			if (!isalpha(name[i]) && name[i] != ' ')
			{
				session->SendNotification("Name must contain letters only. Failed to change your Mercenary's name.");
				return false;
			}
		}

		creature->SetName(name);

		if (player->GetGroup())
			player->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);


		CharacterDatabase.escape_string(name);
		CharacterDatabase.PExecute("UPDATE character_pet SET name='%s' WHERE Id='%u' AND owner='%u'", name.c_str(), mercenary->GetId(), player->GetGUIDLow());
		//#endif
		pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL)));
	}

	player->CLOSE_GOSSIP_MENU();
	return true;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct mercenary_world_gossip : public CreatureAI
{
public:
	mercenary_world_gossip(Creature* pCreature) : CreatureAI(pCreature) {  }
 
    void static CreateMercenary(Player* player, Creature* creature, Mercenary* mercenary, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type)
    {
        if (!mercenary->Create(player, model, race, gender, type, role, creature->GetName()))
        {
            player->GetSession()->SendNotification("Could not hire this Mercenary!");
            player->CLOSE_GOSSIP_MENU();
            return;
        }
    }
	
};

bool OnGossipHello_mercenary_world_gossip(Player* player, Creature* creature) 
{
	player->PlayerTalkClass->ClearMenus();
	if (player->isInCombat())
	{
		player->GetSession()->SendNotification("You are in combat.");
		return false;
	}

	if (Pet* pet = player->GetPet())
	{
		if (pet->GetEntry() != MERCENARY_DEFAULT_ENTRY)
		{
			player->GetSession()->SendNotification("You must dismiss your pet for a Mercenary.");
			return false;
		}
	}

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to hire you.", 0, 1);
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nevermind.", 0, 2);
	//#ifndef MANGOS
	//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
	//#else
	player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
	//#endif
	return true;
}

bool OnGossipSelect_mercenary_world_gossip(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions) 
{
	player->PlayerTalkClass->ClearMenus();
	//#ifndef MANGOS
	//        Mercenary* mercenary = GetMercenaryByOwner(player->GetGUID().GetCounter());
	//#else
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	//#endif
	if (mercenary)
	{
		//#ifdef MANGOS
		creature->MonsterSay("I thought you wanted me as your Mercenary? Get rid of your existing Mercenary!", LANG_UNIVERSAL);
		//#else
		//            creature->Say("I thought you wanted me as your Mercenary? Get rid of your existing Mercenary!", LANG_UNIVERSAL);
		//#endif
		player->CLOSE_GOSSIP_MENU();
		return false;
	}

	MercenaryStarterGear* gear = MercenaryUtil::GetMercenaryStarterGearByEntry(creature->GetEntry());
	if (!gear)
	{
		player->CLOSE_GOSSIP_MENU();
		return false;
	}

	MercenaryWorld* worldData = MercenaryUtil::GetMercenaryWorldData(creature->GetEntry());
	if (!worldData)
	{
		player->CLOSE_GOSSIP_MENU();
		return false;
	}

	if (actions == 1)
	{
		Mercenary* mercenary = new Mercenary();
		mercenary_world_gossip::CreateMercenary(player, creature, mercenary, worldData->modelId, worldData->race, worldData->gender, gear->mercenaryRole, gear->mercenaryType);
	}

	player->CLOSE_GOSSIP_MENU();
	return true;
}
//////////////////////////////////////////////////////////////////////////////////
CreatureAI* GetAI_mercenary_npc_gossip(Creature* pCreature)
{
	return new mercenary_npc_gossip(pCreature);
}

CreatureAI* GetAI_mercenary_bot(Creature* creature)
{
	return new mercenary_bot_AI(creature);
}
CreatureAI* GetAI_mercenary_world_gossip(Creature* pCreature)
{
	return new mercenary_world_gossip(pCreature);
}

void MercenarySetup()
{
/*#ifndef MANGOS
    new mercenary_npc_gossip;
    new mercenary_bot;
    new mercenary_world_load;
    new mercenary_player;
    new mercenary_world_gossip;
#else*/
    Script* s;
	s = new Script;
	s->Name = "mercenary_npc_gossip";
	s->GetAI = &GetAI_mercenary_npc_gossip;
	s->pGossipHello = &GossipHello_mercenary_npc_gossip;
	s->pGossipSelect = &GossipSelect_mercenary_npc_gossip;
	//s->pGossipSelectWithCode = &OnGossipSelectWithCode_mercenary_bot;
    s->RegisterSelf();

	s = new Script;
	s->Name = "mercenary_bot";
	s->GetAI = &GetAI_mercenary_bot;
	s->pGossipHello = &OnGossipHello_mercenary_bot;
	s->pGossipSelect = &OnGossipSelect_mercenary_bot;
    s->RegisterSelf();




    s = new Script;
	s->Name = "mercenary_world_gossip";
	s->GetAI = &GetAI_mercenary_world_gossip;
	s->pGossipHello = &OnGossipHello_mercenary_world_gossip;
	s->pGossipSelect = &OnGossipSelect_mercenary_world_gossip;
    s->RegisterSelf();
//#endif
}