#ifndef _PLAYERContext_H
#define _PLAYERContext_H
#include "GameObject.h"
#include "GamePointMgr.h"

typedef tbb::concurrent_vector<Quest const*> RecommentQuestList;
class MANGOS_DLL_SPEC PlayerContext{
	public:
		PlayerContext(Player* player);
		~PlayerContext();

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

	//ȡ���ض��������NPC��GameObject�б�
	tbb::concurrent_vector<QuestNpcGO> * GetQuestNpcGOVector(uint32 questid);

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
	CreatureData* findCreatureDataByPOI(uint32 mapid, float x, float y);

	//ͨ��POI������Ϸ����
	GameObjectData* findGameObjectDataByPOI(uint32 mapid, float x, float y);

	//��������POI����
	QuestPOIVector const* questPOIVec;
	
	//ȡ���ض�����POI
	QuestPOIVector const*  loadQuestPOI(uint32 questid);
	
	//ȡ����������POI����
	QuestPOIVector const* getQuestPOI(){ return questPOIVec; }
	
	//ȡ�ü��ܱ��ػ���
	std::string & getSpellName(uint32 idx);
	
	//Ѱ������ʼNPC��GO
	int32 findQuestStarterCreatureOrGO(uint32 questid);
	
	//Ѱ���������NPC��GO
	void findQuestInvolvedCreatureOrGO(uint32 questid, std::vector<int32> &result);

	Player* mPlayer;

private :
	bool recommendQuestByQuestList(tbb::concurrent_vector<uint32>* questlist, uint32 num);
};
#endif