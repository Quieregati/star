-- Script for set teleport destination
-- /teleport xxxx, xxxx, x
local teleportSetDestination = TalkAction("/goToUniverse")

function teleportSetDestination.onSay(player, words, param)
	if not player:getGroup():getAccess() or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	if param == "" then
		player:sendCancelMessage("Desired universe required.")
		return false
	end

	local universe = tonumber(param)
	if player:addToUniverse(universe) == false then
		player:sendCancelMessage("Invalid universe.")
		return false
	end

	player:sendTextMessage(
		MESSAGE_EVENT_ADVANCE,
		string.format("Moved to universe %s", universe)
	)

	return true
end

teleportSetDestination:separator(" ")
teleportSetDestination:register()
