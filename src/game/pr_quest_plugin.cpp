#include "pr_quest_plugin.h"

void PrQuestPlugin::AddQuest(uint32 questid){
	switch (questid)
	{
	case 3913:/*Ĺ�����->Ѱ��Ĺ��*/
		player->ResurrectPlayer(true, false);
		player->SetSpeedRate(MOVE_WALK, 1.0f, true);
		player->SetSpeedRate(MOVE_RUN, 1.0f, true);
		player->SetSpeedRate(MOVE_FLIGHT, 1.0f, true);
		player->DestroyItemCount(11243, 1, true, false, false);//ɾ����Ʒ����ҩ����Ϊ�˷�ֹ��ұ��������ʦ���ҩ���������ɾ����
		break;
	}
}
void PrQuestPlugin::CompleteQuest(uint32 questid){


}
void PrQuestPlugin::IncompleteQuest(uint32 questid){

}
void PrQuestPlugin::rewardQuest(uint32 questid){
	switch (questid)
	{
		case 99999://¯ʯ��˵����һ������
			player->addActionButton(0, 11, 6948, 128);//0�ǻ�����������11������λ��(��0��ʼ���)��6948¯ʯ��128��item
			break;
	}
}
void PrQuestPlugin::FailQuest(uint32 questid){

}