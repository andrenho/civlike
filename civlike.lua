local function check_rules(rules)
end

return {

	create_game = function(rules)
		check_rules(rules)
		local game = {}
		return game
	end,

}

-- vim:ts=2:sts=2:sw=2:noexpandtab
