return {
   map_size = { w=10, h=10 },

	terrains = {
		G = { name = "Grassland", color = "#4dff4d" },
	},
	default_terrain = "G",

	nations = {
		B = { name = "Brazil", color = "#990000" },
	},

	players = {
		{ 
			nation = "B",
			initial_units = {
				{ type = "C", position = { x=5, y=5 } },
			},
		},
	},

	unit_types = {
		C = { name = "Colonist", moves_per_round = 1 },
	}

}

-- vim:ts=3:sts=3:sw=3:noexpandtab
