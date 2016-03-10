#include "precompiled.h"
#include "ScriptMgr.h"
#include "sc_gossip.h"
#include <vector>
#include "MercenaryMgr.h"
#include "PetAI.h"
#include "Group.h"
#include "mercenary_bot.h"
#include<stdlib.h>


enum menu_ction_base{
	GOSSIP_ACTION_SPELL_DEF = 1000
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
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySlotIcon(SLOT_HEAD) + MercenaryUtil::GetMercenarySlotName(player, SLOT_HEAD), 0, 14);
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
bool learnOrUnlearnSpell(Mercenary* mercenary, Player* player, Creature* creature, uint32 spell){
	/*以下为数据库配置技能*/
	MercenarySpell* info=MercenaryUtil::findMercenarySpellsInfoBySpell(spell);
	if (info == nullptr)
		return false;

	if (!info->isActive || info->isDefaultAura)
		return false;

	if (creature->removingSpell){
		Pet* pet = (Pet*)creature;
		pet->removeSpell(spell, false);
	}
	else
		mercenary->LearnSpell(player, spell);
				
	player->CLOSE_GOSSIP_MENU();
	if (creature->removingSpell)
		creature->removingSpell = false;
	else
		ChatHandler(player).PSendSysMessage(player->GetMangosString(-2800585));//学习成功，右键点击宠物技能图标启用。

	return true;
}

void addSpellMenu(Player* player, Mercenary* mercenary, Creature* creature, uint32 actions){
	Pet* pet = (Pet*)creature;
	/*以下为数据库配置技能*/
	GroupSpellsMap* groupSpells = MercenaryUtil::findGroupSpellsMapByClass(mercenary->GetType());
	if (groupSpells == nullptr)
		return;

	for (auto it = groupSpells->begin(); it != groupSpells->end(); ++it)
	{
		for (auto itr = it->second->spellLevelVector.begin(), itr2 = it->second->spellIdVector.begin(); itr != it->second->spellLevelVector.end(); ++itr, ++itr2)
		{
			if (*itr <= player->getLevel())
			{
				if (actions == 39)
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySpellIcon(*itr2, player) + it->first, 0, GOSSIP_ACTION_SPELL_DEF + *itr2);
				else if (pet->HasSpell(*itr2))
				{
					player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenarySpellIcon(*itr2, player) + it->first, 0, GOSSIP_ACTION_SPELL_DEF + *itr2);//遗忘
					creature->removingSpell = true;
				}
				break;
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

	if (actions > GOSSIP_ACTION_SPELL_DEF)
	{
		if (mercenary)
			learnOrUnlearnSpell(mercenary, player, creature, actions - GOSSIP_ACTION_SPELL_DEF);/*如果上面都不匹配，actions代表的就是学习或者遗忘的技能*/
		player->CLOSE_GOSSIP_MENU();
		return true;
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

//////////////////////////////////////////////////////////////////////////////////

