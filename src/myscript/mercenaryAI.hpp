#ifndef __PR_MERCENARY_AI_H
#define __PR_MERCENARY_AI_H

#include "precompiled.h"
#include "MercenaryMgr.h"
#include "PetAI.h"

struct mercenary_bot_AI : public PetAI
{
	mercenary_bot_AI(Creature* creature) : PetAI(creature) { Reset(); }

	int talkTimer = 0;
	int spellTimer = 0;
	int currentSpell = 0;
	int updatePetMercenaryMirrorTimer = 5000;//5秒检查1次
	bool updatePetMercenaryMirrorCheck = true;
	
	int checkCancelStealthAuraTimer = 5000;//5秒检查1次是否要取消隐身

	int defaultSpellTimer = 5000;
	std::vector<uint32> defautSpells;
	void Reset() //注意，这里有个overide，发现父类没有该方法，Reset何时调用，必须考虑
	{
		talkTimer = urand(35000, 120000);
		lastMessage = "";

		if (Unit* owner = m_creature->GetOwner())
		{
			mercenary = MercenaryUtil::GetMercenaryByOwner(owner->GetGUIDLow());
			if (mercenary)
			{
				if (mercenary->isRangedAttack())
				{
					m_creature->SetSheath(SHEATH_STATE_RANGED);
					m_creature->clearUnitState(UNIT_STAT_MELEE_ATTACKING);
					//m_creature->addUnitState(UNIT_STAT_FLEEING);
					SetCombatMovement(false);

				}
				else
				{
					m_creature->SetSheath(SHEATH_STATE_MELEE);
					m_creature->addUnitState(UNIT_STAT_MELEE_ATTACKING);
					SetCombatMovement(true);
				}
				//初始化将武器暂时去掉，update再添加上
				m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
				m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, 0);
				m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, 0);
				mercenary->SendMirrorImagePacket(m_creature);


				defautSpells.clear();
				//默认光环技能
				if (GroupSpellsMap* groupSpellMap = MercenaryUtil::findGroupSpellsMapByClass(mercenary->GetType(), mercenary->GetRole()))
				{
					uint32 level = owner->getLevel();
					for (auto it = groupSpellMap->begin(); it != groupSpellMap->end(); ++it)
					{
						MercenarySpellGroup* mSpellGroup = it->second;
						int count = 0;
						for (auto itr = mSpellGroup->spellLevelVector.begin(); itr != mSpellGroup->spellLevelVector.end(); itr++, count++)
						if (*itr <= level)
						{
							uint32 spellid=mSpellGroup->spellIdVector.at(count);
							if (MercenarySpell* info = MercenaryUtil::findMercenarySpellsInfoBySpell(spellid))
							{
								if (!info->isActive || !info->isDefaultAura)
									break;//低等级的技能被跳过

								defautSpells.push_back(spellid);

							}
							break;//低等级的技能被跳过
						}
					}
				}
			}



		}
	}
	void caseSpell(Unit* target, uint32 spell, const uint32 uiDiff){
		if (spell == 0)
			return;
		// Cast spell one on our current target.
		if (!m_creature->HasSpellCooldown(spell))
		{
			CanCastResult result=DoCastSpellIfCan(target, spell);
			if (result == CAST_OK)
			{
				m_creature->AddCreatureSpellCooldown(spell);
				spellTimer = 100;//下一次施法的最短间隔，模拟人的反应速度
				debug_log("Mercenary Spell CAST_OK %u.", spell);
			}
			else
			{
				switch (result)
				{
					case	CAST_FAIL_IS_CASTING:
						debug_log("Mercenary Spell CAST_FAIL_IS_CASTING %u.", spell);
						break;
					case		CAST_FAIL_OTHER:
						debug_log("Mercenary Spell CAST_FAIL_OTHER %u.", spell);
						break;
					case	CAST_FAIL_TOO_FAR:
						debug_log("Mercenary Spell CAST_FAIL_TOO_FAR %u.", spell);
						break;
					case		CAST_FAIL_TOO_CLOSE:
						debug_log("Mercenary Spell CAST_FAIL_TOO_CLOSE %u.", spell);
						break;
					case	CAST_FAIL_POWER:
						debug_log("Mercenary Spell CAST_FAIL_POWER %u.", spell);
						break;
					case		CAST_FAIL_STATE:
						debug_log("Mercenary Spell CAST_FAIL_STATE %u.", spell);
						break;
					case		CAST_FAIL_TARGET_AURA:
						debug_log("Mercenary Spell CAST_FAIL_TARGET_AURA %u.", spell);
						break;
					case		CAST_FAIL_NOT_IN_LOS:
						debug_log("Mercenary Spell CAST_FAIL_NOT_IN_LOS %u.", spell);
						break;					
				}
			}
		}
	}
	bool DoSpellAttackIfReady(const uint32 diff){

		//uint8 spellCount = m_creature->GetPetAutoSpellSize();

		Player* player = m_creature->GetOwner()->ToPlayer();
		if (!player)
			return false;

		mercenary = MercenaryUtil::GetMercenaryByOwner(player->GetGUIDLow());
		//#endif
		if (!mercenary)
			return false;

		if (defaultSpellTimer <= (int)diff)
		{
			for (auto it = defautSpells.begin(); it != defautSpells.end(); it++)
			{
				if (!m_creature->HasAura(*it))
					m_creature->CastSpell(m_creature, *it, true);//没有光环就自动添加光环
			}
			defaultSpellTimer = 30000;//30秒检查1次
		}
		else
			defaultSpellTimer -= diff;

		Unit* target = player->getVictim();
		if (!target)
			target = m_creature->getVictim();
		//if (!target)
		//target = m_creature->SelectRandomUnfriendlyTarget();
		if (!target)
			return false;

		if (spellTimer < (int)diff)
		{
				if (currentSpell >= 4)
					currentSpell = 0;
				else
					currentSpell++;

				if (UnitActionBarEntry const* actionEntry = m_creature->GetCharmInfo()->GetActionBarEntry(ACTION_BAR_INDEX_PET_SPELL_START + currentSpell))
				{
					if (uint32 spellid = actionEntry->GetAction())
					{
						if (actionEntry->GetType() == ACT_ENABLED) //auto cast +castable
							caseSpell(player->context.checkPositiveSpell(spellid) ? player : target, spellid, diff);
					}
				}
		}
		else
			spellTimer -= diff;

		return true;
	};
	void updateStealth(const uint32 diff){
		if (checkCancelStealthAuraTimer < 0)
		{
			checkCancelStealthAuraTimer = 5000;//设置检查间隔5秒

			if (m_creature->GetOwner()->HasStealthAura() && !m_creature->HasStealthAura())
					m_creature->CastSpell(m_creature, 1784, true);//设置潜行		
			else if (!m_creature->GetOwner()->HasStealthAura() && m_creature->HasStealthAura())
					m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);//取消潜行					

		}else
			checkCancelStealthAuraTimer -= diff;
	}
	void UpdateAI(const uint32 diff) override
	{

		if (!mercenary)
			return;

		//updateStealth(diff);

		if (Unit* owner = m_creature->GetOwner())
			if (!m_creature->getVictim() && m_creature->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW) && !m_creature->hasUnitState(UNIT_STAT_FOLLOW))
			{
					if (mercenary->isRangedAttack())
						m_creature->GetMotionMaster()->MoveFollow(owner, 4 * PET_FOLLOW_DIST, M_PI_F*3.0f / 4.0f);
					else
						m_creature->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, -PET_FOLLOW_ANGLE);
			}


			//万能判断方法
			if(updatePetMercenaryMirrorCheck)
			{
				if (updatePetMercenaryMirrorTimer <= 0)
				{
					updatePetMercenaryMirrorTimer = 5000;
					if ((mercenary->gearContainer[SLOT_MAIN_HAND].itemid > 0 && m_creature->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID) == 0) ||
						(mercenary->gearContainer[SLOT_OFF_HAND].itemid > 0 && m_creature->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+1) == 0) ||
						(mercenary->gearContainer[SLOT_RANGED].itemid > 0 && m_creature->GetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID+2) == 0))
					{
						m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, mercenary->gearContainer[SLOT_MAIN_HAND].itemid);
						m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, mercenary->gearContainer[SLOT_OFF_HAND].itemid);
						m_creature->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, mercenary->gearContainer[SLOT_RANGED].itemid);
						mercenary->SendMirrorImagePacket(m_creature);
						updatePetMercenaryMirrorCheck = false;//检查1次不再检查
					}
				}
				else
					updatePetMercenaryMirrorTimer -= diff;

				
			}
			if (talkTimer <= 0)
			{
				std::vector<MercenaryTalking> tempVector = MercenaryUtil::MercenaryGetTalk(mercenary->GetType(), mercenary->GetRole());
				if (tempVector.size() > 0)
				{
					int rnd = MercenaryUtil::GetMercenaryRandom().Next(0, tempVector.size() - 1);
					int rnd2 = MercenaryUtil::GetMercenaryRandom().Next(0, 100);
					mercenaryTalk = tempVector[rnd];
					if (rnd2 <= 50 && mercenaryTalk.healthPercentageToTalk == 100 && lastMessage != mercenaryTalk.text)
					{
						//#ifndef MANGOS
						//                            me->Say(mercenaryTalk.text.c_str(), LANG_UNIVERSAL);
						//#else
						m_creature->MonsterSay(mercenaryTalk.text.c_str(), LANG_UNIVERSAL);
						//#endif
						lastMessage = mercenaryTalk.text;
					}
				}
				talkTimer = urand(35000, 120000);
			}
			else
				talkTimer -= diff;
		

		//if (mercenary->isRangedAttack())
		//{
			//m_creature->SetSheath(SHEATH_STATE_RANGED);
			//m_creature->clearUnitState(UNIT_STAT_MELEE_ATTACKING);
			//m_creature->addUnitState(UNIT_STAT_FLEEING);
			
			DoSpellAttackIfReady(diff);//总是触发法术

		//}
		//else
		if (!mercenary->isRangedAttack())
			DoMeleeAttackIfReady();
	}
private:
	Mercenary* mercenary;
	MercenaryTalking mercenaryTalk;
	std::string lastMessage;
};
#endif