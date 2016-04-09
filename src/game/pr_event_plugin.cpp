#include "pr_event_plugin.h"
class DelayedHandleLogin :public DelayedAction
{
	public:
		explicit DelayedHandleLogin(int _timelimit, Player* _player) :DelayedAction(_timelimit), player(_player){}
		void run() override{
			player->_sendInprogressQuestList();
		}
		Player* player;
};
class DelayedInitPetActionBar :public DelayedAction
{
public:
	explicit DelayedInitPetActionBar(int _timelimit, Player* _player) :DelayedAction(_timelimit), player(_player){}
	void run() override{
		if (Pet * pet = player->GetPet())
			player->PetSpellInitialize();
	}
	Player* player;
};

void PrEventPlugin::sendEvent(PrEvent e){
	switch (e)
	{
		case P_LOGIN_EVENT:
			//player->context.addDelayedAction(new DelayedHandleLogin(5000, player));//�����ܽ�����⣬����Ӹ����б�ȥ����������־�д���
			if (player->context.displayid>0)
				player->SetDisplayId(player->context.displayid);//����
			break;
		case P_LOGOUT_EVENT:
			player->context.ClearMercenary();//�ͻ�������
			break;
		case P_RESUMMON_PET_EVENT:
			//player->context.addDelayedAction(new DelayedInitPetActionBar(1000, player));//�����ٻ������1�룬��ʼ����������
			break;
	}
}