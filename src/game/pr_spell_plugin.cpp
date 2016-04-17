#include "Spell.h"
#include "pr_spell_plugin.h"
#include "Chat.h"
#include "PlayerContext.h"
#include "ObjectGuid.h"

class PlayerGhostAction :public DelayedAction{

public:
	PlayerGhostAction(Player * _player, int _timelimit) : player(_player), DelayedAction(_timelimit){}

	void run() override{

		ChatHandler(player).HandleAuraSelf(8326); //进入幽灵状态
		//ChatHandler(player).HandleAuraSelf(42702);/*衰老，移动速度降低$s1%。*/
		//ChatHandler(player).HandleAuraSelf(70740);/*恶鬼警报,移动速度降低$s1%。*/
		player->SetDeathState(DEAD);

		player->SetSpeedRate(MOVE_WALK, 0.382f, true);
		player->SetSpeedRate(MOVE_RUN, 0.382f, true);
		player->SetSpeedRate(MOVE_FLIGHT, 0.382f, true);

		
		//player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_STAT_NO_FREE_MOVE);

		player->UpdateObjectVisibility();
	};

	Player * player;

};


class PlayerTakeMedicine:public PlayerGhostAction{
public:
	PlayerTakeMedicine(Player * _player, int _timelimit) : PlayerGhostAction(_player,_timelimit){}
	void run() override{
		player->HandleEmoteCommand(387);//死亡动作
	}	
};

void PrSpellPlugin::update(uint32 difftime){

}
bool PrSpellPlugin::spell_handler_deal(Spell * spell){

	switch (spell->m_spellInfo->Id)
	{
		case 14050://任务自杀：在塔纳利斯的墓地中饮用。
			{
				player->StopMoving();
				//player->SetFlag(UNIT_FIELD_FLAGS, UNIT_STAT_NO_FREE_MOVE);

				player->SetSpeedRate(MOVE_WALK, 0.1f, true);
				player->SetSpeedRate(MOVE_RUN, 0.1f, true);
				player->SetSpeedRate(MOVE_FLIGHT, 0.1f, true);

				//player->GetMotionMaster()->Clear(false, true);
				//player->GetMotionMaster()->MoveIdle();
		
				//WorldObject* target = player->GetMap()->GetGameObject(ObjectGuid(HIGHGUID_GAMEOBJECT, uint32(300021), uint32(99879)));//加基森墓地,  
				player->context.changeCamera(player, 10000, 20.0f, player->GetOrientation()*M_PI_F*1.0f / 2.0f);

				player->HandleEmoteCommand(92);//喝药动作
		
				ChatHandler(player).SendSysMessage(-2800677);

				//player->UpdateObjectVisibility();

				player->context.addDelayedAction(new PlayerTakeMedicine(player, 2000));
				player->context.addDelayedAction(new PlayerGhostAction(player, 3000));

				return true;
			}

		case 1784://潜行
		{
			if (Pet *pet = player->GetPet())
			{
					if (!pet->HasStealthAura())
						pet->SetVisibility(VISIBILITY_GROUP_STEALTH);
						//pet->CastSpell(pet, 1784, true);//设置潜行	
			}
			return false;//返回false则spell会继续执行
		}

		case 74035://任务：好侏儒寥寥25229，触发技能：激励
		{
			if (player->GetQuestStatus(25229) != QUEST_STATUS_INCOMPLETE)
				return false;
			switch (urand(1, 3))
			{
				case 1: player->CastSpell(spell->m_targets.getUnitTarget(), 73943, true, nullptr); break;//  召唤被激励的平民，unsumon平民侏儒。  
				case 2: player->CastSpell(player, 73952, true, nullptr); break;// 激励失败，产生光环，变成猪
				case 3: player->CastSpell(player, 74062, true, nullptr); break;// 激励失败，区域光环，角色模型变大
			}
			return true;
		}

		case 73943://任务：好侏儒寥寥25229，说服成功技能：激励
		{
			player->RemoveAurasDueToSpell(73952);//如果有，激励成功，取消变猪
			player->CastSpell(player, 74071, true);//如果模型变大，激励成功，角色模型还原
			player->CastSpell(player, 74034, true);//提醒激励成功，14分钟内必须带去见队长，不然平民会逃跑
			
			return false;//让法术继续执行
		}

		case 75102://任务：巨魔志愿兵25461，触发技能：激励
		{
			if (player->GetQuestStatus(25461) != QUEST_STATUS_INCOMPLETE)
				return false;
			switch (urand(1, 3))
			{
			case 1: player->CastSpell(spell->m_targets.getUnitTarget(), 75088, true, nullptr); break;//  召唤被激励的平民，unsumon平民侏儒。  
			case 2: player->CastSpell(player, 74046, true, nullptr); break;// 激励失败，产生光环，变成兔子
			case 3: player->CastSpell(player, 74062, true, nullptr); break;// 激励失败，区域光环，角色模型变大
			}
			return true;
		}

		case 75088://任务：巨魔志愿兵25461，说服成功技能：激励
		{
			player->RemoveAurasDueToSpell(74046);//如果有，激励成功，取消变兔子
			player->CastSpell(player, 74071, true);//如果模型变大，激励成功，角色模型还原
			//player->CastSpell(player, 74034, true);//提醒激励成功，14分钟内必须带去见队长，不然平民会逃跑

			return false;//让法术继续执行
		}

		case 8283://任务：蓝叶署 1221，技能：指挥地鼠
		{
			if (Unit* uinit = spell->m_targets.getUnitTarget())
			{
				if (Creature* creature = player->FindNearestCreature(4781, 10.0f))//地鼠
				{
					creature->SetPhaseMask(2, false);//进入2阶段，不更新
					if (GameObject* blueleafTubers = creature->FindNearestGameObject(20920, 50.0f))//游戏对象：蓝叶署20920
					{
						blueleafTubers->SetPhaseMask(1, true);
						creature->MonsterMoveWithSpeed(blueleafTubers->GetPositionX(), blueleafTubers->GetPositionY(), blueleafTubers->GetPositionZ(), 1.0f, true, true);
					}
					creature->SetPhaseMask(1, false);//回到1阶段，不更新
				}
		    }
			return true;
		}
	}
	return false;
}

void PrSpellPlugin::spell_handler_cancel(uint32 spellid){
	switch (spellid){
		case 1784://取消潜行
		{
			if (Pet *pet = player->GetPet())
			{
				if (pet->HasStealthAura())
					pet->SetVisibility(VISIBILITY_ON);
					//pet->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);//取消潜行					
			}
			break;
		}
	}
}