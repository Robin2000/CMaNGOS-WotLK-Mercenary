#include "pr_quest_plugin.h"

void PrQuestPlugin::AddQuest(uint32 questid){
	if (questid == 3913){/*Ĺ�����->Ѱ��Ĺ��*/
		player->ResurrectPlayer(true, false);

		player->SetSpeedRate(MOVE_WALK, 1.0f, true);
		player->SetSpeedRate(MOVE_RUN, 1.0f, true);
		player->SetSpeedRate(MOVE_FLIGHT, 1.0f, true);

		player->DestroyItemCount(11243, 1, true, false, false);//ɾ����Ʒ����ҩ����Ϊ�˷�ֹ��ұ��������ʦ���ҩ���������ɾ����
	}
}
void PrQuestPlugin::CompleteQuest(uint32 questid){

}
void PrQuestPlugin::IncompleteQuest(uint32 questid){

}
void PrQuestPlugin::rewardQuest(uint32 questid){

}
void PrQuestPlugin::FailQuest(uint32 questid){

}