/*
* 积分系统
*
*/

#include "Common.h"
#include "DBCStores.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Guild.h"
#include "GuildMgr.h"
#include "Database/DatabaseEnv.h"
#include "World.h"
#include "SpellMgr.h"
#include "ArenaTeam.h"
#include "ProgressBar.h"
#include "Mail.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"
#include "Language.h"
#include "MapManager.h"
#include "Map.h"
#include "InstanceData.h"
#include "DBCStructure.h"
#include "Chat.h"
#include "Config/Config.h"
#include "Player.h"
#include "GamePointMgr.h"
#include <time.h>

/*
#include "Policies/Singleton.h"
INSTANTIATE_SINGLETON_1(GamePointGlobalMgr);*/

GamePointMgr::GamePointMgr(Player* player) :m_player(player){
	oneGamePointCostMoney = sConfig.GetIntDefault("JF.OneGamePointCostMoney", 10);
}

GamePointMgr::~GamePointMgr()
{
}

uint32 GamePointMgr::getGamePoint()  {
	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;       //总点数可以来自充值后执行转帐增加，分为单位，1毛钱1点。 
	uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.onlinetime) / 3600;  //秒换算成小时，1小时一个点。
	return p1 + p2;
}
void GamePointMgr::comsumeGamePoint(CharacterConsumeConsumeType consumeConsumeType, uint32 point) {
	
	if (getGamePoint() < point)
		return; /*点数不足*/

	ConsumeHistoryEntry * history=NULL;
	//history.id = null;							//	自动增长
	history->guid = m_player->GetGUIDLow();			//  角色guid
	history->consumetype = consumeConsumeType;		//  消费类别
	history->consumepoint = point;						//  消费点数
	history->operatetime = time(NULL);				//  操作时间
	consumeHistoryQueue.push(history);

	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;
	uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.onlinetime) / 3600;

	if (p1 > point) 
	{
		m_accountBalance.totalmoney -= point;
		m_accountBalance.consumemoney += point;
	}
	else{
		m_accountBalance.consumemoney += m_accountBalance.totalmoney; //消费的金钱数量
		m_characterExt.onlinetime -= (point - m_accountBalance.totalmoney / oneGamePointCostMoney) * 3600; //消费的在线时间抵扣
		m_accountBalance.totalmoney = 0; //钱扣光
	}
	

	changed = true;
}
void GamePointMgr::_SaveGamePoint() //保存积分数据
{
	if (!changed)
		return;

	static SqlStatementID delGamePoint;
	static SqlStatementID insGamePoint;

	//CharacterDatabase.BeginTransaction();
	if (m_accountBalance.consumemoney > 0){ //有金钱消费
		SqlStatement stmtDel = CharacterDatabase.CreateStatement(delGamePoint, "DELETE FROM jf_account_balance WHERE guid = ?");
		SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_account_balance (id, totalmoney, consumemoney) VALUES (?, ?, ?)");
		stmtDel.PExecute(m_player->GetGUIDLow());
		stmtIns.PExecute(m_player->GetGUIDLow(), m_accountBalance.id, m_accountBalance.totalmoney, m_accountBalance.consumemoney);
	}

	if (m_characterExt.onlinetime > 0){ //有时间消费
		SqlStatement stmtDel = CharacterDatabase.CreateStatement(delGamePoint, "DELETE FROM jf_character_ext WHERE guid = ?");
		SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_character_ext (guid, onlinetime) VALUES (?, ?)");
		stmtDel.PExecute(m_player->GetGUIDLow());
		stmtIns.PExecute(m_player->GetGUIDLow(), m_accountBalance.id, m_accountBalance.totalmoney, m_accountBalance.consumemoney);
	}
	ConsumeHistoryEntry* history;
	while (!consumeHistoryQueue.empty())
	{
		if (!consumeHistoryQueue.try_pop(history))
			continue;

		SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_consume_history (guid, consumetype,consumepoint,operatetime) VALUES (?, ?,?,now())");
		stmtIns.PExecute(m_player->GetGUIDLow(), (uint32)history->consumetype, history->consumepoint);
		
	}
	//CharacterDatabase.CommitTransaction();
	changed = false;

}

void GamePointMgr::_LoadAccountBalance(QueryResult* result){

	//           0           1                 
	//SELECT totalmoney,consumemoney FROM jf_account_balance WHERE guid = '%u'
	if (!result)
		return;

	Field* fields = result->Fetch();

	m_accountBalance.totalmoney = fields[0].GetUInt32();
	m_accountBalance.consumemoney = fields[1].GetUInt32();


	delete result;
}
void GamePointMgr::_LoadCharacterExt(QueryResult* result){

	//           0             
	//SELECT onlinetime FROM jf_character_ext WHERE guid = '%u'
	if (!result)
		return;

	Field* fields = result->Fetch();

	m_characterExt.onlinetime = fields[0].GetUInt32();

	delete result;

}

