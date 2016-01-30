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
	CHARACTERCONSUME_CONSUMETYPE_QUESTQUERY = 0,		// 0 �����ѯ
	CHARACTERCONSUME_CONSUMETYPE_RETURNPOINTSET = 1,	// 1 ���ûسǵ�
	CHARACTERCONSUME_CONSUMETYPE_RETURNPOINTUSE = 2		// 2 ���ػسǵ�
};
//�˻�ƽ���jf_account_balance
struct AccountBalanceEntry
{
	uint32	id;							// 0 �˺�id        
	int64	totalmoney=0;					// 1 �ܳ�ֵ��Ǯ
	int64	consumemoney=0;				// 2 �����ѽ�Ǯ
};

//��ɫ��չ��jf_character_ext
struct CharacterExtEntry
{
	uint32	guid;							// 0 ��ɫguid        
	uint32	consumetime = 0;					// 1 �õ�������ʱ�䣬��λ��
	WorldLocation returnPoint;				// 3 �سǵ�
};

//��ɫ������ʷ��jf_consume_history
struct ConsumeHistoryEntry
{
	uint32	id;									// 0 ��ɫ������Ϸ����ˮ        
	uint32	guid;								// 1 ��ɫguid
	CharacterConsumeConsumeType	consumetype;	// 2 �������
	int64	consumepoint=0;						// 3 ���ѵ���
	time_t	operatetime;						// 4 ����ʱ��
};

//typedef boost::lockfree::queue<ConsumeHistoryEntry *, boost::lockfree::fixed_sized<false> > ConsumeHistoryQueue;    //��ɫ������ʷ

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
				ChatHandler(m_player).SendSysMessage(-2800177);//23 ϵͳ��ʾ��ԭ�����㣬�޷���ɴ˲�����
				return false; /*��������*/
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
