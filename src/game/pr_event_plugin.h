#ifndef PR_EVENT_PLUGIN_H
#define PR_EVENT_PLUGIN_H
enum PrEvent{
	P_LOGIN_EVENT = 0,
	P_LOGOUT_EVENT = 1,
	P_UNSUMMON_PET_EVENT = 2,
	P_RESUMMON_PET_EVENT = 3,
	P_AI_UPDATE_PET_EVENT = 4,
	P_PET_ABANDON_EVENT =5
};
class MANGOS_DLL_SPEC PrEventPlugin{
public:
	PrEventPlugin(Player * _player) :player(_player){}
	bool sendEvent(PrEvent e);
	Player * player;
};

#endif