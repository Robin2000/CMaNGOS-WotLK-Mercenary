#include "pr_quest_plugin.h"

void PrQuestPlugin::AddQuest(uint32 questid){
	switch (questid)
	{
	case 3913:/*墓地相见->寻找墓碑*/
		player->ResurrectPlayer(true, false);
		player->SetSpeedRate(MOVE_WALK, 1.0f, true);
		player->SetSpeedRate(MOVE_RUN, 1.0f, true);
		player->SetSpeedRate(MOVE_FLIGHT, 1.0f, true);
		player->DestroyItemCount(11243, 1, true, false, false);//删除物品灵视药剂，为了防止玩家被灵魂治疗师复活，药剂在这里才删除。
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
		case 99999://炉石传说，第一个任务
			player->addActionButton(0, 11, 6948, 128);//0是基础工具条，11工具条位置(从0开始编号)，6948炉石，128是item
			break;
	}
}
void PrQuestPlugin::FailQuest(uint32 questid){

}