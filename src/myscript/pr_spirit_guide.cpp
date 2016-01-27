/*
18725 飞龙坐骑
13116 Alliance Spirit Guide 人形
13117 Spirit Guide 人形
21320 精灵球
*/
/*
CreatureType=11图腾,InhabitType=7总是飞过任何物体,NpcFlags根据需要随时更改,SpeedWalk=2行走速度为2倍,SpeedRun=2跑动速度2倍,ArmorMultiplier=100倍护甲。
create table tmp (select * from creature_template where entry=21320);
update tmp set entry=100000,name='精灵',CreatureType=11,InhabitType=7,SpeedWalk=2,SpeedRun=2,ArmorMultiplier=100,AIName='TotemAI',ScriptName='pr_spirit_guide' where entry=21320;
insert into creature_template select * from tmp where entry=100000;
drop table tmp;


insert into custom_texts(entry,content_default) values(-2000001,'响应您的召唤。');



insert into npc_text(id,text0_0,text0_1)
insert into gossip_menu(entry,text_id,script_id,condition_id) values(100000,,0,0);












*/


/* 实现玩家全程指导。
1、传送：不同地图。
2、骑乘：相同地图。
3、自动找NPC。
4、自动找怪物。
5、找游戏对象。
6、solo辅助。
7、积分查询。
8、积分消费。
9、玩家加速。
10、玩家加防御。
11、玩家加攻击。
*/

#include "precompiled.h"
#include "sc_gossip.h"

// **** Script Info ****
// 治疗玩家
// 每 30 秒1次 - 通过一个 channeled spell, 它全程自动施放
// 如果精灵向导despawns在他周围的所有玩家将传送到
// 下一个精灵向导
// 这里我不是很确定, 是否虚拟技能 dummyspell存在

// **** 快速信息 ****
// 精灵向导 - 脚本处理gossipHello
// 自动释放channel-spell

#define GOSSIP_JIFEN_QUERY       "jifen"
#define GOSSIP_QUIT       "quit"

enum
{
	SPELL_SPIRIT_HEAL_CHANNEL = 22011,      // Spirit Heal Channel

	SPELL_SPIRIT_HEAL = 22012,              // Spirit Heal
	SPELL_SPIRIT_HEAL_MANA = 44535,         // 玩家无魔法损耗

	SPELL_WAITING_TO_RESURRECT = 2584,       // 玩家取消光环不想复活 resurrection
	SAY_HELP = - 1888801,
	SAY_HELP2 = -2000001,
};

struct pr_spirit_guideAI : public ScriptedAI
{
	pr_spirit_guideAI(Creature* pCreature) : ScriptedAI(pCreature)
	{
		pCreature->SetActiveObjectState(true);
		Reset();
	}

	void Reset() override {}


	void UpdateAI(const uint32 /*uiDiff*/) override
	{
		// auto cast the whole time this spell
		//if (!m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
			//m_creature->CastSpell(m_creature, SPELL_SPIRIT_HEAL_CHANNEL, false);
	}
	void JustSummoned(Creature* pSummoned) override
	{
		DoScriptText(SAY_HELP2, m_creature);
		DoScriptText(SAY_HELP, m_creature);
	}

	void SpellHitTarget(Unit* pUnit, const SpellEntry* pSpellEntry) override
	{
		if (pSpellEntry->Id == SPELL_SPIRIT_HEAL && pUnit->GetTypeId() == TYPEID_PLAYER
			&& pUnit->HasAura(SPELL_WAITING_TO_RESURRECT))
			pUnit->CastSpell(pUnit, SPELL_SPIRIT_HEAL_MANA, true);
	}
};

bool GossipHello_pr_spirit_guide(Player* pPlayer, Creature* pCreature)
{
	//pPlayer->CastSpell(pPlayer, SPELL_WAITING_TO_RESURRECT, true);


	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, GOSSIP_JIFEN_QUERY, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_QUIT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
	pPlayer->SEND_GOSSIP_MENU(907, pCreature->GetObjectGuid());

	return true;
}

CreatureAI* GetAI_pr_spirit_guide(Creature* pCreature)
{
	return new pr_spirit_guideAI(pCreature);
}
// This function is called when the player clicks an option on the gossip menubool
bool pr_menu_click(Player* pPlayer, Creature* npc, uint32 /*uiSender*/, uint32 uiAction)
{
	pPlayer->CLOSE_GOSSIP_MENU();/*无条件关闭旧菜单*/
	
	if (uiAction == GOSSIP_ACTION_INFO_DEF + 1){
		uint32 time=pPlayer->GetLevelPlayedTime();

		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_MONEY_BAG, "dynamic", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_QUIT, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		pPlayer->SEND_GOSSIP_MENU(907, npc->GetObjectGuid());
	}


	return true;
}
void AddSC_PR_SPIRIT_GUIDE()
{
	Script* pNewScript;

	pNewScript = new Script;
	pNewScript->Name = "pr_spirit_guide";
	pNewScript->GetAI = &GetAI_pr_spirit_guide;
	pNewScript->pGossipHello = &GossipHello_pr_spirit_guide;
	pNewScript->pGossipSelect = &pr_menu_click;
	pNewScript->RegisterSelf();
}
