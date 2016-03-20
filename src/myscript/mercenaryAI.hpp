#ifndef __PR_MERCENARY_AI_H
#define __PR_MERCENARY_AI_H

#include "precompiled.h"
#include "MercenaryMgr.h"
#include "PetAI.h"

struct mercenary_bot_AI : public PetAI
{
	mercenary_bot_AI(Creature* creature) : PetAI(creature) { Reset(); }

	uint32 talkTimer = 0;
	uint32 spellTimer = 0;
	int currentSpell = 0;
	int updatePetMercenaryMirrorTimer = 5000;//5秒检查1次
	bool updatePetMercenaryMirrorCheck = true;
	
	int checkCancelStealthAuraTimer = 5000;//5秒检查1次是否要取消隐身
	bool checkCancelStealthAura = false;

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
			}



		}
	}
	void caseSpell(Unit* target, uint32 spell, const uint32 uiDiff){
		if (spell == 0)
			return;
		// Cast spell one on our current target.
		if (DoCastSpellIfCan(target, spell) == CAST_OK)
			spellTimer = 5000;
	}
	bool DoRangeAttackIfReady(const uint32 diff){

		uint8 spellCount = m_creature->GetPetAutoSpellSize();

		if (spellCount <= 0)
			return false;

		Unit* target = m_creature->getVictim();
		//if (!target)
		//target = m_creature->SelectRandomUnfriendlyTarget();
		if (!target)
			return false;

		Player* master = m_creature->GetOwner()->ToPlayer();
		if (!master)
			return false;


		mercenary = MercenaryUtil::GetMercenaryByOwner(master->GetGUIDLow());
		//#endif
		if (!mercenary)
			return false;

		if (spellTimer < diff)
		{
			if (rand() % 50 > 10)//有10%可能随机施法
			{
				uint8 randSpell = rand() % (spellCount + 1);
				caseSpell(target, m_creature->GetPetAutoSpellOnPos(randSpell), diff);
			}
			else
			{
				if (currentSpell >= spellCount)
					currentSpell = 0;
				caseSpell(target, m_creature->GetPetAutoSpellOnPos(currentSpell), diff);
				currentSpell++;
			}

		}
		else
			spellTimer -= diff;

		return true;
	};
	void updateStealth(const uint32 diff){

		if (m_creature->GetOwner()->HasStealthAura())
		{
			if (!checkCancelStealthAura){
				//m_creature->SetVisibility(VISIBILITY_GROUP_STEALTH);//设置潜行		
				m_creature->CastSpell(m_creature, 1784, true);
				checkCancelStealthAura = true; //设置需要检查主人隐身是否取消
				
			}
		}
		else if (checkCancelStealthAura) //如果没有隐身，而且需要检查是否取消隐身
		{
			if (checkCancelStealthAuraTimer <= 0)
			{
				//m_creature->SetVisibility(VISIBILITY_ON);//取消潜行		
				m_creature->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
				checkCancelStealthAuraTimer = 5000;//设置检查间隔5秒
				checkCancelStealthAura = false;	   //不再检查
				
			}
			else{
				checkCancelStealthAuraTimer -= diff;
			}
		}
	}
	void UpdateAI(const uint32 diff) override
	{

		if (!mercenary)
			return;

		updateStealth(diff);

		if (Unit* owner = m_creature->GetOwner())
			if (!m_creature->getVictim() && m_creature->GetCharmInfo()->HasCommandState(COMMAND_FOLLOW))// && !m_creature->hasUnitState(UNIT_STAT_FOLLOW)
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
		

		if (mercenary->isRangedAttack())
		{
			//m_creature->SetSheath(SHEATH_STATE_RANGED);
			m_creature->clearUnitState(UNIT_STAT_MELEE_ATTACKING);
			//m_creature->addUnitState(UNIT_STAT_FLEEING);
			DoRangeAttackIfReady(diff);
		}
		else
			DoMeleeAttackIfReady();
	}
private:
	Mercenary* mercenary;
	MercenaryTalking mercenaryTalk;
	std::string lastMessage;
};
#endif