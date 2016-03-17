#include "precompiled.h"

void hearthstone_prepare_gmtools(Player* pPlayer, Item* pItem, uint32 uiAction){

	pPlayer->PrepareGossipMenu(pPlayer, 65535);
	
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "update zone of quest_poi_points", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 100);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "update zone of z_quest_npcgo_all_map", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 101);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TRAINER, "update zone of Parallel for z_quest_npcgo_all_map", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 107);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_BATTLE, "cheat god on/off", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 102);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "modify aspeed 5", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 103);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "modify aspeed 10", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 104);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_1, "modify aspeed 1", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 105);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "gm fly on/off", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 106);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_DOT, "clear bag", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 108);
	pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_TALK, -2800181, GOSSIP_SENDER_MAIN, 999);//返回主菜单
	pPlayer->SEND_GOSSIP_MENU(16777210, pItem->GetObjectGuid()); //利用原力直达游戏目标。
}
void modifySpeed(Player* pPlayer, float modSpeed){
	pPlayer->UpdateSpeed(MOVE_WALK, true, modSpeed);
	pPlayer->UpdateSpeed(MOVE_RUN, true, modSpeed);
	pPlayer->UpdateSpeed(MOVE_SWIM, true, modSpeed);
	// chr->UpdateSpeed(MOVE_TURN,   true, modSpeed);
	pPlayer->UpdateSpeed(MOVE_FLIGHT, true, modSpeed);
}
void removeAllItem(Player* player){
	for (int i = INVENTORY_SLOT_ITEM_START; i < INVENTORY_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		player->RemoveItem(INVENTORY_SLOT_BAG_0,i, false);

	for (int i = INVENTORY_SLOT_BAG_START; i < INVENTORY_SLOT_BAG_END; ++i)
	if (Bag* pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* pItem = pBag->GetItemByPos(j))
		player->RemoveItem(INVENTORY_SLOT_BAG_0, j, false);

	for (int i = BANK_SLOT_ITEM_START; i < BANK_SLOT_ITEM_END; ++i)
	if (Item* pItem = player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
		player->RemoveItem(INVENTORY_SLOT_BAG_0, i, false);

	for (int i = BANK_SLOT_BAG_START; i < BANK_SLOT_BAG_END; ++i)
	if (Bag* pBag = (Bag*)player->GetItemByPos(INVENTORY_SLOT_BAG_0, i))
	for (uint32 j = 0; j < pBag->GetBagSize(); ++j)
	if (Item* pItem = pBag->GetItemByPos(j))
		player->RemoveItem(INVENTORY_SLOT_BAG_0, j, false);


	int32 count=1;
	uint32 noSpaceForCount = 0;
	ItemPosCountVec dest;
	uint8 msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, 6948, 1, &noSpaceForCount);
	if (msg != EQUIP_ERR_OK)                                // convert to possible store amount
		count -= noSpaceForCount;

	Item* item = player->StoreNewItem(dest, 6948, true, Item::GenerateItemRandomPropertyId(6948));

}
void hearthstone_click_gmtools(Player* pPlayer, Item* pItem, uint32 uiAction){
	switch (uiAction )
	{
		case GOSSIP_ACTION_INFO_DEF + 100:
			pPlayer->context.calculateZone_quest_poi_points();
			break;
		case GOSSIP_ACTION_INFO_DEF + 101:
			pPlayer->context.calculateZone_quest_npcgo_all_map();
			break;
		case GOSSIP_ACTION_INFO_DEF + 102:
			pPlayer->SetGodCheater(!pPlayer->isGodCheater());
			break;
		case GOSSIP_ACTION_INFO_DEF + 103:
			modifySpeed(pPlayer, 5);
			break;
		case GOSSIP_ACTION_INFO_DEF + 104:
			modifySpeed(pPlayer,10);
			break;
		case GOSSIP_ACTION_INFO_DEF + 105:
			modifySpeed(pPlayer, 1);
			break;
		case GOSSIP_ACTION_INFO_DEF + 106:
			pPlayer->SetCanFly(!pPlayer->IsFlying());
			break;
		case GOSSIP_ACTION_INFO_DEF + 107:
			pPlayer->context.calculateParallelZone_quest_npcgo_all_map();
			break;
		case GOSSIP_ACTION_INFO_DEF + 108:
			removeAllItem(pPlayer);
	}
}
