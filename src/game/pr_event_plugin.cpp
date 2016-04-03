#include "pr_event_plugin.h"
class DelayedHandleLogin :public DelayedAction
{
	public:
		explicit DelayedHandleLogin(int _timelimit, Player* _player) :DelayedAction(_timelimit), player(_player){}
		void run() override{
			player->_sendInprogressQuestList();
		}
		Player* player;
};

void PrEventPlugin::sendEvent(PrEvent e){
	switch (e)
	{
		case P_LOGIN_EVENT:
			//player->context.addDelayedAction(new DelayedHandleLogin(5000, player));//并不能解决问题，任务从跟踪列表去掉，但在日志中存在
			break;
		case P_LOGOUT_EVENT:
			player->context.ClearMercenary();//客户端清理
			break;
	}
}