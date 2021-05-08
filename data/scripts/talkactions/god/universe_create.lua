-- Script for set teleport destination
-- /teleport xxxx, xxxx, x
local teleportSetDestination = TalkAction("/createUniverse")

function teleportSetDestination.onSay(player, words, param)
	if not player:getGroup():getAccess() or player:getAccountType() < ACCOUNT_TYPE_GOD then
		return true
	end

	player:sendTextMessage(
		MESSAGE_EVENT_ADVANCE,
		string.format("Universe %s created", Game.createUniverse(tonumber(param)))
	)

	return true
end

teleportSetDestination:separator(" ")
teleportSetDestination:register()
