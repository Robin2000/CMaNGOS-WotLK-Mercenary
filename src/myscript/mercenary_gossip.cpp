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
	GOSSIP_SEL_ROLE_DEF = 900
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
			player->context.gossipMenuType = -1;/*还原默认值*/
			return false;
		}
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct mercenary_bot_AI : public PetAI
{
	mercenary_bot_AI(Creature* creature) : PetAI(creature) { Reset(); }

	uint32 talkTimer = 0;
	uint32 spellTimer=0;
	int currentSpell = 0;

	void Reset() //注意，这里有个overide，发现父类没有该方法，Reset何时调用，必须考虑
	{
		talkTimer = urand(35000, 120000);
		lastMessage = "";

		if (Unit* owner = m_creature->GetOwner())
		{
			mercenary = MercenaryUtil::GetMercenaryByOwner(owner->GetGUIDLow());
			if (mercenary)
			{

				if (mercenary->isRangedAttack())
				{
					m_creature->SetSheath(SHEATH_STATE_RANGED);
					m_creature->clearUnitState(UNIT_STAT_MELEE_ATTACKING);
					//m_creature->addUnitState(UNIT_STAT_FLEEING);

				}
				else
				{
					m_creature->SetSheath(SHEATH_STATE_MELEE);
					m_creature->addUnitState(UNIT_STAT_MELEE_ATTACKING);
				}

			}

		}
	}
	void caseSpell(Unit* target, uint32 spell,const uint32 uiDiff){
			// Cast spell one on our current target.
		if (DoCastSpellIfCan(target, spell) == CAST_OK)
				spellTimer = 5000;
	}
	bool DoRangeAttackIfReady(const uint32 diff){

		uint8 spellCount = m_creature->GetPetAutoSpellSize();
		
		if (spellCount <= 0)
			return false;

		Unit* target = m_creature->getVictim();
		//if (!target)
			//target = m_creature->SelectRandomUnfriendlyTarget();
		if (!target)
			return false;

		Player* master = m_creature->GetOwner()->ToPlayer();
		if (!master)
			return false;

		
		mercenary = MercenaryUtil::GetMercenaryByOwner(master->GetGUIDLow());
		//#endif
		if (!mercenary)
			return false;
		
		if (spellTimer < diff)
		{
			if (rand() % 50 > 10)//有10%可能随机施法
			{
				uint8 randSpell = rand() % (spellCount+1);
				caseSpell(target, m_creature->GetPetAutoSpellOnPos(randSpell), diff);
			}
			else 
			{
				if (currentSpell >= spellCount)
					currentSpell = 0;
				caseSpell(target, m_creature->GetPetAutoSpellOnPos(currentSpell), diff);
				currentSpell++;
			}
			
		}else 
			spellTimer -= diff;

		return true;
	};

	void UpdateAI(const uint32 diff) override
	{

		if (Unit* owner = m_creature->GetOwner()){
			if (!m_creature->getVictim() && m_creature->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW) && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
			{
				if (mercenary->isRangedAttack())
					m_creature->GetMotionMaster()->MoveFollow(owner, 4 * PET_FOLLOW_DIST, M_PI_F*3.0f / 4.0f);
				else
					m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, -PET_FOLLOW_ANGLE);
			}
		}
		if (mercenary)
		{
			if (talkTimer <= 0)
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

		if (mercenary->isRangedAttack())
		{
			//m_creature->SetSheath(SHEATH_STATE_RANGED);
			m_creature->clearUnitState(UNIT_STAT_MELEE_ATTACKING);
			//m_creature->addUnitState(UNIT_STAT_FLEEING);
			DoRangeAttackIfReady(diff);
		}else
			DoMeleeAttackIfReady();
	}
private:
	Mercenary* mercenary;
	MercenaryTalking mercenaryTalk;
	std::string lastMessage;
};

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
