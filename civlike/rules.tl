global record Size
	w: integer
	h: integer
end

global record Terrain
	name: string
	color: string
end

global record Rules
   map_size: Size
	terrains: {string: Terrain}
	default_terrain: string
end

-- vim:ts=3:sts=3:sw=3:noexpandtab
