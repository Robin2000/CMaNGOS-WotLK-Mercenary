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
std::string & GamePointMgr::nextGameTip(){
	std::string &tip = (m_accountBalance.gametips < sObjectMgr.getGameTipsCount()) ? sObjectMgr.getGameTips(m_accountBalance.gametips) : sObjectMgr.getGameTips(0);//����������������ȡ��һ����ʵ��ѭ��
	if (m_accountBalance.gametips >= sObjectMgr.getGameTipsCount())
		m_accountBalance.gametips = 0;//����ɫ�������ݹ�Ԫ
	else
		m_accountBalance.gametips++;
	return tip;
}
uint32 GamePointMgr::getGamePoint()  {

	if (m_accountBalance.totalmoney < 10) //����1ԭ����Ľ������Ϊ0�����������������������
		m_accountBalance.totalmoney = 0;

	if (m_accountBalance.totalmoney < m_accountBalance.consumemoney)//���ݳ����⣬��������������������������������
		m_accountBalance.consumemoney = m_accountBalance.totalmoney;

	if (m_player->GetTotalPlayedTime() < m_characterExt.consumetime)//���ݳ����⣬��������������������������������
		m_characterExt.consumetime = m_player->GetTotalPlayedTime();

	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;       //�ܵ����������Գ�ֵ��ִ��ת�����ӣ���Ϊ��λ��1ëǮ1�㡣 

	uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.consumetime) / 3600;  //�뻻���Сʱ��1Сʱһ���㡣

	return p1 + p2;
}
bool GamePointMgr::setReturnPoint(uint32 gamePoint){

	if (m_player->GetMap()->Instanceable())
	{
		ChatHandler(m_player).SendSysMessage(-2800180);//ϵͳ��ʾ���޷��ڸ����м�¼�سǵ㡣
		return false;
	}

	if (!comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_RETURNPOINTSET, gamePoint))
		return false;


	m_characterExt.returnPoint.mapid = m_player->GetMapId();
	m_characterExt.returnPoint.coord_x = m_player->GetPositionX();
	m_characterExt.returnPoint.coord_y = m_player->GetPositionY();
	m_characterExt.returnPoint.coord_z = m_player->GetPositionZ();
	m_characterExt.returnPoint.orientation = m_player->GetOrientation();
	
	changed = true;

	ChatHandler(m_player).SendSysMessage(-2800179);// ϵͳ��ʾ�����ûسǵ�ɹ���

	return true;
}
bool GamePointMgr::useReturnPoint(uint32 gamePoint){

	if (m_characterExt.returnPoint.coord_x == 0.0f && m_characterExt.returnPoint.coord_y == 0.0f&&m_characterExt.returnPoint.coord_z == 0.0f)
	{
		ChatHandler(m_player).SendSysMessage(-2800178);// ϵͳ��ʾ����û�����ûسǵ㡣
		return false;
	}
	if (!comsumeGamePoint(CHARACTERCONSUME_CONSUMETYPE_RETURNPOINTUSE, gamePoint))
		return false;
	
	m_player->TeleportTo(m_characterExt.returnPoint);
	return true;
}
bool GamePointMgr::comsumeGamePoint(CharacterConsumeConsumeType consumeConsumeType, uint32 gamePoint) {
	
	if (!checkPoint(gamePoint))
		return false;

	ConsumeHistoryEntry  history;
	//history.id = null;							//	�Զ�����
	history.guid = m_player->GetGUIDLow();			//  ��ɫguid
	history.consumetype = consumeConsumeType;		//  �������
	history.consumepoint = gamePoint;						//  ���ѵ���
	history.operatetime = time(NULL);				//  ����ʱ��
	consumeHistoryQueue.push(history);

	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;
	//uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.consumetime) / 3600;

	if (p1 > gamePoint)
	{
		//m_accountBalance.totalmoney -= gamePoint*oneGamePointCostMoney;//totalmoney���ֲ���䣬�仯����consumemoney
		m_accountBalance.consumemoney += gamePoint*oneGamePointCostMoney;
	}
	else{
		m_accountBalance.consumemoney += m_accountBalance.totalmoney; //���ѵĽ�Ǯ��������ȫ����Ǯ
		m_characterExt.consumetime += (gamePoint - m_accountBalance.totalmoney / oneGamePointCostMoney) * 3600; //���ѵ�����ʱ��ֿ�
		//m_accountBalance.totalmoney = 0; //totalmoney���ֲ���䣬�仯����consumemoney
	}
	

	changed = true;

	return true;
}
void GamePointMgr::_SaveGamePoint() //�����������
{
	//CharacterDatabase.BeginTransaction();
	//if (m_accountBalance.consumemoney > 0)�������ƣ�����������account����Ϊgametips
	{ //�й���Ǯ����

		static SqlStatementID updateAccountBalance;
		//SqlStatement stmtDel = CharacterDatabase.CreateStatement(delGamePoint, "DELETE FROM jf_account_balance WHERE guid = ?");
		//SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_account_balance (id, totalmoney, consumemoney) VALUES (?, ?, ?)");
		//stmtDel.PExecute(m_player->GetGUIDLow());
		SqlStatement stmtIns = CharacterDatabase.CreateStatement(updateAccountBalance, "update jf_account_balance set consumemoney=?,gametips=? where id=?");
		stmtIns.PExecute(m_accountBalance.consumemoney, m_accountBalance.gametips+1, m_player->GetAccountId()); //д�����ݿ�ʱ������1
	}


	if (!changed)
		return;


	static SqlStatementID insCharacterExt;
	static SqlStatementID insConsumeHistory;

	//static SqlStatementID delGamePoint;
	//SqlStatement stmtDel = CharacterDatabase.CreateStatement(delGamePoint, "DELETE FROM jf_character_ext WHERE guid = ?");
	//SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_character_ext (guid, consumetime,map,zone,position_x,position_y,position_z) VALUES (?,?,?,?,?,?,?)");
	//stmtDel.PExecute(m_player->GetGUIDLow());
	SqlStatement stmtIns = CharacterDatabase.CreateStatement(insCharacterExt, "REPLACE INTO jf_character_ext (guid, consumetime,mapid,coord_x,coord_y,coord_z,orientation) VALUES (?,?,?,?,?,?,?)");
	stmtIns.PExecute(m_player->GetGUIDLow(), m_characterExt.consumetime, m_characterExt.returnPoint.mapid, m_characterExt.returnPoint.coord_x, m_characterExt.returnPoint.coord_y, m_characterExt.returnPoint.coord_z, m_characterExt.returnPoint.orientation);

	ConsumeHistoryEntry history;
	while (!consumeHistoryQueue.empty())
	{
		if (!consumeHistoryQueue.try_pop(history))
			continue;

		SqlStatement stmtIns = CharacterDatabase.CreateStatement(insConsumeHistory, "INSERT INTO jf_consume_history (guid, consumetype,consumepoint,operatetime) VALUES (?, ?,?,now())");
		stmtIns.PExecute(m_player->GetGUIDLow(), (uint32)history.consumetype, history.consumepoint);
		
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
	m_accountBalance.gametips = fields[2].GetUInt32();


	delete result;
}
void GamePointMgr::_LoadCharacterExt(QueryResult* result){

	//           0        1      2      3          4        5
	//SELECT consumetime,mapid,coord_x,coord_y,coord_z,orientation FROM jf_character_ext WHERE guid = '%u'
	if (!result)
		return;

	Field* fields = result->Fetch();

	m_characterExt.consumetime = fields[0].GetUInt32();
	m_characterExt.returnPoint.mapid = fields[1].GetUInt32();
	m_characterExt.returnPoint.coord_x = fields[2].GetFloat();
	m_characterExt.returnPoint.coord_y = fields[3].GetFloat();
	m_characterExt.returnPoint.coord_z = fields[4].GetFloat();
	m_characterExt.returnPoint.orientation = fields[5].GetFloat();

	delete result;

}

