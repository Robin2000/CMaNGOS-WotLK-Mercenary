#ifndef _PLAYERContext_H
#define _PLAYERContext_H
#include "GameObject.h"
#include "GamePointMgr.h"

typedef tbb::concurrent_vector<Quest const*> RecommentQuestList;
class MANGOS_DLL_SPEC PlayerContext{
	public:
		PlayerContext(Player* player);
		~PlayerContext();

	//选择的map
	int MAPSEL = -1;

	//选择的zone
	int ZONESEL = -1;

	//选择的zone
	int AREASEL = -1;

	//选择的map页码
	int MAPSELPAGE = -1;

	//选择的zone页码
	int ZONESELPAGE = -1;

	//选择的area页码
	int	AREASELPAGE = -1;

	//任务推荐结果
	RecommentQuestList recommentQuestList;
	
	//积分管理
	GamePointMgr gamePointMgr;

	//炉石菜单大类标志
	int gossipMenuType = -1;
	//菜单Action分类标志
	int gossipActionType = -1;
	
	//炉石菜单当前辅助任务
	uint32 aux_questid=-1;

	Quest const* findAuxQuest();
	
	Quest const* findQuest(uint32 questid);

	//雇佣兵
	Mercenary* mMercenary = nullptr;
	void loadMercenary();
	void OnSaveMercenary();

	//宠物取得种族职业的缺省技能
	void getDefaultSpells( std::vector<uint32> & vec, uint8 race, uint8 cla, uint32 level);

	//计算并更新表的任务区域信息
	void calculateZone_quest_npcgo_all_map();
	void calculateZone_quest_poi_points();
	
	inline std::string * getGameMapsName(uint32 map);
	inline std::string * getGameAreaName(uint32 area);

	//取得游戏地图列表
	tbb::concurrent_unordered_map<uint32, GameMap*> & getGameMaps();
	
	//取得特定地图的信息，其中包含该地图的zone列表
	GameMap * getGameMap(uint32 map);

	//取得特定的zone信息，其中包含该zone的area列表
	GameZone * getGameZone(uint32 zone);

	//取得特定area的信息，其中包含questlist
	GameArea * getGameArea(uint32 area);

	//推荐特定区域的任务，可设置最大推荐的条目数
	tbb::concurrent_vector<Quest const*>& recommendQuestArea(int32 area, uint32 num);
	tbb::concurrent_vector<Quest const*>& recommendQuestZone(int32 zone, uint32 num);

	//取得特定任务相关NPC和GameObject列表
	tbb::concurrent_vector<QuestNpcGO> * GetQuestNpcGOVector(uint32 questid);

	//取得任务本地化标题
	void GetQuestTitleLocale(uint32 quest_id, std::string * title);

	//取得生物或游戏对象本地化标题
	void GetCreatureOrGOTitleLocale(int32 entry, const char  ** name);

	//查找生物模板数据
	CreatureData* findCreatureDataByEntry(uint32 entry);

	//查找游戏对象模板数据
	GameObjectData* findGameObjectDataByEntry(uint32 entry);

	//查找任务开始NPC位置信息
	CreatureData* findQuestStarterCreature(uint32 quest_id);

	//查找任务开始游戏对象位置信息
	GameObjectData* findQuestStarterGameObject(uint32 quest_id);

	//通过POI查找生物
	CreatureData* findCreatureDataByPOI(uint32 mapid, float x, float y);

	//通过POI查找游戏对象
	GameObjectData* findGameObjectDataByPOI(uint32 mapid, float x, float y);

	//缓存任务POI向量
	QuestPOIVector const* questPOIVec;
	
	//取得特定任务POI
	QuestPOIVector const*  loadQuestPOI(uint32 questid);
	
	//取得整个任务POI向量
	QuestPOIVector const* getQuestPOI(){ return questPOIVec; }
	
	//取得技能本地化名
	std::string & getSpellName(uint32 idx);
	
	//寻找任务开始NPC或GO
	int32 findQuestStarterCreatureOrGO(uint32 questid);
	
	//寻找任务结束NPC或GO
	void findQuestInvolvedCreatureOrGO(uint32 questid, std::vector<int32> &result);

	Player* mPlayer;

private :
	bool recommendQuestByQuestList(tbb::concurrent_vector<uint32>* questlist, uint32 num);
};
#endif