#ifndef PR_QUEST_PLUGIN_H
#define PR_QUEST_PLUGIN_H

class MANGOS_DLL_SPEC PrQuestPlugin{
public:
	
	void AddQuest(Player* player, uint32 questid);
	void CompleteQuest(Player* player, uint32 questid);
	void IncompleteQuest(Player* player, uint32 questid);
	void rewardQuest(Player* player, uint32 questid);
	void FailQuest(Player* player, uint32 questid);
};

#endif