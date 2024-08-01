require 'game'
require 'rules'

return {
   create_game = function(rules: Rules) : Game

		local function create_map() : GameMap
			local terrains : {string} = {}
			for y=1,rules.map_size.h do
				terrains[y] = string.rep(rules.default_terrain, rules.map_size.w)
			end
			return { terrain = terrains }
		end
		
		local game : Game = {}
		game.map = create_map()
		return game

   end
}

-- vim:ts=3:sts=3:sw=3:noexpandtab
