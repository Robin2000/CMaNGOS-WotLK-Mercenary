#include "pr_quest_plugin.h"
void PrQuestPlugin::AddQuest(Player* player, uint32 questid){

}
void PrQuestPlugin::CompleteQuest(Player* player, uint32 questid){

}
void PrQuestPlugin::IncompleteQuest(Player* player, uint32 questid){

}
void PrQuestPlugin::rewardQuest(Player* player, uint32 questid){
	if (questid == 3913){/*Ĺ�����-Ѱ��Ĺ��*/
		player->SetDeathState(JUST_ALIVED);
	}
}
void PrQuestPlugin::FailQuest(Player* player, uint32 questid){

}