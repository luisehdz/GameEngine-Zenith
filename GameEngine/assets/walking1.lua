SCREEN_WIDTH = GetClientWidth()
SCREEN_HEIGHT = GetClientHeight()

function Start()
	SetWindowTitle("Player Movement")

	groundTile = {}
	groundTile.image = LoadImage("gme/ground.png")
	groundTile.size = 32

	player = {}
	player.image = LoadImage("gme/player.png")
	player.size = 32
	player.speed = 512
	player.posX = SCREEN_WIDTH / 2 - player.size / 2
	player.posY = SCREEN_HEIGHT - groundTile.size - player.size
end

function Update()
	local dt = GetDeltaTime()
	
	if IsKeyDown(SDL_SCANCODE_LEFT) then
		player.posX = player.posX - player.speed * dt
	elseif IsKeyDown(SDL_SCANCODE_RIGHT) then
		player.posX = player.posX + player.speed * dt
	end
	
	if player.posX < 0 then
		player.posX = 0
	elseif player.posX > SCREEN_WIDTH - player.size then
		player.posX = SCREEN_WIDTH - player.size
	end
end

function Draw()
	ClearScreen(68, 136, 204)
	DrawImage(player.image, 0, player.posX, player.posY, player.size)
	
	local x = 0;
	while x < SCREEN_WIDTH do
		DrawImage(groundTile.image, 0, x, SCREEN_HEIGHT - groundTile.size, groundTile.size)
		x = x + groundTile.size
	end
end