C = require 'civlike'
local rules = require 'sample_rules'
local game

function love.load(args)
	if args[1] ~= nil then
		rules = loadfile(args[1])()
	end
	game = C.create_game(rules)
end

function love.draw()
	love.graphics.print("Hello world", 400, 300)
end

function love.keypressed(key, scancode, isrepeat)
	if key == "escape" then love.event.quit() end
end

-- vim:ts=2:sts=2:sw=2:noexpandtab
