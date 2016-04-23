#include "pr_event_plugin.h"
#include "Mercenary.h"
#include "TemporarySummon.h"
#include "DBCStores.h"
#include "Config/Config.h"

class DelayedHandleLogin :public DelayedAction
{
	public:
		explicit DelayedHandleLogin(int _timelimit, Player* _player) :DelayedAction(_timelimit), player(_player){}
		void run() override{
			player->_sendInprogressQuestList();
		}
		Player* player;
};
class DelayedInitPetActionBar :public DelayedAction
{
public:
	explicit DelayedInitPetActionBar(int _timelimit, Player* _player) :DelayedAction(_timelimit), player(_player){}
	void run() override{
		//if (Pet * pet = player->GetPet())
			//player->PetSpellInitialize();
		if (Pet * pet = player->GetPet())
		{
			
			player->PetSpellInitialize();
			if (pet->isMercenary())
			{
				if (Mercenary * mercenary = player->context.GetMercenary())
				{
					mercenary->disableMirroClone = false;

					if (mercenary->isRangedAttack())
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, mercenary->gearContainer[SLOT_RANGED].itemid,true);
					else
					{
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, mercenary->gearContainer[SLOT_MAIN_HAND].itemid, true);
						pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, mercenary->gearContainer[SLOT_OFF_HAND].itemid, true);
					}
					mercenary->SendMirrorImagePacket(pet);
				}
			}
		}
	}
	Player* player;
};

void updateMapDifficultyMultiplier(Player * player, uint32 mapid)
{
	if (MapEntry const * map = sMapStore.LookupEntry(mapid))
		{
			MapDifficultyEntry const* mapDiff = nullptr;
			if (map->IsRaid())
				mapDiff = GetMapDifficultyData(mapid, player->GetDifficulty(true));
			else if (map->IsDungeon())
				mapDiff = GetMapDifficultyData(mapid, player->GetDifficulty(false));
			
			uint32 maxPlayers = mapDiff ? mapDiff->maxPlayers : 1;

			float oldHealth = player->context.mapDifficultyHealthMultiplier;
			float oldDamage = player->context.mapDifficultyDamageMultiplier;

			if (maxPlayers > 1)
			{
				if (Group * group = player->GetGroup())
				{
					player->context.mapDifficultyHealthMultiplier = sConfig.getPLAYER_MAP_DIFFICULTY_HEALTH_RATE() * float(maxPlayers) / float(group->GetMembersCount());//0.5 * 5 /2 表示，5人本2人玩的情况，0.5为修正值。
					player->context.mapDifficultyDamageMultiplier = sConfig.getPLAYER_MAP_DIFFICULTY_DAMAGE_RATE() * float(maxPlayers) / float(group->GetMembersCount());//0.5 * 5 /2 表示，5人本2人玩的情况，0.5为修正值。
				}
				else
				{
					player->context.mapDifficultyHealthMultiplier = sConfig.getPLAYER_MAP_DIFFICULTY_HEALTH_RATE() * float(maxPlayers);//初始化为配置中的地图难度倍率;
					player->context.mapDifficultyDamageMultiplier = sConfig.getPLAYER_MAP_DIFFICULTY_DAMAGE_RATE() * float(maxPlayers);//初始化为配置中的地图难度倍率;
				}
			}
			else
			{
				player->context.mapDifficultyHealthMultiplier = 1.0f;//非副本的情况，总是原始倍率
				player->context.mapDifficultyDamageMultiplier = 1.0f;//非副本的情况，总是原始倍率
			}
			if (player->context.mapDifficultyHealthMultiplier - oldHealth != 0)
			{
				player->UpdateMaxHealth();
				
				if (Pet *pet = player->GetPet())
				{
					if (pet->isMercenary())
					{
						((MercenaryPet*)pet)->UpdateMaxHealth();
					}
				}
			}			
			if (player->context.mapDifficultyDamageMultiplier - oldDamage != 0){
				player->UpdateAttackPowerAndDamage(player->GetSheath() == SHEATH_STATE_RANGED);//角色根据是否远程的伤害类型更新伤害数值
				if (Pet *pet = player->GetPet())
				{
					if (pet->isMercenary())
					{
						((MercenaryPet*)pet)->UpdateAttackPowerAndDamage(player->context.GetMercenary()->isRangedAttack());//雇佣兵根据是否远程计算攻击
					}
				}

			}

		}
}
bool PrEventPlugin::sendEvent(PrEvent e){
	switch (e)
	{
		case P_LOGIN_EVENT:
			//player->context.addDelayedAction(new DelayedHandleLogin(5000, player));//并不能解决问题，任务从跟踪列表去掉，但在日志中存在
			if (player->context.displayid>0)
				player->SetDisplayId(player->context.displayid);//易容

			break;
		case P_LOGOUT_EVENT:
			player->context.ClearMercenary();//客户端清理
			break;
		case P_RESUMMON_PET_EVENT:		
			if (Pet * pet = player->GetPet())
			{
				if (pet->isMercenary())
				{
					if (Mercenary * mercenary = player->context.GetMercenary())
					{
						mercenary->disableMirroClone = true;

						if (mercenary->isRangedAttack())
							pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 2, 0, true);
						else
						{
							pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0, true);
							pet->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID + 1, 0, true);
						}
						
						mercenary->SendMirrorImagePacket(pet, false);
						player->context.addDelayedAction(new DelayedInitPetActionBar(1000, player));//3秒，初始化						
					}
				}
			}
			break;
		case P_PET_ABANDON_EVENT:
		{	
			Pet * pet = player->GetPet();
			if (pet->isMercenary())
			{
				Mercenary * mercenary = player->context.GetMercenary();
				for (auto it = mercenary->gearContainer.begin(); it != mercenary->gearContainer.end(); it++)
				{
					if (it->second.itemguid > 0)
					{
						ChatHandler(player).SendSysMessage(-2800696);
						return false;
					}
				}
			}
			break;
		}

	}

	return true;
}
void PrEventPlugin::sendTeleportEvent(PrEvent e, uint32 mapid){
	updateMapDifficultyMultiplier(player,mapid); //依据地图难度更新血量，攻击。
}
bool PrEventPlugin::sendCreatureEvent(PrEvent e, Creature * creature)
{
	switch (e)
	{
		case P_SUMMON_CREATURE:
		switch (creature->GetCreatureInfo()->Entry)
		{
			case 28845://收割者的礼物12698：被控制的血色矿工28845
				player->KilledMonster(creature->GetCreatureInfo(), creature->GetObjectGuid());
				break;
			case 39466://任务：好侏儒寥寥25229：NPC:被鼓动的平民39466
			case 39624://任务：好侏儒寥寥25229：NPC:被鼓动的平民39624
			case 40264://任务：巨魔志愿兵25461: NPC:巨魔志愿者:40264
			case 40260://任务：巨魔志愿兵25461: NPC:巨魔志愿者:40260

			float x = creature->GetPositionX();
			float y = creature->GetPositionY();
			float z = creature->GetPositionZ();
			float a = creature->GetAngle(player);

			int dist = 1 + rand() % 5;  /*1-4*/
			int angle = 3.1415926 * 2 * (1 + rand() % 11) / 10; /*(1-10)/10*/

			followGuid.push_back(creature->GetObjectGuid().GetRawValue());
			creature->GetMotionMaster()->MoveFollow(player, dist, angle);
			creature->SendPetAIReaction();
				
				break;
		}
		break;
		case P_GOSSIP_CREATURE:
		{
			
		}
		break;
	}
	return true;
}

bool PrEventPlugin::sendUnitEvent(PrEvent e, Unit * unit)//id根据事件不同代表不同含义
{
	switch (e){
	case P_SELECT_UNIT:
		switch (unit->GetEntry())
		{
			case 40253://任务：巨魔志愿兵25461,NPC:勇士乌拉金40253	
				{	
					/*ObjectGuid guid;
					for (auto it = followGuid.begin(); it != followGuid.end(); it++)
					{
						guid.Set(*it);
						if (Creature* follower = player->GetMap()->GetCreature(guid))
						{
							player->KilledMonsterCredit(40260);
							((TemporarySummon*)follower)->UnSummon();
						}
					}*/
					followGuid.clear();
					break;
				}
				break;
			}
			case 39675://任务：好侏儒寥寥25229,NPC:崔德·火钻队长39675	
			{
				ObjectGuid guid;
				for (auto it = followGuid.begin(); it != followGuid.end(); it++)
				{
					guid.Set(*it);
					if (Creature* follower = player->GetMap()->GetCreature(guid))
					{
						player->KilledMonsterCredit(39466);
						((TemporarySummon*)follower)->UnSummon();
					}
				}
				followGuid.clear();
				break;
			}
				break;
	}
	return true;
}
