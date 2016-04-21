#ifndef PR_EVENT_PLUGIN_H
#define PR_EVENT_PLUGIN_H
enum PrEvent{
	P_LOGIN_EVENT			= 0,
	P_LOGOUT_EVENT			= 1,
	P_UNSUMMON_PET_EVENT	= 2,
	P_RESUMMON_PET_EVENT	= 3,
	P_AI_UPDATE_PET_EVENT	= 4,
	P_PET_ABANDON_EVENT		= 5,
	P_SUMMON_CREATURE		= 6,
	P_GOSSIP_CREATURE		= 7,
	P_SELECT_UNIT			= 8,
	P_ADD_TO_MAP_EVENT		= 9
};
class MANGOS_DLL_SPEC PrEventPlugin{
public:
	PrEventPlugin(Player * _player) :player(_player){}
	bool sendEvent(PrEvent e);//id根据事件不同代表不同含义
	bool sendCreatureEvent(PrEvent e, Creature * creature);//id根据事件不同代表不同含义
	bool sendUnitEvent(PrEvent e, Unit * unit);//id根据事件不同代表不同含义
	void sendTeleportEvent(PrEvent e, uint32 mapid);
	Player * player;
	std::vector<uint64> followGuid;
};

#endif