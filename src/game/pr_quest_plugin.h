#ifndef PR_QUEST_PLUGIN_H
#define PR_QUEST_PLUGIN_H

class MANGOS_DLL_SPEC PrQuestPlugin{
public:
	PrQuestPlugin(Player * _player) :player(_player){}
	void AddQuest(uint32 questid);
	void CompleteQuest(uint32 questid);
	void IncompleteQuest(uint32 questid);
	void rewardQuest(uint32 questid);
	void FailQuest(uint32 questid);
	Player* player;
};

#endif