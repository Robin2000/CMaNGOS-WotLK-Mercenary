#include "Spell.h"
#include "pr_spell_plugin.h"
#include "Chat.h"
#include "PlayerContext.h"


class PlayerGhostAction :public DelayedAction{

public:
	PlayerGhostAction(Player * _player, int _timelimit) : player(_player), DelayedAction(_timelimit){}

	void run() override{

		player->RemoveAllAurasOnDeath();
		player->RemoveGuardians();
		player->RemoveMiniPet();
		player->UnsummonAllTotems();
		player->ModifyAuraState(AURA_STATE_HEALTHLESS_20_PERCENT, false);
		player->ModifyAuraState(AURA_STATE_HEALTHLESS_35_PERCENT, false);
		// remove aurastates allowing special moves
		player->ClearAllReactives();
		player->ClearDiminishings();


		player->StopMirrorTimers();                                     // disable timers(bars)

		player->SetDeathState(CORPSE);

		player->SetUInt32Value(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_NONE);

		// update visibility
		//player->SetDeathState(DEAD);
		player->GetCamera().ResetView(false);
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
		//m_caster->RemoveSpellsCausingAura(SPELL_AURA_FEIGN_DEATH);
		
		player->GetCamera().SetView(player, false);

		player->HandleEmoteCommand(92);//喝药动作
		ChatHandler(player).SendSysMessage(-2800677);
		player->context.addDelayedAction(new PlayerTakeMedicine(player, 3000));
		player->context.addDelayedAction(new PlayerGhostAction(player, 6000));

		return true;
	}

	return false;
}