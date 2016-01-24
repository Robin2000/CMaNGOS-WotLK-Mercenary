/*
* ����ϵͳ
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
	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;       //�ܵ����������Գ�ֵ��ִ��ת�����ӣ���Ϊ��λ��1ëǮ1�㡣 
	uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.onlinetime) / 3600;  //�뻻���Сʱ��1Сʱһ���㡣
	return p1 + p2;
}
void GamePointMgr::comsumeGamePoint(CharacterConsumeConsumeType consumeConsumeType, uint32 point) {
	
	if (getGamePoint() < point)
		return; /*��������*/

	ConsumeHistoryEntry * history=NULL;
	//history.id = null;							//	�Զ�����
	history->guid = m_player->GetGUIDLow();			//  ��ɫguid
	history->consumetype = consumeConsumeType;		//  �������
	history->consumepoint = point;						//  ���ѵ���
	history->operatetime = time(NULL);				//  ����ʱ��
	consumeHistoryQueue.push(history);

	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;
	uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.onlinetime) / 3600;

	if (p1 > point) 
	{
		m_accountBalance.totalmoney -= point;
		m_accountBalance.consumemoney += point;
	}
	else{
		m_accountBalance.consumemoney += m_accountBalance.totalmoney; //���ѵĽ�Ǯ����
		m_characterExt.onlinetime -= (point - m_accountBalance.totalmoney / oneGamePointCostMoney) * 3600; //���ѵ�����ʱ��ֿ�
		m_accountBalance.totalmoney = 0; //Ǯ�۹�
	}
	

	changed = true;
}
void GamePointMgr::_SaveGamePoint() //�����������
{
	if (!changed)
		return;

	static SqlStatementID delGamePoint;
	static SqlStatementID insGamePoint;

	//CharacterDatabase.BeginTransaction();
	if (m_accountBalance.consumemoney > 0){ //�н�Ǯ����
		SqlStatement stmtDel = CharacterDatabase.CreateStatement(delGamePoint, "DELETE FROM jf_account_balance WHERE guid = ?");
		SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_account_balance (id, totalmoney, consumemoney) VALUES (?, ?, ?)");
		stmtDel.PExecute(m_player->GetGUIDLow());
		stmtIns.PExecute(m_player->GetGUIDLow(), m_accountBalance.id, m_accountBalance.totalmoney, m_accountBalance.consumemoney);
	}

	if (m_characterExt.onlinetime > 0){ //��ʱ������
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

