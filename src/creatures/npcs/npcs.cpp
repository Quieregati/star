/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2022 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.org/
*/

#include "pch.hpp"

#include "creatures/combat/combat.h"
#include "creatures/combat/spells.h"
#include "creatures/npcs/npcs.h"
#include "creatures/creature.h"
#include "declarations.hpp"
#include "game/game.h"

bool NpcType::canSpawn(const Position& pos)
{
	bool canSpawn = true;
	bool isDay = g_game().gameIsDay();

	if ((isDay && info.respawnType.period == RESPAWNPERIOD_NIGHT) ||
		(!isDay && info.respawnType.period == RESPAWNPERIOD_DAY)) {
		// It will ignore day and night if underground
		canSpawn = (pos.z > MAP_INIT_SURFACE_LAYER && info.respawnType.underground);
	}

	return canSpawn;
}

bool NpcType::loadCallback(LuaScriptInterface* scriptInterface)
{
	int32_t id = scriptInterface->getEvent();
	if (id == -1) {
		SPDLOG_WARN("[NpcType::loadCallback] - Event not found");
		return false;
	}

	info.scriptInterface = scriptInterface;
	switch (info.eventType) {
		case NPCS_EVENT_THINK:
			info.thinkEvent = id;
			break;
		case NPCS_EVENT_APPEAR:
			info.creatureAppearEvent = id;
			break;
		case NPCS_EVENT_DISAPPEAR:
			info.creatureDisappearEvent = id;
			break;
		case NPCS_EVENT_MOVE:
			info.creatureMoveEvent = id;
			break;
		case NPCS_EVENT_SAY:
			info.creatureSayEvent = id;
			break;
		case NPCS_EVENT_PLAYER_BUY:
			info.playerBuyEvent = id;
			break;
		case NPCS_EVENT_PLAYER_SELL:
			info.playerSellEvent = id;
			break;
		case NPCS_EVENT_PLAYER_CHECK_ITEM:
			info.playerLookEvent = id;
			break;
		case NPCS_EVENT_PLAYER_CLOSE_CHANNEL:
			info.playerCloseChannel = id;
			break;
		default:
			break;
	}

	return true;
}

void NpcType::loadShop(NpcType* npcType, ShopBlock shopBlock)
{
	ItemType & iType = Item::items.getItemType(shopBlock.itemId);

	// Registering item prices globaly.
	if (shopBlock.itemSellPrice > iType.sellPrice) {
		iType.sellPrice = shopBlock.itemSellPrice;
	}
	if (shopBlock.itemBuyPrice > iType.buyPrice) {
		iType.buyPrice = shopBlock.itemBuyPrice;
	}
	
	if (shopBlock.childShop.empty()) {
		bool isContainer = iType.isContainer();
		if (isContainer) {
			for (ShopBlock child : shopBlock.childShop) {
				shopBlock.childShop.push_back(child);
			}
		}
		npcType->info.shopItemVector.push_back(shopBlock);
	} else {
		npcType->info.shopItemVector.push_back(shopBlock);
	}
}

NpcType* Npcs::getNpcType(const std::string& name, bool create /* = false*/)
{
	std::string key = asLowerCaseString(name);
	auto it = npcs.find(key);

	if (it != npcs.end()) {
		return it->second;
	}

	if (!create) {
		return nullptr;
	}

	npcs[key] = new NpcType(name);

	return npcs[key];
}
