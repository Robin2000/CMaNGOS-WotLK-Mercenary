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
bool PrSpellPlugin::spell_handler_deal(Spell * spell, Unit* caster){

	if (spell->m_spellInfo->Id == 14050)//任务自杀：在塔纳利斯的墓地中饮用。
	{
		Player * player = (Player*)(spell->GetCaster());
		
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

	return false;
}