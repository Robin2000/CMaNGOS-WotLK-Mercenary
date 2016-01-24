/*
 * ����ϵͳ
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


//����ϵͳ
enum CharacterConsumeConsumeType{
	// value1         value2        comment
	CHARACTERCONSUME_CONSUMETYPE_QUESTQUERY = 0,    // 0 �����ѯ
	CHARACTERCONSUME_CONSUMETYPE_BINDHOME = 1		// 1 ����󶨻سǵ�
};

//�˻�ƽ���jf_account_balance
struct AccountBalanceEntry
{
	uint32	id;							// 0 �˺�id        
	int64	totalmoney;					// 1 �ܳ�ֵ��Ǯ
	int64	consumemoney;				// 2 �����ѽ�Ǯ
};

//��ɫ��չ��jf_character_ext
struct CharacterExtEntry
{
	uint32	guid;							// 0 ��ɫguid        
	uint32	onlinetime;						// 1 �õ�������ʱ�䣬��λ��
};

//��ɫ������ʷ��jf_consume_history
struct ConsumeHistoryEntry
{
	uint32	id;									// 0 ��ɫ������Ϸ����ˮ        
	uint32	guid;								// 1 ��ɫguid
	CharacterConsumeConsumeType	consumetype;	// 2 �������
	int64	consumepoint;						// 3 ���ѵ���
	time_t	operatetime;						// 4 ����ʱ��
};

//typedef boost::lockfree::queue<ConsumeHistoryEntry *, boost::lockfree::fixed_sized<false> > ConsumeHistoryQueue;    //��ɫ������ʷ

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
