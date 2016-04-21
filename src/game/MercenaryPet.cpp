#include "Unit.h"
#include "SpellAuras.h"
#include "Pet.h"
#include "MercenaryPet.h"
#include "MercenaryMgr.h"
#include "SpellMgr.h"
#include "World.h"

#define PLAYER_SKILL_INDEX(x)       (PLAYER_SKILL_INFO_1_1 + ((x)*3))
#define PLAYER_SKILL_VALUE_INDEX(x) (PLAYER_SKILL_INDEX(x)+1)
#define PLAYER_SKILL_BONUS_INDEX(x) (PLAYER_SKILL_INDEX(x)+2)

#define SKILL_VALUE(x)         PAIR32_LOPART(x)
#define SKILL_MAX(x)           PAIR32_HIPART(x)
#define MAKE_SKILL_VALUE(v, m) MAKE_PAIR32(v,m)

#define SKILL_TEMP_BONUS(x)    int16(PAIR32_LOPART(x))
#define SKILL_PERM_BONUS(x)    int16(PAIR32_HIPART(x))
#define MAKE_SKILL_BONUS(t, p) MAKE_PAIR32(t,p)

MercenaryPet::MercenaryPet(PetType type) :Pet(type)
{
	//m_objectType |= TYPEMASK_PLAYER;
	//m_objectTypeId = TYPEID_PLAYER;
	m_valuesCount = PLAYER_END;
	SetActiveObjectState(true);                             // player is always active object
	m_usedTalentCount = 0;
	m_regenTimer = 0;

	memset(m_items, 0, sizeof(Item*)*PLAYER_SLOTS_COUNT);


	// group is initialized in the reference constructor
	m_auraUpdateMask = 0;

	m_canParry = false;
	m_canBlock = false;
	m_ammoDPS = 0.0f;

	m_resetTalentsCost = 0;
	m_resetTalentsTime = 0;


	for (int i = 0; i < BASEMOD_END; ++i)
	{
		m_auraBaseMod[i][FLAT_MOD] = 0.0f;
		m_auraBaseMod[i][PCT_MOD] = 1.0f;
	}

	for (int i = 0; i < MAX_COMBAT_RATING; ++i)
		m_baseRatingValue[i] = 0;

	m_baseSpellPower = 0;
	m_baseFeralAP = 0;
	m_baseManaRegen = 0;
	m_armorPenetrationPct = 0.0f;
	m_spellPenetrationItemMod = 0;

	_mercenary = nullptr;

}
Mercenary* MercenaryPet::getMercenary(){

	if (_mercenary != nullptr)
		return _mercenary;

	if (GetOwner() == nullptr)
		return nullptr;

	_mercenary = sMercenaryMgr->GetMercenary(GetOwner()->GetGUIDLow());

	return _mercenary;
}
/*MercenaryPet::~MercenaryPet()
{
	不允许有析构函数，因为Pet *pet=new MercenaryPet的情况，析构时只会调用Pet的析构
}*/


/*#######################################
########                         ########
########   MercenaryPet STAT SYSTEM   ########
########                         ########
#######################################*/

bool MercenaryPet::UpdateStats(Stats stat)
{
	if (!isMercenary()){
		return Pet::UpdateStats(stat);
	}
	if (stat > STAT_SPIRIT)
		return false;

	// value = ((base_value * base_pct) + total_value) * total_pct
	float value = GetTotalStatValue(stat);

	SetStat(stat, int32(value));

	if (stat == STAT_STAMINA || stat == STAT_INTELLECT)
	{
		MercenaryPet* pet = (MercenaryPet*)GetPet();
		if (pet)
			pet->UpdateStats(stat);//确认方法调用是MercenaryPet::UpdateStats(stat)
	}

	switch (stat)
	{
	case STAT_STRENGTH:
		UpdateShieldBlockValue();
		break;
	case STAT_AGILITY:
		UpdateArmor();
		UpdateAllCritPercentages();
		UpdateDodgePercentage();
		break;
	case STAT_STAMINA:   UpdateMaxHealth(); break;
	case STAT_INTELLECT:
		UpdateMaxPower(POWER_MANA);
		UpdateAllSpellCritChances();
		UpdateArmor();                                  // SPELL_AURA_MOD_RESISTANCE_OF_INTELLECT_PERCENT, only armor currently
		break;

	case STAT_SPIRIT:
		break;

	default:
		break;
	}
	// Need update (exist AP from stat auras)
	UpdateAttackPowerAndDamage();
	UpdateAttackPowerAndDamage(true);

	UpdateSpellDamageAndHealingBonus();
	UpdateManaRegen();

	// Update ratings in exist SPELL_AURA_MOD_RATING_FROM_STAT and only depends from stat
	uint32 mask = 0;
	AuraList const& modRatingFromStat = GetAurasByType(SPELL_AURA_MOD_RATING_FROM_STAT);

	for (AuraList::const_iterator i = modRatingFromStat.begin(); i != modRatingFromStat.end(); ++i)
	if (Stats((*i)->GetMiscBValue()) == stat)
		mask |= (*i)->GetMiscValue();

	if (mask)
	{
		for (uint32 rating = 0; rating < MAX_COMBAT_RATING; ++rating)
		if (mask & (1 << rating))
			ApplyRatingMod(CombatRating(rating), 0, true);

	}
	return true;
}

void MercenaryPet::ApplySpellPowerBonus(int32 amount, bool apply)
{
	m_baseSpellPower += apply ? amount : -amount;

	// For speed just update for client
	ApplyModUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, amount, apply);
	for (int i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
		ApplyModUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, amount, apply);;
}

void MercenaryPet::UpdateSpellDamageAndHealingBonus()
{
	// Magic damage modifiers implemented in Unit::SpellDamageBonusDone
	// This information for client side use only
	// Get healing bonus for all schools
	SetStatInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, SpellBaseHealingBonusDone(SPELL_SCHOOL_MASK_ALL));
	// Get damage bonus for all schools
	for (int i = SPELL_SCHOOL_HOLY; i < MAX_SPELL_SCHOOL; ++i)
		SetStatInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, SpellBaseDamageBonusDone(SpellSchoolMask(1 << i)));
}

bool MercenaryPet::UpdateAllStats()
{
	if (!isMercenary()){
		return Pet::UpdateAllStats();
	}
	
	/*初始化开始*/
	Mercenary * mercenary = getMercenary();
	Player * player = getPlayer();

	uint32 level = player->getLevel();
	uint8 petClass = mercenary->GetType();
	uint8 race = mercenary->GetRace();

	SetLevel(level);
	SetByteValue(UNIT_FIELD_BYTES_0, 0, race);
	SetByteValue(UNIT_FIELD_BYTES_0, 1, petClass);



	/*装备更新*/
	for (int i = 0; i < PLAYER_SLOTS_COUNT; i++)
		m_items[i] = nullptr;

	for (auto itr = mercenary->gearContainer.begin(); itr != mercenary->gearContainer.end(); itr++){
		Item* item = mercenary->GetItemByGuid(itr->second.itemguid);
		m_items[itr->first] = (item != nullptr) ? item : nullptr;
	}

	for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
	{
		float value = GetTotalStatValue(Stats(i));
		SetStat(Stats(i), (int32)value);
	}

	UpdateArmor();
	// calls UpdateAttackPowerAndDamage() in UpdateArmor for SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR
	UpdateAttackPowerAndDamage(true);
	UpdateMaxHealth();

	for (int i = POWER_MANA; i < MAX_POWERS; ++i)
		UpdateMaxPower(Powers(i));

	UpdateAllRatings();
	UpdateAllCritPercentages();
	UpdateAllSpellCritChances();
	UpdateDefenseBonusesMod();
	UpdateShieldBlockValue();
	UpdateArmorPenetration();
	UpdateSpellDamageAndHealingBonus();
	UpdateManaRegen();
	UpdateExpertise(BASE_ATTACK);
	UpdateExpertise(OFF_ATTACK);
	for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
		UpdateResistances(i);

	return true;
}

void MercenaryPet::UpdateResistances(uint32 school)
{
	if (!isMercenary()){
		Pet::UpdateResistances(school);
		return;
	}

	if (school > SPELL_SCHOOL_NORMAL)
	{
		float value = GetTotalAuraModValue(UnitMods(UNIT_MOD_RESISTANCE_START + school));
		SetResistance(SpellSchools(school), int32(value));

		MercenaryPet* pet = (MercenaryPet*)GetPet();
		if (pet)
			pet->UpdateResistances(school);
	}
	else
		UpdateArmor();
}

void MercenaryPet::UpdateArmor()
{
	if (!isMercenary()){
		Pet::UpdateArmor();
		return;
	}
	float value;
	UnitMods unitMod = UNIT_MOD_ARMOR;

	value = GetModifierValue(unitMod, BASE_VALUE);         // base armor (from items)
	value *= GetModifierValue(unitMod, BASE_PCT);           // armor percent from items
	value += GetStat(STAT_AGILITY) * 2.0f;                  // armor bonus from stats
	value += GetModifierValue(unitMod, TOTAL_VALUE);

	// add dynamic flat mods
	AuraList const& mResbyIntellect = GetAurasByType(SPELL_AURA_MOD_RESISTANCE_OF_STAT_PERCENT);
	for (AuraList::const_iterator i = mResbyIntellect.begin(); i != mResbyIntellect.end(); ++i)
	{
		Modifier* mod = (*i)->GetModifier();
		if (mod->m_miscvalue & SPELL_SCHOOL_MASK_NORMAL)
			value += int32(GetStat(Stats((*i)->GetMiscBValue())) * mod->m_amount / 100.0f);
	}

	value *= GetModifierValue(unitMod, TOTAL_PCT);

	SetArmor(int32(value));

	UpdateAttackPowerAndDamage();                           // armor dependent auras update for SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR
}
bool MercenaryPet::InitStatsForLevel(uint32 level, Unit* owner){
	if (isMercenary()){
		//return true;/*这种情况在数据库加载时不更新，直到Mercenary::init完成后再更新。*/
		
		//if (1==1)
			//return Pet::InitStatsForLevel(level, owner);
		GiveLevel(level);
		return true;
		
	}
	else
		return Pet::InitStatsForLevel(level, owner);
}
float MercenaryPet::GetHealthBonusFromStamina()
{
	float stamina = GetStat(STAT_STAMINA);

	float baseStam = stamina < 20 ? stamina : 20;
	float moreStam = stamina - baseStam;

	return baseStam + (moreStam * 10.0f);
}

float MercenaryPet::GetManaBonusFromIntellect()
{
	float intellect = GetStat(STAT_INTELLECT);

	float baseInt = intellect < 20 ? intellect : 20;
	float moreInt = intellect - baseInt;

	return baseInt + (moreInt * 15.0f);
}

void MercenaryPet::UpdateMaxHealth()
{
	if (!isMercenary()){
		Pet::UpdateMaxHealth();
		return;
	}

	UnitMods unitMod = UNIT_MOD_HEALTH;

	float value = GetModifierValue(unitMod, BASE_VALUE) + GetCreateHealth();
	value *= GetModifierValue(unitMod, BASE_PCT);
	value += GetModifierValue(unitMod, TOTAL_VALUE) + GetHealthBonusFromStamina();
	value *= GetModifierValue(unitMod, TOTAL_PCT);

	if (Player * player = getPlayer())
		value *= player->context.mapDifficultyMultiplier;
	
	float old = float(GetHealth()) / float(GetMaxHealth());//百分比计算
	SetMaxHealth((uint32)value);
	SetHealth(old * GetMaxHealth());
}

void MercenaryPet::UpdateMaxPower(Powers power)
{

	if (!isMercenary()){
		Pet::UpdateMaxPower(power);
		return;
	}

	UnitMods unitMod = UnitMods(UNIT_MOD_POWER_START + power);

	uint32 create_power = GetCreatePowers(power);

	// ignore classes without mana
	float bonusPower = (power == POWER_MANA && create_power > 0) ? GetManaBonusFromIntellect() : 0;

	float value = GetModifierValue(unitMod, BASE_VALUE) + create_power;
	value *= GetModifierValue(unitMod, BASE_PCT);
	value += GetModifierValue(unitMod, TOTAL_VALUE) + bonusPower;
	value *= GetModifierValue(unitMod, TOTAL_PCT);

	SetMaxPower(power, uint32(value));
}

void MercenaryPet::ApplyFeralAPBonus(int32 amount, bool apply)
{
	m_baseFeralAP += apply ? amount : -amount;
	UpdateAttackPowerAndDamage();
}

void MercenaryPet::UpdateAttackPowerAndDamage(bool ranged)
{
	if (!isMercenary()){
		Pet::UpdateAttackPowerAndDamage(ranged);
		return;
	}
	float val2 = 0.0f;
	float level = float(getLevel());

	UnitMods unitMod = ranged ? UNIT_MOD_ATTACK_POWER_RANGED : UNIT_MOD_ATTACK_POWER;

	uint16 index = UNIT_FIELD_ATTACK_POWER;
	uint16 index_mod = UNIT_FIELD_ATTACK_POWER_MODS;
	uint16 index_mult = UNIT_FIELD_ATTACK_POWER_MULTIPLIER;

	if (ranged)
	{
		index = UNIT_FIELD_RANGED_ATTACK_POWER;
		index_mod = UNIT_FIELD_RANGED_ATTACK_POWER_MODS;
		index_mult = UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER;

		switch (getClass())
		{
		case CLASS_HUNTER: val2 = level * 2.0f + GetStat(STAT_AGILITY) - 10.0f;    break;
		case CLASS_ROGUE:  val2 = level + GetStat(STAT_AGILITY) - 10.0f;    break;
		case CLASS_WARRIOR: val2 = level + GetStat(STAT_AGILITY) - 10.0f;    break;
		case CLASS_DRUID:
			switch (GetShapeshiftForm())
			{
			case FORM_CAT:
			case FORM_BEAR:
			case FORM_DIREBEAR:
				val2 = 0.0f; break;
			default:
				val2 = GetStat(STAT_AGILITY) - 10.0f; break;
			}
			break;
		default: val2 = GetStat(STAT_AGILITY) - 10.0f; break;
		}
	}
	else
	{
		switch (getClass())
		{
		case CLASS_WARRIOR:      val2 = level * 3.0f + GetStat(STAT_STRENGTH) * 2.0f - 20.0f; break;
		case CLASS_PALADIN:      val2 = level * 3.0f + GetStat(STAT_STRENGTH) * 2.0f - 20.0f; break;
		case CLASS_DEATH_KNIGHT: val2 = level * 3.0f + GetStat(STAT_STRENGTH) * 2.0f - 20.0f; break;
		case CLASS_ROGUE:        val2 = level * 2.0f + GetStat(STAT_STRENGTH) + GetStat(STAT_AGILITY) - 20.0f; break;
		case CLASS_HUNTER:       val2 = level * 2.0f + GetStat(STAT_STRENGTH) + GetStat(STAT_AGILITY) - 20.0f; break;
		case CLASS_SHAMAN:       val2 = level * 2.0f + GetStat(STAT_STRENGTH) + GetStat(STAT_AGILITY) - 20.0f; break;
		case CLASS_DRUID:
		{
							ShapeshiftForm form = GetShapeshiftForm();
							// Check if Predatory Strikes is skilled
							float mLevelBonus = 0.0f;
							float mBonusWeaponAtt = 0.0f;
							switch (form)
							{
							case FORM_CAT:
							case FORM_BEAR:
							case FORM_DIREBEAR:
							case FORM_MOONKIN:
							{
												 Unit::AuraList const& mDummy = GetAurasByType(SPELL_AURA_DUMMY);
												 for (Unit::AuraList::const_iterator itr = mDummy.begin(); itr != mDummy.end(); ++itr)
												 {
													 if ((*itr)->GetSpellProto()->SpellIconID != 1563)
														 continue;

													 // Predatory Strikes (effect 0)
													 if ((*itr)->GetEffIndex() == EFFECT_INDEX_0 && IsInFeralForm())
														 mLevelBonus = getLevel() * (*itr)->GetModifier()->m_amount / 100.0f;
													 // Predatory Strikes (effect 1)
													 else if ((*itr)->GetEffIndex() == EFFECT_INDEX_1)
														 mBonusWeaponAtt = (*itr)->GetModifier()->m_amount * m_baseFeralAP / 100.0f;

													 if (mLevelBonus != 0.0f && mBonusWeaponAtt != 0.0f)
														 break;
												 }
												 break;
							}
							default: break;
							}

							switch (form)
							{
							case FORM_CAT:
								val2 = GetStat(STAT_STRENGTH) * 2.0f + GetStat(STAT_AGILITY) - 20.0f + mLevelBonus + m_baseFeralAP + mBonusWeaponAtt; break;
							case FORM_BEAR:
							case FORM_DIREBEAR:
								val2 = GetStat(STAT_STRENGTH) * 2.0f - 20.0f + mLevelBonus + m_baseFeralAP + mBonusWeaponAtt; break;
							case FORM_MOONKIN:
								val2 = GetStat(STAT_STRENGTH) * 2.0f - 20.0f + m_baseFeralAP + mBonusWeaponAtt; break;
							default:
								val2 = GetStat(STAT_STRENGTH) * 2.0f - 20.0f; break;
							}
							break;
		}
		case CLASS_MAGE:    val2 = GetStat(STAT_STRENGTH) - 10.0f; break;
		case CLASS_PRIEST:  val2 = GetStat(STAT_STRENGTH) - 10.0f; break;
		case CLASS_WARLOCK: val2 = GetStat(STAT_STRENGTH) - 10.0f; break;
		}
	}

	SetModifierValue(unitMod, BASE_VALUE, val2);

	float base_attPower = GetModifierValue(unitMod, BASE_VALUE) * GetModifierValue(unitMod, BASE_PCT);
	float attPowerMod = GetModifierValue(unitMod, TOTAL_VALUE);

	// add dynamic flat mods
	if (ranged)
	{
		if ((getClassMask() & CLASSMASK_WAND_USERS) == 0)
		{
			AuraList const& mRAPbyStat = GetAurasByType(SPELL_AURA_MOD_RANGED_ATTACK_POWER_OF_STAT_PERCENT);
			for (AuraList::const_iterator i = mRAPbyStat.begin(); i != mRAPbyStat.end(); ++i)
				attPowerMod += int32(GetStat(Stats((*i)->GetModifier()->m_miscvalue)) * (*i)->GetModifier()->m_amount / 100.0f);
		}
	}
	else
	{
		AuraList const& mAPbyStat = GetAurasByType(SPELL_AURA_MOD_ATTACK_POWER_OF_STAT_PERCENT);
		for (AuraList::const_iterator i = mAPbyStat.begin(); i != mAPbyStat.end(); ++i)
			attPowerMod += int32(GetStat(Stats((*i)->GetModifier()->m_miscvalue)) * (*i)->GetModifier()->m_amount / 100.0f);

		AuraList const& mAPbyArmor = GetAurasByType(SPELL_AURA_MOD_ATTACK_POWER_OF_ARMOR);
		for (AuraList::const_iterator iter = mAPbyArmor.begin(); iter != mAPbyArmor.end(); ++iter)
			// always: ((*i)->GetModifier()->m_miscvalue == 1 == SPELL_SCHOOL_MASK_NORMAL)
			attPowerMod += int32(GetArmor() / (*iter)->GetModifier()->m_amount);
	}

	float attPowerMultiplier = GetModifierValue(unitMod, TOTAL_PCT) - 1.0f;

	SetInt32Value(index, (uint32)base_attPower);            // UNIT_FIELD_(RANGED)_ATTACK_POWER field
	SetInt32Value(index_mod, (uint32)attPowerMod);          // UNIT_FIELD_(RANGED)_ATTACK_POWER_MODS field
	SetFloatValue(index_mult, attPowerMultiplier);          // UNIT_FIELD_(RANGED)_ATTACK_POWER_MULTIPLIER field

	// automatically update weapon damage after attack power modification
	if (ranged)
	{
		UpdateDamagePhysical(RANGED_ATTACK);

		MercenaryPet* pet = (MercenaryPet*)GetPet();                                // update pet's AP
		if (pet)
			pet->UpdateAttackPowerAndDamage();
	}
	else
	{
		UpdateDamagePhysical(BASE_ATTACK);
		//if (CanDualWield() && haveOffhandWeapon())        // allow update offhand damage only if player knows DualWield Spec and has equipped offhand weapon
		if (haveOffhandWeapon())							//雇佣兵只要有副手武器即可
		  UpdateDamagePhysical(OFF_ATTACK);
	}
}

void MercenaryPet::UpdateShieldBlockValue()
{
	SetUInt32Value(PLAYER_SHIELD_BLOCK, GetShieldBlockValue());
}

void MercenaryPet::CalculateMinMaxDamage(WeaponAttackType attType, bool normalized, float& min_damage, float& max_damage)
{
	UnitMods unitMod;
	UnitMods attPower;

	switch (attType)
	{
	case BASE_ATTACK:
	default:
		unitMod = UNIT_MOD_DAMAGE_MAINHAND;
		attPower = UNIT_MOD_ATTACK_POWER;
		break;
	case OFF_ATTACK:
		unitMod = UNIT_MOD_DAMAGE_OFFHAND;
		attPower = UNIT_MOD_ATTACK_POWER;
		break;
	case RANGED_ATTACK:
		unitMod = UNIT_MOD_DAMAGE_RANGED;
		attPower = UNIT_MOD_ATTACK_POWER_RANGED;
		break;
	}

	float att_speed = GetAPMultiplier(attType, normalized);

	float base_value = GetModifierValue(unitMod, BASE_VALUE) + GetTotalAttackPowerValue(attType) / 14.0f * att_speed;
	float base_pct = GetModifierValue(unitMod, BASE_PCT);
	float total_value = GetModifierValue(unitMod, TOTAL_VALUE);
	float total_pct = GetModifierValue(unitMod, TOTAL_PCT);

	float weapon_mindamage = GetWeaponDamageRange(attType, MINDAMAGE);
	float weapon_maxdamage = GetWeaponDamageRange(attType, MAXDAMAGE);

	if (IsInFeralForm())                                    // check if player is druid and in cat or bear forms, non main hand attacks not allowed for this mode so not check attack type
	{
		uint32 lvl = getLevel();
		if (lvl > 60)
			lvl = 60;

		weapon_mindamage = lvl * 0.85f * att_speed;
		weapon_maxdamage = lvl * 1.25f * att_speed;
	}
	else if (!CanUseEquippedWeapon(attType))                // check if player not in form but still can't use weapon (broken/etc)
	{
		weapon_mindamage = BASE_MINDAMAGE;
		weapon_maxdamage = BASE_MAXDAMAGE;
	}
	else if (attType == RANGED_ATTACK)                      // add ammo DPS to ranged damage
	{
		weapon_mindamage += GetAmmoDPS() * att_speed;
		weapon_maxdamage += GetAmmoDPS() * att_speed;
	}

	min_damage = ((base_value + weapon_mindamage) * base_pct + total_value) * total_pct;
	max_damage = ((base_value + weapon_maxdamage) * base_pct + total_value) * total_pct;

	if (Player* player = getPlayer())
	{
		min_damage *= player->context.mapDifficultyMultiplier;//最小伤害值变化
		max_damage *= player->context.mapDifficultyMultiplier;//最大伤害值变化
	}
}

void MercenaryPet::UpdateDamagePhysical(WeaponAttackType attType)
{
	if (!isMercenary()){
		Pet::UpdateDamagePhysical(attType);
		return;
	}

	float mindamage;
	float maxdamage;

	CalculateMinMaxDamage(attType, false, mindamage, maxdamage);

	switch (attType)
	{
	case BASE_ATTACK:
	default:
		SetStatFloatValue(UNIT_FIELD_MINDAMAGE, mindamage);
		SetStatFloatValue(UNIT_FIELD_MAXDAMAGE, maxdamage);
		break;
	case OFF_ATTACK:
		SetStatFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, mindamage);
		SetStatFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, maxdamage);
		break;
	case RANGED_ATTACK:
		SetStatFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, mindamage);
		SetStatFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, maxdamage);
		break;
	}
}

void MercenaryPet::UpdateDefenseBonusesMod()
{
	UpdateBlockPercentage();
	UpdateParryPercentage();
	UpdateDodgePercentage();
}

void MercenaryPet::UpdateBlockPercentage()
{
	// No block
	float value = 0.0f;
	if (CanBlock())
	{
		// Base value
		value = 5.0f;
		// Modify value from defense skill
		value += (int32(GetDefenseSkillValue()) - int32(GetMaxSkillValueForLevel())) * 0.04f;
		// Increase from SPELL_AURA_MOD_BLOCK_PERCENT aura
		value += GetTotalAuraModifier(SPELL_AURA_MOD_BLOCK_PERCENT);
		// Increase from rating
		value += GetRatingBonusValue(CR_BLOCK);
		value = value < 0.0f ? 0.0f : value;
	}
	SetStatFloatValue(PLAYER_BLOCK_PERCENTAGE, value);
}

void MercenaryPet::UpdateCritPercentage(WeaponAttackType attType)
{
	BaseModGroup modGroup;
	uint16 index;
	CombatRating cr;

	switch (attType)
	{
	case OFF_ATTACK:
		modGroup = OFFHAND_CRIT_PERCENTAGE;
		index = PLAYER_OFFHAND_CRIT_PERCENTAGE;
		cr = CR_CRIT_MELEE;
		break;
	case RANGED_ATTACK:
		modGroup = RANGED_CRIT_PERCENTAGE;
		index = PLAYER_RANGED_CRIT_PERCENTAGE;
		cr = CR_CRIT_RANGED;
		break;
	case BASE_ATTACK:
	default:
		modGroup = CRIT_PERCENTAGE;
		index = PLAYER_CRIT_PERCENTAGE;
		cr = CR_CRIT_MELEE;
		break;
	}

	float value = GetTotalPercentageModValue(modGroup) + GetRatingBonusValue(cr);
	// Modify crit from weapon skill and maximized defense skill of same level victim difference
	value += (int32(GetWeaponSkillValue(attType)) - int32(GetMaxSkillValueForLevel())) * 0.04f;
	value = value < 0.0f ? 0.0f : value;
	SetStatFloatValue(index, value);
}

void MercenaryPet::UpdateAllCritPercentages()
{
	float value = GetMeleeCritFromAgility();

	SetBaseModValue(CRIT_PERCENTAGE, PCT_MOD, value);
	SetBaseModValue(OFFHAND_CRIT_PERCENTAGE, PCT_MOD, value);
	SetBaseModValue(RANGED_CRIT_PERCENTAGE, PCT_MOD, value);

	UpdateCritPercentage(BASE_ATTACK);
	UpdateCritPercentage(OFF_ATTACK);
	UpdateCritPercentage(RANGED_ATTACK);
}

const float MercenaryPet::m_diminishing_k[MAX_CLASSES] =
{
	0.9560f,  // Warrior
	0.9560f,  // Paladin
	0.9880f,  // Hunter
	0.9880f,  // Rogue
	0.9830f,  // Priest
	0.9560f,  // DK
	0.9880f,  // Shaman
	0.9830f,  // Mage
	0.9830f,  // Warlock
	0.0f,     // ??
	0.9720f   // Druid
};

void MercenaryPet::UpdateParryPercentage()
{
	const float parry_cap[MAX_CLASSES] =
	{
		47.003525f,  // Warrior
		47.003525f,  // Paladin
		145.560408f,  // Hunter
		145.560408f,  // Rogue
		0.0f,       // Priest
		47.003525f,  // DK
		145.560408f,  // Shaman
		0.0f,       // Mage
		0.0f,       // Warlock
		0.0f,       // ??
		0.0f        // Druid
	};

	// No parry
	float value = 0.0f;
	uint32 pclass = getClass() - 1;
	if (CanParry() && parry_cap[pclass] > 0.0f)
	{
		// Base parry
		float nondiminishing = 5.0f;
		// Parry from rating
		float diminishing = GetRatingBonusValue(CR_PARRY);
		// Modify value from defense skill (only bonus from defense rating diminishes)
		nondiminishing += (GetSkillValue(SKILL_DEFENSE) - GetMaxSkillValueForLevel()) * 0.04f;
		diminishing += (int32(GetRatingBonusValue(CR_DEFENSE_SKILL))) * 0.04f;
		// Parry from SPELL_AURA_MOD_PARRY_PERCENT aura
		nondiminishing += GetTotalAuraModifier(SPELL_AURA_MOD_PARRY_PERCENT);
		// apply diminishing formula to diminishing parry chance
		value = nondiminishing + diminishing * parry_cap[pclass] /
			(diminishing + parry_cap[pclass] * m_diminishing_k[pclass]);
		value = value < 0.0f ? 0.0f : value;
	}
	SetStatFloatValue(PLAYER_PARRY_PERCENTAGE, value);
}

void MercenaryPet::UpdateDodgePercentage()
{
	const float dodge_cap[MAX_CLASSES] =
	{
		88.129021f,  // Warrior
		88.129021f,  // Paladin
		145.560408f,  // Hunter
		145.560408f,  // Rogue
		150.375940f,  // Priest
		88.129021f,  // DK
		145.560408f,  // Shaman
		150.375940f,  // Mage
		150.375940f,  // Warlock
		0.0f,       // ??
		116.890707f   // Druid
	};

	float diminishing = 0.0f, nondiminishing = 0.0f;
	// Dodge from agility
	GetDodgeFromAgility(diminishing, nondiminishing);
	// Modify value from defense skill (only bonus from defense rating diminishes)
	nondiminishing += (GetSkillValue(SKILL_DEFENSE) - GetMaxSkillValueForLevel()) * 0.04f;
	diminishing += (int32(GetRatingBonusValue(CR_DEFENSE_SKILL))) * 0.04f;
	// Dodge from SPELL_AURA_MOD_DODGE_PERCENT aura
	nondiminishing += GetTotalAuraModifier(SPELL_AURA_MOD_DODGE_PERCENT);
	// Dodge from rating
	diminishing += GetRatingBonusValue(CR_DODGE);
	// apply diminishing formula to diminishing dodge chance
	uint32 pclass = getClass() - 1;
	float value = nondiminishing + (diminishing * dodge_cap[pclass] /
		(diminishing + dodge_cap[pclass] * m_diminishing_k[pclass]));
	value = value < 0.0f ? 0.0f : value;
	SetStatFloatValue(PLAYER_DODGE_PERCENTAGE, value);
}

void MercenaryPet::UpdateSpellCritChance(uint32 school)
{
	// For normal school set zero crit chance
	if (school == SPELL_SCHOOL_NORMAL)
	{
		SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1, 0.0f);
		return;
	}
	// For others recalculate it from:
	float crit = 0.0f;
	// Crit from Intellect
	crit += GetSpellCritFromIntellect();
	// Increase crit from SPELL_AURA_MOD_SPELL_CRIT_CHANCE
	crit += GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_CRIT_CHANCE);
	// Increase crit from SPELL_AURA_MOD_ALL_CRIT_CHANCE
	crit += GetTotalAuraModifier(SPELL_AURA_MOD_ALL_CRIT_CHANCE);
	// Increase crit by school from SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL
	crit += GetTotalAuraModifierByMiscMask(SPELL_AURA_MOD_SPELL_CRIT_CHANCE_SCHOOL, 1 << school);
	// Increase crit from spell crit ratings
	crit += GetRatingBonusValue(CR_CRIT_SPELL);

	// Store crit value
	SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + school, crit);
}

void MercenaryPet::UpdateMeleeHitChances()
{
	m_modMeleeHitChance = GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
	m_modMeleeHitChance += GetRatingBonusValue(CR_HIT_MELEE);
}

void MercenaryPet::UpdateRangedHitChances()
{
	m_modRangedHitChance = GetTotalAuraModifier(SPELL_AURA_MOD_HIT_CHANCE);
	m_modRangedHitChance += GetRatingBonusValue(CR_HIT_RANGED);
}

void MercenaryPet::UpdateSpellHitChances()
{
	m_modSpellHitChance = GetTotalAuraModifier(SPELL_AURA_MOD_SPELL_HIT_CHANCE);
	m_modSpellHitChance += GetRatingBonusValue(CR_HIT_SPELL);
}

void MercenaryPet::UpdateAllSpellCritChances()
{
	for (int i = SPELL_SCHOOL_NORMAL; i < MAX_SPELL_SCHOOL; ++i)
		UpdateSpellCritChance(i);
}

void MercenaryPet::UpdateExpertise(WeaponAttackType attack)
{
	if (attack == RANGED_ATTACK)
		return;

	int32 expertise = int32(GetRatingBonusValue(CR_EXPERTISE));

	Item* weapon = GetWeaponForAttack(attack);

	AuraList const& expAuras = GetAurasByType(SPELL_AURA_MOD_EXPERTISE);
	for (AuraList::const_iterator itr = expAuras.begin(); itr != expAuras.end(); ++itr)
	{
		// item neutral spell
		if ((*itr)->GetSpellProto()->EquippedItemClass == -1)
			expertise += (*itr)->GetModifier()->m_amount;
		// item dependent spell
		else if (weapon && weapon->IsFitToSpellRequirements((*itr)->GetSpellProto()))
			expertise += (*itr)->GetModifier()->m_amount;
	}

	if (expertise < 0)
		expertise = 0;

	switch (attack)
	{
	case BASE_ATTACK: SetUInt32Value(PLAYER_EXPERTISE, expertise);         break;
	case OFF_ATTACK:  SetUInt32Value(PLAYER_OFFHAND_EXPERTISE, expertise); break;
	default: break;
	}
}

void MercenaryPet::UpdateArmorPenetration()
{
	m_armorPenetrationPct = GetRatingBonusValue(CR_ARMOR_PENETRATION);

	AuraList const& armorAuras = GetAurasByType(SPELL_AURA_MOD_TARGET_ARMOR_PCT);
	for (AuraList::const_iterator itr = armorAuras.begin(); itr != armorAuras.end(); ++itr)
	{
		// affects all weapons
		if ((*itr)->GetSpellProto()->EquippedItemClass == -1)
		{
			m_armorPenetrationPct += (*itr)->GetModifier()->m_amount;
			continue;
		}

		// dependent on weapon class
		for (uint8 i = 0; i < MAX_ATTACK; ++i)
		{
			Item* weapon = GetWeaponForAttack(WeaponAttackType(i));
			if (weapon && weapon->IsFitToSpellRequirements((*itr)->GetSpellProto()))
			{
				m_armorPenetrationPct += (*itr)->GetModifier()->m_amount;
				break;
			}
		}
	}
}

void MercenaryPet::ApplyManaRegenBonus(int32 amount, bool apply)
{
	m_baseManaRegen += apply ? amount : -amount;
	UpdateManaRegen();
}

void MercenaryPet::UpdateManaRegen()
{
	float Intellect = GetStat(STAT_INTELLECT);
	// Mana regen from spirit and intellect
	float power_regen = sqrt(Intellect) * OCTRegenMPPerSpirit();
	// Apply PCT bonus from SPELL_AURA_MOD_POWER_REGEN_PERCENT aura on spirit base regen
	power_regen *= GetTotalAuraMultiplierByMiscValue(SPELL_AURA_MOD_POWER_REGEN_PERCENT, POWER_MANA);

	// Mana regen from SPELL_AURA_MOD_POWER_REGEN aura
	float power_regen_mp5 = (GetTotalAuraModifierByMiscValue(SPELL_AURA_MOD_POWER_REGEN, POWER_MANA) + m_baseManaRegen) / 5.0f;

	// Get bonus from SPELL_AURA_MOD_MANA_REGEN_FROM_STAT aura
	AuraList const& regenAura = GetAurasByType(SPELL_AURA_MOD_MANA_REGEN_FROM_STAT);
	for (AuraList::const_iterator i = regenAura.begin(); i != regenAura.end(); ++i)
	{
		Modifier* mod = (*i)->GetModifier();
		power_regen_mp5 += GetStat(Stats(mod->m_miscvalue)) * mod->m_amount / 500.0f;
	}

	// Set regen rate in cast state apply only on spirit based regen
	int32 modManaRegenInterrupt = GetTotalAuraModifier(SPELL_AURA_MOD_MANA_REGEN_INTERRUPT);
	if (modManaRegenInterrupt > 100)
		modManaRegenInterrupt = 100;
	SetStatFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, power_regen_mp5 + power_regen * modManaRegenInterrupt / 100.0f);

	SetStatFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, power_regen_mp5 + power_regen);
}

void MercenaryPet::_ApplyAllStatBonuses()
{
	SetCanModifyStats(false);

	_ApplyAllAuraMods();
	_ApplyAllItemMods();

	SetCanModifyStats(true);

	UpdateAllStats();
}

void MercenaryPet::_RemoveAllStatBonuses()
{
	SetCanModifyStats(false);

	_RemoveAllItemMods();
	_RemoveAllAuraMods();

	SetCanModifyStats(true);

	UpdateAllStats();
}

void MercenaryPet::ApplyRatingMod(CombatRating cr, int32 value, bool apply)
{
	m_baseRatingValue[cr] += (apply ? value : -value);

	// explicit affected values
	switch (cr)
	{
	case CR_HASTE_MELEE:
	{
						   float RatingChange = value * GetRatingMultiplier(cr);
						   ApplyAttackTimePercentMod(BASE_ATTACK, RatingChange, apply);
						   ApplyAttackTimePercentMod(OFF_ATTACK, RatingChange, apply);
						   break;
	}
	case CR_HASTE_RANGED:
	{
							float RatingChange = value * GetRatingMultiplier(cr);
							ApplyAttackTimePercentMod(RANGED_ATTACK, RatingChange, apply);
							break;
	}
	case CR_HASTE_SPELL:
	{
						   float RatingChange = value * GetRatingMultiplier(cr);
						   ApplyCastTimePercentMod(RatingChange, apply);
						   break;
	}
	default:
		break;
	}

	UpdateRating(cr);
}

void MercenaryPet::UpdateRating(CombatRating cr)
{
	int32 amount = m_baseRatingValue[cr];
	// 应用奖励来自SPELL_AURA_MOD_RATING_FROM_STAT
	// stat used stored in miscValueB for this aura
	AuraList const& modRatingFromStat = GetAurasByType(SPELL_AURA_MOD_RATING_FROM_STAT);
	for (AuraList::const_iterator i = modRatingFromStat.begin(); i != modRatingFromStat.end(); ++i)
	if ((*i)->GetMiscValue() & (1 << cr))
		amount += int32(GetStat(Stats((*i)->GetMiscBValue())) * (*i)->GetModifier()->m_amount / 100.0f);
	if (amount < 0)
		amount = 0;
	SetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + cr, uint32(amount));

	bool affectStats = CanModifyStats();

	switch (cr)
	{
	case CR_WEAPON_SKILL:                               // Implemented in Unit::RollMeleeOutcomeAgainst
	case CR_DEFENSE_SKILL:
		UpdateDefenseBonusesMod();
		break;
	case CR_DODGE:
		UpdateDodgePercentage();
		break;
	case CR_PARRY:
		UpdateParryPercentage();
		break;
	case CR_BLOCK:
		UpdateBlockPercentage();
		break;
	case CR_HIT_MELEE:
		UpdateMeleeHitChances();
		break;
	case CR_HIT_RANGED:
		UpdateRangedHitChances();
		break;
	case CR_HIT_SPELL:
		UpdateSpellHitChances();
		break;
	case CR_CRIT_MELEE:
		if (affectStats)
		{
			UpdateCritPercentage(BASE_ATTACK);
			UpdateCritPercentage(OFF_ATTACK);
		}
		break;
	case CR_CRIT_RANGED:
		if (affectStats)
			UpdateCritPercentage(RANGED_ATTACK);
		break;
	case CR_CRIT_SPELL:
		if (affectStats)
			UpdateAllSpellCritChances();
		break;
	case CR_HIT_TAKEN_MELEE:                            // Implemented in Unit::MeleeMissChanceCalc
	case CR_HIT_TAKEN_RANGED:
		break;
	case CR_HIT_TAKEN_SPELL:                            // Implemented in Unit::MagicSpellHitResult
		break;
	case CR_CRIT_TAKEN_MELEE:                           // Implemented in Unit::RollMeleeOutcomeAgainst (only for chance to crit)
	case CR_CRIT_TAKEN_RANGED:
		break;
	case CR_CRIT_TAKEN_SPELL:                           // Implemented in Unit::SpellCriticalBonus (only for chance to crit)
		break;
	case CR_HASTE_MELEE:                                // Implemented in Player::ApplyRatingMod
	case CR_HASTE_RANGED:
	case CR_HASTE_SPELL:
		break;
	case CR_WEAPON_SKILL_MAINHAND:                      // Implemented in Unit::RollMeleeOutcomeAgainst
	case CR_WEAPON_SKILL_OFFHAND:
	case CR_WEAPON_SKILL_RANGED:
		break;
	case CR_EXPERTISE:
		if (affectStats)
		{
			UpdateExpertise(BASE_ATTACK);
			UpdateExpertise(OFF_ATTACK);
		}
		break;
	case CR_ARMOR_PENETRATION:
		if (affectStats)
			UpdateArmorPenetration();
		break;
	}
}

float MercenaryPet::GetRatingMultiplier(CombatRating cr) const
{
	uint32 level = getLevel();

	if (level > GT_MAX_LEVEL) level = GT_MAX_LEVEL;

	GtCombatRatingsEntry const* Rating = sGtCombatRatingsStore.LookupEntry(cr * GT_MAX_LEVEL + level - 1);
	// gtOCTClassCombatRatingScalarStore.dbc starts with 1, CombatRating with zero, so cr+1
	GtOCTClassCombatRatingScalarEntry const* classRating = sGtOCTClassCombatRatingScalarStore.LookupEntry((getClass() - 1) * GT_MAX_RATING + cr + 1);
	if (!Rating || !classRating)
		return 1.0f;                                        // By default use minimum coefficient (not must be called)

	return classRating->ratio / Rating->ratio;
}
void MercenaryPet::_RemoveAllItemMods()
{
	DEBUG_LOG("MercenaryPet _RemoveAllItemMods start.");

	for (int i = 0; i < INVENTORY_SLOT_BAG_END; ++i)
	{
		if (m_items[i])
		{
			ItemPrototype const* proto = m_items[i]->GetProto();
			if (!proto)
				continue;

			// 物品套装奖励不依赖物品打断状态
			//if (proto->ItemSet)
				//RemoveItemsSetItem(this, proto);

			if (m_items[i]->IsBroken())
				continue;

			ApplyItemEquipSpell(m_items[i], false);
			ApplyEnchantment(m_items[i], false);
		}
	}

	for (int i = 0; i < INVENTORY_SLOT_BAG_END; ++i)
	{
		if (m_items[i])
		{
			if (m_items[i]->IsBroken())
				continue;
			ItemPrototype const* proto = m_items[i]->GetProto();
			if (!proto)
				continue;

			uint32 attacktype = Player::GetAttackBySlot(i);
			if (attacktype < MAX_ATTACK)
				_ApplyWeaponDependentAuraMods(m_items[i], WeaponAttackType(attacktype), false);

			_ApplyItemBonuses(proto, i, false);

			if (i == EQUIPMENT_SLOT_RANGED)
				_ApplyAmmoBonuses();
		}
	}

	DEBUG_LOG("MercenaryPet _RemoveAllItemMods complete.");
}
void MercenaryPet::_ApplyAmmoBonuses()
{
	// check ammo
	uint32 ammo_id = GetUInt32Value(PLAYER_AMMO_ID);//【这里取得为null，改为通过player取得】
	if (!ammo_id)
		return;

	float currentAmmoDPS;

	ItemPrototype const* ammo_proto = ObjectMgr::GetItemPrototype(ammo_id);
	if (!ammo_proto || ammo_proto->Class != ITEM_CLASS_PROJECTILE || !CheckAmmoCompatibility(ammo_proto))
		currentAmmoDPS = 0.0f;
	else
		currentAmmoDPS = ammo_proto->Damage[0].DamageMin;

	if (currentAmmoDPS == GetAmmoDPS())
		return;

	m_ammoDPS = currentAmmoDPS;

	if (CanModifyStats())
		UpdateDamagePhysical(RANGED_ATTACK);
}
void MercenaryPet::_ApplyItemBonuses(ItemPrototype const* proto, uint8 slot, bool apply, bool only_level_scale /*= false*/)
{
	if (slot >= INVENTORY_SLOT_BAG_END || !proto)
		return;

	ScalingStatDistributionEntry const* ssd = proto->ScalingStatDistribution ? sScalingStatDistributionStore.LookupEntry(proto->ScalingStatDistribution) : nullptr;
	if (only_level_scale && !ssd)
		return;

	// req. check at equip, but allow use for extended range if range limit max level, set proper level
	uint32 ssd_level = getLevel();
	if (ssd && ssd_level > ssd->MaxLevel)
		ssd_level = ssd->MaxLevel;

	ScalingStatValuesEntry const* ssv = proto->ScalingStatValue ? sScalingStatValuesStore.LookupEntry(ssd_level) : nullptr;
	if (only_level_scale && !ssv)
		return;

	for (uint32 i = 0; i < MAX_ITEM_PROTO_STATS; ++i)
	{
		uint32 statType = 0;
		int32  val = 0;
		// If set ScalingStatDistribution need get stats and values from it
		if (ssd && ssv)
		{
			if (ssd->StatMod[i] < 0)
				continue;
			statType = ssd->StatMod[i];
			val = (ssv->getssdMultiplier(proto->ScalingStatValue) * ssd->Modifier[i]) / 10000;
		}
		else
		{
			if (i >= proto->StatsCount)
				continue;
			statType = proto->ItemStat[i].ItemStatType;
			val = proto->ItemStat[i].ItemStatValue;
		}

		if (val == 0)
			continue;

		switch (statType)
		{
		case ITEM_MOD_MANA:
			HandleStatModifier(UNIT_MOD_MANA, BASE_VALUE, float(val), apply);
			break;
		case ITEM_MOD_HEALTH:                           // modify HP
			HandleStatModifier(UNIT_MOD_HEALTH, BASE_VALUE, float(val), apply);
			break;
		case ITEM_MOD_AGILITY:                          // modify agility
			HandleStatModifier(UNIT_MOD_STAT_AGILITY, BASE_VALUE, float(val), apply);
			ApplyStatBuffMod(STAT_AGILITY, float(val), apply);
			break;
		case ITEM_MOD_STRENGTH:                         // modify strength
			HandleStatModifier(UNIT_MOD_STAT_STRENGTH, BASE_VALUE, float(val), apply);
			ApplyStatBuffMod(STAT_STRENGTH, float(val), apply);
			break;
		case ITEM_MOD_INTELLECT:                        // modify intellect
			HandleStatModifier(UNIT_MOD_STAT_INTELLECT, BASE_VALUE, float(val), apply);
			ApplyStatBuffMod(STAT_INTELLECT, float(val), apply);
			break;
		case ITEM_MOD_SPIRIT:                           // modify spirit
			HandleStatModifier(UNIT_MOD_STAT_SPIRIT, BASE_VALUE, float(val), apply);
			ApplyStatBuffMod(STAT_SPIRIT, float(val), apply);
			break;
		case ITEM_MOD_STAMINA:                          // modify stamina
			HandleStatModifier(UNIT_MOD_STAT_STAMINA, BASE_VALUE, float(val), apply);
			ApplyStatBuffMod(STAT_STAMINA, float(val), apply);
			break;
		case ITEM_MOD_DEFENSE_SKILL_RATING:
			ApplyRatingMod(CR_DEFENSE_SKILL, int32(val), apply);
			break;
		case ITEM_MOD_DODGE_RATING:
			ApplyRatingMod(CR_DODGE, int32(val), apply);
			break;
		case ITEM_MOD_PARRY_RATING:
			ApplyRatingMod(CR_PARRY, int32(val), apply);
			break;
		case ITEM_MOD_BLOCK_RATING:
			ApplyRatingMod(CR_BLOCK, int32(val), apply);
			break;
		case ITEM_MOD_HIT_MELEE_RATING:
			ApplyRatingMod(CR_HIT_MELEE, int32(val), apply);
			break;
		case ITEM_MOD_HIT_RANGED_RATING:
			ApplyRatingMod(CR_HIT_RANGED, int32(val), apply);
			break;
		case ITEM_MOD_HIT_SPELL_RATING:
			ApplyRatingMod(CR_HIT_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_MELEE_RATING:
			ApplyRatingMod(CR_CRIT_MELEE, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_RANGED_RATING:
			ApplyRatingMod(CR_CRIT_RANGED, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_SPELL_RATING:
			ApplyRatingMod(CR_CRIT_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_HIT_TAKEN_MELEE_RATING:
			ApplyRatingMod(CR_HIT_TAKEN_MELEE, int32(val), apply);
			break;
		case ITEM_MOD_HIT_TAKEN_RANGED_RATING:
			ApplyRatingMod(CR_HIT_TAKEN_RANGED, int32(val), apply);
			break;
		case ITEM_MOD_HIT_TAKEN_SPELL_RATING:
			ApplyRatingMod(CR_HIT_TAKEN_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_TAKEN_MELEE_RATING:
			ApplyRatingMod(CR_CRIT_TAKEN_MELEE, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_TAKEN_RANGED_RATING:
			ApplyRatingMod(CR_CRIT_TAKEN_RANGED, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_TAKEN_SPELL_RATING:
			ApplyRatingMod(CR_CRIT_TAKEN_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_HASTE_MELEE_RATING:
			ApplyRatingMod(CR_HASTE_MELEE, int32(val), apply);
			break;
		case ITEM_MOD_HASTE_RANGED_RATING:
			ApplyRatingMod(CR_HASTE_RANGED, int32(val), apply);
			break;
		case ITEM_MOD_HASTE_SPELL_RATING:
			ApplyRatingMod(CR_HASTE_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_HIT_RATING:
			ApplyRatingMod(CR_HIT_MELEE, int32(val), apply);
			ApplyRatingMod(CR_HIT_RANGED, int32(val), apply);
			ApplyRatingMod(CR_HIT_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_RATING:
			ApplyRatingMod(CR_CRIT_MELEE, int32(val), apply);
			ApplyRatingMod(CR_CRIT_RANGED, int32(val), apply);
			ApplyRatingMod(CR_CRIT_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_HIT_TAKEN_RATING:
			ApplyRatingMod(CR_HIT_TAKEN_MELEE, int32(val), apply);
			ApplyRatingMod(CR_HIT_TAKEN_RANGED, int32(val), apply);
			ApplyRatingMod(CR_HIT_TAKEN_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_CRIT_TAKEN_RATING:
			ApplyRatingMod(CR_CRIT_TAKEN_MELEE, int32(val), apply);
			ApplyRatingMod(CR_CRIT_TAKEN_RANGED, int32(val), apply);
			ApplyRatingMod(CR_CRIT_TAKEN_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_RESILIENCE_RATING:
			ApplyRatingMod(CR_CRIT_TAKEN_MELEE, int32(val), apply);
			ApplyRatingMod(CR_CRIT_TAKEN_RANGED, int32(val), apply);
			ApplyRatingMod(CR_CRIT_TAKEN_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_HASTE_RATING:
			ApplyRatingMod(CR_HASTE_MELEE, int32(val), apply);
			ApplyRatingMod(CR_HASTE_RANGED, int32(val), apply);
			ApplyRatingMod(CR_HASTE_SPELL, int32(val), apply);
			break;
		case ITEM_MOD_EXPERTISE_RATING:
			ApplyRatingMod(CR_EXPERTISE, int32(val), apply);
			break;
		case ITEM_MOD_ATTACK_POWER:
			HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, float(val), apply);
			HandleStatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, float(val), apply);
			break;
		case ITEM_MOD_RANGED_ATTACK_POWER:
			HandleStatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, float(val), apply);
			break;
		case ITEM_MOD_MANA_REGENERATION:
			ApplyManaRegenBonus(int32(val), apply);
			break;
		case ITEM_MOD_ARMOR_PENETRATION_RATING:
			ApplyRatingMod(CR_ARMOR_PENETRATION, int32(val), apply);
			break;
		case ITEM_MOD_SPELL_POWER:
			ApplySpellPowerBonus(int32(val), apply);
			break;
		case ITEM_MOD_SPELL_PENETRATION:
			ApplyModInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, -int32(val), apply);
			m_spellPenetrationItemMod += apply ? val : -val;
			break;
		case ITEM_MOD_BLOCK_VALUE:
			HandleBaseModValue(SHIELD_BLOCK_VALUE, FLAT_MOD, float(val), apply);
			break;
			// deprecated item mods
		case ITEM_MOD_FERAL_ATTACK_POWER:
		case ITEM_MOD_SPELL_HEALING_DONE:
		case ITEM_MOD_SPELL_DAMAGE_DONE:
			break;
		}
	}

	// Apply Spell Power from ScalingStatValue if set
	if (ssv)
	{
		if (int32 spellbonus = ssv->getSpellBonus(proto->ScalingStatValue))
			ApplySpellPowerBonus(spellbonus, apply);
	}

	// If set ScalingStatValue armor get it or use item armor
	uint32 armor = proto->Armor;
	if (ssv)
	{
		if (uint32 ssvarmor = ssv->getArmorMod(proto->ScalingStatValue))
			armor = ssvarmor;
	}
	// Add armor bonus from ArmorDamageModifier if > 0
	if (proto->ArmorDamageModifier > 0)
		armor += uint32(proto->ArmorDamageModifier);

	if (armor)
	{
		switch (proto->InventoryType)
		{
		case INVTYPE_TRINKET:
		case INVTYPE_NECK:
		case INVTYPE_CLOAK:
		case INVTYPE_FINGER:
			HandleStatModifier(UNIT_MOD_ARMOR, TOTAL_VALUE, float(armor), apply);
			break;
		default:
			HandleStatModifier(UNIT_MOD_ARMOR, BASE_VALUE, float(armor), apply);
			break;
		}
	}

	if (proto->Block)
		HandleBaseModValue(SHIELD_BLOCK_VALUE, FLAT_MOD, float(proto->Block), apply);

	if (proto->HolyRes)
		HandleStatModifier(UNIT_MOD_RESISTANCE_HOLY, BASE_VALUE, float(proto->HolyRes), apply);

	if (proto->FireRes)
		HandleStatModifier(UNIT_MOD_RESISTANCE_FIRE, BASE_VALUE, float(proto->FireRes), apply);

	if (proto->NatureRes)
		HandleStatModifier(UNIT_MOD_RESISTANCE_NATURE, BASE_VALUE, float(proto->NatureRes), apply);

	if (proto->FrostRes)
		HandleStatModifier(UNIT_MOD_RESISTANCE_FROST, BASE_VALUE, float(proto->FrostRes), apply);

	if (proto->ShadowRes)
		HandleStatModifier(UNIT_MOD_RESISTANCE_SHADOW, BASE_VALUE, float(proto->ShadowRes), apply);

	if (proto->ArcaneRes)
		HandleStatModifier(UNIT_MOD_RESISTANCE_ARCANE, BASE_VALUE, float(proto->ArcaneRes), apply);

	WeaponAttackType attType = BASE_ATTACK;
	float damage = 0.0f;

	if (slot == EQUIPMENT_SLOT_RANGED && (
		proto->InventoryType == INVTYPE_RANGED || proto->InventoryType == INVTYPE_THROWN ||
		proto->InventoryType == INVTYPE_RANGEDRIGHT))
	{
		attType = RANGED_ATTACK;
	}
	else if (slot == EQUIPMENT_SLOT_OFFHAND)
	{
		attType = OFF_ATTACK;
	}

	float minDamage = proto->Damage[0].DamageMin;
	float maxDamage = proto->Damage[0].DamageMax;
	int32 extraDPS = 0;
	// If set dpsMod in ScalingStatValue use it for min (70% from average), max (130% from average) damage
	if (ssv)
	{
		if ((extraDPS = ssv->getDPSMod(proto->ScalingStatValue)))
		{
			float average = extraDPS * proto->Delay / 1000.0f;
			minDamage = 0.7f * average;
			maxDamage = 1.3f * average;
		}
	}
	if (minDamage > 0)
	{
		damage = apply ? minDamage : BASE_MINDAMAGE;
		SetBaseWeaponDamage(attType, MINDAMAGE, damage);
		// sLog.outError("applying mindam: assigning %f to weapon mindamage, now is: %f", damage, GetWeaponDamageRange(attType, MINDAMAGE));
	}

	if (maxDamage  > 0)
	{
		damage = apply ? maxDamage : BASE_MAXDAMAGE;
		SetBaseWeaponDamage(attType, MAXDAMAGE, damage);
	}

	// Apply feral bonus from ScalingStatValue if set
	if (ssv)
	{
		if (int32 feral_bonus = ssv->getFeralBonus(proto->ScalingStatValue))
			ApplyFeralAPBonus(feral_bonus, apply);
	}
	// Druids get feral AP bonus from weapon dps (also use DPS from ScalingStatValue)
	if (getClass() == CLASS_DRUID)
	{
		int32 feral_bonus = proto->getFeralBonus(extraDPS);
		if (feral_bonus > 0)
			ApplyFeralAPBonus(feral_bonus, apply);
	}

	if (!CanUseEquippedWeapon(attType))
		return;

	if (proto->Delay)
	{
		if (slot == EQUIPMENT_SLOT_RANGED)
			SetAttackTime(RANGED_ATTACK, apply ? proto->Delay : BASE_ATTACK_TIME);
		else if (slot == EQUIPMENT_SLOT_MAINHAND)
			SetAttackTime(BASE_ATTACK, apply ? proto->Delay : BASE_ATTACK_TIME);
		else if (slot == EQUIPMENT_SLOT_OFFHAND)
			SetAttackTime(OFF_ATTACK, apply ? proto->Delay : BASE_ATTACK_TIME);
	}

	if (CanModifyStats() && (damage || proto->Delay))
		UpdateDamagePhysical(attType);
}
	void MercenaryPet::HandleBaseModValue(BaseModGroup modGroup, BaseModType modType, float amount, bool apply)
	{
		if (modGroup >= BASEMOD_END || modType >= MOD_END)
		{
			sLog.outError("ERROR in HandleBaseModValue(): nonexistent BaseModGroup of wrong BaseModType!");
			return;
		}

		float val = 1.0f;

		switch (modType)
		{
		case FLAT_MOD:
			m_auraBaseMod[modGroup][modType] += apply ? amount : -amount;
			break;
		case PCT_MOD:
			if (amount <= -100.0f)
				amount = -200.0f;

			val = (100.0f + amount) / 100.0f;
			m_auraBaseMod[modGroup][modType] *= apply ? val : (1.0f / val);
			break;
		default:
			break;
		}

		if (!CanModifyStats())
			return;

		switch (modGroup)
		{
		case CRIT_PERCENTAGE:              UpdateCritPercentage(BASE_ATTACK);                          break;
		case RANGED_CRIT_PERCENTAGE:       UpdateCritPercentage(RANGED_ATTACK);                        break;
		case OFFHAND_CRIT_PERCENTAGE:      UpdateCritPercentage(OFF_ATTACK);                           break;
		case SHIELD_BLOCK_VALUE:           UpdateShieldBlockValue();                                   break;
		default: break;
		}
	}
	void MercenaryPet::UpdateAllRatings()
	{
		for (int cr = 0; cr < MAX_COMBAT_RATING; ++cr)
			UpdateRating(CombatRating(cr));
	}
	void MercenaryPet::SetCanBlock(bool value)
	{
		if (m_canBlock == value)
			return;

		m_canBlock = value;
		UpdateBlockPercentage();
	}
	float MercenaryPet::GetRatingBonusValue(CombatRating cr) const
	{
		return float(GetUInt32Value(PLAYER_FIELD_COMBAT_RATING_1 + cr)) * GetRatingMultiplier(cr);
	}
	float MercenaryPet::GetMeleeCritFromAgility()
	{
		uint32 level = getLevel();
		uint32 pclass = getClass();

		if (level > GT_MAX_LEVEL) level = GT_MAX_LEVEL;

		GtChanceToMeleeCritBaseEntry const* critBase = sGtChanceToMeleeCritBaseStore.LookupEntry(pclass - 1);
		GtChanceToMeleeCritEntry     const* critRatio = sGtChanceToMeleeCritStore.LookupEntry((pclass - 1) * GT_MAX_LEVEL + level - 1);
		if (critBase == nullptr || critRatio == nullptr)
			return 0.0f;

		float crit = critBase->base + GetStat(STAT_AGILITY) * critRatio->ratio;
		return crit * 100.0f;
	}
	uint16 MercenaryPet::GetSkillValue(uint32 skill) const
	{
		if (!skill)
			return 0;

		SkillStatusMap::const_iterator itr = mSkillStatus.find(skill);
		if (itr == mSkillStatus.end())
			return 0;

		SkillStatusData const& skillStatus = itr->second;
		if (skillStatus.uState == SKILL_DELETED)
			return 0;

		uint32 bonus = GetUInt32Value(PLAYER_SKILL_BONUS_INDEX(skillStatus.pos));

		int32 result = int32(SKILL_VALUE(GetUInt32Value(PLAYER_SKILL_VALUE_INDEX(skillStatus.pos))));
		result += SKILL_TEMP_BONUS(bonus);
		result += SKILL_PERM_BONUS(bonus);
		return result < 0 ? 0 : result;
	}
	void MercenaryPet::GetDodgeFromAgility(float& diminishing, float& nondiminishing)
	{
		// Table for base dodge values
		const float dodge_base[MAX_CLASSES] =
		{
			0.036640f, // Warrior
			0.034943f, // Paladin
			-0.040873f, // Hunter
			0.020957f, // Rogue
			0.034178f, // Priest
			0.036640f, // DK
			0.021080f, // Shaman
			0.036587f, // Mage
			0.024211f, // Warlock
			0.0f,      // ??
			0.056097f  // Druid
		};
		// Crit/agility to dodge/agility coefficient multipliers; 3.2.0 increased required agility by 15%
		const float crit_to_dodge[MAX_CLASSES] =
		{
			0.85f / 1.15f,  // Warrior
			1.00f / 1.15f,  // Paladin
			1.11f / 1.15f,  // Hunter
			2.00f / 1.15f,  // Rogue
			1.00f / 1.15f,  // Priest
			0.85f / 1.15f,  // DK
			1.60f / 1.15f,  // Shaman
			1.00f / 1.15f,  // Mage
			0.97f / 1.15f,  // Warlock (?)
			0.0f,           // ??
			2.00f / 1.15f   // Druid
		};

		uint32 level = getLevel();
		uint32 pclass = getClass();

		if (level > GT_MAX_LEVEL) level = GT_MAX_LEVEL;

		// Dodge per agility is proportional to crit per agility, which is available from DBC files
		GtChanceToMeleeCritEntry  const* dodgeRatio = sGtChanceToMeleeCritStore.LookupEntry((pclass - 1) * GT_MAX_LEVEL + level - 1);
		if (dodgeRatio == nullptr || pclass > MAX_CLASSES)
			return;

		// TODO: research if talents/effects that increase total agility by x% should increase non-diminishing part
		float base_agility = GetCreateStat(STAT_AGILITY) * m_auraModifiersGroup[UNIT_MOD_STAT_START + STAT_AGILITY][BASE_PCT];
		float bonus_agility = GetStat(STAT_AGILITY) - base_agility;
		// calculate diminishing (green in char screen) and non-diminishing (white) contribution
		diminishing = 100.0f * bonus_agility * dodgeRatio->ratio * crit_to_dodge[pclass - 1];
		nondiminishing = 100.0f * (dodge_base[pclass - 1] + base_agility * dodgeRatio->ratio * crit_to_dodge[pclass - 1]);
	}
	float MercenaryPet::GetSpellCritFromIntellect()
	{
		uint32 level = getLevel();
		uint32 pclass = getClass();

		if (level > GT_MAX_LEVEL) level = GT_MAX_LEVEL;

		GtChanceToSpellCritBaseEntry const* critBase = sGtChanceToSpellCritBaseStore.LookupEntry(pclass - 1);
		GtChanceToSpellCritEntry     const* critRatio = sGtChanceToSpellCritStore.LookupEntry((pclass - 1) * GT_MAX_LEVEL + level - 1);
		if (critBase == nullptr || critRatio == nullptr)
			return 0.0f;

		float crit = critBase->base + GetStat(STAT_INTELLECT) * critRatio->ratio;
		return crit * 100.0f;
	}

	Player* MercenaryPet::getPlayer(){
		if (Unit * unit = GetOwner())
			return unit->ToPlayer();
		return nullptr;
	}
	Item* MercenaryPet::GetWeaponForAttack(WeaponAttackType attackType, bool nonbroken, bool useable)
	{
		Mercenary* mercenary = getMercenary();
		if (mercenary == nullptr)
			return nullptr;

		uint8 slot;
		switch (attackType)
		{
		case BASE_ATTACK:   slot = EQUIPMENT_SLOT_MAINHAND; break;
		case OFF_ATTACK:    slot = EQUIPMENT_SLOT_OFFHAND;  break;
		case RANGED_ATTACK: slot = EQUIPMENT_SLOT_RANGED;   break;
		default: return nullptr;
		}



		Item* item = mercenary->GetItemByGuid(mercenary->gearContainer[slot].itemguid);
		if (item == nullptr)
			return nullptr;

		if (item->GetProto()->Class != ITEM_CLASS_WEAPON)
			return nullptr;

		if (useable && !CanUseEquippedWeapon(attackType))
			return nullptr;

		if (nonbroken && item->IsBroken())
			return nullptr;

		return item;
	}
	float MercenaryPet::OCTRegenMPPerSpirit()
	{
		uint32 level = getLevel();
		uint32 pclass = getClass();

		if (level > GT_MAX_LEVEL) level = GT_MAX_LEVEL;

		//    GtOCTRegenMPEntry     const *baseRatio = sGtOCTRegenMPStore.LookupEntry((pclass-1)*GT_MAX_LEVEL + level-1);
		GtRegenMPPerSptEntry  const* moreRatio = sGtRegenMPPerSptStore.LookupEntry((pclass - 1) * GT_MAX_LEVEL + level - 1);
		if (moreRatio == nullptr)
			return 0.0f;

		// Formula get from PaperDollFrame script
		float spirit = GetStat(STAT_SPIRIT);
		float regen = spirit * moreRatio->ratio;
		return regen;
	}
	void MercenaryPet::_ApplyAllItemMods()
	{
		DEBUG_LOG("MercenaryPet _ApplyAllItemMods start.");

		for (int i = 0; i < INVENTORY_SLOT_BAG_END; ++i)
		{
			if (m_items[i])
			{
				if (m_items[i]->IsBroken())
					continue;

				ItemPrototype const* proto = m_items[i]->GetProto();
				if (!proto)
					continue;

				uint32 attacktype = Player::GetAttackBySlot(i);
				if (attacktype < MAX_ATTACK)
					_ApplyWeaponDependentAuraMods(m_items[i], WeaponAttackType(attacktype), true);

				_ApplyItemBonuses(proto, i, true);

				if (i == EQUIPMENT_SLOT_RANGED)
					_ApplyAmmoBonuses();
			}
		}

		for (int i = 0; i < INVENTORY_SLOT_BAG_END; ++i)
		{
			if (m_items[i])
			{
				ItemPrototype const* proto = m_items[i]->GetProto();
				if (!proto)
					continue;

				// item set bonuses not dependent from item broken state
				//if (proto->ItemSet)
					//AddItemsSetItem(this, m_items[i]);

				if (m_items[i]->IsBroken())
					continue;

				ApplyItemEquipSpell(m_items[i], true);
				ApplyEnchantment(m_items[i], true);
			}
		}

		DEBUG_LOG("MercenaryPet _ApplyAllItemMods complete.");
	}
	void MercenaryPet::ApplyItemEquipSpell(Item* item, bool apply, bool form_change)
	{
		if (!item)
			return;

		ItemPrototype const* proto = item->GetProto();
		if (!proto)
			return;

		for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			_Spell const& spellData = proto->Spells[i];

			// no spell
			if (!spellData.SpellId)
				continue;

			if (apply)
			{
				// apply only at-equip spells
				if (spellData.SpellTrigger != ITEM_SPELLTRIGGER_ON_EQUIP)
					continue;
			}
			else
			{
				// at un-apply remove all spells (not only at-apply, so any at-use active affects from item and etc)
				// except on form change and with at-use with negative charges, so allow consuming item spells (including with extra flag that prevent consume really)
				// applied to player after item remove from equip slot
				if (spellData.SpellTrigger == ITEM_SPELLTRIGGER_ON_USE && (form_change || spellData.SpellCharges < 0))
					continue;
			}

			// check if it is valid spell
			SpellEntry const* spellproto = sSpellStore.LookupEntry(spellData.SpellId);
			if (!spellproto)
				continue;

			ApplyEquipSpell(spellproto, item, apply, form_change);
		}
	}
	void MercenaryPet::ApplyEquipSpell(SpellEntry const* spellInfo, Item* item, bool apply, bool form_change)
	{
		if (apply)
		{
			// Cannot be used in this stance/form
			if (GetErrorAtShapeshiftedCast(spellInfo, GetShapeshiftForm()) != SPELL_CAST_OK)
				return;

			if (form_change)                                    // check aura active state from other form
			{
				bool found = false;
				for (int k = 0; k < MAX_EFFECT_INDEX; ++k)
				{
					SpellAuraHolderBounds spair = GetSpellAuraHolderBounds(spellInfo->Id);
					for (SpellAuraHolderMap::const_iterator iter = spair.first; iter != spair.second; ++iter)
					{
						if (!item || iter->second->GetCastItemGuid() == item->GetObjectGuid())
						{
							found = true;
							break;
						}
					}
					if (found)
						break;
				}

				if (found)                                      // and skip re-cast already active aura at form change
					return;
			}

			DEBUG_LOG("WORLD: cast %s Equip spellId - %i", (item ? "item" : "itemset"), spellInfo->Id);

			CastSpell(this, spellInfo, true, item);
		}
		else
		{
			if (form_change)                                    // check aura compatibility
			{
				// Cannot be used in this stance/form
				if (GetErrorAtShapeshiftedCast(spellInfo, GetShapeshiftForm()) == SPELL_CAST_OK)
					return;                                     // and remove only not compatible at form change
			}

			if (item)
				RemoveAurasDueToItemSpell(item, spellInfo->Id); // un-apply all spells , not only at-equipped
			else
				RemoveAurasDueToSpell(spellInfo->Id);           // un-apply spell (item set case)
		}
	}
	void MercenaryPet::ApplyEnchantment(Item* item, bool apply)
	{
		for (uint32 slot = 0; slot < MAX_ENCHANTMENT_SLOT; ++slot)
			ApplyEnchantment(item, EnchantmentSlot(slot), apply);
	}

	void MercenaryPet::ApplyEnchantment(Item* item, EnchantmentSlot slot, bool apply, bool apply_dur, bool ignore_condition)
	{
		if (!item)
			return;

		if (!item->IsEquipped())
			return;

		if (slot >= MAX_ENCHANTMENT_SLOT)
			return;

		uint32 enchant_id = item->GetEnchantmentId(slot);
		if (!enchant_id)
			return;

		SpellItemEnchantmentEntry const* pEnchant = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
		if (!pEnchant)
			return;

		if (!ignore_condition && pEnchant->EnchantmentCondition && !EnchantmentFitsRequirements(pEnchant->EnchantmentCondition, -1))
			return;

		if (!item->IsBroken())
		{
			for (int s = 0; s < 3; ++s)
			{
				uint32 enchant_display_type = pEnchant->type[s];
				uint32 enchant_amount = pEnchant->amount[s];
				uint32 enchant_spell_id = pEnchant->spellid[s];

				switch (enchant_display_type)
				{
				case ITEM_ENCHANTMENT_TYPE_NONE:
					break;
				case ITEM_ENCHANTMENT_TYPE_COMBAT_SPELL:
					// processed in Player::CastItemCombatSpell
					break;
				case ITEM_ENCHANTMENT_TYPE_DAMAGE:
					if (item->GetSlot() == EQUIPMENT_SLOT_MAINHAND)
						HandleStatModifier(UNIT_MOD_DAMAGE_MAINHAND, TOTAL_VALUE, float(enchant_amount), apply);
					else if (item->GetSlot() == EQUIPMENT_SLOT_OFFHAND)
						HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_VALUE, float(enchant_amount), apply);
					else if (item->GetSlot() == EQUIPMENT_SLOT_RANGED)
						HandleStatModifier(UNIT_MOD_DAMAGE_RANGED, TOTAL_VALUE, float(enchant_amount), apply);
					break;
				case ITEM_ENCHANTMENT_TYPE_EQUIP_SPELL:
				{
														  if (enchant_spell_id)
														  {
															  if (apply)
															  {
																  int32 basepoints = 0;
																  // Random Property Exist - try found basepoints for spell (basepoints depends from item suffix factor)
																  if (item->GetItemRandomPropertyId())
																  {
																	  ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(abs(item->GetItemRandomPropertyId()));
																	  if (item_rand)
																	  {
																		  // Search enchant_amount
																		  for (int k = 0; k < 3; ++k)
																		  {
																			  if (item_rand->enchant_id[k] == enchant_id)
																			  {
																				  basepoints = int32((item_rand->prefix[k] * item->GetItemSuffixFactor()) / 10000);
																				  break;
																			  }
																		  }
																	  }
																  }
																  // Cast custom spell vs all equal basepoints got from enchant_amount
																  if (basepoints)
																	  CastCustomSpell(this, enchant_spell_id, &basepoints, &basepoints, &basepoints, true, item);
																  else
																	  CastSpell(this, enchant_spell_id, true, item);
															  }
															  else
																  RemoveAurasDueToItemSpell(item, enchant_spell_id);
														  }
														  break;
				}
				case ITEM_ENCHANTMENT_TYPE_RESISTANCE:
					if (!enchant_amount)
					{
						ItemRandomSuffixEntry const* item_rand = sItemRandomSuffixStore.LookupEntry(abs(item->GetItemRandomPropertyId()));
						if (item_rand)
						{
							for (int k = 0; k < 3; ++k)
							{
								if (item_rand->enchant_id[k] == enchant_id)
								{
									enchant_amount = uint32((item_rand->prefix[k] * item->GetItemSuffixFactor()) / 10000);
									break;
								}
							}
						}
					}

					HandleStatModifier(UnitMods(UNIT_MOD_RESISTANCE_START + enchant_spell_id), TOTAL_VALUE, float(enchant_amount), apply);
					break;
				case ITEM_ENCHANTMENT_TYPE_STAT:
				{
												   if (!enchant_amount)
												   {
													   ItemRandomSuffixEntry const* item_rand_suffix = sItemRandomSuffixStore.LookupEntry(abs(item->GetItemRandomPropertyId()));
													   if (item_rand_suffix)
													   {
														   for (int k = 0; k < 3; ++k)
														   {
															   if (item_rand_suffix->enchant_id[k] == enchant_id)
															   {
																   enchant_amount = uint32((item_rand_suffix->prefix[k] * item->GetItemSuffixFactor()) / 10000);
																   break;
															   }
														   }
													   }
												   }

												   DEBUG_LOG("Adding %u to stat nb %u", enchant_amount, enchant_spell_id);
												   switch (enchant_spell_id)
												   {
												   case ITEM_MOD_MANA:
													   DEBUG_LOG("+ %u MANA", enchant_amount);
													   HandleStatModifier(UNIT_MOD_MANA, BASE_VALUE, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_HEALTH:
													   DEBUG_LOG("+ %u HEALTH", enchant_amount);
													   HandleStatModifier(UNIT_MOD_HEALTH, BASE_VALUE, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_AGILITY:
													   DEBUG_LOG("+ %u AGILITY", enchant_amount);
													   HandleStatModifier(UNIT_MOD_STAT_AGILITY, TOTAL_VALUE, float(enchant_amount), apply);
													   ApplyStatBuffMod(STAT_AGILITY, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_STRENGTH:
													   DEBUG_LOG("+ %u STRENGTH", enchant_amount);
													   HandleStatModifier(UNIT_MOD_STAT_STRENGTH, TOTAL_VALUE, float(enchant_amount), apply);
													   ApplyStatBuffMod(STAT_STRENGTH, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_INTELLECT:
													   DEBUG_LOG("+ %u INTELLECT", enchant_amount);
													   HandleStatModifier(UNIT_MOD_STAT_INTELLECT, TOTAL_VALUE, float(enchant_amount), apply);
													   ApplyStatBuffMod(STAT_INTELLECT, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_SPIRIT:
													   DEBUG_LOG("+ %u SPIRIT", enchant_amount);
													   HandleStatModifier(UNIT_MOD_STAT_SPIRIT, TOTAL_VALUE, float(enchant_amount), apply);
													   ApplyStatBuffMod(STAT_SPIRIT, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_STAMINA:
													   DEBUG_LOG("+ %u STAMINA", enchant_amount);
													   HandleStatModifier(UNIT_MOD_STAT_STAMINA, TOTAL_VALUE, float(enchant_amount), apply);
													   ApplyStatBuffMod(STAT_STAMINA, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_DEFENSE_SKILL_RATING:
													   ApplyRatingMod(CR_DEFENSE_SKILL, enchant_amount, apply);
													   DEBUG_LOG("+ %u DEFENCE", enchant_amount);
													   break;
												   case  ITEM_MOD_DODGE_RATING:
													   ApplyRatingMod(CR_DODGE, enchant_amount, apply);
													   DEBUG_LOG("+ %u DODGE", enchant_amount);
													   break;
												   case ITEM_MOD_PARRY_RATING:
													   ApplyRatingMod(CR_PARRY, enchant_amount, apply);
													   DEBUG_LOG("+ %u PARRY", enchant_amount);
													   break;
												   case ITEM_MOD_BLOCK_RATING:
													   ApplyRatingMod(CR_BLOCK, enchant_amount, apply);
													   DEBUG_LOG("+ %u SHIELD_BLOCK", enchant_amount);
													   break;
												   case ITEM_MOD_HIT_MELEE_RATING:
													   ApplyRatingMod(CR_HIT_MELEE, enchant_amount, apply);
													   DEBUG_LOG("+ %u MELEE_HIT", enchant_amount);
													   break;
												   case ITEM_MOD_HIT_RANGED_RATING:
													   ApplyRatingMod(CR_HIT_RANGED, enchant_amount, apply);
													   DEBUG_LOG("+ %u RANGED_HIT", enchant_amount);
													   break;
												   case ITEM_MOD_HIT_SPELL_RATING:
													   ApplyRatingMod(CR_HIT_SPELL, enchant_amount, apply);
													   DEBUG_LOG("+ %u SPELL_HIT", enchant_amount);
													   break;
												   case ITEM_MOD_CRIT_MELEE_RATING:
													   ApplyRatingMod(CR_CRIT_MELEE, enchant_amount, apply);
													   DEBUG_LOG("+ %u MELEE_CRIT", enchant_amount);
													   break;
												   case ITEM_MOD_CRIT_RANGED_RATING:
													   ApplyRatingMod(CR_CRIT_RANGED, enchant_amount, apply);
													   DEBUG_LOG("+ %u RANGED_CRIT", enchant_amount);
													   break;
												   case ITEM_MOD_CRIT_SPELL_RATING:
													   ApplyRatingMod(CR_CRIT_SPELL, enchant_amount, apply);
													   DEBUG_LOG("+ %u SPELL_CRIT", enchant_amount);
													   break;
													   //                        Values from ITEM_STAT_MELEE_HA_RATING to ITEM_MOD_HASTE_RANGED_RATING are never used
													   //                        in Enchantments
													   //                        case ITEM_MOD_HIT_TAKEN_MELEE_RATING:
													   //                            ApplyRatingMod(CR_HIT_TAKEN_MELEE, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_HIT_TAKEN_RANGED_RATING:
													   //                            ApplyRatingMod(CR_HIT_TAKEN_RANGED, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_HIT_TAKEN_SPELL_RATING:
													   //                            ApplyRatingMod(CR_HIT_TAKEN_SPELL, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_CRIT_TAKEN_MELEE_RATING:
													   //                            ApplyRatingMod(CR_CRIT_TAKEN_MELEE, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_CRIT_TAKEN_RANGED_RATING:
													   //                            ApplyRatingMod(CR_CRIT_TAKEN_RANGED, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_CRIT_TAKEN_SPELL_RATING:
													   //                            ApplyRatingMod(CR_CRIT_TAKEN_SPELL, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_HASTE_MELEE_RATING:
													   //                            ApplyRatingMod(CR_HASTE_MELEE, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_HASTE_RANGED_RATING:
													   //                            ApplyRatingMod(CR_HASTE_RANGED, enchant_amount, apply);
													   //                            break;
												   case ITEM_MOD_HASTE_SPELL_RATING:
													   ApplyRatingMod(CR_HASTE_SPELL, enchant_amount, apply);
													   break;
												   case ITEM_MOD_HIT_RATING:
													   ApplyRatingMod(CR_HIT_MELEE, enchant_amount, apply);
													   ApplyRatingMod(CR_HIT_RANGED, enchant_amount, apply);
													   ApplyRatingMod(CR_HIT_SPELL, enchant_amount, apply);
													   DEBUG_LOG("+ %u HIT", enchant_amount);
													   break;
												   case ITEM_MOD_CRIT_RATING:
													   ApplyRatingMod(CR_CRIT_MELEE, enchant_amount, apply);
													   ApplyRatingMod(CR_CRIT_RANGED, enchant_amount, apply);
													   ApplyRatingMod(CR_CRIT_SPELL, enchant_amount, apply);
													   DEBUG_LOG("+ %u CRITICAL", enchant_amount);
													   break;
													   //                        Values ITEM_MOD_HIT_TAKEN_RATING and ITEM_MOD_CRIT_TAKEN_RATING are never used in Enchantment
													   //                        case ITEM_MOD_HIT_TAKEN_RATING:
													   //                            ApplyRatingMod(CR_HIT_TAKEN_MELEE, enchant_amount, apply);
													   //                            ApplyRatingMod(CR_HIT_TAKEN_RANGED, enchant_amount, apply);
													   //                            ApplyRatingMod(CR_HIT_TAKEN_SPELL, enchant_amount, apply);
													   //                            break;
													   //                        case ITEM_MOD_CRIT_TAKEN_RATING:
													   //                            ApplyRatingMod(CR_CRIT_TAKEN_MELEE, enchant_amount, apply);
													   //                            ApplyRatingMod(CR_CRIT_TAKEN_RANGED, enchant_amount, apply);
													   //                            ApplyRatingMod(CR_CRIT_TAKEN_SPELL, enchant_amount, apply);
													   //                            break;
												   case ITEM_MOD_RESILIENCE_RATING:
													   ApplyRatingMod(CR_CRIT_TAKEN_MELEE, enchant_amount, apply);
													   ApplyRatingMod(CR_CRIT_TAKEN_RANGED, enchant_amount, apply);
													   ApplyRatingMod(CR_CRIT_TAKEN_SPELL, enchant_amount, apply);
													   DEBUG_LOG("+ %u RESILIENCE", enchant_amount);
													   break;
												   case ITEM_MOD_HASTE_RATING:
													   ApplyRatingMod(CR_HASTE_MELEE, enchant_amount, apply);
													   ApplyRatingMod(CR_HASTE_RANGED, enchant_amount, apply);
													   ApplyRatingMod(CR_HASTE_SPELL, enchant_amount, apply);
													   DEBUG_LOG("+ %u HASTE", enchant_amount);
													   break;
												   case ITEM_MOD_EXPERTISE_RATING:
													   ApplyRatingMod(CR_EXPERTISE, enchant_amount, apply);
													   DEBUG_LOG("+ %u EXPERTISE", enchant_amount);
													   break;
												   case ITEM_MOD_ATTACK_POWER:
													   HandleStatModifier(UNIT_MOD_ATTACK_POWER, TOTAL_VALUE, float(enchant_amount), apply);
													   HandleStatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, float(enchant_amount), apply);
													   DEBUG_LOG("+ %u ATTACK_POWER", enchant_amount);
													   break;
												   case ITEM_MOD_RANGED_ATTACK_POWER:
													   HandleStatModifier(UNIT_MOD_ATTACK_POWER_RANGED, TOTAL_VALUE, float(enchant_amount), apply);
													   DEBUG_LOG("+ %u RANGED_ATTACK_POWER", enchant_amount);
													   break;
												   case ITEM_MOD_MANA_REGENERATION:
													   ApplyManaRegenBonus(enchant_amount, apply);
													   DEBUG_LOG("+ %u MANA_REGENERATION", enchant_amount);
													   break;
												   case ITEM_MOD_ARMOR_PENETRATION_RATING:
													   ApplyRatingMod(CR_ARMOR_PENETRATION, enchant_amount, apply);
													   DEBUG_LOG("+ %u ARMOR PENETRATION", enchant_amount);
													   break;
												   case ITEM_MOD_SPELL_POWER:
													   ApplySpellPowerBonus(enchant_amount, apply);
													   DEBUG_LOG("+ %u SPELL_POWER", enchant_amount);
													   break;
												   case ITEM_MOD_BLOCK_VALUE:
													   HandleBaseModValue(SHIELD_BLOCK_VALUE, FLAT_MOD, float(enchant_amount), apply);
													   break;
												   case ITEM_MOD_FERAL_ATTACK_POWER:
												   case ITEM_MOD_SPELL_HEALING_DONE:   // deprecated
												   case ITEM_MOD_SPELL_DAMAGE_DONE:    // deprecated
												   default:
													   break;
												   }
												   break;
				}
				case ITEM_ENCHANTMENT_TYPE_TOTEM:           // Shaman Rockbiter Weapon
				{
																if (getClass() == CLASS_SHAMAN)
																{
																	float addValue = 0.0f;
																	if (item->GetSlot() == EQUIPMENT_SLOT_MAINHAND)
																	{
																		addValue = float(enchant_amount * item->GetProto()->Delay / 1000.0f);
																		HandleStatModifier(UNIT_MOD_DAMAGE_MAINHAND, TOTAL_VALUE, addValue, apply);
																	}
																	else if (item->GetSlot() == EQUIPMENT_SLOT_OFFHAND)
																	{
																		addValue = float(enchant_amount * item->GetProto()->Delay / 1000.0f);
																		HandleStatModifier(UNIT_MOD_DAMAGE_OFFHAND, TOTAL_VALUE, addValue, apply);
																	}
																}
																break;
				}
				case ITEM_ENCHANTMENT_TYPE_USE_SPELL:
					// processed in Player::CastItemUseSpell
					break;
				case ITEM_ENCHANTMENT_TYPE_PRISMATIC_SOCKET:
					// nothing do..
					break;
				default:
					sLog.outError("Unknown item enchantment (id = %d) display type: %d", enchant_id, enchant_display_type);
					break;
				}                                               /*switch(enchant_display_type)*/
			}                                                   /*for*/
		}

		// visualize enchantment at player and equipped items
		if (slot == PERM_ENCHANTMENT_SLOT)
			SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (item->GetSlot() * 2), 0, apply ? item->GetEnchantmentId(slot) : 0);

		if (slot == TEMP_ENCHANTMENT_SLOT)
			SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (item->GetSlot() * 2), 1, apply ? item->GetEnchantmentId(slot) : 0);

		if (apply_dur)
		{
			if (apply)
			{
				// set duration
				uint32 duration = item->GetEnchantmentDuration(slot);
				if (duration > 0)
					AddEnchantmentDuration(item, slot, duration);
			}
			else
			{
				// duration == 0 will remove EnchantDuration
				AddEnchantmentDuration(item, slot, 0);
			}
		}
	}
	void MercenaryPet::_ApplyWeaponDependentAuraMods(Item* item, WeaponAttackType attackType, bool apply)
	{
		AuraList const& auraCritList = GetAurasByType(SPELL_AURA_MOD_CRIT_PERCENT);
		for (AuraList::const_iterator itr = auraCritList.begin(); itr != auraCritList.end(); ++itr)
			_ApplyWeaponDependentAuraCritMod(item, attackType, *itr, apply);

		AuraList const& auraDamageFlatList = GetAurasByType(SPELL_AURA_MOD_DAMAGE_DONE);
		for (AuraList::const_iterator itr = auraDamageFlatList.begin(); itr != auraDamageFlatList.end(); ++itr)
			_ApplyWeaponDependentAuraDamageMod(item, attackType, *itr, apply);

		AuraList const& auraDamagePCTList = GetAurasByType(SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
		for (AuraList::const_iterator itr = auraDamagePCTList.begin(); itr != auraDamagePCTList.end(); ++itr)
			_ApplyWeaponDependentAuraDamageMod(item, attackType, *itr, apply);
	}

	void MercenaryPet::_ApplyWeaponDependentAuraCritMod(Item* item, WeaponAttackType attackType, Aura* aura, bool apply)
	{
		// generic not weapon specific case processes in aura code
		if (aura->GetSpellProto()->EquippedItemClass == -1)
			return;

		BaseModGroup mod = BASEMOD_END;
		switch (attackType)
		{
		case BASE_ATTACK:   mod = CRIT_PERCENTAGE;        break;
		case OFF_ATTACK:    mod = OFFHAND_CRIT_PERCENTAGE; break;
		case RANGED_ATTACK: mod = RANGED_CRIT_PERCENTAGE; break;
		default: return;
		}

		if (item->IsFitToSpellRequirements(aura->GetSpellProto()))
		{
			HandleBaseModValue(mod, FLAT_MOD, float(aura->GetModifier()->m_amount), apply);
		}
	}

	void MercenaryPet::_ApplyWeaponDependentAuraDamageMod(Item* item, WeaponAttackType attackType, Aura* aura, bool apply)
	{
		// ignore spell mods for not wands
		Modifier const* modifier = aura->GetModifier();
		if ((modifier->m_miscvalue & SPELL_SCHOOL_MASK_NORMAL) == 0 && (getClassMask() & CLASSMASK_WAND_USERS) == 0)
			return;

		// generic not weapon specific case processes in aura code
		if (aura->GetSpellProto()->EquippedItemClass == -1)
			return;

		UnitMods unitMod = UNIT_MOD_END;
		switch (attackType)
		{
		case BASE_ATTACK:   unitMod = UNIT_MOD_DAMAGE_MAINHAND; break;
		case OFF_ATTACK:    unitMod = UNIT_MOD_DAMAGE_OFFHAND;  break;
		case RANGED_ATTACK: unitMod = UNIT_MOD_DAMAGE_RANGED;   break;
		default: return;
		}

		UnitModifierType unitModType = TOTAL_VALUE;
		switch (modifier->m_auraname)
		{
		case SPELL_AURA_MOD_DAMAGE_DONE:         unitModType = TOTAL_VALUE; break;
		case SPELL_AURA_MOD_DAMAGE_PERCENT_DONE: unitModType = TOTAL_PCT;   break;
		default: return;
		}

		if (item->IsFitToSpellRequirements(aura->GetSpellProto()))
		{
			HandleStatModifier(unitMod, unitModType, float(modifier->m_amount), apply);
		}
	}
	bool MercenaryPet::CheckAmmoCompatibility(const ItemPrototype* ammo_proto) 
	{
		if (!ammo_proto)
			return false;

		// check ranged weapon
		Item* weapon = GetWeaponForAttack(WeaponAttackType(RANGED_ATTACK), true, false);
		if (!weapon)
			return false;

		ItemPrototype const* weapon_proto = weapon->GetProto();
		if (!weapon_proto || weapon_proto->Class != ITEM_CLASS_WEAPON)
			return false;

		// check ammo ws. weapon compatibility
		switch (weapon_proto->SubClass)
		{
		case ITEM_SUBCLASS_WEAPON_BOW:
		case ITEM_SUBCLASS_WEAPON_CROSSBOW:
			if (ammo_proto->SubClass != ITEM_SUBCLASS_ARROW)
				return false;
			break;
		case ITEM_SUBCLASS_WEAPON_GUN:
			if (ammo_proto->SubClass != ITEM_SUBCLASS_BULLET)
				return false;
			break;
		default:
			return false;
		}

		return true;
	}

	Item* MercenaryPet::GetItemByPos(uint16 pos) 
	{
		Unit* unit = GetOwner();
		if (unit == nullptr)
			return nullptr;


		Mercenary* mercenary = getMercenary();
		if (mercenary == nullptr)
			return nullptr;

		return mercenary->GetItemByGuid(mercenary->gearContainer[pos].itemguid);

		//uint8 bag = pos >> 8;
		//uint8 slot = pos & 255;
		//return GetItemByPos(bag, slot);
	}

	// slot to be excluded while counting
	bool MercenaryPet::EnchantmentFitsRequirements(uint32 enchantmentcondition, int8 slot)
	{
		if (!enchantmentcondition)
			return true;

		SpellItemEnchantmentConditionEntry const* Condition = sSpellItemEnchantmentConditionStore.LookupEntry(enchantmentcondition);

		if (!Condition)
			return true;

		uint8 curcount[4] = { 0, 0, 0, 0 };

		// counting current equipped gem colors
		for (uint8 i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_END; ++i)
		{
			if (i == slot)
				continue;
			Item* pItem2 = GetItemByPos(i);
			if (pItem2 && !pItem2->IsBroken() && pItem2->GetProto()->Socket[0].Color)
			{
				for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + 3; ++enchant_slot)
				{
					uint32 enchant_id = pItem2->GetEnchantmentId(EnchantmentSlot(enchant_slot));
					if (!enchant_id)
						continue;

					SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
					if (!enchantEntry)
						continue;

					uint32 gemid = enchantEntry->GemID;
					if (!gemid)
						continue;

					ItemPrototype const* gemProto = sItemStorage.LookupEntry<ItemPrototype>(gemid);
					if (!gemProto)
						continue;

					GemPropertiesEntry const* gemProperty = sGemPropertiesStore.LookupEntry(gemProto->GemProperties);
					if (!gemProperty)
						continue;

					uint8 GemColor = gemProperty->color;

					for (uint8 b = 0, tmpcolormask = 1; b < 4; ++b, tmpcolormask <<= 1)
					{
						if (tmpcolormask & GemColor)
							++curcount[b];
					}
				}
			}
		}

		bool activate = true;

		for (int i = 0; i < 5; ++i)
		{
			if (!Condition->Color[i])
				continue;

			uint32 _cur_gem = curcount[Condition->Color[i] - 1];

			// if have <CompareColor> use them as count, else use <value> from Condition
			uint32 _cmp_gem = Condition->CompareColor[i] ? curcount[Condition->CompareColor[i] - 1] : Condition->Value[i];

			switch (Condition->Comparator[i])
			{
			case 2:                                         // requires less <color> than (<value> || <comparecolor>) gems
				activate &= (_cur_gem < _cmp_gem) ? true : false;
				break;
			case 3:                                         // requires more <color> than (<value> || <comparecolor>) gems
				activate &= (_cur_gem > _cmp_gem) ? true : false;
				break;
			case 5:                                         // requires at least <color> than (<value> || <comparecolor>) gems
				activate &= (_cur_gem >= _cmp_gem) ? true : false;
				break;
			}
		}

		DEBUG_LOG("Checking Condition %u, there are %u Meta Gems, %u Red Gems, %u Yellow Gems and %u Blue Gems, Activate:%s", enchantmentcondition, curcount[0], curcount[1], curcount[2], curcount[3], activate ? "yes" : "no");

		return activate;
	}

	void MercenaryPet::AddEnchantmentDurations(Item* item)
	{
		for (int x = 0; x < MAX_ENCHANTMENT_SLOT; ++x)
		{
			if (!item->GetEnchantmentId(EnchantmentSlot(x)))
				continue;

			uint32 duration = item->GetEnchantmentDuration(EnchantmentSlot(x));
			if (duration > 0)
				AddEnchantmentDuration(item, EnchantmentSlot(x), duration);
		}
	}

	void MercenaryPet::RemoveEnchantmentDurations(Item* item)
	{
		for (EnchantDurationList::iterator itr = m_enchantDuration.begin(); itr != m_enchantDuration.end();)
		{
			if (itr->item == item)
			{
				// save duration in item
				item->SetEnchantmentDuration(EnchantmentSlot(itr->slot), itr->leftduration);
				itr = m_enchantDuration.erase(itr);
			}
			else
				++itr;
		}
	}

	void MercenaryPet::RemoveAllEnchantments(EnchantmentSlot slot)
	{
		// remove enchantments from equipped items first to clean up the m_enchantDuration list
		for (EnchantDurationList::iterator itr = m_enchantDuration.begin(), next; itr != m_enchantDuration.end(); itr = next)
		{
			next = itr;
			if (itr->slot == slot)
			{
				if (itr->item && itr->item->GetEnchantmentId(slot))
				{
					// remove from stats
					ApplyEnchantment(itr->item, slot, false, false);
					// remove visual
					itr->item->ClearEnchantment(slot);
				}
				// remove from update list
				next = m_enchantDuration.erase(itr);
			}
			else
				++next;
		}

		// remove enchants from inventory items
		// NOTE: no need to remove these from stats, since these aren't equipped
		// in inventory
		Mercenary *mercenary = getMercenary();
		for (auto itr = mercenary->gearContainer.begin(); itr != mercenary->gearContainer.end(); itr++){
			Item* pItem = mercenary->GetItemByGuid(itr->second.itemguid);

			if (pItem&&pItem->GetEnchantmentId(slot))
				pItem->ClearEnchantment(slot);

		}
	}

	// duration == 0 will remove item enchant
	void MercenaryPet::AddEnchantmentDuration(Item* item, EnchantmentSlot slot, uint32 duration)
	{
		if (!item)
			return;

		if (slot >= MAX_ENCHANTMENT_SLOT)
			return;

		for (EnchantDurationList::iterator itr = m_enchantDuration.begin(); itr != m_enchantDuration.end(); ++itr)
		{
			if (itr->item == item && itr->slot == slot)
			{
				itr->item->SetEnchantmentDuration(itr->slot, itr->leftduration);
				m_enchantDuration.erase(itr);
				break;
			}
		}
		if (item && duration > 0)
		{
			getPlayer()->GetSession()->SendItemEnchantTimeUpdate(GetObjectGuid(), item->GetObjectGuid(), slot, uint32(duration / 1000));
			m_enchantDuration.push_back(EnchantDuration(item, slot, duration));
		}
	}

	// This functions sets a skill line value (and adds if doesn't exist yet)``
	// To "remove" a skill line, set it's values to zero
	void MercenaryPet::SetSkill(uint16 id, uint16 currVal, uint16 maxVal, uint16 step /*=0*/)
	{
		if (!id)
			return;

		SkillStatusMap::iterator itr = mSkillStatus.find(id);

		// has skill
		if (itr != mSkillStatus.end() && itr->second.uState != SKILL_DELETED)
		{
			SkillStatusData& skillStatus = itr->second;
			if (currVal)
			{
				if (step)                                      // need update step
					SetUInt32Value(PLAYER_SKILL_INDEX(skillStatus.pos), MAKE_PAIR32(id, step));

				// update value
				SetUInt32Value(PLAYER_SKILL_VALUE_INDEX(skillStatus.pos), MAKE_SKILL_VALUE(currVal, maxVal));
				if (skillStatus.uState != SKILL_NEW)
					skillStatus.uState = SKILL_CHANGED;

				//learnSkillRewardedSpells(id, currVal);
			}
			else                                                // remove
			{
				// clear skill fields
				SetUInt32Value(PLAYER_SKILL_INDEX(skillStatus.pos), 0);
				SetUInt32Value(PLAYER_SKILL_VALUE_INDEX(skillStatus.pos), 0);
				SetUInt32Value(PLAYER_SKILL_BONUS_INDEX(skillStatus.pos), 0);

				// mark as deleted or simply remove from map if not saved yet
				if (skillStatus.uState != SKILL_NEW)
					skillStatus.uState = SKILL_DELETED;
				else
					mSkillStatus.unsafe_erase(itr);

				// remove all spells that related to this skill
				for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
				if (SkillLineAbilityEntry const* pAbility = sSkillLineAbilityStore.LookupEntry(j))
				if (pAbility->skillId == id)
					removeSpell(sSpellMgr.GetFirstSpellInChain(pAbility->spellId),false,true);
			}
		}
		else if (currVal)                                       // add
		{
			for (int i = 0; i < PLAYER_MAX_SKILLS; ++i)
			{
				if (!GetUInt32Value(PLAYER_SKILL_INDEX(i)))
				{
					SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(id);
					if (!pSkill)
					{
						sLog.outError("Skill not found in SkillLineStore: skill #%u", id);
						return;
					}

					SetUInt32Value(PLAYER_SKILL_INDEX(i), MAKE_PAIR32(id, step));
					SetUInt32Value(PLAYER_SKILL_VALUE_INDEX(i), MAKE_SKILL_VALUE(currVal, maxVal));

					// insert new entry or update if not deleted old entry yet
					if (itr != mSkillStatus.end())
					{
						itr->second.pos = i;
						itr->second.uState = SKILL_CHANGED;
					}
					else
						mSkillStatus.insert(SkillStatusMap::value_type(id, SkillStatusData(i, SKILL_NEW)));

					// apply skill bonuses
					SetUInt32Value(PLAYER_SKILL_BONUS_INDEX(i), 0);

					// temporary bonuses
					AuraList const& mModSkill = GetAurasByType(SPELL_AURA_MOD_SKILL);
					for (AuraList::const_iterator j = mModSkill.begin(); j != mModSkill.end(); ++j)
					if ((*j)->GetModifier()->m_miscvalue == int32(id))
						(*j)->ApplyModifier(true);

					// permanent bonuses
					AuraList const& mModSkillTalent = GetAurasByType(SPELL_AURA_MOD_SKILL_TALENT);
					for (AuraList::const_iterator j = mModSkillTalent.begin(); j != mModSkillTalent.end(); ++j)
					if ((*j)->GetModifier()->m_miscvalue == int32(id))
						(*j)->ApplyModifier(true);

					// Learn all spells for skill
					//learnSkillRewardedSpells(id, currVal);
					return;
				}
			}
		}
	}
	//////////////////////////初始化属性///////////////////////
	//                                                      //
	/////////////////////////////////////////////////////////
	void MercenaryPet::InitStatsForLevelPlayer(bool reapplyMods)
	{
		/*初始化开始*/
		Mercenary * mercenary = getMercenary();
		Player * player = getPlayer();

		uint32 level = player->getLevel();
		uint8 petClass = mercenary->GetType();
		uint8 race = mercenary->GetRace();

		SetLevel(level);
		SetByteValue(UNIT_FIELD_BYTES_0, 0, race);
		SetByteValue(UNIT_FIELD_BYTES_0, 1, petClass);



		/*装备更新*/
		for (int i = 0; i < PLAYER_SLOTS_COUNT; i++)
			m_items[i] = nullptr;

		for (auto itr = mercenary->gearContainer.begin(); itr != mercenary->gearContainer.end(); itr++){
			Item* item = mercenary->GetItemByGuid(itr->second.itemguid);
			m_items[itr->first] = (item != nullptr) ? item : nullptr;
		}


		if (reapplyMods)                                        // reapply stats values only on .reset stats (level) command
			_RemoveAllStatBonuses();


		PlayerClassLevelInfo classInfo;
		sObjectMgr.GetPlayerClassLevelInfo(petClass, level, &classInfo);

		PlayerLevelInfo info;
		sObjectMgr.GetPlayerLevelInfo(race, petClass, level, &info);

		SetUInt32Value(PLAYER_FIELD_MAX_LEVEL, sWorld.getConfig(CONFIG_UINT32_MAX_PLAYER_LEVEL));
		SetUInt32Value(PLAYER_NEXT_LEVEL_XP, sObjectMgr.GetXPForLevel(level));

		// reset before any aura state sources (health set/aura apply)
		SetUInt32Value(UNIT_FIELD_AURASTATE, 0);

		UpdateSkillsForLevel();

		// set default cast time multiplier
		SetFloatValue(UNIT_MOD_CAST_SPEED, 1.0f);

		// save base values (bonuses already included in stored stats
		for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
			SetCreateStat(Stats(i), info.stats[i]);

		for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
			SetStat(Stats(i), info.stats[i]);

		SetCreateHealth(classInfo.basehealth);

		// set create powers
		SetCreateMana(classInfo.basemana);

		SetArmor(int32(m_createStats[STAT_AGILITY] * 2));

		InitStatBuffMods();

		// reset rating fields values
		for (uint16 index = PLAYER_FIELD_COMBAT_RATING_1; index < PLAYER_FIELD_COMBAT_RATING_1 + MAX_COMBAT_RATING; ++index)
			SetUInt32Value(index, 0);

		SetUInt32Value(PLAYER_FIELD_MOD_HEALING_DONE_POS, 0);
		for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
		{
			SetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_NEG + i, 0);
			SetUInt32Value(PLAYER_FIELD_MOD_DAMAGE_DONE_POS + i, 0);
			SetFloatValue(PLAYER_FIELD_MOD_DAMAGE_DONE_PCT + i, 1.00f);
		}

		// reset attack power, damage and attack speed fields
		SetFloatValue(UNIT_FIELD_BASEATTACKTIME, 2000.0f);
		SetFloatValue(UNIT_FIELD_BASEATTACKTIME + 1, 2000.0f);  // offhand attack time
		SetFloatValue(UNIT_FIELD_RANGEDATTACKTIME, 2000.0f);

		SetFloatValue(UNIT_FIELD_MINDAMAGE, 0.0f);
		SetFloatValue(UNIT_FIELD_MAXDAMAGE, 0.0f);
		SetFloatValue(UNIT_FIELD_MINOFFHANDDAMAGE, 0.0f);
		SetFloatValue(UNIT_FIELD_MAXOFFHANDDAMAGE, 0.0f);
		SetFloatValue(UNIT_FIELD_MINRANGEDDAMAGE, 0.0f);
		SetFloatValue(UNIT_FIELD_MAXRANGEDDAMAGE, 0.0f);

		SetInt32Value(UNIT_FIELD_ATTACK_POWER, 0);
		SetInt32Value(UNIT_FIELD_ATTACK_POWER_MODS, 0);
		SetFloatValue(UNIT_FIELD_ATTACK_POWER_MULTIPLIER, 0.0f);
		SetInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER, 0);
		SetInt32Value(UNIT_FIELD_RANGED_ATTACK_POWER_MODS, 0);
		SetFloatValue(UNIT_FIELD_RANGED_ATTACK_POWER_MULTIPLIER, 0.0f);

		// Base crit values (will be recalculated in UpdateAllStats() at loading and in _ApplyAllStatBonuses() at reset
		SetFloatValue(PLAYER_CRIT_PERCENTAGE, 0.0f);
		SetFloatValue(PLAYER_OFFHAND_CRIT_PERCENTAGE, 0.0f);
		SetFloatValue(PLAYER_RANGED_CRIT_PERCENTAGE, 0.0f);

		// Init spell schools (will be recalculated in UpdateAllStats() at loading and in _ApplyAllStatBonuses() at reset
		for (uint8 i = 0; i < MAX_SPELL_SCHOOL; ++i)
			SetFloatValue(PLAYER_SPELL_CRIT_PERCENTAGE1 + i, 0.0f);

		SetFloatValue(PLAYER_PARRY_PERCENTAGE, 0.0f);
		SetFloatValue(PLAYER_BLOCK_PERCENTAGE, 0.0f);
		SetUInt32Value(PLAYER_SHIELD_BLOCK, 0);

		// Dodge percentage
		SetFloatValue(PLAYER_DODGE_PERCENTAGE, 0.0f);

		// set armor (resistance 0) to original value (create_agility*2)
		SetArmor(int32(m_createStats[STAT_AGILITY] * 2));
		SetResistanceBuffMods(SpellSchools(0), true, 0.0f);
		SetResistanceBuffMods(SpellSchools(0), false, 0.0f);
		// set other resistance to original value (0)
		for (int i = 1; i < MAX_SPELL_SCHOOL; ++i)
		{
			SetResistance(SpellSchools(i), 0);
			SetResistanceBuffMods(SpellSchools(i), true, 0.0f);
			SetResistanceBuffMods(SpellSchools(i), false, 0.0f);
		}

		SetUInt32Value(PLAYER_FIELD_MOD_TARGET_RESISTANCE, 0);
		SetUInt32Value(PLAYER_FIELD_MOD_TARGET_PHYSICAL_RESISTANCE, 0);
		for (int i = 0; i < MAX_SPELL_SCHOOL; ++i)
		{
			SetUInt32Value(UNIT_FIELD_POWER_COST_MODIFIER + i, 0);
			SetFloatValue(UNIT_FIELD_POWER_COST_MULTIPLIER + i, 0.0f);
		}
		// Reset no reagent cost field
		for (int i = 0; i < 3; ++i)
			SetUInt32Value(PLAYER_NO_REAGENT_COST_1 + i, 0);
		// Init data for form but skip reapply item mods for form
		InitDataForForm(reapplyMods);

		// save new stats
		for (int i = POWER_MANA; i < MAX_POWERS; ++i)
			SetMaxPower(Powers(i), GetCreatePowers(Powers(i)));

		SetMaxHealth(classInfo.basehealth);                     // stamina bonus will applied later

		// cleanup mounted state (it will set correctly at aura loading if player saved at mount.
		SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 0);

		// cleanup unit flags (will be re-applied if need at aura load).
		RemoveFlag(UNIT_FIELD_FLAGS,
			UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_ATTACKABLE_1 |
			UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_PASSIVE | UNIT_FLAG_LOOTING |
			UNIT_FLAG_PET_IN_COMBAT | UNIT_FLAG_SILENCED | UNIT_FLAG_PACIFIED |
			UNIT_FLAG_STUNNED | UNIT_FLAG_IN_COMBAT | UNIT_FLAG_DISARMED |
			UNIT_FLAG_CONFUSED | UNIT_FLAG_FLEEING | UNIT_FLAG_NOT_SELECTABLE |
			UNIT_FLAG_SKINNABLE | UNIT_FLAG_MOUNT | UNIT_FLAG_TAXI_FLIGHT);
		SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PVP_ATTACKABLE);    // must be set

		SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER); // must be set

		// cleanup player flags (will be re-applied if need at aura load), to avoid have ghost flag without ghost aura, for example.
		RemoveFlag(PLAYER_FLAGS, PLAYER_FLAGS_AFK | PLAYER_FLAGS_DND | PLAYER_FLAGS_GM | PLAYER_FLAGS_GHOST);

		RemoveStandFlags(UNIT_STAND_FLAGS_ALL);                 // one form stealth modified bytes
		RemoveByteFlag(UNIT_FIELD_BYTES_2, 1, UNIT_BYTE2_FLAG_FFA_PVP | UNIT_BYTE2_FLAG_SANCTUARY);

		// restore if need some important flags
		SetUInt32Value(PLAYER_FIELD_BYTES2, 0);                 // flags empty by default

		if (reapplyMods)                                        // reapply stats values only on .reset stats (level) command
			_ApplyAllStatBonuses();

		// set current level health and mana/energy to maximum after applying all mods.
		SetHealth(GetMaxHealth());
		SetPower(POWER_MANA, GetMaxPower(POWER_MANA));
		SetPower(POWER_ENERGY, GetMaxPower(POWER_ENERGY));
		if (GetPower(POWER_RAGE) > GetMaxPower(POWER_RAGE))
			SetPower(POWER_RAGE, GetMaxPower(POWER_RAGE));
		SetPower(POWER_FOCUS, 0);
		SetPower(POWER_HAPPINESS, 0);
		SetPower(POWER_RUNIC_POWER, 0);

	}
	void MercenaryPet::UpdateSkillsForLevel()
	{
		uint16 maxconfskill = sWorld.GetConfigMaxSkillValue();
		uint32 maxSkill = GetMaxSkillValueForLevel();

		//bool alwaysMaxSkill = sWorld.getConfig(CONFIG_BOOL_ALWAYS_MAX_SKILL_FOR_LEVEL);
		bool alwaysMaxSkill = true; /*雇佣兵总是最大技能*/

		for (SkillStatusMap::iterator itr = mSkillStatus.begin(); itr != mSkillStatus.end(); ++itr)
		{
			SkillStatusData& skillStatus = itr->second;
			if (skillStatus.uState == SKILL_DELETED)
				continue;

			uint32 pskill = itr->first;

			SkillLineEntry const* pSkill = sSkillLineStore.LookupEntry(pskill);
			if (!pSkill)
				continue;

			if (GetSkillRangeType(pSkill, false) != SKILL_RANGE_LEVEL)
				continue;

			uint32 valueIndex = PLAYER_SKILL_VALUE_INDEX(skillStatus.pos);
			uint32 data = GetUInt32Value(valueIndex);
			uint32 max = SKILL_MAX(data);
			uint32 val = SKILL_VALUE(data);

			/// update only level dependent max skill values
			if (max != 1)
			{
				/// maximize skill always
				if (alwaysMaxSkill)
				{
					SetUInt32Value(valueIndex, MAKE_SKILL_VALUE(maxSkill, maxSkill));
					if (skillStatus.uState != SKILL_NEW)
						skillStatus.uState = SKILL_CHANGED;					
				}
				else if (max != maxconfskill)                   /// update max skill value if current max skill not maximized
				{
					SetUInt32Value(valueIndex, MAKE_SKILL_VALUE(val, maxSkill));
					if (skillStatus.uState != SKILL_NEW)
						skillStatus.uState = SKILL_CHANGED;
				}
			}
		}
	}

	void MercenaryPet::InitDataForForm(bool reapplyMods)
	{
		ShapeshiftForm form = GetShapeshiftForm();

		SpellShapeshiftFormEntry const* ssEntry = sSpellShapeshiftFormStore.LookupEntry(form);
		if (ssEntry && ssEntry->attackSpeed)
		{
			SetAttackTime(BASE_ATTACK, ssEntry->attackSpeed);
			SetAttackTime(OFF_ATTACK, ssEntry->attackSpeed);
			SetAttackTime(RANGED_ATTACK, BASE_ATTACK_TIME);
		}
		else
			SetRegularAttackTime();

		switch (form)
		{
		case FORM_CAT:
		{
						 if (GetPowerType() != POWER_ENERGY)
							 SetPowerType(POWER_ENERGY);
						 break;
		}
		case FORM_BEAR:
		case FORM_DIREBEAR:
		{
							  if (GetPowerType() != POWER_RAGE)
								  SetPowerType(POWER_RAGE);
							  break;
		}
		default:                                            // 0, for example
		{
																ChrClassesEntry const* cEntry = sChrClassesStore.LookupEntry(getClass());
																if (cEntry && cEntry->powerType < MAX_POWERS && uint32(GetPowerType()) != cEntry->powerType)
																	SetPowerType(Powers(cEntry->powerType));
																break;
		}
		}

		// update auras at form change, ignore this at mods reapply (.reset stats/etc) when form not change.
		if (!reapplyMods)
			UpdateEquipSpellsAtFormChange();

		UpdateAttackPowerAndDamage();
		UpdateAttackPowerAndDamage(true);
	}
	void MercenaryPet::SetRegularAttackTime()
	{
		for (int i = 0; i < MAX_ATTACK; ++i)
		{
			Item* tmpitem = GetWeaponForAttack(WeaponAttackType(i), true, false);
			if (tmpitem)
			{
				ItemPrototype const* proto = tmpitem->GetProto();
				if (proto->Delay)
					SetAttackTime(WeaponAttackType(i), proto->Delay);
				else
					SetAttackTime(WeaponAttackType(i), BASE_ATTACK_TIME);
			}
		}
	}

	void MercenaryPet::UpdateEquipSpellsAtFormChange()
	{
		for (int i = 0; i < INVENTORY_SLOT_BAG_END; ++i)
		{
			if (m_items[i] && !m_items[i]->IsBroken())
			{
				ApplyItemEquipSpell(m_items[i], false, true);   // remove spells that not fit to form
				ApplyItemEquipSpell(m_items[i], true, true);    // add spells that fit form but not active
			}
		}		
	}
	//---------------------------------------------------------//

	void MercenaryPet::_ApplyItemMods(Item* item, uint8 slot, bool apply)
	{
		if (slot >= INVENTORY_SLOT_BAG_END || !item)
			return;

		// not apply/remove mods for broken item
		if (item->IsBroken())
			return;

		ItemPrototype const* proto = item->GetProto();

		if (!proto)
			return;

		DETAIL_LOG("applying mods for item %u ", item->GetGUIDLow());

		uint32 attacktype = Player::GetAttackBySlot(slot);
		if (attacktype < MAX_ATTACK)
			_ApplyWeaponDependentAuraMods(item, WeaponAttackType(attacktype), apply);

		_ApplyItemBonuses(proto, slot, apply);

		if (slot == EQUIPMENT_SLOT_RANGED)
			_ApplyAmmoBonuses();

		ApplyItemEquipSpell(item, apply);
		ApplyEnchantment(item, apply);

		if (proto->Socket[0].Color)                             // only (un)equipping of items with sockets can influence metagems, so no need to waste time with normal items
			CorrectMetaGemEnchants(slot, apply);

		DEBUG_LOG("_ApplyItemMods complete.");
	}

	void MercenaryPet::CorrectMetaGemEnchants(uint8 exceptslot, bool apply)
	{
		Mercenary * mercenary = getMercenary();
		// cycle all equipped items
		for (auto itr = mercenary->gearContainer.begin(); itr != mercenary->gearContainer.end(); itr++){
			// enchants for the slot being socketed are handled by Player::ApplyItemMods
			if (itr->first == exceptslot)
				continue;

			Item* pItem = mercenary->GetItemByGuid(itr->second.itemguid);

			if (!pItem || !pItem->GetProto()->Socket[0].Color)
				continue;

			for (uint32 enchant_slot = SOCK_ENCHANTMENT_SLOT; enchant_slot < SOCK_ENCHANTMENT_SLOT + 3; ++enchant_slot)
			{
				uint32 enchant_id = pItem->GetEnchantmentId(EnchantmentSlot(enchant_slot));
				if (!enchant_id)
					continue;

				SpellItemEnchantmentEntry const* enchantEntry = sSpellItemEnchantmentStore.LookupEntry(enchant_id);
				if (!enchantEntry)
					continue;

				uint32 condition = enchantEntry->EnchantmentCondition;
				if (condition)
				{
					// was enchant active with/without item?
					bool wasactive = EnchantmentFitsRequirements(condition, apply ? exceptslot : -1);
					// should it now be?
					if (wasactive != EnchantmentFitsRequirements(condition, apply ? -1 : exceptslot))
					{
						// ignore item gem conditions
						// if state changed, (dis)apply enchant
						ApplyEnchantment(pItem, EnchantmentSlot(enchant_slot), !wasactive, true, true);
					}
				}
			}
		}
	}

	Item* MercenaryPet::EquipItem(uint16 slot, Item* pItem, bool update)
	{
		switch (slot)
		{
			case SLOT_MAIN_HAND:
				SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, pItem->GetEntry());
				break;
			case SLOT_OFF_HAND:
				SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, pItem->GetEntry());/*副手有可能被移除*/
				break;
			case SLOT_RANGED:
				SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, pItem->GetEntry());
		}

		m_items[slot] = pItem;

		AddEnchantmentDurations(pItem);
		
		ItemPrototype const* pProto = pItem->GetProto();

		_ApplyItemMods(pItem, slot, true);

		ApplyItemOnStoreSpell(pItem, true);//应用物品带的法术技能


		if (slot == EQUIPMENT_SLOT_MAINHAND)
		{
			UpdateExpertise(BASE_ATTACK);//更新基础攻击
			UpdateArmorPenetration();//更新护甲穿透能力
		}
		else if (slot == EQUIPMENT_SLOT_OFFHAND)
		{
			UpdateExpertise(OFF_ATTACK);//更新副手攻击
			UpdateArmorPenetration();//更新护甲穿透能力
		}

		return pItem;
	}

	void MercenaryPet::ApplyItemOnStoreSpell(Item* item, bool apply)
	{
		if (!item)
			return;

		ItemPrototype const* proto = item->GetProto();
		if (!proto)
			return;

		for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
		{
			_Spell const& spellData = proto->Spells[i];

			// no spell
			if (!spellData.SpellId)
				continue;

			// apply/unapply only at-store spells
			if (spellData.SpellTrigger != ITEM_SPELLTRIGGER_ON_STORE)
				continue;

			if (apply)
			{
				// can be attempt re-applied at move in inventory slots
				if (!HasAura(spellData.SpellId))
					CastSpell(this, spellData.SpellId, true, item);
			}
			else
				RemoveAurasDueToItemSpell(item, spellData.SpellId);
		}
	}
	void MercenaryPet::AutoUnequipOffhandIfNeed()
	{
		Mercenary *mercenary = getMercenary();
		uint32 offhandItemguid=mercenary->gearContainer[EQUIPMENT_SLOT_OFFHAND].itemguid;
		if (offhandItemguid == 0)
			return;

		Item* offItem = mercenary->GetItemByGuid(offhandItemguid);
		if (offItem == nullptr)
			return;

		bool IsTwoHandUsed = false;
		GearEntry& gearEntry=mercenary->gearContainer[EQUIPMENT_SLOT_MAINHAND];
		if (gearEntry.itemid>0)
		{
			Item* mainItem = mercenary->GetItemByGuid(gearEntry.itemguid);
			if (mainItem && mainItem->GetProto()->InventoryType == INVTYPE_2HWEAPON)
				IsTwoHandUsed = true;
		}
		// need unequip offhand for 2h-weapon without TitanGrip (in any from hands)
		ItemPrototype const* itemProto = offItem->GetProto();
		if ((itemProto->InventoryType == INVTYPE_SHIELD || itemProto->InventoryType == INVTYPE_HOLDABLE) &&
			((itemProto->InventoryType != INVTYPE_2HWEAPON && !IsTwoHandUsed)))
			return;
		
		ChatHandler(getPlayer()).PSendSysMessage(getPlayer()->GetMangosString(-2800653));//系统提示：因主手装备不匹配(如长柄武器)，雇佣兵副手装备被自动移除。
		RemoveItem(EQUIPMENT_SLOT_OFFHAND, true); //需调用此方法移除

	}


	void MercenaryPet::RemoveItem(uint8 slot, bool update)
	{
		uint16 blankPos = getPlayer()->findEmptyPos();
		if (blankPos == 0)
		{
			ChatHandler(getPlayer()).SendSysMessage(-2800700);
			return;
		}
		switch (slot)
		{
			case SLOT_MAIN_HAND:
				SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
				break;
			case SLOT_OFF_HAND:
				SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, 0);
				break;
			case SLOT_RANGED:
				SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, 0);
		}

		m_items[slot] = nullptr;

		Mercenary *mercenary = getMercenary();

		if (mercenary->gearContainer[slot].itemguid == 0)
			return;

		Item* pItem = mercenary->GetItemByGuid(mercenary->gearContainer[slot].itemguid);//取得物品
		if (!pItem)
			return;

		

		getPlayer()->SwapItem((uint16(INVENTORY_SLOT_BAG_0) << 8) | (M_EQUIPMENT_SLOT_START + slot), blankPos);//交换到雇佣兵的对应装备位置
		//getPlayer()->VisualizeItem(blankPos & 255, pItem);
		//pItem->AddToUpdateQueueOf(getPlayer());
		pItem->AddToClientUpdateList();



		mercenary->gearContainer[slot].itemid = 0;//删除容器中记录
		mercenary->gearContainer[slot].itemguid = 0;

		if (pItem == nullptr)//如果找不到物品，只能全面刷新属性
		{
			UpdateAllStats();
			return;
		}
		// note: removeitem does not actually change the item
		// it only takes the item out of storage temporarily
		// note2: if removeitem is to be used for delinking
		// the item must be removed from the player's updatequeue


		RemoveEnchantmentDurations(pItem);

		ItemPrototype const* pProto = pItem->GetProto();
		// item set bonuses applied only at equip and removed at unequip, and still active for broken items

		
		_ApplyItemMods(pItem, slot, false);

		// remove item dependent auras and casts (only weapon and armor slots)
		RemoveItemDependentAurasAndCasts(pItem);//移除物品带的法术技能

		// remove held enchantments, update expertise
		if (slot == EQUIPMENT_SLOT_MAINHAND)
		{
			if (pItem->GetItemSuffixFactor())
			{
				pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_3);
				pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_4);
			}
			else
			{
				pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_0);
				pItem->ClearEnchantment(PROP_ENCHANTMENT_SLOT_1);
			}

			UpdateExpertise(BASE_ATTACK);//更新基础攻击
			UpdateArmorPenetration();//更新护甲穿透能力
		}
		else if (slot == EQUIPMENT_SLOT_OFFHAND)
		{
			UpdateExpertise(OFF_ATTACK);//更新副手攻击
			UpdateArmorPenetration();//更新护甲穿透能力
		}
	
	}


	void MercenaryPet::RemoveItemDependentAurasAndCasts(Item* pItem)
	{
		SpellAuraHolderMap& auras = GetSpellAuraHolderMap();
		for (SpellAuraHolderMap::const_iterator itr = auras.begin(); itr != auras.end();)
		{
			SpellAuraHolder* holder = itr->second;

			// skip passive (passive item dependent spells work in another way) and not self applied auras
			SpellEntry const* spellInfo = holder->GetSpellProto();
			if (holder->IsPassive() || holder->GetCasterGuid() != GetObjectGuid())
			{
				++itr;
				continue;
			}

			// skip if not item dependent or have alternative item
			if (HasItemFitToSpellReqirements(spellInfo, pItem))
			{
				++itr;
				continue;
			}

			// no alt item, remove aura, restart check
			RemoveAurasDueToSpell(holder->GetId());
			itr = auras.begin();
		}

		// currently casted spells can be dependent from item
		for (uint32 i = 0; i < CURRENT_MAX_SPELL; ++i)
		if (Spell* spell = GetCurrentSpell(CurrentSpellTypes(i)))
		if (spell->getState() != SPELL_STATE_DELAYED && !HasItemFitToSpellReqirements(spell->m_spellInfo, pItem))
			InterruptSpell(CurrentSpellTypes(i));
	}

	bool MercenaryPet::HasItemFitToSpellReqirements(SpellEntry const* spellInfo, Item const* ignoreItem)
	{
		if (spellInfo->EquippedItemClass < 0)
			return true;

		Mercenary *mercenary = getMercenary();

		// scan other equipped items for same requirements (mostly 2 daggers/etc)
		// for optimize check 2 used cases only
		switch (spellInfo->EquippedItemClass)
		{
		case ITEM_CLASS_WEAPON:
		{
								  for (int i = EQUIPMENT_SLOT_MAINHAND; i < EQUIPMENT_SLOT_TABARD; ++i)
								  {
									  if (Item* item = mercenary->GetItemByGuid(mercenary->gearContainer[i].itemguid))
									  if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
										  return true;
								  }
								  break;
		}
		case ITEM_CLASS_ARMOR:
		{
								 // tabard not have dependent spells
								 for (int i = EQUIPMENT_SLOT_START; i < EQUIPMENT_SLOT_MAINHAND; ++i)
								 {
									 if (Item* item = mercenary->GetItemByGuid(mercenary->gearContainer[i].itemguid))
									 if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
										 return true;
								 }

								 // shields can be equipped to offhand slot
								 {
									 if (Item* item = mercenary->GetItemByGuid(mercenary->gearContainer[EQUIPMENT_SLOT_OFFHAND].itemguid))
									 if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
										 return true;
								 }
								 // ranged slot can have some armor subclasses
								 if (Item* item = mercenary->GetItemByGuid(mercenary->gearContainer[EQUIPMENT_SLOT_RANGED].itemguid))
								 if (item != ignoreItem && item->IsFitToSpellRequirements(spellInfo))
									 return true;

								 break;
		}
		default:
			sLog.outError("HasItemFitToSpellReqirements: Not handled spell requirement for item class %u", spellInfo->EquippedItemClass);
			break;
		}

		return false;
	}
	void MercenaryPet::GiveLevel(uint32 level)
	{
		PlayerLevelInfo info;
		sObjectMgr.GetPlayerLevelInfo(getRace(), getClass(), level, &info);

		PlayerClassLevelInfo classInfo;
		sObjectMgr.GetPlayerClassLevelInfo(getClass(), level, &classInfo);

		SetUInt32Value(PLAYER_NEXT_LEVEL_XP, sObjectMgr.GetXPForLevel(level));

		SetLevel(level);

		UpdateSkillsForLevel();

		// save base values (bonuses already included in stored stats
		for (int i = STAT_STRENGTH; i < MAX_STATS; ++i)
			SetCreateStat(Stats(i), info.stats[i]);

		SetCreateHealth(classInfo.basehealth);
		SetCreateMana(classInfo.basemana);

		InitTalentForLevel();

		UpdateAllStats();

		// set current level health and mana/energy to maximum after applying all mods.
		if (isAlive())
			SetHealth(GetMaxHealth());
		SetPower(POWER_MANA, GetMaxPower(POWER_MANA));
		SetPower(POWER_ENERGY, GetMaxPower(POWER_ENERGY));
		if (GetPower(POWER_RAGE) > GetMaxPower(POWER_RAGE))
			SetPower(POWER_RAGE, GetMaxPower(POWER_RAGE));
		SetPower(POWER_FOCUS, 0);
		SetPower(POWER_HAPPINESS, 0);

	}

	void MercenaryPet::SetVirtualItemSlot(uint8 i, Item* item)
	{
		MANGOS_ASSERT(i < 3);
		if (i < 2 && item)
		{
			if (!item->GetEnchantmentId(TEMP_ENCHANTMENT_SLOT))
				return;
			uint32 charges = item->GetEnchantmentCharges(TEMP_ENCHANTMENT_SLOT);
			if (charges == 0)
				return;
			if (charges > 1)
				item->SetEnchantmentCharges(TEMP_ENCHANTMENT_SLOT, charges - 1);
			else if (charges <= 1)
			{
				ApplyEnchantment(item, TEMP_ENCHANTMENT_SLOT, false);
				item->ClearEnchantment(TEMP_ENCHANTMENT_SLOT);
			}
		}
	}

	void MercenaryPet::SetSheath(SheathState sheathed)
	{
		Unit::SetSheath(sheathed);                              // this must visualize Sheath changing for other players...

		if (!isMercenary())
			return;

		switch (sheathed)
		{
		case SHEATH_STATE_UNARMED:                          // no prepared weapon
			SetVirtualItemSlot(0, nullptr);
			SetVirtualItemSlot(1, nullptr);
			SetVirtualItemSlot(2, nullptr);
			break;
		case SHEATH_STATE_MELEE:                            // prepared melee weapon
		{
			SetVirtualItemSlot(0, GetWeaponForAttack(BASE_ATTACK, true, true));
			SetVirtualItemSlot(1, GetWeaponForAttack(OFF_ATTACK, true, true));
			SetVirtualItemSlot(2, nullptr);
		};  break;
		case SHEATH_STATE_RANGED:                           // prepared ranged weapon
			SetVirtualItemSlot(0, nullptr);
			SetVirtualItemSlot(1, nullptr);
			SetVirtualItemSlot(2, GetWeaponForAttack(RANGED_ATTACK, true, true));
			break;
		default:
			SetVirtualItemSlot(0, nullptr);
			SetVirtualItemSlot(1, nullptr);
			SetVirtualItemSlot(2, nullptr);
			break;
		}
		
	}