#include "pr_quest_plugin.h"
void PrQuestPlugin::AddQuest(Player* player, uint32 questid){
	if (questid == 3913){/*墓地相见->寻找墓碑*/
		player->ResurrectPlayer(true, false);

		player->SetSpeedRate(MOVE_WALK, 1.0f, true);
		player->SetSpeedRate(MOVE_RUN, 1.0f, true);
		player->SetSpeedRate(MOVE_FLIGHT, 1.0f, true);

		player->DestroyItemCount(11243, 1, true, false, false);//删除物品灵视药剂，为了防止玩家被灵魂治疗师复活，药剂在这里才删除。

	}
}
void PrQuestPlugin::CompleteQuest(Player* player, uint32 questid){

}
void PrQuestPlugin::IncompleteQuest(Player* player, uint32 questid){

}
void PrQuestPlugin::rewardQuest(Player* player, uint32 questid){

}
void PrQuestPlugin::FailQuest(Player* player, uint32 questid){

}