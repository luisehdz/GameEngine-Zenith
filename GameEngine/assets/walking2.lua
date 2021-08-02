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
	player.xPos = SCREEN_WIDTH / 2 - player.size / 2
	player.xVel = 0
	player.xAcc = 0
	player.accInput = 1024
	player.maxSpeed = 512
	player.drag = 10
	player.posY = SCREEN_HEIGHT - groundTile.size - player.size
end

function Update()
	local dt = GetDeltaTime()
	
	--update accleration based on input
	if IsKeyDown(SDL_SCANCODE_LEFT) then
		player.xAcc = player.xAcc - player.accInput * dt
	elseif IsKeyDown(SDL_SCANCODE_RIGHT) then
		player.xAcc = player.xAcc + player.accInput * dt
	else
		player.xAcc = 0
	end
	
	--Update velocity and position
	player.xVel = player.xVel + player.xAcc * dt
	player.xPos = player.xPos + player.xVel * dt
	
	--clamp player max speed
	if player.xVel < -player.maxSpeed then
		player.xVel = -player.maxSpeed
	elseif player.xVel > player.maxSpeed then	
		player.xVel = player.maxSpeed
	end
	
	--applies drag
	player.xVel = player.xVel - (player.xVel * player.drag * dt)
	
	--Locks player to the observable screen
	if player.xPos < 0 then
		player.xPos = 0
	elseif player.xPos > SCREEN_WIDTH - player.size then
		player.xPos = SCREEN_WIDTH - player.size
	end
end

function Draw()
	ClearScreen(68, 136, 204)
	DrawImage(player.image, 0, player.xPos, player.posY, player.size)
	
	--Draws the ground
	local x = 0;
	while x < SCREEN_WIDTH do
		DrawImage(groundTile.image, 0, x, SCREEN_HEIGHT - groundTile.size, groundTile.size)
		x = x + groundTile.size
	end
end