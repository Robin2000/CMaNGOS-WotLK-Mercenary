
#ifndef MERCENARY_H
#define MERCENARY_H

#include "Player.h"
#include "Chat.h"
#include "Pet.h"
#include "Spell.h"
#include "MercenaryPet.h"
#include "ObjectMgr.h"

struct GearEntry{
	uint32 itemguid=0;
	uint32 itemid=0;
};

typedef tbb::concurrent_unordered_map<uint8, GearEntry> GearMap;/*slot , GearEntry*/

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
	uint32 groupid; //每个技能可取得其guoupid，再取得本组其它技能
	//std::string comment;
	uint32 spellLevel;
	bool isDefaultAura;
    bool isActive;
	bool learnFromTalent=false;
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
enum RaceKey
{
	HUMAN = 1,
	ORC = 2,
	DWARF = 3,
	NIGHTELF = 4,
	SCOURGE = 5,
	TAUREN = 6,
	GNOME = 7,
	TROLL = 8,
	GGBLIN =9,
	BLOODELF = 10,
	DRAENEI = 11
};
#define MERCENARY_DEFAULT_ENTRY 70000 //70000 1128


class MANGOS_DLL_SPEC Mercenary
{
public:
	Mercenary(Player * player);
	Mercenary(Player * player,uint32 model, uint8 race, uint8 gender, uint8 role, uint8 type);
    ~Mercenary();

	GearMap gearContainer;

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
	bool LoadFromDB(QueryResult* result);
//#endif
    /*
    * Temporarily adds a new Mercenary being created into the Mercenary map
    * If creation of this temporary Mercenary isn't finished the Mercenary will NOT save
    */
    bool Create();
    /*
    * Creates and spawns the Mercenary
    */
	bool Create( uint32 model, uint8 race, uint8 gender, uint8 mercenaryType, uint8 role, const std::string& name = "");
    /*
    * Mercenary will learn the given spellId
    * Maximum spells Mercenary can have is 4
    */
    bool LearnSpell(uint32 spellId);
    /*
    * Summons the Mercenary
    */
    bool Summon();
    /*
    * Initializes Mercenary's stats
    */
	bool InitStats(MercenaryPet* pet);
    /*
    * Updates Mercenary's stats
    */
    //bool UpdateStats(Player* player, Stats /*stats*/, Pet* pet);
    /*
    * Updates all Mercenary's stats
    */
    //bool UpdateAllStats(Player* player, Pet* pet);
    /*
    * Returns true if the Mercenary can equip the specified item
    */
	bool EquipItemIfCan(Item* item, bool silenceUpdate = false);
    /*
    * Initializes Mercenary's stats, gear and other summon values
    */
	void Initialize(MercenaryPet* pet, bool create);

	void InitializeNEW(Pet* pet, bool create);

	/*清理不匹配的技能*/
	void cleanNoMatchSpell(Pet* pet);

	/*清理不匹配的装备*/
	void clearnNoMatchEquipItem();

	void RemoveItemBySlot(MercenaryPet* pet, uint32 editSlot);

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
    bool SetType(const uint8 newType) { 
		if (type != newType)//职业切换，装备必须切换
		{
			for (auto it = gearContainer.begin(); it != gearContainer.end(); it++)
			{
				if (it->second.itemguid > 0)
				{
					ChatHandler(player).SendSysMessage(-2800697);
					return false;
				}
			}
		}
		type = newType; 
		return true;
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
	void SendMirrorImagePacket(Creature* creature, bool showWeapon=true);
    /*
    * Removes the Mercenary's off hand weapon
    */


	void SetSheath(Pet* pet, SheathState sheathed);

    /*
    * Returns InventorySlot Id by supplying a CharacterSlot Id
    */
    /*uint8 GetInvTypeSlot(uint8 characterSlot) const;*/
	bool InvToSlot(uint8 invType, uint8 characterSlot);

	bool disableMirroClone = false;//临时禁用clone标志

	uint32 GetPetNumber() const { return petnumber; }
    uint32 GetOwnerGUID() const { return ownerGUID; }
    uint8 GetRole() const { return role; }
    uint32 GetDisplay() const { return displayId; }
	void SetDisplay(uint32 _displayId) { displayId = _displayId; }
    uint8 GetRace() const { return race; }
    uint8 GetGender() const { return gender; }
    uint8 GetType() const { return type; }
    bool IsSummoned() const { return summoned; }
    uint8 GetEditSlot() const { return editSlot; }

	Item* GetItemByGuid(uint32 guid);


    /*
    * Returns an item entry Id by slotId. If no item is found, returns NULL
    * @slot: Requires CharacterSlot data type
    */
	bool isItemsEquippable(Item* item, uint8 slot);
    /*
    * Returns a vector of item entryIds if any items are found that can be equipped on your Mercenary.
    * @player: The player that gets their inventory checked. Most likely the Mercenary owner.
    * @slot: Requires CharacterSlot data type
    */
	void GetEquippableItems(uint8 slot, std::vector<Item*>& result);

    /*
    * Returns true if the Mercenary has a weapon equipped, false if not.
    * @parameter: If true, Mercenary has an off hand weapon equipped.
    * If false it checks for a main hand weapon equipped.
    */
    bool HasWeapon(bool offhand)
    {
		return offhand ? gearContainer[SLOT_OFF_HAND].itemid>0 : gearContainer[SLOT_MAIN_HAND].itemid>0;
    }
	bool isRangedAttack(){
		return type == MERCENARY_TYPE_HUNTER || type == MERCENARY_TYPE_MAGE || type == MERCENARY_TYPE_WARLOCK || role == 13 || role == 27 || role == 18;
		//法师、术士、猎人、牧师（暗影系）、德鲁伊（平衡系）以及萨满（元素系）
	}
	bool hasItemEquipped(Item* item);
	
	uint32 getCharFemalModel(uint32 race){
		switch (race){
		case HUMAN:
			return HUMAN_FEMALE_MODEL;
		case ORC:
			return ORC_FEMALE_MODEL;
		case DWARF:
			return DWARF_FEMALE_MODEL;
		case NIGHTELF:
			return NIGHTELF_FEMALE_MODEL;
		case SCOURGE:
			return SCOURGE_FEMALE_MODEL;
		case TAUREN:
			return TAUREN_FEMALE_MODEL;
		case GNOME:
			return GNOME_FEMALE_MODEL;
		case TROLL:
			return TROLL_FEMALE_MODEL;
		case BLOODELF:
			return BLOODELF_FEMALE_MODEL;
		case DRAENEI:
			return DRAENEI_FEMALE_MODEL;
		default:
			return HUMAN_FEMALE_MODEL;
		}
	}
	uint32 getCharMaleModel(uint32 race){
		switch (race){
		case HUMAN:
			return HUMAN_MALE_MODEL;
		case ORC:
			return ORC_MALE_MODEL;
		case DWARF:
			return DWARF_MALE_MODEL;
		case NIGHTELF:
			return NIGHTELF_MALE_MODEL;
		case SCOURGE:
			return SCOURGE_MALE_MODEL;
		case TAUREN:
			return TAUREN_MALE_MODEL;
		case GNOME:
			return GNOME_MALE_MODEL;
		case TROLL:
			return TROLL_MALE_MODEL;
		case BLOODELF:
			return BLOODELF_MALE_MODEL;
		case DRAENEI:
			return DRAENEI_MALE_MODEL;
		};
		return HUMAN_MALE_MODEL;
	}
private:
	uint32 petnumber;
    uint32 ownerGUID;
	uint8 role = 0;
    uint32 displayId;
    uint8 race;
    uint8 gender;
	uint8 type = MERCENARY_TYPE_NONE;
    bool summoned=false;
    uint8 editSlot;
	std::vector<uint32>* defaultSpellVec;
	Player * player;
};

#endif