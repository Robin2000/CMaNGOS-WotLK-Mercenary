#include "pr_event_plugin.h"
#include "Mercenary.h"
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
		//if (Pet * pet = player->GetPet())
			//player->PetSpellInitialize();
		if (Pet * pet = player->GetPet())
		{
			
			player->PetSpellInitialize();
			if (pet->isMercenary())
			{
				if (Mercenary * mercenary = player->context.GetMercenary())
				{
					if (mercenary->isRangedAttack())
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, mercenary->gearContainer[SLOT_RANGED].itemid);
					else
					{
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, mercenary->gearContainer[SLOT_MAIN_HAND].itemid);
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, mercenary->gearContainer[SLOT_OFF_HAND].itemid);
					}
					
				}
			}
		}
	}
	Player* player;
};

void PrEventPlugin::sendEvent(PrEvent e){
	switch (e)
	{
		case P_LOGIN_EVENT:
			//player->context.addDelayedAction(new DelayedHandleLogin(5000, player));//并不能解决问题，任务从跟踪列表去掉，但在日志中存在
			if (player->context.displayid>0)
				player->SetDisplayId(player->context.displayid);//易容
			else
			{
				if (Pet * pet = player->GetPet())
				if (pet->isMercenary())
					player->context.GetMercenary()->SendMirrorImagePacket(pet);
			}
			break;
		case P_LOGOUT_EVENT:
			player->context.ClearMercenary();//客户端清理
			break;
		case P_RESUMMON_PET_EVENT:		
			if (Pet * pet = player->GetPet())
			{
				if (pet->isMercenary())
				{
					if (Mercenary * mercenary = player->context.GetMercenary())
					{
						pet->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_CLONED);
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, 0);
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, 0);
						player->context.addDelayedAction(new DelayedInitPetActionBar(2000, player));//1秒，初始化
					}
				}

			}
		
			break;

	}
}