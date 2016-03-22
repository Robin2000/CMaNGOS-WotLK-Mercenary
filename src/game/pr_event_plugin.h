#ifndef PR_EVENT_PLUGIN_H
#define PR_EVENT_PLUGIN_H
enum PrEvent{
	P_LOGIN_EVENT = 0
};
class MANGOS_DLL_SPEC PrEventPlugin{
public:
	PrEventPlugin(Player * _player) :player(_player){}
	void sendEvent(PrEvent e);
	Player * player;
};

#endif