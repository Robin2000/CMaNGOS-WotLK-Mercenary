
#include "Mercenary.h"
#include "MercenaryMgr.h"
#include "ObjectMgr.h"
#include "MercenaryPet.h"

Mercenary::Mercenary() { }

Mercenary::Mercenary(uint32 model, uint8 r, uint8 g, uint8 role, uint8 type)
	: displayId(model), race(r), gender(g), role(role), type(type) { }

Mercenary::~Mercenary()
{
    gearContainer.clear();
}

void Mercenary::LoadGearFromDB()
{
    gearContainer.clear();
	QueryResult* result = CharacterDatabase.PQuery("SELECT slot,itemguid,itemid  FROM mercenary_gear WHERE guid='%u'", GetOwnerGUID());

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
			GearEntry gearEntry;
			gearEntry.itemguid = fields[1].GetUInt32();
			gearEntry.itemid = fields[2].GetUInt32();
			gearContainer.insert(std::make_pair(fields[0].GetUInt8(),gearEntry));
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
	for (auto itr = gearContainer.begin(); itr != gearContainer.end(); ++itr)
	if (itr->second.itemguid>0)
		CharacterDatabase.PExecute("INSERT INTO mercenary_gear (guid, slot ,itemid ,itemguid ) VALUES ('%u', '%u', '%u', '%u')", GetOwnerGUID(), itr->first, itr->second.itemid,itr->second.itemguid);

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
	if (mercType >= MAX_MERCENARY_TYPES || mercType == MERCENARY_TYPE_NONE)
		return false;

	Pet *petOld=player->GetPet();

	if (petOld != nullptr&&!petOld->isMercenary())//有宠物但不是雇佣兵
	{
		ChatHandler(player->GetSession()).SendSysMessage(-2800634);//必须首先解散你的宠物
		return false;
	}

	
	if (petOld != nullptr&&petOld->isMercenary())//有宠物且是雇佣兵
	{
		if (petOld->GetMapId() == player->GetMapId())
		{
			petOld->NearTeleportTo(player->GetPositionX() + 1.0f, player->GetPositionY() + 1.0f, player->GetPositionZ()+0.5f, player->GetOrientation());
			return true;
		}
		else
		{
			petOld->NearTeleportTo(player->GetPositionX() + 1.0f, player->GetPositionY() + 1.0f, player->GetPositionZ() + 0.5f, player->GetOrientation());//不在同一个地图使用NearTeleportTo会如何？
			return true;
		}
	}

	MercenaryPet * pet = new MercenaryPet;

    float x, y, z, o = 0;
    player->GetPosition(x, y, z);


	pet->Relocate(x + 1.0, y + 1.0, z, o);
    if (!pet->IsPositionValid())
    {
        delete pet;
        return false;
    }

    Map* map = player->GetMap();
    CreatureCreatePos pos(player, player->GetOrientation(),1.0f);
	CreatureInfo const* creatureInfo = ObjectMgr::GetCreatureTemplate(MERCENARY_DEFAULT_ENTRY);//1128         MERCENARY_DEFAULT_ENTRY
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
	else
		pet->SetName(creatureInfo->Name);
	
	
	//sMercenaryMgr->SaveToList(this);在create的时候已经放入列表

	Initialize(player, pet, true, petNumber);

	ChatHandler(player->GetSession()).SendSysMessage(-2800646);//Successfully created a mercenary!
	pet->MonsterSay(player->GetMangosString(-2800647), LANG_UNIVERSAL, player);//Thanks for choosing me as your mercenary! Talk to me to setup my skills, gear, etc.

    editSlot = SLOT_EMPTY;
    summoned = true;

	//player->learnSpell(883, true);//召唤宠物
	//player->learnSpell(982, true);//复活宠物

	//if (!reSummon){
	//    SaveToDB();	    
	//	}创建时不再保存

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

	if (pet->isMercenary())
		Initialize(player, (MercenaryPet*)pet, false,pet->GetCharmInfo()->GetPetNumber());

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
void Mercenary::clearnNoMatchEquipItem(Player * player)
{
	std::vector<uint32> del_slots;
	for (auto itr = gearContainer.begin(); itr != gearContainer.end(); ++itr)
	{
		Item* item = GetItemByGuid(player,itr->second.itemguid);
		if (item == nullptr)
		{
			itr->second.itemguid = 0;
			itr->second.itemid = 0;
		}
		else{
			ItemPrototype const* proto = item->GetProto();
			if (proto == nullptr)
			{
				itr->second.itemguid = 0;
				itr->second.itemid = 0;
			}
			else if (!sMercenaryMgr->CheckProficiencies(type, proto->Class, proto->SubClass))
			{
				itr->second.itemguid = 0;
				itr->second.itemid = 0;
			}
			//itr->second->itemguid = item->GetGUIDLow();/*更新guid*/
		}

	}
}

class UpdatePetActionBar :public DelayedAction{

public:
	UpdatePetActionBar(Player * _player, MercenaryPet* _pet, int _timelimit) : DelayedAction(_timelimit), player(_player), pet(_pet){}

	void run() override{

		player->PetSpellInitialize();//刷新客户端ActionBar
		pet->GetCharmInfo()->SetReactState(REACT_DEFENSIVE);
	}
	Player * player;
	MercenaryPet    * pet;
};
void Mercenary::InitializeNEW(Player* player, Pet* pet, bool create,uint32 petnumber)
{
	
	if (!create)
	{
		pet->SetDisplayId(GetDisplay());
		pet->SetNativeDisplayId(GetDisplay());
		pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));
		pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

		pet->InitStatsForLevel(player->getLevel(), player);

		pet->SetUInt32Value(UNIT_FIELD_PETNUMBER, petnumber);

	}
	else
	{
		pet->SetUInt32Value(UNIT_FIELD_PETNUMBER, petnumber);
		pet->SetOwnerGuid(player->GetObjectGuid());
		pet->SetCreatorGuid(player->GetObjectGuid());
		pet->SetPowerType(POWER_MANA);
		pet->setPetType(SUMMON_PET);
		//#endif
		pet->GetCharmInfo()->SetPetNumber(petnumber, true);
		pet->SetUInt32Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
		pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		pet->SetDisplayId(GetDisplay());
		pet->SetNativeDisplayId(GetDisplay());
		pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));

		if (player->IsPvP())
			pet->SetPvP(true);
		if (player->IsFFAPvP())
			pet->SetFFAPvP(true);
		pet->InitStatsForLevel(player->getLevel(), player);

		pet->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
		pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
		pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
		pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL)));
		pet->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED | UNIT_CAN_BE_ABANDONED);//可重命名，可放弃


		pet->InitPetCreateSpells();
	}
		pet->SetHealth(pet->GetMaxHealth());
		//pet->SetHealth(1000);
		pet->AIM_Initialize();
		player->GetMap()->Add((Creature*)pet);
		player->SetPet(pet);
		//pet->SetDeathState(ALIVE);

		pet->GetCharmInfo()->SetReactState(REACT_DEFENSIVE);//和actionbar有关
		pet->GetCharmInfo()->SetCommandState(COMMAND_FOLLOW);//和actionbar有关
		player->PetSpellInitialize();

		SendMirrorImagePacket(pet);
		pet->HandleEmoteCommandHappy();

}
void Mercenary::Initialize(Player* player, MercenaryPet* pet, bool create,uint32 petnumber)
{
	uint32 level = player->getLevel();

	//@TODO Below, Change by stats (gear)
	/*pet->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, float(level * 50));
	pet->SetAttackTime(BASE_ATTACK, BASE_ATTACK_TIME);
	pet->SetAttackTime(OFF_ATTACK, BASE_ATTACK_TIME);
	pet->SetAttackTime(RANGED_ATTACK, BASE_ATTACK_TIME);
	pet->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);
	pet->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);
	
	CreatureInfo const* creatureInfo = pet->GetCreatureInfo();
	int32 createResistance[MAX_SPELL_SCHOOL] = { 0, 0, 0, 0, 0, 0, 0 };
	createResistance[SPELL_SCHOOL_HOLY] = creatureInfo->ResistanceHoly;
	createResistance[SPELL_SCHOOL_FIRE] = creatureInfo->ResistanceFire;
	createResistance[SPELL_SCHOOL_NATURE] = creatureInfo->ResistanceNature;
	createResistance[SPELL_SCHOOL_FROST] = creatureInfo->ResistanceFrost;
	createResistance[SPELL_SCHOOL_SHADOW] = creatureInfo->ResistanceShadow;
	createResistance[SPELL_SCHOOL_ARCANE] = creatureInfo->ResistanceArcane;

	for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
		pet->SetModifierValue(UnitMods(UNIT_MOD_RESISTANCE_START + i), BASE_VALUE, float(createResistance[i]));


	pet->SetCreateStat(STAT_STRENGTH, 22);
	pet->SetCreateStat(STAT_AGILITY, 22);
	pet->SetCreateStat(STAT_STAMINA, 25);
	pet->SetCreateStat(STAT_INTELLECT, 28);
	pet->SetCreateStat(STAT_SPIRIT, 27);


	pet->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, float(level - (level / 4)));
	pet->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, float(level + (level / 4)));

	for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
		pet->UpdateStats(Stats(i));
	
	for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
		pet->UpdateMaxPower(Powers(i));

	 for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
		pet->UpdateResistances(i);*/



    if (!create)
    {
        pet->SetDisplayId(GetDisplay());
        pet->SetNativeDisplayId(GetDisplay());

        pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));
		pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

		
		pet->GetCharmInfo()->SetPetNumber(petnumber, true);


		clearnNoMatchEquipItem(player);/*初始化时清理掉不存在的装备*/

		//开始重新装备
		/*
		for (auto itr = gearContainer.begin(); itr != gearContainer.end(); ++itr)  //全部重新装载，如果必要
			if (Item * item = GetItemByGuid(player,itr->second->itemguid))
				EquipItemIfCan(player, item, true);
		*/

		pet->InitStatsForLevel(level);

		pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);//升级效果
		
    }
    else
    {
		pet->SetDisplayId(GetDisplay());
		pet->SetNativeDisplayId(GetDisplay());

		//pet->SetMaxPower(POWER_MANA, pet->GetCreatePowers(POWER_MANA));
		//pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));
		
		pet->SetPowerType(POWER_MANA);
		pet->SetMaxPower(POWER_MANA,1000);
		pet->SetPower(POWER_MANA, 1000);
		
		/*pet->SetMaxPower(POWER_HAPPINESS, 1000000);
		pet->SetPower(POWER_HAPPINESS, 166500);
		pet->SetPowerType(POWER_FOCUS);*/


		pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL)));
		pet->SetUInt32Value(UNIT_FIELD_PETEXPERIENCE, 0);
		pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, 1000);
		pet->SetUInt32Value(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
		pet->SetName(pet->GetNameForLocaleIdx(sObjectMgr.GetDBCLocaleIndex()));

		{
			pet->SetByteValue(UNIT_FIELD_BYTES_0, 0, GetRace());
			pet->SetByteValue(UNIT_FIELD_BYTES_0, 1, GetType());
			pet->SetByteValue(UNIT_FIELD_BYTES_0, 2, GetGender());
			//pet->SetByteValue(UNIT_FIELD_BYTES_0, 3, POWER_FOCUS);前面已经设置pet->SetPowerType(POWER_FOCUS);
			//pet->SetSheath(SHEATH_STATE_MELEE);
			pet->SetByteFlag(UNIT_FIELD_BYTES_2, 2, UNIT_CAN_BE_RENAMED | UNIT_CAN_BE_ABANDONED);//可重命名，可放弃
			pet->SetUInt32Value(UNIT_MOD_CAST_SPEED, 1.0f);
		}

        pet->SetOwnerGuid(player->GetObjectGuid());
        pet->SetCreatorGuid(player->GetObjectGuid());
		pet->setFaction(player->getFaction());
		//pet->SetUInt32Value(UNIT_CREATED_BY_SPELL, 13481);//驯服野兽

		pet->setPetType(SUMMON_PET);



        if (player->IsPvP())
            pet->SetPvP(true);
        if (player->IsFFAPvP())
            pet->SetFFAPvP(true);

        
		//pet->SetUInt32Value(UNIT_FIELD_BYTES_0, 2048);//未知用途
		//InitStats(player, pet);
		//((MercenaryPet*)pet)->_ApplyAllStatBonuses();
       
		pet->InitStatsForLevel(level);

		pet->GetCharmInfo()->SetPetNumber(petnumber, true);
		
		//pet->SetUInt32Value(UNIT_FIELD_PETNUMBER, GetOwnerGUID());//通过pet->GetCharmInfo()->SetPetNumber(sObjectMgr.GeneratePetNumber(), true);来达到相同目的
		
		pet->AIM_Initialize();
		pet->InitPetCreateSpells();
		pet->InitLevelupSpellsForLevel();//先注释掉，不学习spell
		pet->InitTalentForLevel();  //先注释掉，不学习TalentForLevel
		//pet->SetHealth(pet->GetMaxHealth());
		//pet->SetHealth(1000);
		
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, level - 1);
		player->GetMap()->Add((Creature*)pet);// 添加到地图
		pet->SetUInt32Value(UNIT_FIELD_LEVEL, level);//升级效果

        player->SetPet(pet);
        player->PetSpellInitialize();//刷新客户端ActionBar
		
		
    }

	((MercenaryPet*)pet)->_ApplyAllStatBonuses();

	pet->SetStandState(UNIT_STAND_STATE_STAND);
	SendMirrorImagePacket(pet);
	pet->HandleEmoteCommandHappy();
	
	//player->context.addDelayedAction(new UpdatePetActionBar(player,pet,1000));//一秒后更新actionbar
}

bool Mercenary::EquipItemIfCan(Player* player, Item* item,bool silenceUpdate)
{
	if (!item)
		return false;

	MercenaryPet* pet = (MercenaryPet*)player->GetPet();
	if (!pet)
		return false;

    const ItemPrototype* proto = item->GetProto();
    if (!proto)
    {
		if (!silenceUpdate)
		{
			ChatHandler(player->GetSession()).SendSysMessage(-2800648);//"Invalid item! Your mercenary could not equip this item."
			sLog.outError("Tried to equip invalid item %u.Item does not have a template.", item->GetEntry());
		}
        return false;
    }

    uint32 itemClass = proto->Class;
    uint32 itemSubClass = proto->SubClass;
    uint32 invType = proto->InventoryType;

	//等级检查
    bool isCorrectLevel = player->getLevel() >= proto->RequiredLevel;
	if (proto->RequiredLevel > 0 && !isCorrectLevel)
	{
		if (!silenceUpdate)
			ChatHandler(player->GetSession()).PSendSysMessage(-2800649, proto->RequiredLevel);//"装备物品失败，物品等级太高。需要达到等级%u"
		return false;
	}

	//插孔匹配检查
	if (!InvToSlot(invType, editSlot))
	{
		if (!silenceUpdate)
			ChatHandler(player->GetSession()).SendSysMessage(-2800652);//装备无法放入这个位置
		return false;
	}

	//职业专精检查
	bool isCorrectProficiency = sMercenaryMgr->CheckProficiencies(type, itemClass, itemSubClass);
    if (!isCorrectProficiency)
    {
		if (!silenceUpdate&&itemClass == ITEM_CLASS_ARMOR)
			ChatHandler(player->GetSession()).SendSysMessage(-2800650);//装备物品失败，护甲专精不符。
		else if (!silenceUpdate&&itemClass == ITEM_CLASS_WEAPON)
			ChatHandler(player->GetSession()).SendSysMessage(-2800651);//装备物品失败，武器专精不符。
        return false;
    }

	uint32 itemid = item->GetEntry();

	if (!silenceUpdate){

			gearContainer[editSlot].itemguid = item->GetGUIDLow();
			gearContainer[editSlot].itemid = itemid;
				
			pet->EquipItem(editSlot, item, true);//这里更新状态，比较复杂，暂时不采用
			pet->AutoUnequipOffhandIfNeed();		//一旦副手移除，要调用pet->RemoveItem(editSlot, true);
			//pet->UpdateAllStats();//全面更新，无效
    }


	if (!silenceUpdate){
		SendMirrorImagePacket(pet);
	}
    return true;
}

Item* Mercenary::GetItemByGuid(Player * player,uint32 guid) 
{
	/*for (int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (pItem->GetObjectGuid().GetCounter() == guid)
		return pItem;

	for (int i = KEYRING_SLOT_START; i < CURRENCYTOKEN_SLOT_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (pItem->GetObjectGuid().GetCounter() == guid)
		return pItem;
	*/
	for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (pItem->GetObjectGuid().GetCounter() == guid)
		return pItem;

	for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
	if (Bag* pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* pItem = pBag->GetItemByPos(j))
	if (pItem->GetObjectGuid().GetCounter() == guid)
		return pItem;

	for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (pItem->GetObjectGuid().GetCounter() == guid)
		return pItem;

	for (int i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
	if (Bag* pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* pItem = pBag->GetItemByPos(j))
	if (pItem->GetObjectGuid().GetCounter() == guid)
		return pItem;

	return nullptr;
}

bool Mercenary::InitStats(Player* player, MercenaryPet* pet)
{
    uint8 mercenaryLevel = player->getLevel();
    //CreatureInfo const* creatureInfo = pet->GetCreatureInfo();
    //if (!creatureInfo)
    //{
     //   sLog.outError("Failed to load creature info for Mercenary");
     //   return false;
    //}

  
    //pet->SetMeleeDamageSchool(SpellSchools(creatureInfo->DamageSchool));

    //@TODO Below, Change by stats (gear)
    //pet->SetModifierValue(UNIT_MOD_ARMOR, BASE_VALUE, float(mercenaryLevel * 50));
    //pet->SetAttackTime(BASE_ATTACK, BASE_ATTACK_TIME);
    //pet->SetAttackTime(OFF_ATTACK, BASE_ATTACK_TIME);
    //pet->SetAttackTime(RANGED_ATTACK, BASE_ATTACK_TIME);
    //pet->SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);

    //pet->SetObjectScale(1.0f);

    //int32 createResistance[MAX_SPELL_SCHOOL] = { 0, 0, 0, 0, 0, 0, 0 };
    //createResistance[SPELL_SCHOOL_HOLY] = creatureInfo->ResistanceHoly;
    //createResistance[SPELL_SCHOOL_FIRE] = creatureInfo->ResistanceFire;
    //createResistance[SPELL_SCHOOL_NATURE] = creatureInfo->ResistanceNature;
    //createResistance[SPELL_SCHOOL_FROST] = creatureInfo->ResistanceFrost;
    //createResistance[SPELL_SCHOOL_SHADOW] = creatureInfo->ResistanceShadow;
    //createResistance[SPELL_SCHOOL_ARCANE] = creatureInfo->ResistanceArcane;
    //for (uint8 i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
       //pet->SetModifierValue(UnitMods(UNIT_MOD_RESISTANCE_START + i), BASE_VALUE, float(createResistance[i]));


	/*装备更新*/
	
	for (int i = 0; i < PLAYER_SLOTS_COUNT;i++)
		pet->m_items[i]=nullptr;

	for (auto itr = gearContainer.begin(); itr != gearContainer.end(); itr++){
		Item* item = GetItemByGuid(player, itr->second.itemguid);
		pet->m_items[itr->first] = (item != nullptr) ? item : nullptr;
	}
	
	//((MercenaryPet*)pet)->InitStatsForLevelPlayer(false);
	((MercenaryPet*)pet)->GiveLevel(mercenaryLevel);
	

    //pet->SetCreateHealth(uint32(((float(creatureInfo->MaxLevelHealth) / creatureInfo->MaxLevel) / (1 + 2 * creatureInfo->Rank)) * mercenaryLevel));
    //pet->SetCreateMana(uint32(((float(creatureInfo->MaxLevelMana) / creatureInfo->MaxLevel) / (1 + 2 * creatureInfo->Rank)) * mercenaryLevel));

    //pet->SetCreateStat(STAT_STRENGTH, 22);
    //pet->SetCreateStat(STAT_AGILITY, 22);
    //pet->SetCreateStat(STAT_STAMINA, 25);
    //pet->SetCreateStat(STAT_INTELLECT, 28);
    //pet->SetCreateStat(STAT_SPIRIT, 27);


    //pet->SetUInt32Value(UNIT_FIELD_PETNEXTLEVELEXP, uint32(sObjectMgr.GetXPForLevel(mercenaryLevel) * 0.05f));

    //pet->SetBaseWeaponDamage(BASE_ATTACK, MINDAMAGE, float(mercenaryLevel - (mercenaryLevel / 4)));
    //pet->SetBaseWeaponDamage(BASE_ATTACK, MAXDAMAGE, float(mercenaryLevel + (mercenaryLevel / 4)));

    //for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i)
        //UpdateStats(player, Stats(i), pet);
    //for (uint8 i = POWER_MANA; i < MAX_POWERS; ++i)
        //UpdateMaxPower(Powers(i), pet);

   // for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
        //pet->UpdateResistances(i);
    //pet->SetHealth(pet->GetMaxHealth());

    //pet->SetPower(POWER_MANA, pet->GetMaxPower(POWER_MANA));

	if (isRangedAttack())
		SetSheath(pet,SHEATH_STATE_RANGED);
	else
		SetSheath(pet, SHEATH_STATE_MELEE);

	//((MercenaryPet*)pet)->InitStatsForLevelPlayer(false);
	//((MercenaryPet*)pet)->UpdateAllStats();

	
    return true;
}
void Mercenary::SetSheath(Pet* pet,SheathState sheathed)
{
	if (!pet->isMercenary())
	{
		pet->SetSheath(sheathed);
		return;
	}
	((MercenaryPet*)pet)->SetSheath(sheathed);
	
	//pet->SetSheath(sheathed);                              // this must visualize Sheath changing for other players...
}
bool Mercenary::isItemsEquippable(Item* item, uint8 slot){

	const ItemPrototype* proto = item->GetProto();
	if (!proto)
		return false;
	return InvToSlot(proto->InventoryType, slot);
}

void Mercenary::GetEquippableItems(Player* player, uint8 slot, std::vector<Item*>& result)
{
	/*for (int i = EQUIPMENT_SLOT_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (isItemsEquippable(item, slot))
	tempVector.push_back(item->GetEntry());//身上装备

	for (int i = KEYRING_SLOT_START; i < CURRENCYTOKEN_SLOT_END; ++i)
	if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (isItemsEquippable(item, slot))
	tempVector.push_back(item->GetEntry());//钥匙袋中
	*/

	for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (isItemsEquippable(item, slot))
		result.push_back(item);//行李栏

	for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
	if (Bag* pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* item = pBag->GetItemByPos(j))
	if (isItemsEquippable(item, slot))
		result.push_back(item);//行李栏包裹

	for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
	if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	if (isItemsEquippable(item, slot))
		result.push_back(item);//银行

	for (int i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
	if (Bag* pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* item = pBag->GetItemByPos(j))
	if (isItemsEquippable(item, slot))
		result.push_back(item);//银行包裹

}/*
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

	for (auto itr = gearContainer.begin(); itr != gearContainer.end(); ++itr)
    {
		const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->second->itemid);
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
}*/

void Mercenary::SetValues(uint32 model, uint8 r, uint8 g)
{
    displayId = model;
    race = r;
    gender = g;
}
void Mercenary::RemoveItemBySlot(Player* player,MercenaryPet* pet, uint32 editSlot)
{
	if (!pet)
		return;


	
	//gearContainer[editSlot].itemid = 0; 在RemoveItem方法中取得item后删除
	//gearContainer[editSlot].itemguid = 0;




	pet->RemoveItem(editSlot, true);//移除并更新统计状态，比较复杂

	//pet->UpdateAllStats();//全部更新，无效

	SendMirrorImagePacket(pet);
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

	if (creature->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
		data << uint32(0);
	else
		data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_HEAD].itemid));

	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_SHOULDERS].itemid)); 
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_SHIRT].itemid)); // Shirt?
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_CHEST].itemid)); 
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_WAIST].itemid)); // Waist?
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_LEGS].itemid)); 
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_FEET].itemid)); 
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_WRISTS].itemid)); // Wrists?
	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_HANDS].itemid));
	
	if (creature->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
		data << uint32(0);
	else
		data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_BACK].itemid)); // Cloak?

	data << uint32(sMercenaryMgr->GetItemDisplayId(gearContainer[SLOT_TABARD].itemid)); // Tabard?
	data << uint32(0); // SLOT_EMPTY?

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
	for (auto itr = gearContainer.begin(); itr != gearContainer.end(); ++itr)
    {
        if (itr->first == SLOT_MAIN_HAND || itr->first == SLOT_OFF_HAND)
        {
			const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->second.itemid);
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

	for (auto itr = gearContainer.begin(); itr != gearContainer.end(); ++itr)
    {
        const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itr->second.itemid);
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

bool Mercenary::InvToSlot(uint8 invType, uint8 characterSlot)
{
	switch (characterSlot){
		case SLOT_HEAD:
			return invType == INVTYPE_HEAD;
		case SLOT_NECK:
			return 	invType==INVTYPE_NECK;
		case SLOT_SHOULDERS:
			return invType==INVTYPE_SHOULDERS;
		case SLOT_SHIRT:
			return invType == INVTYPE_BODY ;//衬衣
		case SLOT_CHEST:
			return invType == INVTYPE_CHEST || invType == INVTYPE_ROBE; //胸部，连衣裙
		case SLOT_WAIST:
			return invType==INVTYPE_WAIST;
		case SLOT_LEGS:
			return invType==INVTYPE_LEGS;
		case SLOT_FEET:
			return invType==INVTYPE_FEET;
		case SLOT_WRISTS:
			return invType==INVTYPE_WRISTS;
		case SLOT_HANDS:
			return invType==INVTYPE_HANDS;
		case SLOT_FINGER1:
			return invType==INVTYPE_FINGER;
		case SLOT_FINGER2:
			return invType==INVTYPE_FINGER;
		case SLOT_TRINKET1:
			return invType==INVTYPE_TRINKET;
		case SLOT_TRINKET2:
			return invType==INVTYPE_TRINKET;
		case SLOT_BACK:
			return invType == INVTYPE_CLOAK ;//披风，连衣裙
		case SLOT_MAIN_HAND:
			return invType == INVTYPE_WEAPON || invType == INVTYPE_WEAPONMAINHAND || invType == INVTYPE_2HWEAPON;//武器，主手武器，或双手武器
		case SLOT_OFF_HAND:
			return invType == INVTYPE_WEAPON || invType == INVTYPE_WEAPONOFFHAND || invType == INVTYPE_SHIELD || invType == INVTYPE_HOLDABLE;//副手武器或者盾牌
		case SLOT_RANGED:
			return invType == INVTYPE_RANGED || invType == INVTYPE_THROWN || invType == INVTYPE_RANGEDRIGHT;//远程或投掷
		case SLOT_TABARD:
			return invType==INVTYPE_TABARD;//战袍徽章
		//case SLOT_EMPTY:
			//return invType == INVTYPE_AMMO || invType == INVTYPE_RANGEDRIGHT; //弹药或者远程武器右侧的
	}

	return false;
}
/*遗留问题

SLOT_SHIRT---->INVTYPE_BODY = 4  对吗?
SLOT_EMPTY---->INVTYPE_AMMO = 24,对吗?
SLOT_EMPTY---->INVTYPE_RANGEDRIGHT = 26,

INVTYPE_RELIC = 28 圣物不能装备
INVTYPE_BAG = 18,不能装备
INVTYPE_QUIVER = 27,和包裹一样，都不能装备到角色solot，只能放行李栏


*/
/*
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
*/