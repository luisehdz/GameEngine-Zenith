SCREEN_WIDTH = GetClientWidth()
SCREEN_HEIGHT = GetClientHeight()

function Start()
	SetWindowTitle("Jumping")

	groundTile = {}
	groundTile.image = LoadImage("gme/ground.png")
	groundTile.size = 32

	player = {}
	player.image = LoadImage("gme/player.png")
	player.size = 32
	player.xPos = SCREEN_WIDTH / 2 - player.size / 2
	player.xPosVel = 0
	player.xPosAcc = 0
	player.accelerationInput = 2048
	player.maxSpeed = 512
	player.drag = 10
	player.yPos = SCREEN_HEIGHT - groundTile.size - player.size
	player.yPosVel = 0
	player.yPosAcc = 0
	player.jumpImpulse = 800
	player.gravity = 8000
	player.jumpCount = 0
	
	GROUND_HEIGHT = player.yPos 
end

function Update()
	local dt = GetDeltaTime()
	
	-- check for jump input
	if IsKeyPressed(SDL_SCANCODE_UP) and player.jumpCount < 2 then
		player.yPosVel = player.yPosVel - player.jumpImpulse
		player.jumpCount = player.jumpCount + 1
	end
	
	-- update acceleration based on input
	if IsKeyDown(SDL_SCANCODE_LEFT) then
		player.xPosAcc = player.xPosAcc - player.accelerationInput * dt
	elseif IsKeyDown(SDL_SCANCODE_RIGHT) then
		player.xPosAcc = player.xPosAcc + player.accelerationInput * dt
	else
		player.xPosAcc = 0
	end
	
	-- apply gravity
	player.yPosAcc = player.yPosAcc + player.gravity * dt
	
	-- update velocity and position
	player.xPosVel = player.xPosVel + player.xPosAcc * dt
	player.xPos = player.xPos + player.xPosVel * dt
	
	player.yPosVel = player.yPosVel + player.yPosAcc * dt
	player.yPos = player.yPos + player.yPosVel * dt
	
	-- clamp player max speed
	if player.xPosVel < -player.maxSpeed then
		player.xPosVel = -player.maxSpeed
	elseif player.xPosVel > player.maxSpeed then
		player.xPosVel = player.maxSpeed
	end
	
	-- apply drag
	player.xPosVel = player.xPosVel - (player.xPosVel * player.drag * dt)
	
	-- check for ground collision
	if player.yPos > GROUND_HEIGHT then
		player.yPos = GROUND_HEIGHT
		player.jumpCount = 0
		player.yPosAcc = 0
		player.yPosVel = 0
	end
	
	-- lock player to screen
	if player.xPos < 0 then
		player.xPos = 0
	elseif player.xPos > SCREEN_WIDTH - player.size then
		player.xPos = SCREEN_WIDTH - player.size
	end
end

function Draw()
	ClearScreen(68, 136, 204)
	DrawImage(player.image, 0, player.xPos, player.yPos, player.size)
	
	local x = 0;
	while x < SCREEN_WIDTH do
		DrawImage(groundTile.image, 0, x, SCREEN_HEIGHT - groundTile.size, groundTile.size)
		x = x + groundTile.size
	end
end