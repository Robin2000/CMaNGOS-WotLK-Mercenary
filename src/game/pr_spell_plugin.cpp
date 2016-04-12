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


	switch (spell->m_spellInfo->Id){
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
						pet->CastSpell(pet, 1784, true);//设置潜行	
			}
			return false;//返回false则spell会继续执行
		}
		case 74035:
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
		case 73943:{
			player->RemoveAurasDueToSpell(73952);//如果有，激励成功，取消变猪
			player->CastSpell(player, 74071, true);//如果模型变大，激励成功，角色模型还原

			/*Quest const* qInfo = sObjectMgr.GetQuestTemplate(25229);//任务：好侏儒寥寥
			uint32 maxCount = 0;
			for (int j = 0; j < QUEST_OBJECTIVES_COUNT; ++j)
			{
				if (qInfo->ReqCreatureOrGOId[j] == 39623) //侏儒平民
				{
					maxCount = qInfo->ReqCreatureOrGOCount[j];
					break;
				}
			}
			uint32 curCount = player->GetReqKillOrCastCurrentCount(25229, 39623);//当前杀怪数量
			if (curCount <= maxCount)*/
			
			player->CastSpell(player, 74034, true);//提醒激励成功，14分钟内必须带去见队长，不然平民会逃跑
			
			return false;//让法术继续执行
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
					pet->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);//取消潜行					
			}
			break;
		}
	}
}