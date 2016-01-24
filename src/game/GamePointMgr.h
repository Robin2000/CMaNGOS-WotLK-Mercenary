/*
 * 积分系统
 *
 */
#ifndef __MANGOS_GAMEPOINTMGR_H
#define __MANGOS_GAMEPOINTMGR_H

#include "Common.h"
//#include "Policies/Singleton.h"
#include "Database/DatabaseEnv.h"
//#include "DBCEnums.h"
#include "SharedDefines.h"
//#include "ObjectGuid.h"
#include "Player.h"
#include "pr_threadpool.h"
//#include "boost/lockfree/queue.hpp"
#include "tbb/concurrent_queue.h"

//#include <string>


//积分系统
enum CharacterConsumeConsumeType{
	// value1         value2        comment
	CHARACTERCONSUME_CONSUMETYPE_QUESTQUERY = 0,    // 0 任务查询
	CHARACTERCONSUME_CONSUMETYPE_BINDHOME = 1		// 1 任意绑定回城点
};

//账户平衡表jf_account_balance
struct AccountBalanceEntry
{
	uint32	id;							// 0 账号id        
	int64	totalmoney;					// 1 总充值金钱
	int64	consumemoney;				// 2 已消费金钱
};

//角色扩展表jf_character_ext
struct CharacterExtEntry
{
	uint32	guid;							// 0 角色guid        
	uint32	onlinetime;						// 1 用掉的在线时间，单位秒
};

//角色消费历史表jf_consume_history
struct ConsumeHistoryEntry
{
	uint32	id;									// 0 角色消费游戏点流水        
	uint32	guid;								// 1 角色guid
	CharacterConsumeConsumeType	consumetype;	// 2 消费类别
	int64	consumepoint;						// 3 消费点数
	time_t	operatetime;						// 4 操作时间
};

//typedef boost::lockfree::queue<ConsumeHistoryEntry *, boost::lockfree::fixed_sized<false> > ConsumeHistoryQueue;    //角色消费历史

class GamePointMgr
{
    public:
		GamePointMgr(Player* pl);
		~GamePointMgr();

		Player* GetPlayer()  { return m_player; }
		uint32 getGamePoint() ;
		void comsumeGamePoint(CharacterConsumeConsumeType consumeConsumeType,uint32 point);
		void _LoadAccountBalance(QueryResult* result);
		void _LoadCharacterExt(QueryResult* result);
		void _SaveGamePoint();
    private:
        Player* m_player;
		
		bool changed;

		AccountBalanceEntry m_accountBalance;
		CharacterExtEntry m_characterExt;		 	
		//ConsumeHistoryQueue consumeHistoryQueue; 
		//boost::lockfree::queue<ConsumeHistoryEntry *,boost::lockfree::fixed_sized<false>> consumeHistoryQueue;
		tbb::concurrent_queue<ConsumeHistoryEntry *> consumeHistoryQueue;
		int oneGamePointCostMoney;

};
/*class GamePointGlobalMgr
{
public:
	AllGamePointMap & GetAllGamePointMap(){
		return m_allGamePointMap;
	}

	void LoadAllGamePointMap();

private:
	AllGamePointMap m_allGamePointMap;
};
#define sGamePointMgr MaNGOS::Singleton<GamePointGlobalMgr>::Instance()
*/
#endif
