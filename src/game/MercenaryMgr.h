/*
    ?013-2016 EmuDevs <http://www.emudevs.com/>
    MaNGOS <http://getmangos.eu>
    TrinityCore <http://www.trinitycore.org>
*/
#ifndef MERCENARYMGR_H
#define MERCENARYMGR_H


#include "Mercenary.h"
#include <random>
#include <unordered_map>
#include "SharedDefines.h"


typedef tbb::concurrent_unordered_map<std::string,MercenarySpellGroup*> GroupSpellsMap;
typedef tbb::concurrent_unordered_map<uint32, GroupSpellsMap*> MercenarySpellsMap;/*key为雇佣兵职业class,value对应技能组*/
typedef tbb::concurrent_unordered_map<uint32, MercenarySpell*> MercenarySpellsInfo;/*key为技能*/
typedef tbb::concurrent_unordered_map<uint32, std::vector<MercenaryRoleDef*>*> MercenaryRoleDefMap;/*key为雇佣兵职业class，value对应角色列表*/


typedef std::vector<MercenaryTalking> MercenaryTalk;
typedef std::vector<MercenaryProficiency> MercenaryProficiencies;
typedef std::unordered_map<uint32, Mercenary*> MercenaryMap;
typedef std::unordered_map<uint32, MercenaryWorld> MercenaryWorldMap;

class MANGOS_DLL_SPEC Random
{
public:
    Random() = default;
    Random(std::mt19937::result_type seed) : eng(seed) { }

    int Next(int min, int max)
    {
        return std::uniform_int_distribution < int > {min, max}(eng);
    }

private:
    std::mt19937 eng{ std::random_device{}() };
};

class  MercenaryMgr
{
public:
    static MercenaryMgr* GetInstance()
    {
        static MercenaryMgr instance;
        return &instance;
    }
	void LoadMercenaries();
    /*
    * Loads Mercenaries from Database
    */
	Mercenary*  LoadMercenarie(Player* player);
    /*
    * Saves Mercenary to a container
    */
    void SaveToList(Mercenary* mercenary);

    /*
    * On Mercenary save to database
    * Also updates gear and saves Mercenary spells
    * Cals when players saves
    */
    void OnSave(Player* player);

    /*
    * On Mercenary summon
    * Calls when you change zones
    * If your Mercenary isn't with you, you can change zones to summon it back automatically
    */
    void OnSummon(Player* player);
    /*
    * Returns the item's icon
    * Thanks Rochet2 for the function
    */
    std::string GetItemIcon(uint32 entry, bool rawPath = false) const;
    /*
    * Returns the slot icon, empty bag if no icon is found for a slot
    * Thanks Rochet2 for the function
    */
    std::string GetSlotIcon(uint8 slot) const;
    /*
    * Returns item link
    * Thanks Rochet2 for the function
    */
    std::string GetItemLink(uint32 entry, WorldSession* session) const;
    /*
    * Returns slot name
    * Thanks Rochet2 for the function
    */
	const char* GetSlotName(Player* player,uint8 slot) const;
    /*
    * Returns spell icon & spell name
    */
	std::string GetSpellIcon(uint32 entry, Player* player) const;

    const char* GetAIName() const { return "mercenary_bot"; }

    MercenaryMap::const_iterator MercenaryBegin() const { return MercenaryContainer.begin(); }
    MercenaryMap::const_iterator MercenaryEnd() const { return MercenaryContainer.end(); }
	MercenarySpellsMap::const_iterator MercenarySpellsBegin() const { return mercenarySpellsMap.begin(); }
	MercenarySpellsMap::const_iterator MercenarySpellsEnd() const { return mercenarySpellsMap.end(); }
    MercenaryTalk::const_iterator MercenaryTalkBegin() const { return MercenaryTalkContainer.begin(); }
    MercenaryTalk::const_iterator MercenaryTalkEnd() const { return MercenaryTalkContainer.end(); }

    /*
    * Returns MercenaryContainer size
    * Used for `fakeEntry` when creating a Mercenary
    */
    uint32 MaxMercenaryId()
    {
        return MercenaryContainer.size();
    }

    /*
    * Returns Mercenary by player guidLow
    */
    Mercenary* GetMercenary(uint32 Id)
    {
        auto itr = MercenaryContainer.find(Id);
        if (itr != MercenaryEnd())
            return itr->second;

        return nullptr;
    }

    /*
    * Returns Mercenary's class by search for owner's guid
    */
    Mercenary* GetMercenaryByOwner(uint32 ownerGUID)
    {
        for (auto itr = MercenaryBegin(); itr != MercenaryEnd(); ++itr)
            if (itr->second->GetOwnerGUID() == ownerGUID)
                return itr->second;

        return nullptr;
    }
	GroupSpellsMap* findGroupSpellsMapByClass(uint32 classType){
		auto itr = mercenarySpellsMap.find(classType);
		if (itr == mercenarySpellsMap.end())
			return nullptr;
		return itr->second;
	}
	std::vector<MercenaryRoleDef*>* findRoleVectorByClass(uint32 classType){
		auto itr = mercenaryRoleDefMap.find(classType);
		if (itr == mercenaryRoleDefMap.end())
			return nullptr;
		return itr->second;
	}

	MercenarySpell* findMercenarySpellsInfoBySpell(uint32 spellid){ 
		auto itr = mercenarySpellsInfo.find(spellid);
		if (itr == mercenarySpellsInfo.end())
			return nullptr;
		return itr->second;
	} 
    /*
    *  Returns a vector of the correct talk sayings for that Mercenary type and role
    */
    std::vector<MercenaryTalking> GetTalk(uint8 type, uint8 role)
    {
        std::vector<MercenaryTalking> tempTalk;
        for (auto itr = MercenaryTalkBegin(); itr != MercenaryTalkEnd(); ++itr)
            if (itr->type == type && itr->role == role)
                tempTalk.push_back(*itr);

        return tempTalk;
    }

    /*
    * Returns Mercenary world data
    * `MercenaryWorld` data is for Mercenaries spawned in the world
    * On creation, modelId, race and gender will be used, so the data must be
    *     actual race, modelId and gender values
    */
    MercenaryWorld* GetMercenaryWorldData(uint32 entry)
    {
        auto it = MercenaryWorldContainer.find(entry);
        if (it != MercenaryWorldContainer.end())
            return &it->second;

        return nullptr;
    }

    /*
    * Returns item's displayId by itemEntry
    */
    uint32 GetItemDisplayId(uint32 itemEntry)
    {
//#ifndef MANGOS
//        const ItemTemplate* proto = sObjectMgr->GetItemTemplate(itemEntry);
//#else
        const ItemPrototype* proto = sObjectMgr.GetItemPrototype(itemEntry);
//#endif
        if (proto)
            return proto->DisplayInfoID;

        return NULL;
    }

    /*
    * Returns true if the Mercenary type can use that armor or weapon proficiency
    * If for example armor proficiency column is 0 and weapon proficiency column contains
    *     a value, that row will be loaded (considered) as weapon proficiency. Vice versa is implied here.
	*  简单的说，就是z_mercenary_proficiencies表中type匹配，对应的护甲（武器）匹配
    */
    bool CheckProficiencies(uint8 type, uint32 itemClass, uint32 itemSubClass)
    {
        bool check = false;
        for (auto& itr : MercenaryProficiencyContainer)
        {
            if (itr.type != type)
                continue;

            if (itemClass == ITEM_CLASS_ARMOR)
            {
				if (itr.armorSubClass == 0 && !(itr.armorSubClass == 0 && itr.weaponSubClass == 0))//仅weaponSubClass不为0，不是针对护甲的
                    continue;

                if (itr.armorSubClass >= itemSubClass)//将等于改为大于等于，表示预定义的专精大于物品实际的，例如：能穿板甲，就一定能穿锁甲
                {
                    check = true;
                    break;
                }
            }
            else if (itemClass == ITEM_CLASS_WEAPON)
            {
                if (itr.weaponSubClass == 0 && !(itr.weaponSubClass == 0 && itr.armorSubClass == 0))//仅armorSubClass不为0，不是针对武器的
                    continue;

				if (itr.weaponSubClass == itemSubClass)//武器必须精确等于，用杖的只能用杖
                {
                    check = true;
                    break;
                }
            }
        }

        return check;
    }

    Random random;

private:
    MercenaryMgr();
    ~MercenaryMgr();

	MercenarySpellsInfo mercenarySpellsInfo;
	MercenarySpellsMap mercenarySpellsMap;
	MercenaryRoleDefMap mercenaryRoleDefMap;
    MercenaryProficiencies MercenaryProficiencyContainer;
    MercenaryMap MercenaryContainer;
    MercenaryTalk MercenaryTalkContainer;
    MercenaryWorldMap MercenaryWorldContainer;
};

#define sMercenaryMgr MercenaryMgr::GetInstance()

class MANGOS_DLL_SPEC MercenaryUtil{
public:
	static Random GetMercenaryRandom();
	static Mercenary* GetMercenaryByOwner(uint32 ownerGUID);
	static GroupSpellsMap* MercenaryUtil::findGroupSpellsMapByClass(uint32 classType);
	static std::vector<MercenaryRoleDef*>* MercenaryUtil::findRoleVectorByClass(uint32 classType);
	static MercenarySpell* MercenaryUtil::findMercenarySpellsInfoBySpell(uint32 spellid);
	static std::vector<MercenaryTalking> MercenaryGetTalk(uint8 type, uint8 role);
	static std::string GetMercenarySlotIcon(uint8 slot);
	static const char* GetMercenarySlotName(Player* player,uint8 slot);
	static std::string GetMercenaryItemIcon(uint32 entry, bool rawPath = false);
	static std::string GetMercenaryItemLink(uint32 entry, WorldSession* session);
	static const ItemPrototype* GetItemPrototype(uint32 entry);
	static std::string GetMercenarySpellIcon(uint32 entry, Player* player);
	static MercenaryWorld* GetMercenaryWorldData(uint32 entry);
};

#endif