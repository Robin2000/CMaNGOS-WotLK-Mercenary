#include "precompiled.h"
#include "ScriptMgr.h"
#include "sc_gossip.h"
#include <vector>
#include "MercenaryMgr.h"
#include "PetAI.h"
#include "Group.h"
#include "mercenary_bot.h"
#include "MercenaryPet.h"
#include<stdlib.h>

enum BOT_GOSSIP_ACTION_TYPE{
	GOSSIP_REMOVE_ITEM = 100
};
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
		player->context.gossipActionType = -1;
		//#ifndef MANGOS
		//        if (mercenary->GetOwnerGUID() == player->GetGUID().GetCounter())
		//#else
		if (mercenary->GetOwnerGUID() == player->GetGUIDLow())
			//#endif
		{
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800635, 0, 1);//更换装备
			player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800636, 0, 2);//移除装备
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
	std::string static getSlotIcon(Mercenary* mercenary, uint8 slot)
	{
		uint32 size = mercenary->gearContainer.size();
		uint32 itemid=mercenary->gearContainer[slot].itemid;
		if (itemid>0)
			return MercenaryUtil::GetMercenaryItemIcon(itemid);
		
		return MercenaryUtil::GetMercenarySlotIcon(slot);
	}
	void static SendEquipGear(Player* player, Creature* creature, Mercenary* mercenary)
	{
		if (mercenary->GetEditSlot() != SLOT_EMPTY)
			mercenary->SetEditSlot(SLOT_EMPTY);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary,SLOT_HEAD) + MercenaryUtil::GetMercenarySlotName(player, SLOT_HEAD), 0, 14);
		
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_NECK) + MercenaryUtil::GetMercenarySlotName(player, SLOT_NECK), 0, 15);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_SHOULDERS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_SHOULDERS), 0, 13);

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_SHIRT) + MercenaryUtil::GetMercenarySlotName(player, SLOT_SHIRT), 0, 16);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_CHEST) + MercenaryUtil::GetMercenarySlotName(player, SLOT_CHEST), 0, 9);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_WAIST) + MercenaryUtil::GetMercenarySlotName(player, SLOT_WAIST), 0, 17);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_LEGS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_LEGS), 0, 10);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_FEET) + MercenaryUtil::GetMercenarySlotName(player, SLOT_FEET), 0, 11);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_WRISTS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_WRISTS), 0, 18);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_HANDS) + MercenaryUtil::GetMercenarySlotName(player, SLOT_HANDS), 0, 12);
		
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_FINGER1) + MercenaryUtil::GetMercenarySlotName(player, SLOT_FINGER1), 0, 19);//左手手指
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_FINGER2) + MercenaryUtil::GetMercenarySlotName(player, SLOT_FINGER2), 0, 20);//右手手指
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_TRINKET1) + MercenaryUtil::GetMercenarySlotName(player, SLOT_TRINKET1), 0, 21);//左手饰品
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_TRINKET2) + MercenaryUtil::GetMercenarySlotName(player, SLOT_TRINKET2), 0, 22);//右手饰品
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_BACK) + MercenaryUtil::GetMercenarySlotName(player, SLOT_BACK), 0, 23);//背
		
		
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_MAIN_HAND) + MercenaryUtil::GetMercenarySlotName(player, SLOT_MAIN_HAND), 0, 8);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_OFF_HAND) + MercenaryUtil::GetMercenarySlotName(player, SLOT_OFF_HAND), 0, 7);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_RANGED) + MercenaryUtil::GetMercenarySlotName(player, SLOT_RANGED), 0, 6);
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, getSlotIcon(mercenary, SLOT_TABARD) + MercenaryUtil::GetMercenarySlotName(player, SLOT_TABARD), 0, 24);
		

		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800602, 0, GOSSIP_ACTION_SPELL_DEF+36);//取消
		//#ifndef MANGOS
		//        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		//#else
		player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
		//#endif
	}

	void static SendItemList(Player* player, Creature* creature, Mercenary* mercenary, uint8 slot)
	{
		mercenary->SetEditSlot(slot);
		std::vector<Item*> tempVector;
		mercenary->GetEquippableItems(player, slot, tempVector);
		for (auto itr = tempVector.begin(); itr != tempVector.end(); ++itr)
		{
			Item* item = *itr;

			std::ostringstream ss;
			if (mercenary->gearContainer[slot].itemguid != item->GetGUIDLow())
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(item->GetEntry()) + MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), player->GetSession()), 0, item->GetEntry());
			else
			{
				ss << MercenaryUtil::GetMercenaryItemLink(item->GetEntry(), player->GetSession()) << player->GetMangosString(-2800638);// [|cff990000Already Equipped|r]
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(item->GetEntry()) + ss.str().c_str(), 0, GOSSIP_ACTION_SPELL_DEF+36);
			}
			
		}

		//if (slot == SLOT_OFF_HAND && mercenary->HasWeapon(true))
			//player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800639, 0, 5);移除副手装备
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800181, 0, GOSSIP_ACTION_SPELL_DEF+36);//"Back to Main Menu"
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
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800181, 0, GOSSIP_ACTION_SPELL_DEF+36);//"Back to Main Menu"
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
		MercenaryPet* pet = (MercenaryPet*)creature;
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
	MercenaryPet * pet = (MercenaryPet*)creature;
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

	player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800181, 0, GOSSIP_ACTION_SPELL_DEF+36);//"Back to Main Menu"
	//#ifndef MANGOS
	//                player->SEND_GOSSIP_MENU(1, creature->GetGUID());
	//#else
	player->SEND_GOSSIP_MENU(1, creature->GetObjectGuid());
	//#endif
}
bool OnGossipSelect_mercenary_bot(Player* player, Creature* creature, uint32 /*sender*/, uint32 actions)
{
	player->PlayerTalkClass->ClearMenus();

	Mercenary* mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
	WorldSession* session = player->GetSession();

	if (mercenary->GetEditSlot() != SLOT_EMPTY) /*mercenary中标记变量editSlot标明actions是否为编辑装备*/
	{
		std::vector<Item*> tempVector;
		mercenary->GetEquippableItems(player, mercenary->GetEditSlot(), tempVector);
		for (auto itr = tempVector.begin(); itr != tempVector.end(); ++itr)
		{
			Item* item =*itr;
			
			if (actions == item->GetEntry())
			{
				player->CLOSE_GOSSIP_MENU();
				if (!mercenary->EquipItemIfCan(player, item))
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
	if (actions == GOSSIP_ACTION_SPELL_DEF + 36)/*后退优先级最高*/
	{
		mercenary_bot::SendToHello(player, creature, mercenary);
		return true;
	}
	if (player->context.gossipActionType == GOSSIP_REMOVE_ITEM)//移除装备
	{
		mercenary->RemoveItemBySlot(player, (MercenaryPet*)creature, actions);

		player->context.gossipActionType = -1;

		player->CLOSE_GOSSIP_MENU();
		return true;
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
	case 2://移除装备
		player->context.gossipActionType = GOSSIP_REMOVE_ITEM;
		for (auto itr = mercenary->gearContainer.begin(); itr != mercenary->gearContainer.end(); ++itr)
		{
			//Item* item = mercenary->GetItemByGuid(player, itr->second->itemguid);
			//if (item)此时不应检查行李栏是否存在物品，因为物品可能丢弃了
			if (itr->second.itemid>0)
				player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, MercenaryUtil::GetMercenaryItemIcon(itr->second.itemid) + MercenaryUtil::GetMercenaryItemLink(itr->second.itemid, session), 0, itr->first);
		}
		player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, -2800606, 0, GOSSIP_ACTION_SPELL_DEF+36);//后退
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

	case 15: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_NECK);
		break;
	case 16: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_SHIRT);
		break;
	case 17: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_WAIST);
		break;
	case 18: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_WRISTS);
		break;
	case 19: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_FINGER1);
		break;
	case 20: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_FINGER2);
		break;
	case 21: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_TRINKET1);
		break;
	case 22: 
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_TRINKET2);
		break;
	case 23:
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_BACK);
		break;

	case 24:
		mercenary_bot::SendItemList(player, creature, mercenary, SLOT_TABARD);
		break;

		//
	case GOSSIP_ACTION_SPELL_DEF+36:
		mercenary_bot::SendToHello(player, creature, mercenary);
		break;
	case 39:
	case 40:
		Pet* pet = player->GetPet();
		if (mercenary&&pet)
		{
			mercenary->cleanNoMatchSpell(pet);
			addSpellMenu(player, mercenary, creature, actions);/*学习39或者遗忘40菜单*/
		}
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

