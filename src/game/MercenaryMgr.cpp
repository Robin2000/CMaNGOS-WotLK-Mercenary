/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#include "MercenaryMgr.h"
#include "SpellMgr.h"

MercenaryMgr::MercenaryMgr() { }

MercenaryMgr::~MercenaryMgr()
{
}

void MercenaryMgr::SaveToList(Mercenary* mercenary)
{
	MercenaryContainer[mercenary->GetOwnerGUID()] = mercenary;
}
/*角色登录时加载，注销时不删除，每人限一个雇佣兵*/
Mercenary*  MercenaryMgr::LoadMercenarie(Player* player)
{
	QueryResult* result = CharacterDatabase.PQuery("SELECT petnumber,role, displayId, race, gender, type FROM mercenaries  where ownerGUID='%u'", player->GetGUIDLow());
	if (result)
	{
			Mercenary* mercenary = new Mercenary(player);
			if (!mercenary->LoadFromDB(result))
			{
				delete mercenary;
				return nullptr;
			}
			MercenaryContainer[mercenary->GetOwnerGUID()] = mercenary;
			return mercenary;
	}
	return nullptr;
}
/*加载配置信息，应该在world库中*/
void MercenaryMgr::	LoadMercenaries()
{
    sLog.outBasic("Loading Mercenaries...");
    QueryResult* result = WorldDatabase.Query("SELECT type, role, healthpct, message FROM z_mercenary_talk");
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
	//										0   1      2          3            4       5         6         
	result = WorldDatabase.Query("SELECT type, role, spellId, isDefaultAura, isActive,groupid,spelllevel FROM z_mercenary_spells order by type,role,spelllevel desc");
	//									 职业  角色   技能      是否默认     是否激活  分组id    等级
    if (result)
    {
        do
        {
            Field* fields = result->Fetch();
			MercenarySpell* spells = new MercenarySpell();
            spells->type = fields[0].GetUInt8();
			spells->role = fields[1].GetUInt8();
			spells->spellId = fields[2].GetUInt32();
			spells->isDefaultAura = fields[3].GetBool();
			spells->isActive = fields[4].GetBool();
			spells->groupid = fields[5].GetUInt32();
			spells->spellLevel = fields[6].GetUInt32();

			// skip spells with first rank learned as talent (and all talents then also)
			uint32 first_rank = sSpellMgr.GetFirstSpellInChain(spells->spellId);
			if (GetTalentSpellCost(first_rank) > 0)
			{
				spells->learnFromTalent=true;
				//sLog.outBasic("error spellid in z_mercenary_spells %u,first rank learned as talent!", spells->spellId);
				continue;
			}

			SpellEntry const* spellInfo=sSpellStore.LookupEntry(spells->spellId);
			// skip broken spells
			if (!spellInfo||!SpellMgr::IsSpellValid(spellInfo, nullptr, false))
			{
				sLog.outBasic("error spellid in z_mercenary_spells %u,skip broken spells!", spells->spellId);
				continue;
			}
			
			/*-----------------
			mercenarySpellsInfo<spellid,MercenarySpell* >
			mercenarySpellsMap<petClass_petRole,GroupSpellsMap*>
			mercenarySpellGroup<groupid,MercenarySpellGroup*>

			uint8:  0--255
			uint16: 0--65535

			((uint16(petClass)<<3)|petRole

			uint32: 0--4294967295
			petClass_petRole = petClass*1000+petRole 容易理解
			*/
			mercenarySpellsInfo.insert(std::make_pair(spells->spellId, spells));//基本技能-信息
			
			//第一层取得职业对应map
			if (mercenarySpellsMap.find(spells->type) == mercenarySpellsMap.end())
				mercenarySpellsMap.insert(std::make_pair(uint32(spells->type) * 1000 + spells->role, new GroupSpellsMap()));
			
			GroupSpellsMap* groupSpellsMap = mercenarySpellsMap[uint32(spells->type) * 1000 + spells->role];
			
			//第二层取得技能名对应map
			if (groupSpellsMap->find(spells->groupid) == groupSpellsMap->end())
				groupSpellsMap->insert(std::make_pair(spells->groupid, new MercenarySpellGroup()));

			MercenarySpellGroup* mercenarySpellGroup = groupSpellsMap->find(spells->groupid)->second;
			
			mercenarySpellGroup->spellIdVector.push_back(spells->spellId);
			mercenarySpellGroup->spellLevelVector.push_back(fields[6].GetUInt32());//order by type,role,spelllevel

        } while (result->NextRow());
    }

	result = WorldDatabase.Query("SELECT classid, role, rolename,classname FROM z_mercenary_spell_role");
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();

			MercenaryRoleDef* roleDef = new MercenaryRoleDef();
			uint32 classid = fields[0].GetUInt32();
			roleDef->role = fields[1].GetUInt32();
			roleDef->name = fields[2].GetString();
			roleDef->classname = fields[3].GetString();

			std::vector<MercenaryRoleDef*>* roleDefV;
			auto itr = mercenaryRoleDefMap.find(classid);

			if (itr == mercenaryRoleDefMap.end())
			{
				roleDefV = new std::vector<MercenaryRoleDef*>();
				mercenaryRoleDefMap[classid] = roleDefV;
			}
			else
				roleDefV = itr->second;

			roleDefV->push_back(roleDef);


		} while (result->NextRow());
	}
	result = WorldDatabase.Query("SELECT entry, modelId, race, gender FROM z_mercenary_world");
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

	result = WorldDatabase.Query("SELECT type, armor_prof, weapon_prof FROM z_mercenary_proficiencies");
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
	{
		//mercenary->cleanNoMatchSpell(player->GetPet());在数据库加载时已经移除
		mercenary->Summon();
	}
}

std::string MercenaryMgr::GetSpellIcon(uint32 entry, Player* player) const
{
    std::ostringstream ss;
    ss << "|T";
//#ifndef MANGOS
//    const SpellInfo* temp = sSpellMgr->GetSpellInfo(entry);
//#else
    const SpellEntry* temp = sSpellStore.LookupEntry(entry);
//#endif
    const SpellIconEntry* icon = nullptr;
    if (temp)
    {
        icon = sSpellIconStore.LookupEntry(temp->SpellIconID);
        if (icon)
            ss << icon->spellIcon;
    }
    if (!icon)
        ss << "Interface/ICONS/InventoryItems/WoWUnknownItem01";
    ss << ":" << 32 << ":" << 32 << ":" << -18 << ":" << 0 << "|t";
	ss << "|cff0000ff|Hspell:" << entry << "|h[" << player->context.getSpellName(entry) << "]|h|r";
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
		case SLOT_NECK: ss << "UI-PaperDoll-Slot-Neck"; break;
        case SLOT_SHIRT: ss << "UI-PaperDoll-Slot-Shirt"; break;
        case SLOT_CHEST: ss << "UI-PaperDoll-Slot-Chest"; break;
        case SLOT_WAIST: ss << "UI-PaperDoll-Slot-Waist"; break;
        case SLOT_LEGS: ss << "UI-PaperDoll-Slot-Legs"; break;
        case SLOT_FEET: ss << "UI-PaperDoll-Slot-Feet"; break;
        case SLOT_WRISTS: ss << "UI-PaperDoll-Slot-Wrists"; break;
        case SLOT_HANDS: ss << "UI-PaperDoll-Slot-Hands"; break;

		case SLOT_FINGER1: ss << "UI-PaperDoll-Slot-Finger"; break;
		case SLOT_FINGER2: ss << "UI-PaperDoll-Slot-Finger"; break;
		case SLOT_TRINKET1: ss << "UI-PaperDoll-Slot-Trinket"; break;
		case SLOT_TRINKET2: ss << "UI-PaperDoll-Slot-Trinket"; break;

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
	case SLOT_NECK: return  player->GetMangosString(-2800668); //"Shoulders";
	case SLOT_SHOULDERS: return  player->GetMangosString(-2800655); //"Shoulders";
	case SLOT_SHIRT: return  player->GetMangosString(-2800656); //"Shirt";
	case SLOT_CHEST: return  player->GetMangosString(-2800657); //"Chest";
	case SLOT_WAIST: return  player->GetMangosString(-2800658); //"Waist";
	case SLOT_LEGS: return player->GetMangosString(-2800659);// "Legs";
	case SLOT_FEET: return player->GetMangosString(-2800660);// "Feet";
	case SLOT_WRISTS: return player->GetMangosString(-2800661);// "Wrists";
	case SLOT_HANDS: return player->GetMangosString(-2800662);// "Hands";

	case SLOT_FINGER1: return player->GetMangosString(-2800669);// "Left FINGER";
	case SLOT_FINGER2: return player->GetMangosString(-2800670);// "Right FINGER";
	case SLOT_TRINKET1: return player->GetMangosString(-2800671);// "左手饰品";
	case SLOT_TRINKET2: return player->GetMangosString(-2800672);// "右手饰品";

	case SLOT_BACK: return player->GetMangosString(-2800663);// "Back";
	case SLOT_MAIN_HAND: return  player->GetMangosString(-2800664); //"Main hand";
	case SLOT_OFF_HAND: return player->GetMangosString(-2800665);// "Off hand";
	case SLOT_RANGED: return player->GetMangosString(-2800666);// "Ranged";
	case SLOT_TABARD: return player->GetMangosString(-2800667);// "Tabard";
        default: return "";
    }
}
std::string & MercenaryMgr::getSpellName(uint32 spellid){
	MercenarySpell* mercenarySpell = findMercenarySpellsInfoBySpell(spellid);
	if (mercenarySpell != nullptr)
		return sObjectMgr.getSpellNameByGroupid(mercenarySpell->groupid);
	else
		return sObjectMgr.getSpellNameByGroupid(9999);
}
std::string MercenaryMgr::GetItemLink(uint32 entry, WorldSession* session) const
{
//#ifndef MANGOS
//    const ItemTemplate* temp = sObjectMgr->GetItemTemplate(entry);
//#else
    const ItemPrototype* temp = sObjectMgr.GetItemPrototype(entry);
	if (temp == nullptr)
		return "";
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
GroupSpellsMap* MercenaryUtil::findGroupSpellsMapByClass(uint8 classType, uint8 role){ return sMercenaryMgr->findGroupSpellsMapByClass(classType,role); }
std::vector<MercenaryRoleDef*>* MercenaryUtil::findRoleVectorByClass(uint32 classType){ return sMercenaryMgr->findRoleVectorByClass(classType); }
MercenarySpell* MercenaryUtil::findMercenarySpellsInfoBySpell(uint32 spellid){ return sMercenaryMgr->findMercenarySpellsInfoBySpell(spellid); }

MercenarySpell* findMercenarySpellsInfoBySpell(uint32 spellid){ return sMercenaryMgr->findMercenarySpellsInfoBySpell(spellid); }

std::vector<MercenaryTalking> MercenaryUtil::MercenaryGetTalk(uint8 type, uint8 role){ return sMercenaryMgr->GetTalk(type, role); }
std::string MercenaryUtil::GetMercenarySlotIcon(uint8 slot)  { return sMercenaryMgr->GetSlotIcon(slot); }
const char* MercenaryUtil::GetMercenarySlotName(Player* player, uint8 slot)  { return sMercenaryMgr->GetSlotName(player,slot); }
std::string MercenaryUtil::GetMercenaryItemIcon(uint32 entry, bool rawPath) { return sMercenaryMgr->GetItemIcon(entry, rawPath); }
std::string MercenaryUtil::GetMercenaryItemLink(uint32 entry, WorldSession* session) { return sMercenaryMgr->GetItemLink(entry, session); }
const ItemPrototype* MercenaryUtil::GetItemPrototype(uint32 entry){ return sObjectMgr.GetItemPrototype(entry); }
std::string MercenaryUtil::GetMercenarySpellIcon(uint32 entry, Player* player) { return sMercenaryMgr->GetSpellIcon(entry, player); }
MercenaryWorld* MercenaryUtil::GetMercenaryWorldData(uint32 entry){ return sMercenaryMgr->GetMercenaryWorldData(entry); }
std::string & MercenaryUtil::getSpellName(uint32 spellid){ return sMercenaryMgr->getSpellName(spellid); }