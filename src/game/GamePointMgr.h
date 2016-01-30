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
	CHARACTERCONSUME_CONSUMETYPE_QUESTQUERY = 0,		// 0 任务查询
	CHARACTERCONSUME_CONSUMETYPE_RETURNPOINTSET = 1,	// 1 设置回城点
	CHARACTERCONSUME_CONSUMETYPE_RETURNPOINTUSE = 2		// 2 返回回城点
};
//账户平衡表jf_account_balance
struct AccountBalanceEntry
{
	uint32	id;							// 0 账号id        
	int64	totalmoney=0;					// 1 总充值金钱
	int64	consumemoney=0;				// 2 已消费金钱
};

//角色扩展表jf_character_ext
struct CharacterExtEntry
{
	uint32	guid;							// 0 角色guid        
	uint32	consumetime = 0;					// 1 用掉的在线时间，单位秒
	WorldLocation returnPoint;				// 3 回城点
};

//角色消费历史表jf_consume_history
struct ConsumeHistoryEntry
{
	uint32	id;									// 0 角色消费游戏点流水        
	uint32	guid;								// 1 角色guid
	CharacterConsumeConsumeType	consumetype;	// 2 消费类别
	int64	consumepoint=0;						// 3 消费点数
	time_t	operatetime;						// 4 操作时间
};

//typedef boost::lockfree::queue<ConsumeHistoryEntry *, boost::lockfree::fixed_sized<false> > ConsumeHistoryQueue;    //角色消费历史

class MANGOS_DLL_SPEC GamePointMgr
{
    public:
		GamePointMgr(Player* pl);
		~GamePointMgr();

		Player* GetPlayer()  { return m_player; }
		uint32 getGamePoint() ;
		bool comsumeGamePoint(CharacterConsumeConsumeType consumeConsumeType,uint32 gamePoint);
		bool setReturnPoint(uint32 gamePoint);
		bool useReturnPoint(uint32 gamePoint);
		inline bool checkPoint(uint32 gamePoint){
			if (getGamePoint() < gamePoint)
			{
				ChatHandler(m_player).SendSysMessage(-2800177);//23 系统提示：原力不足，无法完成此操作。
				return false; /*点数不足*/
			}
			return true;
		}
		const WorldLocation  &getReturnPoint(){ return m_characterExt.returnPoint; };
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
		tbb::concurrent_queue<ConsumeHistoryEntry> consumeHistoryQueue;
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
