require 'game'
require 'rules'

local function create_game(rules: Rules) : (Game, boolean)

	local function create_map() : GameMap
		local terrains : {string} = {}
		for y=1,rules.map_size.h do
			terrains[y] = string.rep(rules.default_terrain, rules.map_size.w)
		end
		return { terrain = terrains }
	end

	local function create_units() : {Unit}
		return {}
	end
	
	local game : Game = {}
	game.map = create_map()
	game.units = create_units()
	return game, true

end

return {
	create_game = create_game
}

-- vim:ts=3:sts=3:sw=3:noexpandtab
