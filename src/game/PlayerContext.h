#ifndef PR_PLAYERContext_H
#define PR_PLAYERContext_H

#include "GameObject.h"
#include "GamePointMgr.h"
#include <boost/lockfree/queue.hpp>
#include "pr_event_plugin.h"
#include "pr_spell_plugin.h"
#include "pr_quest_plugin.h"

/*结束动作*/
class MANGOS_DLL_SPEC StopAction
{
public:
	explicit StopAction(){}
	virtual void run() = 0;
};

/*延迟执行动作*/
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
typedef tbb::concurrent_vector<QuestNpcGO*> RecommentQuestList;

class MANGOS_DLL_SPEC PlayerContext{

	public:
		PlayerContext(Player* player);
		~PlayerContext();

	DelayActionQueue delayActionQueue;

	bool firstQuestChecked = false;

	uint32 displayid = 0;
	std::vector<uint32> creaturehistory;
	std::vector<uint32> displayhistory;
	void addMorphHistory(uint32 creature_entry, uint32 displayid);

	bool IsHostileTo(uint32 faction) const;

	void checkFirstGuideQuest();
	//选择的大陆
	int CONTINENTSEL = -1;

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
	
	//记录上次的action值，允许重新弹出选择框
	uint32 gossipActionOld = -1;

	//炉石菜单当前辅助任务
	uint32 aux_questid=-1;
	uint32 aux_npcgo_idx = -1;
	uint32 aux_poi_idx = -1;//使用POI快速定位

	int heart_stone_cooldown = 0;

	Quest const* findAuxQuest();
	
	Quest const* findQuest(uint32 questid);

	//雇佣兵

	void loadMercenary();
	void OnSaveMercenary();
	Mercenary* GetMercenary(){ return mMercenary; }
	void SetMercenary(Mercenary * mercenary){ mMercenary = mercenary; }

	//宠物取得种族职业的缺省技能
	void getDefaultSpells( std::vector<uint32> & vec, uint8 race, uint8 cla, uint32 level);

	//计算并更新表的任务区域信息
	void calculateZone_quest_npcgo_all_map();
	void calculateParallelZone_quest_npcgo_all_map();
	void calculateZone_quest_poi_points();
	
	//计算gameobject个creature的zone和area
	void calculateParallelZone_GameObject();
	void calculateParallelZone_Creature();

	inline std::string * getGameMapsName(uint32 map);
	inline std::string * getGameAreaName(uint32 area);

	tbb::concurrent_unordered_map<uint32, GameInstance*> & getGameInstanceMap();

	//取得游戏地图列表
	tbb::concurrent_unordered_map<uint32,GameMap*> & getGameMaps();

	//取得地图传送列表
	tbb::concurrent_unordered_map<uint8,GameTransport*> & getGameTransportMaps();

	//取得特定地图的信息，其中包含该地图的zone列表
	//GameMap * getGameMap(uint32 map);

	//取得特定的zone信息，其中包含该zone的area列表
	GameZone * getGameZone(uint32 zone);

	//取得特定area的信息，其中包含questlist
	GameArea * getGameArea(uint32 area);

	//推荐特定区域的任务，可设置最大推荐的条目数
	tbb::concurrent_vector<QuestNpcGO*>& recommendQuestArea(int32 area, uint32 num);
	tbb::concurrent_vector<QuestNpcGO*>& recommendQuestZone(int32 zone, uint32 num);


	//取得任务本地化标题
	void GetQuestTitleLocale(uint32 quest_id, std::string * title);

	//取得生物或游戏对象本地化标题
	void GetCreatureOrGOTitleLocale(int32 entry, const char  ** name);

	//查找生物模板数据
	CreatureInfo const* findCreatureInfoByEntry(uint32 entry);

	//查找生物实例数据
	CreatureData* findCreatureDataByEntry(uint32 entry);

	//查找游戏对象模板数据
	GameObjectData* findGameObjectDataByEntry(uint32 entry);

	//查找任务开始NPC位置信息
	CreatureData* findQuestStarterCreature(uint32 quest_id);

	//查找任务开始游戏对象位置信息
	GameObjectData* findQuestStarterGameObject(uint32 quest_id);

	//通过POI查找生物
	CreatureData* findCreatureDataByPOI(uint64 mapxy);

	//通过POI查找游戏对象
	GameObjectData* findGameObjectDataByPOI(uint64 mapxy);

	//缓存任务NPCGO向量，改变量指向sObjectMgr，仅引用，不维护生命周期
	tbb::concurrent_vector<QuestNpcGO *> * questNpcGOVec;

	//缓存任务POI向量，改变量需要维护生命周期
	tbb::concurrent_vector<QuestPOIPoint *> * questPOIVec;

	//取得缓存任务POI向量,需先调用loadQuestAux准备数据
	tbb::concurrent_vector<QuestPOIPoint *> * GetQuestPOI(){ return questPOIVec; }

	//取得特定任务相关NPC和GameObject列表,需先调用loadQuestAux准备数据
	tbb::concurrent_vector<QuestNpcGO *> * GetQuestNpcGOVector(){ return questNpcGOVec; }

	//根据任务获得POI和NPCGO
	void loadQuestAux(uint32 questid);

	//寻找任务开始NPC或GO
	int32 findQuestStarterCreatureOrGO(uint32 questid);
	
	//寻找任务结束NPC或GO
	void findQuestInvolvedCreatureOrGO(uint32 questid, std::vector<int32> &result);

	//取得技能本地化名
	std::string& getSpellName(uint32 spellid);

	//取得执业对应的种族列表
	tbb::concurrent_unordered_set<uint32> & GetRaceSetByClass(uint32 charClass);

	bool checkPositiveSpell(uint32 spellid);

	void teleport(uint32 mapid, float x, float y, float z, float orientation);
	bool moveFast(uint32 mapid, uint32 zone, uint32 area, float x, float y, float z, float orientation);

	bool moveFast(QuestNpcGO const * questNpcGO);
	bool moveFast(QuestPOIPoint * point);
	bool moveFast(uint32 mapid, uint32 zone, uint32 area, CreatureData* data);
	bool moveFast(uint32 mapid, uint32 zone, uint32 area, GameObjectData* data);

	void changeCamera(QuestNpcGO const* questNpcGO);
	void changeCamera(QuestPOIPoint * point);
	void changeCamera(CreatureData* data);
	void changeCamera(GameObjectData* data);
	void changeCamera(WorldObject* target, int32 duration, float radius, float orientation);
	void changeCamera(uint32 mapid, float x, float y, float z, float orientation, int32 duration, float radius);

	const char* getQuestType(uint32 quest);

	//任务传送
	bool isMovesplineStopNow = false;//结束的瞬间
	bool isMovesplineRunning = false;//进行中
	bool disableFindPath = false;//禁止寻路

	StopAction* transportStopAction = nullptr;

	//更新状态时间
	void Update(uint32 update_diff, uint32 time);

	void deletePOIFromDB(uint32 questId, QuestPOIPoint * point);//删除无用的POI

	void addSelectedToPOI(uint32 questId,WorldObject * target); //添加目标位置到POI

	void addDelayedAction(DelayedAction * action);
	
	void ClearMercenary();//清理雇佣兵装备，避免客户端崩溃

	PrSpellPlugin & GetSpellPlugin(){ return prSpellPlugin; }

	PrEventPlugin & GetEventPlugin(){return eventPlugin;}

	PrQuestPlugin & GetQuestPlugin(){ return prQuestPlugin; }

	const char * GetContinent(uint8 cid, bool instance=false);

	Player* mPlayer;
	
	float mapDifficultyHealthMultiplier = 1.0f;
	float mapDifficultyDamageMultiplier = 1.0f;

private :
	bool recommendQuestByQuestList(tbb::concurrent_unordered_set<QuestNpcGO*>* questlist, uint32 num);
	PrEventPlugin eventPlugin;
	PrSpellPlugin prSpellPlugin;
	PrQuestPlugin prQuestPlugin;
	Mercenary* mMercenary = nullptr;
};
#endif