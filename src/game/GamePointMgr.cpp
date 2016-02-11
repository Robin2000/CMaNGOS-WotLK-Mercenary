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
std::string & GamePointMgr::nextGameTip(){
	std::string &tip = (m_accountBalance.gametips < sObjectMgr.getGameTipsCount()) ? sObjectMgr.getGameTips(m_accountBalance.gametips) : sObjectMgr.getGameTips(0);//如果索引超过最大，则取第一条，实现循环
	if (m_accountBalance.gametips >= sObjectMgr.getGameTipsCount())
		m_accountBalance.gametips = 0;//将角色播放内容归元
	else
		m_accountBalance.gametips++;
	return tip;
}
uint32 GamePointMgr::getGamePoint()  {

	if (m_accountBalance.totalmoney < 10) //不到1原力点的金额设置为0，否则后续计算会带来大问题
		m_accountBalance.totalmoney = 0;

	if (m_accountBalance.totalmoney < m_accountBalance.consumemoney)//数据出问题，必须纠正，，否则后续计算会带来大问题
		m_accountBalance.consumemoney = m_accountBalance.totalmoney;

	if (m_player->GetTotalPlayedTime() < m_characterExt.consumetime)//数据出问题，必须纠正，，否则后续计算会带来大问题
		m_characterExt.consumetime = m_player->GetTotalPlayedTime();

	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;       //总点数可以来自充值后执行转帐增加，分为单位，1毛钱1点。 

	uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.consumetime) / 3600;  //秒换算成小时，1小时一个点。

	return p1 + p2;
}
bool GamePointMgr::setReturnPoint(uint32 gamePoint){

	if (m_player->GetMap()->Instanceable())
	{
		ChatHandler(m_player).SendSysMessage(-2800180);//系统提示：无法在副本中记录回城点。
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

	ChatHandler(m_player).SendSysMessage(-2800179);// 系统提示：设置回城点成功。

	return true;
}
bool GamePointMgr::useReturnPoint(uint32 gamePoint){

	if (m_characterExt.returnPoint.coord_x == 0.0f && m_characterExt.returnPoint.coord_y == 0.0f&&m_characterExt.returnPoint.coord_z == 0.0f)
	{
		ChatHandler(m_player).SendSysMessage(-2800178);// 系统提示：还没有设置回城点。
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
	//history.id = null;							//	自动增长
	history.guid = m_player->GetGUIDLow();			//  角色guid
	history.consumetype = consumeConsumeType;		//  消费类别
	history.consumepoint = gamePoint;						//  消费点数
	history.operatetime = time(NULL);				//  操作时间
	consumeHistoryQueue.push(history);

	uint32 p1 = (m_accountBalance.totalmoney - m_accountBalance.consumemoney) / oneGamePointCostMoney;
	//uint32 p2 = (m_player->GetTotalPlayedTime() - m_characterExt.consumetime) / 3600;

	if (p1 > gamePoint)
	{
		//m_accountBalance.totalmoney -= gamePoint*oneGamePointCostMoney;//totalmoney数字不会变，变化的是consumemoney
		m_accountBalance.consumemoney += gamePoint*oneGamePointCostMoney;
	}
	else{
		m_accountBalance.consumemoney += m_accountBalance.totalmoney; //消费的金钱数量等于全部金钱
		m_characterExt.consumetime += (gamePoint - m_accountBalance.totalmoney / oneGamePointCostMoney) * 3600; //消费的在线时间抵扣
		//m_accountBalance.totalmoney = 0; //totalmoney数字不会变，变化的是consumemoney
	}
	

	changed = true;

	return true;
}
void GamePointMgr::_SaveGamePoint() //保存积分数据
{
	//CharacterDatabase.BeginTransaction();
	//if (m_accountBalance.consumemoney > 0)不再限制，无条件保存account表，因为gametips
	{ //有过金钱消费

		static SqlStatementID updateAccountBalance;
		//SqlStatement stmtDel = CharacterDatabase.CreateStatement(delGamePoint, "DELETE FROM jf_account_balance WHERE guid = ?");
		//SqlStatement stmtIns = CharacterDatabase.CreateStatement(insGamePoint, "INSERT INTO jf_account_balance (id, totalmoney, consumemoney) VALUES (?, ?, ?)");
		//stmtDel.PExecute(m_player->GetGUIDLow());
		SqlStatement stmtIns = CharacterDatabase.CreateStatement(updateAccountBalance, "update jf_account_balance set consumemoney=?,gametips=? where id=?");
		stmtIns.PExecute(m_accountBalance.consumemoney, m_accountBalance.gametips+1, m_player->GetAccountId()); //写入数据库时，增加1
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

