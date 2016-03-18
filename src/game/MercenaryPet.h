#ifndef Mercenary_PET_H
#define Mercenary_PET_H
#include "Pet.h"

//所有的Pet都初始化为MercenaryPet,通过判断Creature::isMercenary在本类中选择性覆盖父类方法
class MANGOS_DLL_SPEC MercenaryPet : public Pet
{
public:
	explicit MercenaryPet(PetType type = MAX_PET_TYPE);

	int16 m_baseRatingValue[MAX_COMBAT_RATING];
	Item* m_items[PLAYER_SLOTS_COUNT];
	int32 m_spellPenetrationItemMod;
	uint16 m_baseSpellPower;
	uint16 m_baseFeralAP;
	float m_ammoDPS;
	bool m_canParry;
	bool m_canBlock;
	float m_auraBaseMod[BASEMOD_END][MOD_END];
	float m_armorPenetrationPct;
	uint16 m_baseManaRegen;
	SkillStatusMap mSkillStatus;

	static const float m_diminishing_k[MAX_CLASSES];

	EnchantDurationList m_enchantDuration;
	ItemDurationList m_itemDuration;

	/*以下是工具方法*/
	Mercenary* getMercenary();
	Player* getPlayer();

	/*以下是覆盖的Pet方法*/
	bool UpdateStats(Stats stat) override;
	bool UpdateAllStats() override;
	void UpdateResistances(uint32 school) override;
	void UpdateArmor() override;
	void UpdateMaxHealth() override;
	void UpdateMaxPower(Powers power) override;
	void UpdateDamagePhysical(WeaponAttackType attType) override;
	void UpdateAttackPowerAndDamage(bool ranged = false) override;

	bool InitStatsForLevel(uint32 level, Unit* owner = nullptr) override;
	void InitStatsForLevelPlayer(bool reapplyMods);

	/*以下是Player特有方法*/
	void ApplySpellPowerBonus(int32 amount, bool apply);
	void UpdateSpellDamageAndHealingBonus();
	float GetHealthBonusFromStamina();
	float GetManaBonusFromIntellect();
	void ApplyFeralAPBonus(int32 amount, bool apply);
	void UpdateShieldBlockValue();
	void CalculateMinMaxDamage(WeaponAttackType attType, bool normalized, float& min_damage, float& max_damage);
	void UpdateDefenseBonusesMod();
	void UpdateBlockPercentage();
	void UpdateCritPercentage(WeaponAttackType attType);
	void UpdateAllCritPercentages();
	void UpdateParryPercentage();
	void UpdateDodgePercentage();
	void UpdateSpellCritChance(uint32 school);
	void UpdateMeleeHitChances();
	void UpdateRangedHitChances();
	void UpdateSpellHitChances();
	void UpdateAllSpellCritChances();
	void UpdateExpertise(WeaponAttackType attack);
	void UpdateArmorPenetration();
	void ApplyManaRegenBonus(int32 amount, bool apply);
	void UpdateManaRegen();
	void _ApplyAllStatBonuses();
	void _RemoveAllStatBonuses();

	/*内部引用的方法*/
	void ApplyRatingMod(CombatRating cr, int32 value, bool apply);
	void UpdateRating(CombatRating cr);
	float GetRatingMultiplier(CombatRating cr) const;
	void _RemoveAllItemMods();
	void _ApplyAmmoBonuses();
	void _ApplyItemBonuses(ItemPrototype const* proto, uint8 slot, bool apply, bool only_level_scale = false);
	void HandleBaseModValue(BaseModGroup modGroup, BaseModType modType, float amount, bool apply);
	void UpdateAllRatings();
	float GetAmmoDPS() const { return m_ammoDPS; }
	void SetCanBlock(bool value);
	bool CanBlock() const { return m_canBlock; }
	float GetRatingBonusValue(CombatRating cr) const;
	float GetTotalPercentageModValue(BaseModGroup modGroup) const { return m_auraBaseMod[modGroup][FLAT_MOD] + m_auraBaseMod[modGroup][PCT_MOD]; }
	float GetMeleeCritFromAgility();
	void SetBaseModValue(BaseModGroup modGroup, BaseModType modType, float value) { m_auraBaseMod[modGroup][modType] = value; }
	bool CanParry() const { return m_canParry; }
	void SetCanParry(bool value);
	uint16 GetSkillValue(uint32 skill) const;           // skill value + perm. bonus + temp bonus
	void GetDodgeFromAgility(float& diminishing, float& nondiminishing);
	float GetSpellCritFromIntellect();
	Item* GetWeaponForAttack(WeaponAttackType attackType) { return GetWeaponForAttack(attackType, false, false); }
	Item* GetWeaponForAttack(WeaponAttackType attackType, bool nonbroken, bool useable);
	float OCTRegenMPPerSpirit();
	void _ApplyAllItemMods();
	void ApplyItemEquipSpell(Item* item, bool apply, bool form_change = false);
	void ApplyEquipSpell(SpellEntry const* spellInfo, Item* item, bool apply, bool form_change = false);

	void ApplyEnchantment(Item* item, EnchantmentSlot slot, bool apply, bool apply_dur = true, bool ignore_condition = false);
	void ApplyEnchantment(Item* item, bool apply);

	void _ApplyWeaponDependentAuraMods(Item* item, WeaponAttackType attackType, bool apply);
	void _ApplyWeaponDependentAuraCritMod(Item* item, WeaponAttackType attackType, Aura* aura, bool apply);
	void _ApplyWeaponDependentAuraDamageMod(Item* item, WeaponAttackType attackType, Aura* aura, bool apply);

	bool CheckAmmoCompatibility(const ItemPrototype* ammo_proto);

	Item* GetItemByPos(uint16 pos);
	bool EnchantmentFitsRequirements(uint32 enchantmentcondition, int8 slot);

	void AddEnchantmentDurations(Item* item);
	void RemoveEnchantmentDurations(Item* item);
	void RemoveAllEnchantments(EnchantmentSlot slot);
	// duration == 0 will remove item enchant
	void AddEnchantmentDuration(Item* item, EnchantmentSlot slot, uint32 duration);

	void SetSkill(uint16 id, uint16 currVal, uint16 maxVal, uint16 step /*=0*/);

	//初始化属性
	void UpdateSkillsForLevel();
	void InitDataForForm(bool reapplyMods);
	void SetRegularAttackTime();
	void UpdateEquipSpellsAtFormChange();
	void _ApplyItemMods(Item* item, uint8 slot, bool apply);
	void CorrectMetaGemEnchants(uint8 exceptslot, bool apply);
	Item* EquipItem(uint16 pos, Item* pItem, bool update);
	void ApplyItemOnStoreSpell(Item* item, bool apply);
	void AutoUnequipOffhandIfNeed();
	void RemoveItem(uint8 slot, bool update);
	void RemoveItemDependentAurasAndCasts(Item* pItem);
	bool HasItemFitToSpellReqirements(SpellEntry const* spellInfo, Item const* ignoreItem);
	void GiveLevel(uint32 level);
};

#endif