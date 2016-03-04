/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#include "MercenaryMgr.h"

MercenaryMgr::MercenaryMgr() { }

MercenaryMgr::~MercenaryMgr()
{
}

void MercenaryMgr::SaveToList(Mercenary* mercenary)
{
	MercenaryContainer[mercenary->GetOwnerGUID()] = mercenary;
}
/*角色登录时加载，注销时不删除，每人限一个雇佣兵*/
void MercenaryMgr::LoadMercenarie(Player* player)
{
	QueryResult* result = CharacterDatabase.PQuery("SELECT role, displayId, race, gender, type FROM mercenaries  where ownerGUID='%u'", player->GetGUIDLow());
	if (result)
	{
			Mercenary* mercenary = new Mercenary;
			if (!mercenary->LoadFromDB(player,result))
			{
				delete mercenary;
				return;
			}
			MercenaryContainer[mercenary->GetOwnerGUID()] = mercenary;
	}
}
/*加载配置信息，应该在world库中*/
void MercenaryMgr::	LoadMercenaries()
{
    sLog.outBasic("Loading Mercenaries...");
    QueryResult* result = WorldDatabase.PQuery("SELECT mercenaryType, mercenaryRole, entry, headEntry, shoulderEntry, chestEntry, legEntry, handEntry, feetEntry, weaponEntry, "
		"offHandEntry, rangedEntry FROM mercenary_start_gear");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenaryStarterGear starterGear;
            starterGear.mercenaryType = fields[0].GetUInt8();
            starterGear.mercenaryRole = fields[1].GetUInt8();
            starterGear.creature_entry = fields[2].GetUInt32();
            starterGear.headEntry = fields[3].GetUInt32();
            starterGear.shoulderEntry = fields[4].GetUInt32();
            starterGear.chestEntry = fields[5].GetUInt32();
            starterGear.legEntry = fields[6].GetUInt32();
            starterGear.handEntry = fields[7].GetUInt32();
            starterGear.feetEntry = fields[8].GetUInt32();
            starterGear.weaponEntry = fields[9].GetUInt32();
            starterGear.offHandEntry = fields[10].GetUInt32();
            starterGear.rangedEntry = fields[11].GetUInt32();

            MercenaryStartGearContainer.push_back(starterGear);
        } while (result->NextRow());
    }

	result = WorldDatabase.Query("SELECT type, role, healthpct, message FROM mercenary_talk");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenaryTalking mercenaryTalking;
            mercenaryTalking.type = fields[0].GetUInt8();
            mercenaryTalking.role = fields[1].GetUInt8();
            mercenaryTalking.healthPercentageToTalk = fields[2].GetUInt8();
            mercenaryTalking.text = fields[3].GetString();

            MercenaryTalkContainer.push_back(mercenaryTalking);
        } while (result->NextRow());
    }

	result = WorldDatabase.Query("SELECT type, role, spellId, isDefaultAura, isActive FROM mercenary_spells");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenarySpell spells;
            spells.type = fields[0].GetUInt8();
            spells.role = fields[1].GetUInt8();
            spells.spellId = fields[2].GetUInt32();
            spells.isDefaultAura = fields[3].GetBool();
            spells.isActive = fields[4].GetBool();

            MercenarySpellsContainer.push_back(spells);
        } while (result->NextRow());
    }

	result = WorldDatabase.Query("SELECT entry, modelId, race, gender FROM mercenary_world");
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            MercenaryWorld world;
            world.modelId = fields[1].GetUInt32();
            world.race = fields[2].GetUInt8();
            world.gender = fields[3].GetUInt8();

            MercenaryWorldContainer[fields[0].GetUInt32()] = world;
        } while (result->NextRow());
    }

	result = WorldDatabase.Query("SELECT type, armor_prof, weapon_prof FROM mercenary_proficiencies");
    if (result)
    {
        do
        {
            Field* field = result->Fetch();

            MercenaryProficiency prof;
            prof.type = field[0].GetUInt8();
            prof.armorSubClass = uint32(field[1].GetUInt8());
            prof.weaponSubClass = uint32(field[2].GetUInt8());

            MercenaryProficiencyContainer.push_back(prof);
        } while (result->NextRow());
    }

    sLog.outBasic("Loading Mercenaries Completed..");

}


void MercenaryMgr::OnSave(Player* player)
{

    Mercenary* mercenary = GetMercenaryByOwner(player->GetGUIDLow());
    if (!mercenary)
        return;
	if (mercenary->IsSummoned())
		mercenary->SaveToDB();

}

void MercenaryMgr::OnSummon(Player* player)
{
    if (Mercenary* mercenary = GetMercenaryByOwner(player->GetGUIDLow()))
        mercenary->Summon(player);
}

std::string MercenaryMgr::GetSpellIcon(uint32 entry, WorldSession* session) const
{
    std::ostringstream ss;
    ss << "|T";
//#ifndef MANGOS
//    const SpellInfo* temp = sSpellMgr->GetSpellInfo(entry);
//#else
    const SpellEntry* temp = sSpellStore.LookupEntry(entry);
//#endif
    const SpellIconEntry* icon = nullptr;
    std::string name = "";
    if (temp)
    {
        name = temp->SpellName[session->GetSessionDbcLocale()];
        icon = sSpellIconStore.LookupEntry(temp->SpellIconID);
        if (icon)
            ss << icon->spellIcon;
    }
    if (!icon)
        ss << "Interface/ICONS/InventoryItems/WoWUnknownItem01";
    ss << ":" << 32 << ":" << 32 << ":" << -18 << ":" << 0 << "|t" << name;
    return ss.str();
}

std::string MercenaryMgr::GetItemIcon(uint32 entry, bool rawPath) const
{
    std::ostringstream ss;
    ss << "|TInterface";
//#ifndef MANGOS
//    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
//#else
    const ItemPrototype* temp = sObjectMgr.GetItemPrototype(entry);
//#endif
    const ItemDisplayInfoEntry* dispInfo = nullptr;
    if (temp)
    {
        dispInfo = sItemDisplayInfoStore.LookupEntry(temp->DisplayInfoID);
        if (dispInfo)
        {
            ss << "/ICONS/" << dispInfo->inventoryIcon;
            if (rawPath)
            {
                ss.str("");
                ss << "Interface/ICONS/" << dispInfo->inventoryIcon;
                return ss.str();
            }
        }
    }
    if (!dispInfo)
        ss << "/InventoryItems/WoWUnknownItem01";
    ss << ":" << 32 << ":" << 32 << ":" << -18 << ":" << 0 << "|t";
    return ss.str();
}

std::string MercenaryMgr::GetSlotIcon(uint8 slot) const
{
    std::ostringstream ss;
    ss << "|TInterface/PaperDoll/";
	switch (slot)
    {
        case SLOT_HEAD: ss << "UI-PaperDoll-Slot-Head"; break;
        case SLOT_SHOULDERS: ss << "UI-PaperDoll-Slot-Shoulder"; break;
        case SLOT_SHIRT: ss << "UI-PaperDoll-Slot-Shirt"; break;
        case SLOT_CHEST: ss << "UI-PaperDoll-Slot-Chest"; break;
        case SLOT_WAIST: ss << "UI-PaperDoll-Slot-Waist"; break;
        case SLOT_LEGS: ss << "UI-PaperDoll-Slot-Legs"; break;
        case SLOT_FEET: ss << "UI-PaperDoll-Slot-Feet"; break;
        case SLOT_WRISTS: ss << "UI-PaperDoll-Slot-Wrists"; break;
        case SLOT_HANDS: ss << "UI-PaperDoll-Slot-Hands"; break;
        case SLOT_BACK: ss << "UI-PaperDoll-Slot-Chest"; break;
        case SLOT_MAIN_HAND: ss << "UI-PaperDoll-Slot-MainHand"; break;
        case SLOT_OFF_HAND: ss << "UI-PaperDoll-Slot-SecondaryHand"; break;
        case SLOT_RANGED: ss << "UI-PaperDoll-Slot-Ranged"; break;
        case SLOT_TABARD: ss << "UI-PaperDoll-Slot-Tabard"; break;
        default: ss << "UI-Backpack-EmptySlot";
    }
    ss << ":" << 32 << ":" << 32 << ":" << -18 << ":" << 0 << "|t";
    return ss.str();
}

const char* MercenaryMgr::GetSlotName(Player* player,uint8 slot) const
{
    switch (slot)
    {
	case SLOT_HEAD: return  player->GetMangosString(-2800654);// "Head";
	case SLOT_SHOULDERS: return  player->GetMangosString(-2800655); //"Shoulders";
	case SLOT_SHIRT: return  player->GetMangosString(-2800656); //"Shirt";
	case SLOT_CHEST: return  player->GetMangosString(-2800657); //"Chest";
	case SLOT_WAIST: return  player->GetMangosString(-2800658); //"Waist";
	case SLOT_LEGS: return player->GetMangosString(-2800659);// "Legs";
	case SLOT_FEET: return player->GetMangosString(-2800660);// "Feet";
	case SLOT_WRISTS: return player->GetMangosString(-2800661);// "Wrists";
	case SLOT_HANDS: return player->GetMangosString(-2800662);// "Hands";
	case SLOT_BACK: return player->GetMangosString(-2800663);// "Back";
	case SLOT_MAIN_HAND: return  player->GetMangosString(-2800664); //"Main hand";
	case SLOT_OFF_HAND: return player->GetMangosString(-2800665);// "Off hand";
	case SLOT_RANGED: return player->GetMangosString(-2800666);// "Ranged";
	case SLOT_TABARD: return player->GetMangosString(-2800667);// "Tabard";
        default: return "";
    }
}

std::string MercenaryMgr::GetItemLink(uint32 entry, WorldSession* session) const
{
//#ifndef MANGOS
//    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
//#else
    const ItemPrototype* temp = sObjectMgr.GetItemPrototype(entry);
//#endif
    int loc_idx = session->GetSessionDbLocaleIndex();
    std::string name = temp->Name1;
//#ifndef MANGOS
//    if (ItemLocale const* il = sObjectMgr->GetItemLocale(entry))
//        ObjectMgr::GetLocaleString(il->Name, loc_idx, name);
//#else
    if (ItemLocale const* il = sObjectMgr.GetItemLocale(entry))
		sObjectMgr.GetItemLocaleStrings(entry, loc_idx, &name);
	//void GetItemLocaleStrings(uint32 entry, int32 loc_idx, std::string* namePtr, std::string* descriptionPtr = nullptr) const;
//#endif

    std::ostringstream oss;
    oss << "|c" << std::hex << ItemQualityColors[temp->Quality] << std::dec <<
        "|Hitem:" << entry << ":0:0:0:0:0:0:0:0:0|h[" << name << "]|h|r";

    return oss.str();
}


Random MercenaryUtil::GetMercenaryRandom(){ return sMercenaryMgr->random; }
Mercenary* MercenaryUtil::GetMercenaryByOwner(uint32 ownerGUID){ return sMercenaryMgr->GetMercenaryByOwner(ownerGUID); }
MercenarySpells::const_iterator MercenaryUtil::MercenarySpellsBegin(){ return sMercenaryMgr->MercenarySpellsBegin(); }
MercenarySpells::const_iterator MercenaryUtil::MercenarySpellsEnd(){ return sMercenaryMgr->MercenarySpellsEnd(); }
std::vector<MercenaryTalking> MercenaryUtil::MercenaryGetTalk(uint8 type, uint8 role){ return sMercenaryMgr->GetTalk(type, role); }
std::string MercenaryUtil::GetMercenarySlotIcon(uint8 slot)  { return sMercenaryMgr->GetSlotIcon(slot); }
const char* MercenaryUtil::GetMercenarySlotName(Player* player, uint8 slot)  { return sMercenaryMgr->GetSlotName(player,slot); }
std::string MercenaryUtil::GetMercenaryItemIcon(uint32 entry, bool rawPath) { return sMercenaryMgr->GetItemIcon(entry, rawPath); }
std::string MercenaryUtil::GetMercenaryItemLink(uint32 entry, WorldSession* session) { return sMercenaryMgr->GetItemLink(entry, session); }
MercenaryStarterGear* MercenaryUtil::GetMercenaryStarterGearByEntry(uint32 entry){ return sMercenaryMgr->GetStarterGearByEntry(entry); }
const ItemPrototype* MercenaryUtil::GetItemPrototype(uint32 entry){ return sObjectMgr.GetItemPrototype(entry); }
std::string MercenaryUtil::GetMercenarySpellIcon(uint32 entry, WorldSession* session) { return sMercenaryMgr->GetSpellIcon(entry, session); }
MercenaryWorld* MercenaryUtil::GetMercenaryWorldData(uint32 entry){ return sMercenaryMgr->GetMercenaryWorldData(entry); }
