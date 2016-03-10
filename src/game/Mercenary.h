/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#ifndef MERCENARY_H
#define MERCENARY_H

#include "Player.h"
#include "Chat.h"
#include "Pet.h"
#include "Spell.h"

//#ifndef MANGOS
//#include "SpellAuraEffects.h"
//#else
#include "ObjectMgr.h"
//#endif

struct MercenaryRoleDef
{
	uint8 role;
	std::string name;
	std::string classname;
};
struct MercenarySpellGroup
{
	tbb::concurrent_vector<uint32> spellLevelVector;/*由大到小排序*/
	tbb::concurrent_vector<uint32> spellIdVector;   /*id数组*/
};
struct MercenarySpell
{
    uint8 type;
    uint8 role;
    uint32 spellId;
	//std::string comment;
	//uint32 spelllevel;
	bool isDefaultAura;
    bool isActive;
};


struct MercenaryStarterGear
{
    uint8 mercenaryType;
    uint8 mercenaryRole;
    uint32 creature_entry;
    uint32 headEntry;
    uint32 shoulderEntry;
    uint32 chestEntry;
    uint32 legEntry;
    uint32 handEntry;
    uint32 feetEntry;
    uint32 weaponEntry;
    uint32 offHandEntry;
    uint32 rangedEntry;
};

struct MercenaryTalking
{
    uint8 type;
    uint8 role;
    // Leave at 100 if you want to use it for random talking
    uint8 healthPercentageToTalk;
    std::string text;
};

struct MANGOS_DLL_SPEC MercenaryWorld
{
    uint32 modelId;
    uint8 race;
    uint8 gender;
};

struct MercenaryProficiency
{
    uint8 type;
    uint32 armorSubClass;
    uint32 weaponSubClass;
};

struct RandomMercenary
{
    uint32 model;
    uint8 race;
    uint8 gender;
};

struct RandomMercenaryTypeRole
{
    uint8 type;
    uint8 role;
};

/*将雇佣兵的职业修改为一致
update mercenary_proficiencies set type=5 where type=3;
update mercenary_proficiencies set type=6 where type=4;
update mercenary_proficiencies set type=9 where type=5;
update mercenary_proficiencies set type=8 where type=6;
update mercenary_proficiencies set type=11 where type=7;
update mercenary_proficiencies set type=3 where type=8;
update mercenary_proficiencies set type=4 where type=9;
update mercenary_proficiencies set type=7 where type=10;

update mercenary_spells set type=5 where type=3;
update mercenary_spells set type=6 where type=4;
update mercenary_spells set type=9 where type=5;
update mercenary_spells set type=8 where type=6;
update mercenary_spells set type=11 where type=7;
update mercenary_spells set type=3 where type=8;
update mercenary_spells set type=4 where type=9;
update mercenary_spells set type=7 where type=10;

update mercenary_start_gear set mercenaryType=5 where mercenaryType=3;
update mercenary_start_gear set mercenaryType=6 where mercenaryType=4;
update mercenary_start_gear set mercenaryType=9 where mercenaryType=5;
update mercenary_start_gear set mercenaryType=8 where mercenaryType=6;
update mercenary_start_gear set mercenaryType=11 where mercenaryType=7;
update mercenary_start_gear set mercenaryType=3 where mercenaryType=8;
update mercenary_start_gear set mercenaryType=4 where mercenaryType=9;
update mercenary_start_gear set mercenaryType=7 where mercenaryType=10;

*/
enum MercenaryType
{
    MERCENARY_TYPE_NONE=0,
    MERCENARY_TYPE_WARRIOR=1,//战士，能拿盾护甲专精6，能穿护甲专精锁甲4
    MERCENARY_TYPE_PALADIN=2,//圣骑士，能拿盾护甲专精6，能穿护甲专精锁甲4
	MERCENARY_TYPE_HUNTER = 3, //猎人，能穿护甲专精3锁甲
	MERCENARY_TYPE_ROGUE = 4, //潜行者，能穿护甲专精2皮甲
	MERCENARY_TYPE_PRIEST=5,  //牧师，护甲专精1
    MERCENARY_TYPE_DK=6,     //死亡骑士，能穿护甲专精锁甲4
	MERCENARY_TYPE_SHAMAN = 7,  //萨满，能拿盾护甲专精6
	MERCENARY_TYPE_MAGE=8,   //法师，护甲专精1
	MERCENARY_TYPE_WARLOCK = 9, //术士，布甲，护甲专精1
	MERCENARY_TYPE_NOUSE = 10, //没有用占位置
    MERCENARY_TYPE_DRUID=11, //德鲁伊，能穿护甲专精2皮甲


    MAX_MERCENARY_TYPES=12
};

enum RaceModels
{
    HUMAN_MALE_MODEL = 49,
    HUMAN_FEMALE_MODEL = 50,
    ORC_MALE_MODEL = 51,
    ORC_FEMALE_MODEL = 52,
    DWARF_MALE_MODEL = 53,
    DWARF_FEMALE_MODEL = 54,
    NIGHTELF_MALE_MODEL = 55,
    NIGHTELF_FEMALE_MODEL = 56,
    SCOURGE_MALE_MODEL = 57,
    SCOURGE_FEMALE_MODEL = 58,
    TAUREN_MALE_MODEL = 59,
    TAUREN_FEMALE_MODEL = 60,
    GNOME_MALE_MODEL = 1563,
    GNOME_FEMALE_MODEL = 1564,
    TROLL_MALE_MODEL = 1478,
    TROLL_FEMALE_MODEL = 1479,
    BLOODELF_MALE_MODEL = 15476,
    BLOODELF_FEMALE_MODEL = 15475,
    DRAENEI_MALE_MODEL = 16125,
    DRAENEI_FEMALE_MODEL = 16126
};

#define MERCENARY_DEFAULT_ENTRY 70000

class MANGOS_DLL_SPEC Mercenary
{
public:
    Mercenary();
    Mercenary(uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type);
    ~Mercenary();

    /*
    * Loads the Mercenary's gear from the database into a vector
    */
    void LoadGearFromDB();
    /*
    * Loads the Mercenary from the database into a map
    */
//#ifndef MANGOS
//    bool LoadFromDB(QueryResult result);
//#else
	bool LoadFromDB(Player* player,QueryResult* result);
//#endif
    /*
    * Temporarily adds a new Mercenary being created into the Mercenary map
    * If creation of this temporary Mercenary isn't finished the Mercenary will NOT save
    */
    bool Create(Player* player);
    /*
    * Creates and spawns the Mercenary
    */
	bool Create(Player* player, uint32 model, uint8 race, uint8 gender, uint8 mercenaryType, uint8 role, const std::string& name = "");
    /*
    * Mercenary will learn the given spellId
    * Maximum spells Mercenary can have is 4
    */
    bool LearnSpell(Player* player, uint32 spellId);
    /*
    * Summons the Mercenary
    */
    bool Summon(Player* player);
    /*
    * Initializes Mercenary's stats
    */
    bool InitStats(Player* player, Pet* pet);
    /*
    * Updates Mercenary's stats
    */
    bool UpdateStats(Player* player, Stats /*stats*/, Pet* pet);
    /*
    * Updates all Mercenary's stats
    */
    bool UpdateAllStats(Player* player, Pet* pet);
    /*
    * Returns true if the Mercenary can equip the specified item
    */
    bool CanEquipItem(Player* player, Item* item);
    /*
    * Initializes Mercenary's stats, gear and other summon values
    */
    void Initialize(Player* player, Pet* pet, bool create);

	/*清理不匹配的技能*/
	void Mercenary::cleanNoMatchSpell(Pet* pet);

	/*清理不匹配的装备*/
	void Mercenary::clearnNoMatchEquipItem();

	/*技能有效性判断*/
	bool isValidSpell(uint32 spell);
    /*
    * Sets the displayId, race and gender values of the Mercenary
    */
    void SetValues(uint32 model, uint8 race, uint8 gender);
    /*
    * Sets the role of the Mercenary
    */
    void SetRole(const uint8 newRole) { 
		//if (role != newRole)
			//GearContainer.clear();攻防切换，装备可以不换
		role = newRole; 
	}
    /*
    * Sets the type of the Mercenary
    */
    void SetType(const uint8 newType) { 
		if (type != newType)//职业切换，装备必须切换
			GearContainer.clear();//穿戴时检查好些
		type = newType; 
	}
    /*
    * Sets the equip slot the Mercenary owner is currently editing
    */
    void SetEditSlot(const uint8 slot) { editSlot = slot; }
    /*
    * Updates the Mercenary's Max Power
    */
    void UpdateMaxPower(Powers /* power */, Pet* pet);
    /*
    * Updates the Mercenary's Max Health
    */
    void UpdateMaxHealth(Pet* pet);
    /*
    * Updates the Mercenary's Armor
    */
    void UpdateArmor(Pet* pet);
    /*
    * Updates the Mercenary's strength
    */
    void UpdateStrength(Pet* pet, bool ranged = false);
    /*
    * Updates the Mercenary's physical damage
    */
    void UpdatePhysicalDamage(WeaponAttackType /* attackType */, Pet* pet);
    /*
    * Saves the Mercenary to the database
    */
    void SaveToDB();
    /*
    * Sends the Mirror Image packet after equipping new armor
    */
    void SendMirrorImagePacket(Creature* creature);
    /*
    * Removes the Mercenary's off hand weapon
    */
    void RemoveOffHand(Creature* creature);

	void SetSheath(Pet* pet, SheathState sheathed);

    /*
    * Returns InventorySlot Id by supplying a CharacterSlot Id
    */
    uint8 GetInvTypeSlot(uint8 characterSlot) const;

    uint32 GetOwnerGUID() const { return ownerGUID; }
    uint8 GetRole() const { return role; }
    uint32 GetDisplay() const { return displayId; }
    uint8 GetRace() const { return race; }
    uint8 GetGender() const { return gender; }
    uint8 GetType() const { return type; }
    bool IsSummoned() const { return summoned; }
    uint8 GetEditSlot() const { return editSlot; }

	typedef tbb::concurrent_unordered_map<uint8, uint32> Gear;/*slot , itemid*/

    Gear::const_iterator GearBegin() const { return GearContainer.begin(); }
    Gear::const_iterator GearEnd() const { return GearContainer.end(); }

    /*
    * Returns an item entry Id by slotId. If no item is found, returns NULL
    * @slot: Requires CharacterSlot data type
    */
    uint32 GetItemBySlot(uint8 slot) const
    {
		Gear::const_iterator itr = GearContainer.find(slot);
		if (itr == GearContainer.end()) 
			return NULL;
		return itr->second;
    }

    /*
    * Returns a vector of item entryIds if any items are found that can be equipped on your Mercenary.
    * @player: The player that gets their inventory checked. Most likely the Mercenary owner.
    * @slot: Requires CharacterSlot data type
    */
    std::vector<uint32> GetEquippableItems(Player* player, uint8 slot)
    {
        std::vector<uint32> tempVector;
        for (uint8 i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; i++)
        {
            if (Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
            {
//#ifdef MANGOS
                const ItemPrototype* proto = item->GetProto();
//#else
//                const ItemTemplate* proto = item->GetTemplate();
//#endif
                if (proto)
                {
                    uint8 invSlot = GetInvTypeSlot(slot);
                    if (proto->InventoryType == invSlot && slot != SLOT_MAIN_HAND && slot != SLOT_OFF_HAND)
                        tempVector.push_back(item->GetEntry());
                    if ((proto->InventoryType == invSlot || proto->InventoryType == INVTYPE_2HWEAPON ||
                        proto->InventoryType == INVTYPE_WEAPON) && slot == SLOT_MAIN_HAND)
                        tempVector.push_back(item->GetEntry());
                    if ((proto->InventoryType == invSlot || proto->InventoryType == INVTYPE_SHIELD ||
                        proto->InventoryType == INVTYPE_HOLDABLE) && slot == SLOT_OFF_HAND)
                        tempVector.push_back(item->GetEntry());
                }
            }
        }

        return tempVector;
    }

    /*
    * Returns true if the Mercenary has a weapon equipped, false if not.
    * @parameter: If true, Mercenary has an off hand weapon equipped.
    * If false it checks for a main hand weapon equipped.
    */
    bool HasWeapon(bool offhand)
    {
		return offhand ? (GearContainer[SLOT_OFF_HAND]>0) : (GearContainer[SLOT_MAIN_HAND]>0);
    }
	bool isRangedAttack(){
		return type == MERCENARY_TYPE_HUNTER || type == MERCENARY_TYPE_MAGE || type == MERCENARY_TYPE_WARLOCK || role == 13 || role == 27 || role == 18;
		//法师、术士、猎人、牧师（暗影系）、德鲁伊（平衡系）以及萨满（元素系）
	}
private:
    uint32 ownerGUID;
	uint8 role = 0;
    uint32 displayId;
    uint8 race;
    uint8 gender;
	uint8 type = MERCENARY_TYPE_NONE;
    bool summoned=false;
    uint8 editSlot;
	std::vector<uint32>* defaultSpellVec;
    Gear GearContainer;
};

#endif