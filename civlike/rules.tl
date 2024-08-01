global record Size
	w: integer
	h: integer
end

global record Position
	x: integer
	y: integer
end

global record Terrain
	name: string
	color: string
end

global record Nation
	name: string
	color: string
end

global record InitialUnit
	type: string
	position: Position
end

global record Player
	nation: string
	initial_units: {InitialUnit}
end

global record UnitType
	name: string
	moves_per_round: integer
end

global record Rules
   map_size: Size
	terrains: {string: Terrain}
	default_terrain: string
	nations: {string: Nation}
	players: {Player}
	unit_types: {string: UnitType}
end

-- vim:ts=3:sts=3:sw=3:noexpandtab
