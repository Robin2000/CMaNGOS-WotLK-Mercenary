#include "Spell.h"
#include "pr_spell_plugin.h"
#include "Chat.h"
#include "PlayerContext.h"


class PlayerGhostAction :public DelayedAction{

public:
	PlayerGhostAction(Player * _player, int _timelimit) : player(_player), DelayedAction(_timelimit){}

	void run() override{
		ChatHandler(player).HandleAuraSelf(8326);
		player->SetDeathState(CORPSE);
		player->GetCamera().ResetView(false);
		player->GetCamera().UpdateVisibilityForOwner();
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
		player->GetCamera().UpdateVisibilityForOwner();

		player->HandleEmoteCommand(92);//喝药动作
		ChatHandler(player).SendSysMessage(-2800677);
		player->context.addDelayedAction(new PlayerTakeMedicine(player, 2000));
		player->context.addDelayedAction(new PlayerGhostAction(player, 4000));

		return true;
	}

	return false;
}