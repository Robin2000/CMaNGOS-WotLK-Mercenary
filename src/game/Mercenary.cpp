/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#include "Mercenary.h"
#include "MercenaryMgr.h"
#include "ObjectMgr.h"
Mercenary::Mercenary() { }

Mercenary::Mercenary(uint32 model, uint8 r, uint8 g, uint8 role, uint8 type)
	: displayId(model), race(r), gender(g), role(role), type(type) { }

Mercenary::~Mercenary()
{
    for (auto itr = GearContainer.begin(); itr != GearContainer.end(); ++itr)
        delete &itr;

    GearContainer.clear();
}

void Mercenary::LoadGearFromDB()
{
    GearContainer.clear();
	QueryResult* result = CharacterDatabase.PQuery("SELECT slot,itemId  FROM mercenary_gear WHERE guid='%u'", GetOwnerGUID());

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

			GearContainer[fields[0].GetUInt8()]=fields[1].GetUInt32();
        } while (result->NextRow());
    }
}

//          0         1    2          3    4                
//"SELECT role, displayId, race, gender, type FROM mercenaries  where ownerGUID=?
bool Mercenary::LoadFromDB(Player* player,QueryResult* result)
{
    Field* fields = result->Fetch();
	ownerGUID = player->GetGUIDLow();
    role = fields[0].GetUInt8();
    displayId = fields[1].GetUInt32();
    race = fields[2].GetUInt8();
    gender = fields[3].GetUInt8();
    type = fields[4].GetUInt8();

    LoadGearFromDB();

	summoned = true;/*凡保存到数据库的，确定summoned为true*/

    return true;
}

void Mercenary::SaveToDB()
{
	CharacterDatabase.BeginTransaction();

    static SqlStatementID insMerc;
    SqlStatement saveMerc = CharacterDatabase.CreateStatement(insMerc, "REPLACE INTO mercenaries (ownerGUID, role, displayId, race, gender, type) VALUES (?, ?, ?, ?, ?, ?)");

    saveMerc.addUInt32(ownerGUID);
    saveMerc.addUInt8(role);
    saveMerc.addUInt32(displayId);
    saveMerc.addUInt8(race);
    saveMerc.addUInt8(gender);
    saveMerc.addUInt8(type);

    saveMerc.Execute();
	CharacterDatabase.PExecute("DELETE FROM mercenary_gear WHERE guid = '%u'", GetOwnerGUID());
	for (auto itr = GearContainer.begin(); itr != GearContainer.end(); ++itr)
		CharacterDatabase.PExecute("INSERT INTO mercenary_gear (guid, slot ,itemId ) VALUES ('%u', '%u', '%u')", GetOwnerGUID(), itr->first, itr->second);

	CharacterDatabase.CommitTransaction();
}


bool Mercenary::Create(Player* player)
{
    if (!player)
        return false;
    ownerGUID = player->GetGUIDLow();
    role = 0;
    displayId = 1;
    race = 0;
    gender = GENDER_NONE;
    type = MERCENARY_TYPE_NONE;

	//player->learnSpell(883, true);//召唤宠物
	//player->learnSpell(982, true);//复活宠物

    sMercenaryMgr->SaveToList(this);

    return true;
}

bool Mercenary::Create(Player* player, uint32 model, uint8 r, uint8 g, uint8 mercType, uint8 mercRole, const std::string& name)
{
	

    if (!player)
        return false;

    Pet* pet = new Pet;
    if (!pet)
        return false;

    if (mercType >= MAX_MERCENARY_TYPES || mercType == MERCENARY_TYPE_NONE)
    {
        delete pet;
        return false;
    }
	
	//pet->SetOwnerGuid(player->GetObjectGuid());//我加的，会失败

    float x, y, z, o = 0;
    player->GetPosition(x, y, z);


    pet->Relocate(x, y, z, o);
    if (!pet->IsPositionValid())
    {
        delete pet;
        return false;
    }

    Map* map = player->GetMap();
    CreatureCreatePos pos(player, player->GetOrientation());
    CreatureInfo const* creatureInfo = ObjectMgr::GetCreatureTemplate(MERCENARY_DEFAULT_ENTRY);
    if (!creatureInfo)
    {
        delete pet;
        return false;
    }

    uint32 petNumber = sObjectMgr.GeneratePetNumber();
    uint32 guid = pos.GetMap()->GenerateLocalLowGuid(HIGHGUID_PET);

    if (!pet->Create(guid, pos, creatureInfo, petNumber))
    {
        delete pet;
        return false;
    }

    ownerGUID = player->GetGUIDLow();
    role = mercRole;
    displayId = model;
    race = r;
    gender = g;
    type = mercType;
    if (!name.empty())
        pet->SetName(name);

    Initialize(player, pet, true);

	ChatHandler(player->GetSession()).SendSysMessage(-2800646);//Successfully created a mercenary!
	pet->MonsterSay(player->GetMangosString(-2800647), LANG_UNIVERSAL, player);//Thanks for choosing me as your mercenary! Talk to me to setup my skills, gear, etc.
	pet->HandleEmoteCommandHappy();
    editSlot = SLOT_EMPTY;
    summoned = true;

	//player->learnSpell(883, true);//召唤宠物
	//player->learnSpell(982, true);//复活宠物

	//if (!reSummon){
	//    SaveToDB();	    
	//	}创建时不再保存
	sMercenaryMgr->SaveToList(this);
    return true;
}

bool Mercenary::LearnSpell(Player* player, uint32 spellId)
{
    if (!player)
        return false;

    Pet* pet = player->GetPet();
    if (!pet)
        return false;

    pet->learnSpell(spellId);

    return true;
}

bool Mercenary::Summon(Player* player)
{
	if (!player)
		return false;

	Pet* pet = player->GetPet();
	if (!pet)
		return false;

	Initialize(player, pet, false);

	return true;
	
}
bool Mercenary::isValidSpell(uint32 spell){
	auto itr = MercenaryUtil::findMercenarySpellsInfoBySpell(spell);
	return  itr != nullptr&&itr->role != role;
}
void Mercenary::cleanNoMatchSpell(Pet* pet){
	//自动移除无用技能
	uint8 petSpellCount = pet->GetPetAutoSpellSize();
	for (int i = 0; i < petSpellCount; i++)
	{
		uint32 spell = pet->GetPetAutoSpellOnPos(i);
		auto itr = MercenaryUtil::findMercenarySpellsInfoBySpell(spell);
		if (itr == nullptr)
		{
			pet->unlearnSpell(spell, false);
			continue;
		}
		if (itr->role != role)
			pet->unlearnSpell(spell, false);
	}

}
//移除不匹配装备
void Mercenary::clearnNoMatchEquipItem()
{
	Gear GearContainerTemp;
	for (auto itr = GearBegin(); itr != GearEnd(); ++itr)
	{
		ItemPrototype const* proto = sObjectMgr.GetItemPrototype(itr->second);
		if (!proto)
			continue;

		if (!sMercenaryMgr->CheckProficiencies(type, proto->Class, proto->SubClass))
			GearContainerTemp[itr->first] = itr->second;

	}
	if (GearContainerTemp.size()>0)
	{
		for (auto itr = GearContainerTemp.begin(); itr != GearContainerTemp.end(); ++itr)
			GearContainer.unsafe_erase(itr->first);
	}
}
void Mercenary::Initialize(Player* player, Pet* pet, bool create)
{
    if (!create)
    {
        pet->SetDisplayId(GetDisplay());
        pet->SetNativeDisplayId(GetDisplay());
        pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));
        pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
        

		pet->SetUInt32Value(UNIT_FIELD_PETNUMBER, GetOwnerGUID());

        for (auto itr = GearBegin(); itr != GearEnd(); ++itr)
        {
			switch (itr->first)
			{
				case SLOT_MAIN_HAND:
					pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, itr->second);
					break;
				case SLOT_OFF_HAND:
					pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, itr->second);
					break;
				case SLOT_RANGED:
					pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, itr->second);
			}
        }

		InitStats(player, pet);
    }
    else
    {
		pet->SetUInt32Value(UNIT_FIELD_PETNUMBER, GetOwnerGUID());
        pet->SetOwnerGuid(player->GetObjectGuid());
        pet->SetCreatorGuid(player->GetObjectGuid());
        pet->SetPowerType(POWER_MANA);
        pet->setPetType(SUMMON_PET);
        pet->GetCharmInfo()->SetPetNumber(GetOwnerGUID(), true);
        pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
        pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
        pet->SetDisplayId(GetDisplay());
        pet->SetNativeDisplayId(GetDisplay());
        pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));

        if (player->IsPvP())
            pet->SetPvP(true);
        if (player->IsFFAPvP())
#ifdef MANGOS
            pet->SetFFAPvP(true);
#else
            pet->SetByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP);
#endif
        

        pet->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
        pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
        pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
        pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL)));
        pet->GetCharmInfo()->SetReactState(REACT_DEFENSIVE);
        pet->InitPetCreateSpells();

        for (auto itr = sMercenaryMgr->MercenaryStartGearBegin(); itr != sMercenaryMgr->MercenaryStartGearEnd(); ++itr)
        {
            if (GetType() == itr->mercenaryType && role == itr->mercenaryRole && itr->creature_entry == pet->GetEntry())
            {
				GearContainer[SLOT_HEAD]=itr->headEntry;
				GearContainer[SLOT_SHOULDERS]=itr->shoulderEntry;
				GearContainer[SLOT_CHEST] = itr->chestEntry;
				GearContainer[SLOT_LEGS] = itr->legEntry;
				GearContainer[SLOT_FEET] = itr->feetEntry;
				GearContainer[SLOT_HANDS] = itr->handEntry;
				GearContainer[SLOT_MAIN_HAND] = itr->weaponEntry;
				GearContainer[SLOT_OFF_HAND] = itr->offHandEntry;
				GearContainer[SLOT_RANGED] = itr->rangedEntry;
                pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, itr->weaponEntry);
                pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, itr->offHandEntry);
                pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, itr->rangedEntry);
            }
        }

        pet->SetHealth(pet->GetMaxHealth());
        pet->AIM_Initialize();
        player->GetMap()->Add((Creature*)pet);
        player->SetPet(pet);
        player->PetSpellInitialize();

		InitStats(player, pet);
    }
	SendMirrorImagePacket(pet);
}

bool Mercenary::CanEquipItem(Player* player, Item* item)
{
    WorldSession* session = player->GetSession();
    if (!item)
        return false;

    const ItemPrototype* proto = item->GetProto();
    if (!proto)
    {
		ChatHandler(session).SendSysMessage(-2800648);//"Invalid item! Your mercenary could not equip this item."
        sLog.outError("Tried to equip invalid item %u.Item does not have a template.", item->GetEntry());
        return false;
    }

    Pet* pet = player->GetPet();
    if (!pet)
        return false;

    uint32 itemClass = proto->Class;
    uint32 itemSubClass = proto->SubClass;
    uint32 invType = proto->InventoryType;
    bool isCorrectProficiency = sMercenaryMgr->CheckProficiencies(type, itemClass, itemSubClass);
    bool isCorrectLevel = pet->getLevel() >= proto->RequiredLevel;
    if (proto->RequiredLevel > 0 && !isCorrectLevel)
    {
		ChatHandler(session).PSendSysMessage(-2800649, proto->RequiredLevel);//"装备物品失败，物品等级太高。需要达到等级%u"
        return false;
    }

    if (itemClass == ITEM_CLASS_ARMOR && (invType != INVTYPE_SHIELD && invType != INVTYPE_HOLDABLE) && !isCorrectProficiency)
    {
		ChatHandler(session).SendSysMessage(-2800650);//装备物品失败，护甲专精不符。
        return false;
    }

    if ((itemClass == ITEM_CLASS_WEAPON || (itemClass == ITEM_CLASS_ARMOR && (invType == INVTYPE_SHIELD || invType == INVTYPE_HOLDABLE))) && !isCorrectProficiency)
    {
		ChatHandler(session).SendSysMessage(-2800651);//装备物品失败，武器专精不符。
        return false;
    }

    if (editSlot == SLOT_MAIN_HAND)
    {
        if (invType != INVTYPE_2HWEAPON && invType != INVTYPE_WEAPONMAINHAND && invType != INVTYPE_WEAPON)
        {
			ChatHandler(session).SendSysMessage(-2800652);//Equip failed. Cannot equip an off hand weapon in your Mercenary's main hand.
            return false;
        }
    }
    else if (editSlot == SLOT_OFF_HAND)
    {
        if (invType == INVTYPE_2HWEAPON && invType == INVTYPE_WEAPONMAINHAND && invType == INVTYPE_WEAPON)
        {
			ChatHandler(session).SendSysMessage(-2800653);//"Equip failed. Cannot equip a two handed, main hand or one handed in your Mercenary's off hand."
            return false;
        }
    }

    uint32 newItemId = item->GetEntry();
        if (itemClass == ITEM_CLASS_ARMOR || itemClass == ITEM_CLASS_WEAPON)
        {
			GearContainer[editSlot] = newItemId;
				
			switch (editSlot)
			{
				case SLOT_MAIN_HAND:
					pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, newItemId);
					break;
				case SLOT_OFF_HAND:
					pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, newItemId);
					break;
				case SLOT_RANGED:
					pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, newItemId);
			}
    }

    //if (editSlot != SLOT_MAIN_HAND || editSlot != SLOT_OFF_HAND || editSlot != SLOT_RANGED)改为总是更新
    SendMirrorImagePacket(pet);

    UpdateAllStats(player, pet);

    return true;
}

bool Mercenary::InitStats(Player* player, Pet* pet)
{
    uint8 mercenaryLevel = player->getLevel();
    CreatureInfo const* creatureInfo = pet->GetCreatureInfo();
    if (!creatureInfo)
    {
        sLog.outError("Failed to load creature info for Mercenary");
        return false;
    }

    pet->SetLevel(mercenaryLevel);
    pet->SetMeleeDamageSchool(SpellSchools(creatureInfo->DamageSchool));

    //@TODO Below, Change by stats (gear)
    pet->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, float(mercenaryLevel * 50));
    pet->SetAttackTime(BASE_ATTACK, BASE_ATTACK_TIME);
    pet->SetAttackTime(OFF_ATTACK, BASE_ATTACK_TIME);
    pet->SetAttackTime(RANGED_ATTACK, BASE_ATTACK_TIME);
    pet->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);

    //pet->SetObjectScale(1.0f);

    int32 createResistance[MAX_SPELL_SCHOOL] = { 0, 0, 0, 0, 0, 0, 0 };
    createResistance[SPELL_SCHOOL_HOLY] = creatureInfo->ResistanceHoly;
    createResistance[SPELL_SCHOOL_FIRE] = creatureInfo->ResistanceFire;
    createResistance[SPELL_SCHOOL_NATURE] = creatureInfo->ResistanceNature;
    createResistance[SPELL_SCHOOL_FROST] = creatureInfo->ResistanceFrost;
    createResistance[SPELL_SCHOOL_SHADOW] = creatureInfo->ResistanceShadow;
    createResistance[SPELL_SCHOOL_ARCANE] = creatureInfo->ResistanceArcane;
    for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
        pet->SetModifierValue(UnitMods(UNIT_MOD_RESISTANCE_START + i), BASE_VALUE, float(createResistance[i]));


    pet->SetCreateHealth(uint32(((float(creatureInfo->MaxLevelHealth) / creatureInfo->MaxLevel) / (1 + 2 * creatureInfo->Rank)) * mercenaryLevel));
    pet->SetCreateMana(uint32(((float(creatureInfo->MaxLevelMana) / creatureInfo->MaxLevel) / (1 + 2 * creatureInfo->Rank)) * mercenaryLevel));

    pet->SetCreateStat(STAT_STRENGTH, 22);
    pet->SetCreateStat(STAT_AGILITY, 22);
    pet->SetCreateStat(STAT_STAMINA, 25);
    pet->SetCreateStat(STAT_INTELLECT, 28);
    pet->SetCreateStat(STAT_SPIRIT, 27);


    pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, uint32(sObjectMgr.GetXPForLevel(mercenaryLevel) * 0.05f));

    pet->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, float(mercenaryLevel - (mercenaryLevel / 4)));
    pet->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, float(mercenaryLevel + (mercenaryLevel / 4)));

    for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
        UpdateStats(player, Stats(i), pet);
    for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
        UpdateMaxPower(Powers(i), pet);

    for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        pet->UpdateResistances(i);
    pet->SetHealth(pet->GetMaxHealth());

    pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));

	if (isRangedAttack()){
		SetSheath(pet,SHEATH_STATE_RANGED);
	}
    return true;
}
void Mercenary::SetSheath(Pet* pet,SheathState sheathed)
{
	/*switch (sheathed)
	{
	case SHEATH_STATE_UNARMED:                          // no prepared weapon
		pet->SetVirtualItemSlot(0, nullptr);
		pet->SetVirtualItemSlot(1, nullptr);
		pet->SetVirtualItemSlot(2, nullptr);
		break;
	case SHEATH_STATE_MELEE:                            // prepared melee weapon
	{
		pet->SetVirtualItemSlot(0, GetWeaponForAttack(BASE_ATTACK, true, true));
		pet->SetVirtualItemSlot(1, GetWeaponForAttack(OFF_ATTACK, true, true));
		pet->SetVirtualItemSlot(2, nullptr);
	};  break;
	case SHEATH_STATE_RANGED:                           // prepared ranged weapon
		pet->SetVirtualItemSlot(0, nullptr);
		pet->SetVirtualItemSlot(1, nullptr);
		pet->SetVirtualItemSlot(2, GetWeaponForAttack(RANGED_ATTACK, true, true));
		break;
	default:
		pet->SetVirtualItemSlot(0, nullptr);
		pet->SetVirtualItemSlot(1, nullptr);
		pet->SetVirtualItemSlot(2, nullptr);
		break;
	}*/
	pet->SetSheath(sheathed);                              // this must visualize Sheath changing for other players...
}
bool Mercenary::UpdateStats(Player* player, Stats stat, Pet* pet)
{
    if (stat >= MAX_STATS)
        return false;

    float value = pet->GetTotalStatValue(stat);
    pet->ApplyStatBuffMod(stat, player->GetStat(stat), false);
    float ownersBonus = 0.0f;

    float mod = 0.75f;
    if (stat == STAT_STAMINA)
    {

        value += float(player->GetStat(stat)) * 0.3f;

    }
    else if (stat == STAT_INTELLECT)
    {
        value += float(player->GetStat(stat)) * 0.3f;
    }
    else if (stat == STAT_STRENGTH)
    {
        value += float(player->GetStat(stat)) * 0.3f;
    }

    for (auto itr = GearBegin(); itr != GearEnd(); ++itr)
    {
		const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->second);
        if (proto)
        {
            for (uint32 i = 0; i < proto->StatsCount; i++)
            {
                uint32 statType = proto->ItemStat[i].ItemStatType;
                uint32 statTypeVal = proto->ItemStat[i].ItemStatValue;
                if ((stat == STAT_STRENGTH && statType == ITEM_MOD_STRENGTH) || (stat == STAT_AGILITY && statType == ITEM_MOD_AGILITY) ||
                    (stat == STAT_INTELLECT && statType == ITEM_MOD_INTELLECT) || (stat == STAT_STAMINA && statType == ITEM_MOD_STAMINA))
                    value += float(statTypeVal);
            }
        }
    }

    pet->SetStat(stat, int32(value));
    pet->ApplyStatBuffMod(stat, player->GetStat(stat) + ownersBonus, true);

    switch (stat)
    {
        case STAT_STRENGTH: UpdateStrength(pet); break;
        case STAT_AGILITY: UpdateArmor(pet); break;
        case STAT_STAMINA: UpdateMaxHealth(pet); break;
        case STAT_INTELLECT: UpdateMaxPower(POWER_MANA, pet); break;
        case STAT_SPIRIT:
        default:
            break;
    }

    return true;
}

bool Mercenary::UpdateAllStats(Player* player, Pet* pet)
{
    if (!player)
        return false;

    if (!pet)
        return false;

    for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
        UpdateStats(player, Stats(i), pet);
    for (int i = POWER_MANA; i < MAX_POWERS; ++i)
        UpdateMaxPower(Powers(i), pet);

    return true;
}

void Mercenary::SetValues(uint32 model, uint8 r, uint8 g)
{
    displayId = model;
    race = r;
    gender = g;
}

void Mercenary::RemoveOffHand(Creature* creature)
{
    if (!creature)
        return;

    for (auto itr = GearContainer.begin(); itr != GearContainer.end(); ++itr)
    {
        if (itr->first == SLOT_OFF_HAND)
			itr->second = 0;
    }
    creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, 0);
}

void Mercenary::SendMirrorImagePacket(Creature* creature)
{
    WorldPacket data(SMSG_MIRRORIMAGE_DATA, 68);
    data << creature->GetObjectGuid();
    data << uint32(GetDisplay());
    data << uint8(GetRace());
    data << uint8(GetGender());
    data << uint8(1);
    data << uint8(0); // Skin
    data << uint8(0); // Face
    data << uint8(0); // Hair
    data << uint8(0); // Hair color
    data << uint8(0); // Facial hair
    data << uint32(0);
    data << uint32(sMercenaryMgr->GetItemDisplayId(GetItemBySlot(SLOT_HEAD)));
    data << uint32(sMercenaryMgr->GetItemDisplayId(GetItemBySlot(SLOT_SHOULDERS)));
    data << uint32(0); // Shirt?
    data << uint32(sMercenaryMgr->GetItemDisplayId(GetItemBySlot(SLOT_CHEST)));
    data << uint32(0); // Waist
    data << uint32(sMercenaryMgr->GetItemDisplayId(GetItemBySlot(SLOT_LEGS)));
    data << uint32(sMercenaryMgr->GetItemDisplayId(GetItemBySlot(SLOT_FEET)));
    data << uint32(0); // Wrists
    data << uint32(sMercenaryMgr->GetItemDisplayId(GetItemBySlot(SLOT_HANDS)));
    data << uint32(0); // Cloak
    data << uint32(0); // Tabard

    creature->SendMessageToSet(&data, false);
}

void Mercenary::UpdateStrength(Pet* pet, bool ranged)
{
    if (ranged)
        return;

    float val = 0.0f;
    float bonusAP = 0.0f;
    UnitMods unitMod = UNIT_MOD_ATTACK_POWER;

	val = 2 * pet->GetStat(STAT_STRENGTH) - 20.0f;

    Unit* owner = pet->GetOwner();

    pet->SetModifierValue(UNIT_MOD_ATTACK_POWER, BASE_VALUE, val + bonusAP);

    float base_attPower = pet->GetModifierValue(unitMod, BASE_VALUE) * pet->GetModifierValue(unitMod, BASE_PCT);
    float attPowerMod = pet->GetModifierValue(unitMod, TOTAL_VALUE);
    float attPowerMultiplier = pet->GetModifierValue(unitMod, TOTAL_PCT) - 1.0f;

    pet->SetInt32Value(UNIT_FIELD_ATTACK_POWER, (int32)base_attPower);
    pet->SetInt32Value(UNIT_FIELD_ATTACK_POWER_MODS, (int32)attPowerMod);
    pet->SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, attPowerMultiplier);

    UpdatePhysicalDamage(BASE_ATTACK, pet);
}

void Mercenary::UpdatePhysicalDamage(WeaponAttackType attackType, Pet* pet)
{
    if (attackType > BASE_ATTACK)
        return;

    UnitMods unitMod = UNIT_MOD_DAMAGE_MAINHAND;

    float att_speed = float(pet->GetAttackTime(BASE_ATTACK)) / 1000.0f;
    float base_value = pet->GetModifierValue(unitMod, BASE_VALUE) + pet->GetTotalAttackPowerValue(attackType) / 14.0f * att_speed;
    float base_pct = pet->GetModifierValue(unitMod, BASE_PCT);
    float total_value = pet->GetModifierValue(unitMod, TOTAL_VALUE);
    float total_pct = pet->GetModifierValue(unitMod, TOTAL_PCT);
    float weapon_mindamage = pet->GetWeaponDamageRange(BASE_ATTACK, MINDAMAGE);
    float weapon_maxdamage = pet->GetWeaponDamageRange(BASE_ATTACK, MAXDAMAGE);
    float mindamage = ((base_value + weapon_mindamage) * base_pct + total_value) * total_pct;
    float maxdamage = ((base_value + weapon_maxdamage) * base_pct + total_value) * total_pct;
    for (auto itr = GearBegin(); itr != GearEnd(); ++itr)
    {
        if (itr->first == SLOT_MAIN_HAND || itr->first == SLOT_OFF_HAND)
        {
			const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->second);
            if (proto)
            {
                mindamage += proto->Damage[0].DamageMin;
                maxdamage += proto->Damage[0].DamageMax;
            }
        }
    }

    pet->SetStatFloatValue(UNIT_FIELD_MINDAMAGE, mindamage);
    pet->SetStatFloatValue(UNIT_FIELD_MAXDAMAGE, maxdamage);
}

void Mercenary::UpdateArmor(Pet* pet)
{
    uint32 armor = 0.0f;
    float value = 0.0f;
    float bonusArmor = 0.0f;
    UnitMods unitMod = UNIT_MOD_ARMOR;

    for (auto itr = GearBegin(); itr != GearEnd(); ++itr)
    {
        const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->second);
        if (proto)
        {
            if (proto->Armor > 0)
                armor += proto->Armor;
        }
    }

    value = pet->GetModifierValue(unitMod, BASE_VALUE);
    value *= pet->GetModifierValue(unitMod, BASE_PCT);
    value += pet->GetStat(STAT_AGILITY) * 1.5f; // Decreased gain by 0.5
    value += pet->GetModifierValue(unitMod, TOTAL_VALUE);
    value *= pet->GetModifierValue(unitMod, TOTAL_PCT);
    value += (float)armor;

    pet->SetArmor(int32(value));
}

void Mercenary::UpdateMaxHealth(Pet* pet)
{
    UnitMods unitMod = UNIT_MOD_HEALTH;
    float stamina = pet->GetStat(STAT_STAMINA) - pet->GetCreateStat(STAT_STAMINA);

    float value = pet->GetModifierValue(unitMod, BASE_VALUE) + pet->GetCreateHealth();
    value *= pet->GetModifierValue(unitMod, BASE_PCT);
    value += pet->GetModifierValue(unitMod, TOTAL_VALUE) + stamina * 10.0f;
    value *= pet->GetModifierValue(unitMod, TOTAL_PCT);

    pet->SetMaxHealth((uint32)value);
}

void Mercenary::UpdateMaxPower(Powers power, Pet* pet)
{
    UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + power);

    float addValue = (power == POWER_MANA) ? pet->GetStat(STAT_INTELLECT) - pet->GetCreateStat(STAT_INTELLECT) : 0.0f;

    float value = pet->GetModifierValue(unitMod, BASE_VALUE) + pet->GetCreatePowers(power);
    value *= pet->GetModifierValue(unitMod, BASE_PCT);
    value += pet->GetModifierValue(unitMod, TOTAL_VALUE) + addValue * 15.0f;
    value *= pet->GetModifierValue(unitMod, TOTAL_PCT);

    pet->SetMaxPower(power, uint32(value));
}

uint8 Mercenary::GetInvTypeSlot(uint8 characterSlot) const
{
    static uint8 invSlots[] =
    {
        INVTYPE_HEAD,
        INVTYPE_NECK,
        INVTYPE_SHOULDERS,
        NULL,
        INVTYPE_CHEST,
        INVTYPE_WAIST,
        INVTYPE_LEGS,
        INVTYPE_FEET,
        INVTYPE_WRISTS,
        INVTYPE_HANDS,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        INVTYPE_WEAPONMAINHAND,
        INVTYPE_WEAPONOFFHAND,
        INVTYPE_RANGED
    };
    return invSlots[characterSlot];
}