global record Position
	x: integer
	y: integer
end

global record GameMap
	terrain: {string}
end

global record Unit
	pos: Position
	type: string
	nation: string
end

global record Game
   map: GameMap
	units: {Unit}
end

-- vim:ts=3:sts=3:sw=3:noexpandtab
