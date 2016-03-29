#ifndef PR_PLAYERContext_H
#define PR_PLAYERContext_H

#include "GameObject.h"
#include "GamePointMgr.h"
#include <boost/lockfree/queue.hpp>
#include "pr_event_plugin.h"

/*��������*/
class MANGOS_DLL_SPEC StopAction
{
public:
	explicit StopAction(){}
	virtual void run() = 0;
};

/*�ӳ�ִ�ж���*/
class MANGOS_DLL_SPEC DelayedAction
{
public:
	explicit DelayedAction(int _timelimit) :timelimit(_timelimit){}

	void Update(uint32 update_diff);

	virtual void run() = 0;

	bool timeout = false;
	int timelimit;
};

/////////////////////////////////
typedef boost::lockfree::queue<DelayedAction *, boost::lockfree::fixed_sized<false>> DelayActionQueue;
typedef tbb::concurrent_vector<Quest const*> RecommentQuestList;


class MANGOS_DLL_SPEC PlayerContext{

	public:
		PlayerContext(Player* player);
		~PlayerContext();

	DelayActionQueue delayActionQueue;

	bool firstQuestChecked = false;

	void checkFirstGuideQuest();
	//ѡ���map
	int MAPSEL = -1;

	//ѡ���zone
	int ZONESEL = -1;

	//ѡ���zone
	int AREASEL = -1;

	//ѡ���mapҳ��
	int MAPSELPAGE = -1;

	//ѡ���zoneҳ��
	int ZONESELPAGE = -1;

	//ѡ���areaҳ��
	int	AREASELPAGE = -1;

	//�����Ƽ����
	RecommentQuestList recommentQuestList;
	
	//���ֹ���
	GamePointMgr gamePointMgr;

	//¯ʯ�˵������־
	int gossipMenuType = -1;
	//�˵�Action�����־
	int gossipActionType = -1;
	
	//¯ʯ�˵���ǰ��������
	uint32 aux_questid=-1;
	uint32 aux_npcgo_idx = -1;
	uint32 aux_poi_idx = -1;//ʹ��POI���ٶ�λ

	int heart_stone_cooldown = 0;

	Quest const* findAuxQuest();
	
	Quest const* findQuest(uint32 questid);

	//��Ӷ��
	Mercenary* mMercenary = nullptr;
	void loadMercenary();
	void OnSaveMercenary();

	//����ȡ������ְҵ��ȱʡ����
	void getDefaultSpells( std::vector<uint32> & vec, uint8 race, uint8 cla, uint32 level);

	//���㲢���±������������Ϣ
	void calculateZone_quest_npcgo_all_map();
	void calculateParallelZone_quest_npcgo_all_map();
	void calculateZone_quest_poi_points();
	
	inline std::string * getGameMapsName(uint32 map);
	inline std::string * getGameAreaName(uint32 area);

	//ȡ����Ϸ��ͼ�б�
	tbb::concurrent_unordered_map<uint32, GameMap*> & getGameMaps();
	
	//ȡ���ض���ͼ����Ϣ�����а����õ�ͼ��zone�б�
	GameMap * getGameMap(uint32 map);

	//ȡ���ض���zone��Ϣ�����а�����zone��area�б�
	GameZone * getGameZone(uint32 zone);

	//ȡ���ض�area����Ϣ�����а���questlist
	GameArea * getGameArea(uint32 area);

	//�Ƽ��ض���������񣬿���������Ƽ�����Ŀ��
	tbb::concurrent_vector<Quest const*>& recommendQuestArea(int32 area, uint32 num);
	tbb::concurrent_vector<Quest const*>& recommendQuestZone(int32 zone, uint32 num);


	//ȡ�����񱾵ػ�����
	void GetQuestTitleLocale(uint32 quest_id, std::string * title);

	//ȡ���������Ϸ���󱾵ػ�����
	void GetCreatureOrGOTitleLocale(int32 entry, const char  ** name);

	//��������ģ������
	CreatureData* findCreatureDataByEntry(uint32 entry);

	//������Ϸ����ģ������
	GameObjectData* findGameObjectDataByEntry(uint32 entry);

	//��������ʼNPCλ����Ϣ
	CreatureData* findQuestStarterCreature(uint32 quest_id);

	//��������ʼ��Ϸ����λ����Ϣ
	GameObjectData* findQuestStarterGameObject(uint32 quest_id);

	//ͨ��POI��������
	CreatureData* findCreatureDataByPOI(uint64 mapxy);

	//ͨ��POI������Ϸ����
	GameObjectData* findGameObjectDataByPOI(uint64 mapxy);

	//��������NPCGO�������ı���ָ��sObjectMgr�������ã���ά����������
	tbb::concurrent_vector<QuestNpcGO const *> * questNpcGOVec;

	//��������POI�������ı�����Ҫά����������
	tbb::concurrent_vector<QuestPOIPoint *> * questPOIVec;

	//ȡ�û�������POI����,���ȵ���loadQuestAux׼������
	tbb::concurrent_vector<QuestPOIPoint *> * GetQuestPOI(){ return questPOIVec; }

	//ȡ���ض��������NPC��GameObject�б�,���ȵ���loadQuestAux׼������
	tbb::concurrent_vector<QuestNpcGO const *> * GetQuestNpcGOVector(){ return questNpcGOVec; }

	//����������POI��NPCGO
	void loadQuestAux(uint32 questid);

	//Ѱ������ʼNPC��GO
	int32 findQuestStarterCreatureOrGO(uint32 questid);
	
	//Ѱ���������NPC��GO
	void findQuestInvolvedCreatureOrGO(uint32 questid, std::vector<int32> &result);

	//ȡ�ü��ܱ��ػ���
	std::string& getSpellName(uint32 spellid);

	//ȡ��ִҵ��Ӧ�������б�
	tbb::concurrent_unordered_set<uint32> & GetRaceSetByClass(uint32 charClass);

	void teleport(uint32 mapid, float x, float y, float z, float orientation);
	void moveFast(uint32 mapid, uint32 zone, uint32 area, float x, float y, float z, float orientation);

	void moveFast(QuestNpcGO const * questNpcGO);
	void moveFast(QuestPOIPoint * point);
	void moveFast(uint32 mapid, uint32 zone, uint32 area, CreatureData* data);
	void moveFast(uint32 mapid, uint32 zone, uint32 area, GameObjectData* data);

	void changeCamera(QuestNpcGO const* questNpcGO);
	void changeCamera(QuestPOIPoint * point);
	void changeCamera(CreatureData* data);
	void changeCamera(GameObjectData* data);
	void changeCamera(WorldObject* target, int32 duration, float radius, float orientation);
	void changeCamera(uint32 mapid, float x, float y, float z, float orientation, int32 duration, float radius);

	//������
	bool isMovesplineStopNow = false;//������˲��
	bool isMovesplineRunning = false;//������
	bool disableFindPath = false;//��ֹѰ·

	StopAction* transportStopAction = nullptr;

	//����״̬ʱ��
	void Update(uint32 update_diff, uint32 time);

	void deletePOIFromDB(uint32 questId, QuestPOIPoint * point);//ɾ�����õ�POI

	void addSelectedToPOI(uint32 questId,WorldObject * target); //���Ŀ��λ�õ�POI

	void addDelayedAction(DelayedAction * action);
	
	PrEventPlugin & getEventPlugin(){return eventPlugin;}

	Player* mPlayer;

private :
	bool recommendQuestByQuestList(tbb::concurrent_unordered_set<uint32>* questlist, uint32 num);
	PrEventPlugin eventPlugin;
};
#endif