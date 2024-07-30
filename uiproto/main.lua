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
   love.graphics.clear(255, 255, 255)

end

function love.keypressed(key, scancode, isrepeat)
   if key == "escape" then love.event.quit() end
end

-- vim:ts=3:sts=3:sw=3:noexpandtab
