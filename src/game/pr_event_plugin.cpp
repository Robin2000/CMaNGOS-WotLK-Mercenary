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
					mercenary->disableMirroClone = false;

					if (mercenary->isRangedAttack())
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, mercenary->gearContainer[SLOT_RANGED].itemid,true);
					else
					{
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, mercenary->gearContainer[SLOT_MAIN_HAND].itemid, true);
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, mercenary->gearContainer[SLOT_OFF_HAND].itemid, true);
					}
					mercenary->SendMirrorImagePacket(pet);
				}
			}
		}
	}
	Player* player;
};

bool PrEventPlugin::sendEvent(PrEvent e){
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
			if (Pet * pet = player->GetPet())
			{
				if (pet->isMercenary())
				{
					if (Mercenary * mercenary = player->context.GetMercenary())
					{
						mercenary->disableMirroClone = true;

						if (mercenary->isRangedAttack())
							pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, 0);
						else
						{
							pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
							pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, 0);
						}
						
						mercenary->SendMirrorImagePacket(pet, false);
						player->context.addDelayedAction(new DelayedInitPetActionBar(1000, player));//3�룬��ʼ��						
					}
				}
			}
			break;
		case P_PET_ABANDON_EVENT:
			Pet * pet=player->GetPet();
			if (pet->isMercenary())
			{
				Mercenary * mercenary=player->context.GetMercenary();
				for (auto it = mercenary->gearContainer.begin(); it != mercenary->gearContainer.end();it++)
				{
					if (it->second.itemguid > 0)
					{
						ChatHandler(player).SendSysMessage(-2800696);
						return false;
					}
				}
			}
			break;
	}

	return true;
}