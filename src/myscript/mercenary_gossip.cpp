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


    RandomMercenary GetRandomMercenary()
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

    RandomMercenaryTypeRole GetRandomTypeAndRole()
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
	void SendHireList(Player* player, Item* item)
	{
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800097, 0, 6);//战士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800098, 0, 7);//圣骑士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800094, 0, 8);//猎人
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800096, 0, 9); //潜行者
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800092, 0, 10);//死亡骑士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800095, 0, 11);//牧师
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800099, 0, 12);//萨满
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800101, 0, 13);//术士
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800093, 0, 14);//德鲁伊

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

	void SendHireOrOptionalList(Player* player,Item* item )
    {
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800603, 0, 5);//随机招募雇佣兵
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800604, 0, 94);//选择最好的招募雇佣兵
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
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800607, 0, 16);//血精灵（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800608, 0, 17);//血精灵（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800609, 0, 18); //德莱尼（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800610, 0, 19);//德莱尼（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800611, 0, 20);//小矮人（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800612, 0, 21);//小矮人（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800613, 0, 22);//侏儒（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800614, 0, 23);//侏儒（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800615, 0, 24);//人类（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800616, 0, 25);//人类（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800617, 0, 26);//暗夜精灵（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800618, 0, 27);//暗夜精灵（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800619, 0, 28);//兽人（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800620, 0, 29);//兽人（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800621, 0, 30); //牛头人（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800622, 0, 31);//牛头人（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800623, 0, 32);//巨魔（男）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800624, 0, 33);//巨魔（女）
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, -2800606, 0, 97); // Send to SendFeatureList后退
        }


		player->SEND_GOSSIP_MENU(1, item->GetObjectGuid());


    }

	void SendRoleList(Player* player, Item* item, uint8 type)
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
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800625, 0, 34);//近战输出（DPS）
        if (isCaster)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800626, 0, 35); // 远程法攻（DPS）
        if (isRanged)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800627, 0, 36);//远程箭攻（DPS）
        if (isHealer)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800628, 0, 37);//治疗(医)
        if (isTank)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800629, 0, 38);//坦克(肉盾)
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

    void CreateMercenary(Player* player, Item* item, Mercenary* mercenary, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type)
    {
        if (!mercenary->Create(player, model, race, gender, type, role))
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
		if (!mercenary->Create(player))
		{
			WorldSession* session = player->GetSession();
			session->SendNotification(-2800633);//未能招募成功!
			player->gossipMenuType = -1;/*还原默认值*/
			return false;
		}
	}

	switch (actions)
	{
		case 5: // Hire random Mercenary【随机雇佣】
		{
					RandomMercenary randomMercenary = GetRandomMercenary();
					RandomMercenaryTypeRole randomClass = GetRandomTypeAndRole();
					mercenary->SetValues(randomMercenary.model, randomMercenary.race, randomMercenary.gender);
					mercenary->SetType(randomClass.type);
					mercenary->SetRole(randomClass.role);
					CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(),
						mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
					player->CLOSE_GOSSIP_MENU();
					player->gossipMenuType = -1;/*还原默认值*/
		}break;
		case 6:
			mercenary->SetType(MERCENARY_TYPE_WARRIOR);
			SendFeatureList(player, item);
			break;
		case 7:
			mercenary->SetType(MERCENARY_TYPE_PALADIN);
			SendFeatureList(player, item);
			break;
		case 8:
			mercenary->SetType(MERCENARY_TYPE_HUNTER);
			SendFeatureList(player, item);
			break;
		case 9:
			mercenary->SetType(MERCENARY_TYPE_ROGUE);
			SendFeatureList(player, item);
			break;
		case 10:
			mercenary->SetType(MERCENARY_TYPE_DK);
			SendFeatureList(player, item);
			break;
		case 11:
			mercenary->SetType(MERCENARY_TYPE_PRIEST);
			SendFeatureList(player, item);
			break;
		case 12:
			mercenary->SetType(MERCENARY_TYPE_SHAMAN);
			SendFeatureList(player, item);
			break;
		case 13:
			mercenary->SetType(MERCENARY_TYPE_WARLOCK);
			SendFeatureList(player, item);
			break;
		case 14:
			mercenary->SetType(MERCENARY_TYPE_DRUID);
			SendFeatureList(player, item);
			break;
		case 16:
			mercenary->SetValues(BLOODELF_MALE_MODEL, RACE_BLOODELF, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 17:
			mercenary->SetValues(BLOODELF_FEMALE_MODEL, RACE_BLOODELF, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 18:
			mercenary->SetValues(DRAENEI_MALE_MODEL, RACE_DRAENEI, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 19:
			mercenary->SetValues(DRAENEI_FEMALE_MODEL, RACE_DRAENEI, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 20:
			mercenary->SetValues(DWARF_MALE_MODEL, RACE_DWARF, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 21:
			mercenary->SetValues(DWARF_FEMALE_MODEL, RACE_DWARF, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 22:
			mercenary->SetValues(GNOME_MALE_MODEL, RACE_GNOME, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 23:
			mercenary->SetValues(GNOME_FEMALE_MODEL, RACE_GNOME, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 24:
			mercenary->SetValues(HUMAN_MALE_MODEL, RACE_HUMAN, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 25:
			mercenary->SetValues(HUMAN_FEMALE_MODEL, RACE_HUMAN, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 26:
			mercenary->SetValues(NIGHTELF_MALE_MODEL, RACE_NIGHTELF, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 27:
			mercenary->SetValues(NIGHTELF_FEMALE_MODEL, RACE_NIGHTELF, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 28:
			mercenary->SetValues(ORC_MALE_MODEL, RACE_ORC, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 29:
			mercenary->SetValues(ORC_FEMALE_MODEL, RACE_ORC, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 30:
			mercenary->SetValues(TAUREN_MALE_MODEL, RACE_TAUREN, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 31:
			mercenary->SetValues(TAUREN_FEMALE_MODEL, RACE_TAUREN, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 32:
			mercenary->SetValues(TROLL_MALE_MODEL, RACE_TROLL, GENDER_MALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 33:
			mercenary->SetValues(TROLL_FEMALE_MODEL, RACE_TROLL, GENDER_FEMALE);
			SendRoleList(player, item, mercenary->GetType());
			break;
		case 34:
			mercenary->SetRole(ROLE_MELEE_DPS);
			//SendConfirmation(player, item);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 35:
			mercenary->SetRole(ROLE_CASTER_DPS);
			//SendConfirmation(player, item);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 36:
			mercenary->SetRole(ROLE_MARKSMAN_DPS);
			//SendConfirmation(player, item);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 37:
			mercenary->SetRole(ROLE_HEALER);
			//SendConfirmation(player, item);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 38:
			mercenary->SetRole(ROLE_TANK);
			//SendConfirmation(player, item);
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(), mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			break;
		case 39:
			CreateMercenary(player, item, mercenary, mercenary->GetDisplay(), mercenary->GetRace(),
				mercenary->GetGender(), mercenary->GetRole(), mercenary->GetType());
			player->CLOSE_GOSSIP_MENU();
			player->gossipMenuType = -1;/*还原默认值*/
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
		case 1: // Hire Mercenary【//我要雇佣帮手+继续雇佣】
				SendHireOrOptionalList(player,item);
			break;
		case GOSSIP_ACTION_INFO_DEF + 999:////取消
			player->CLOSE_GOSSIP_MENU();
			player->gossipMenuType = -1;/*还原默认值*/
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
			player->gossipMenuType = -1;/*还原默认值*/
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct mercenary_bot_AI : public PetAI
{
	mercenary_bot_AI(Creature* creature) : PetAI(creature) { Reset(); }

	uint32 talkTimer;

	void Reset() //注意，这里有个overide，发现父类没有该方法，Reset何时调用，必须考虑
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
			//#ifndef MANGOS  这里没有针对MANGOS的处理代码？？？？？？？？？？？？？？？？？？？？？？？
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
			m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, -PET_FOLLOW_ANGLE);
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
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800635, 0, 1);//更换装备
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800636, 0, 2);//查看装备
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800637, 0, 3); // 更换技能
			player->ADD_GOSSIP_ITEM_EXTENDED(GOSSIP_ICON_CHAT, -2800584, 0, 37, player->GetMangosString(-2800584), 0, true);//雇佣兵改名
        }
        else
            player->CLOSE_GOSSIP_MENU();
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, 4);//取消
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
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_HEAD) + MercenaryUtil::GetMercenarySlotName(player,SLOT_HEAD), 0, 14);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_SHOULDERS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_SHOULDERS), 0, 13);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_HANDS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_HANDS), 0, 12);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_FEET) + MercenaryUtil::GetMercenarySlotName(player, SLOT_FEET), 0, 11);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_LEGS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_LEGS), 0, 10);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_CHEST) + MercenaryUtil::GetMercenarySlotName(player, SLOT_CHEST), 0, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_MAIN_HAND) + MercenaryUtil::GetMercenarySlotName(player, SLOT_MAIN_HAND), 0, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_OFF_HAND) + MercenaryUtil::GetMercenarySlotName(player, SLOT_OFF_HAND), 0, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_RANGED) + MercenaryUtil::GetMercenarySlotName(player, SLOT_RANGED), 0, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, 36);//取消
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
					ss << MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), player->GetSession()) << player->GetMangosString(-2800638);// [|cff990000Already Equipped|r]
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(item->GetEntry()) + ss.str().c_str(), 0, 36);
                }
            }
        }

        if (slot == SLOT_OFF_HAND && mercenary->HasWeapon(true))
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800639, 0, 5);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800181, 0, 36);//"Back to Main Menu"
//#ifndef MANGOS
//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
//#else
        player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
//#endif
    }

    void static SendSpellList(Player* player, Creature* creature, Mercenary* mercenary)
    {
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800640, 0, 39);//教雇佣兵新技能法术
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800641, 0, 40);//遗忘雇佣兵的技能法术
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800181, 0, 36);//"Back to Main Menu"
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
		player->GetSession()->SendNotification(23); // You are in combat.
		return false;
	}
	
	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	if (!mercenary)
		return false;
	mercenary_bot::SendToHello(player, creature, mercenary);
	return true;
}
bool learnOrUnlearnSpell(Mercenary* mercenary ,Player* player, Creature* creature, uint32 actions){
	/*以下为数据库配置技能*/
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
				}
				else
				{
					mercenary->LearnSpell(player, it->spellId);
				}
				player->CLOSE_GOSSIP_MENU();
				return false;
			}
		}
	}
	/*以下为职业默认技能*/
	std::vector<uint32>& defaultSpellVec = std::vector<uint32>();
	ChatHandler(player).getDefaultSpells(defaultSpellVec, mercenary->GetRace(), mercenary->GetType());
	for (auto itr = defaultSpellVec.begin(); itr != defaultSpellVec.end(); ++itr)
	{
		if (actions == *itr)
		{
			if (creature->removingSpell)
			{
				Pet* pet = (Pet*)creature;
				pet->removeSpell(*itr, false);
			}
			else
				mercenary->LearnSpell(player, *itr);
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
	}

	if (creature->removingSpell)
		creature->removingSpell = false;
	else
		ChatHandler(player).PSendSysMessage(player->GetMangosString(-2800585));//学习成功，右键点击宠物技能图标启用。

	return true;
}

void addSpellMenu(Player* player, Mercenary* mercenary, Creature* creature, uint32 actions){
	Pet* pet = (Pet*)creature;
	/*以下为数据库配置技能*/
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
	/*以下为职业默认技能*/
	std::vector<uint32>& defaultSpellVec = std::vector<uint32>();
	ChatHandler(player).getDefaultSpells(defaultSpellVec, mercenary->GetRace(), mercenary->GetType());
	for (auto itr = defaultSpellVec.begin(); itr != defaultSpellVec.end(); ++itr)
	{
		if (actions == 39)
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySpellIcon(*itr, player->GetSession()), 0, *itr);
		else
		{
			if (pet->HasSpell(*itr))
			{
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySpellIcon(*itr, player->GetSession()) + " [Unlearn]", 0, *itr);
				creature->removingSpell = true;
			}
		}
	}
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800181, 0, 36);//"Back to Main Menu"
	//#ifndef MANGOS
	//                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
	//#else
	player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
	//#endif
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
		if (mercenary->GetEditSlot() != SLOT_EMPTY) /*mercenary中标记变量editSlot标明actions是否为编辑装备*/
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
							//成功给你的雇佣兵装备了 %s !
							ChatHandler(session).PSendSysMessage(player->GetMangosString(-2800642), MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), session).c_str());
							mercenary->SetEditSlot(SLOT_EMPTY);
							return false;
						}
					}
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
				const ItemPrototype* proto = MercenaryUtil::GetItemPrototype(itr->second);
				if (proto)
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(itr->second) + MercenaryUtil::GetMercenaryItemLink(itr->second, session), 0, 36);
			}
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800606, 0, 36);//后退
			player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
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
			if (mercenary)
				addSpellMenu(player, mercenary, creature, actions);/*学习39或者遗忘40菜单*/
			break;
		default:
			if (mercenary)
				learnOrUnlearnSpell(mercenary, player, creature, actions);/*如果上面都不匹配，actions代表的就是学习或者遗忘的技能*/
	}

	return true;
}
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
		if (!ChatHandler(player).checkPetName(name))
			return false;

		creature->SetName(name);

		if (player->GetGroup())
			player->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);


		//CharacterDatabase.escape_string(name);
		//CharacterDatabase.PExecute("UPDATE character_pet SET name='%s' WHERE Id='%u' AND owner='%u'", name.c_str(), mercenary->GetId(), player->GetGUIDLow());
		//#endif
		pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL)));
	}

	//player->CLOSE_GOSSIP_MENU();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct mercenary_world_gossip : public CreatureAI
{
public:
	mercenary_world_gossip(Creature* pCreature) : CreatureAI(pCreature) {  }
 
    void static CreateMercenary(Player* player, Creature* creature, Mercenary* mercenary, uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type)
    {
        if (!mercenary->Create(player, model, race, gender, type, role, creature->GetName()))
        {
			player->GetSession()->SendNotification(-2800643);//不能招募这个雇佣兵!
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
		player->GetSession()->SendNotification(23);
		return false;
	}

	if (Pet* pet = player->GetPet())
	{
		if (pet->GetEntry() != MERCENARY_DEFAULT_ENTRY)
		{
			player->GetSession()->SendNotification(player->GetMangosString(-2800634));//"You must dismiss your pet for a Mercenary."
			return false;
		}
	}

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800644, 0, 1);// "I would like to hire you."
	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, 2);
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
		creature->MonsterSay(player->GetMangosString(-2800645), LANG_UNIVERSAL);//"I thought you wanted me as your Mercenary? Get rid of your existing Mercenary!"
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
	//s = new Script;
	//s->Name = "mercenary_npc_gossip";
	//s->GetAI = &GetAI_mercenary_npc_gossip;
	//s->pGossipHello = &GossipHello_mercenary_npc_gossip;
	//s->pGossipSelect = &GossipSelect_mercenary_npc_gossip;
	//s->pGossipSelectWithCode = &OnGossipSelectWithCode_mercenary_bot;
    //s->RegisterSelf();

	s = new Script;
	s->Name = "mercenary_bot";
	s->GetAI = &GetAI_mercenary_bot;
	s->pGossipHello = &OnGossipHello_mercenary_bot;
	s->pGossipSelect = &OnGossipSelect_mercenary_bot;
	s->pGossipSelectWithCode = &OnGossipSelectWithCode_mercenary_bot;
    s->RegisterSelf();




    s = new Script;
	s->Name = "mercenary_world_gossip";
	s->GetAI = &GetAI_mercenary_world_gossip;
	s->pGossipHello = &OnGossipHello_mercenary_world_gossip;
	s->pGossipSelect = &OnGossipSelect_mercenary_world_gossip;
    s->RegisterSelf();
//#endif
}
