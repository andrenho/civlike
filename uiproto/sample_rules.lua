return {

   map_size = { 10, 10 },

   terrains = {
      G = { name = "Grassland", color = "#a5f5b3" },
   },
	default_terrain = "G",

	nations = {
		B = { name = "Brazil", color = "#990000" },
	}

	players = {
		{ 
			nation = "B",
			initial_units = {
				type = "C", position = { 5, 5 },
			},
		},
	}

	unit_types = {
		C = { name = "Colonist", moves_per_round = 1 },
	}

}

-- vim:ts=3:sts=3:sw=3:noexpandtab
