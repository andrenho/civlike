package.path = package.path .. ";../?.lua;lib/?.lua"

C       = require 'civlike'
inspect = require 'inspect'

rules = require 'sample_rules'
game  = nil

TILE_SIZE = 32

function love.load(args)
   if args[1] ~= nil then
      rules = loadfile(args[1])()
   end
   game = C.create_game(rules)
end

local function tocolor(color)
	return { tonumber(color:sub(2, 3), 16) / 255.0, tonumber(color:sub(4, 5), 16) / 255.0, tonumber(color:sub(6, 7), 16) / 255.0 }
end

local function draw_tile(x, y)
	local id = string.sub(game.map.terrain[y], x, x)
	local color = rules.terrains[id].color
	love.graphics.setColor(unpack(tocolor(color)))
	love.graphics.rectangle("fill", (x-1) * TILE_SIZE, (y-1) * TILE_SIZE, TILE_SIZE, TILE_SIZE)
	love.graphics.setColor(0, 0, 0)
	love.graphics.points((x-1) * TILE_SIZE, (y-1) * TILE_SIZE)
end

function love.draw()
   love.graphics.clear(255, 255, 255)
	for x=1,rules.map_size.w do
		for y=1,rules.map_size.h do
			draw_tile(x, y)
		end
	end
end

function love.keypressed(key, scancode, isrepeat)
   if key == "escape" then love.event.quit() end
	if key == "g" then print(inspect(game)) end
end

-- vim:ts=3:sts=3:sw=3:noexpandtab
