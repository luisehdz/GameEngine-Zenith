function Start()
	player = {}
	player.size = 32
	player.speed = 256
	player.image = LoadImage("gme/player.png")
	player.x = GetClientWidth() / 2 - player.size
	player.y = GetClientHeight() / 2 - player.size
	
	SetWindowTitle("ghost blop moving")
end

function Update()
	local dt = GetDeltaTime()
	local distance = player.speed * dt
	
	if IsKeyDown(SDL_SCANCODE_LEFT) then
        player.x = player.x - distance;
    end
    
    if IsKeyDown(SDL_SCANCODE_RIGHT) then
        player.x = player.x + distance
    end
	
	if IsKeyDown(SDL_SCANCODE_UP) then
        player.y = player.y - distance;
    end
    
    if IsKeyDown(SDL_SCANCODE_DOWN) then
        player.y = player.y + distance
    end
end

function Draw()
	ClearScreen(68, 136, 204)
    DrawImage(player.image, 0, player.x, player.y, player.size)
end

