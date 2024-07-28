C = require 'civlike'

function love.draw()
	love.graphics.print("Hello world", 400, 300)
end

function love.keypressed(key, scancode, isrepeat)
	if key == "escape" then love.event.quit() end
end

-- vim:ts=2:sts=2:sw=2:noexpandtab
