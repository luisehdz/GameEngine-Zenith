SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600

SHOTS_PER_SECOND = 8
MS_PER_SHOT = 1000 / SHOTS_PER_SECOND
BULLET_SPEED = 1000

fireTimer = 0
bullets = {}

function Start()
	bulletImage = {}
	bulletImage.image = LoadImage("gme/bullet.png")
	bulletImage.size = 32

	groundTile = {}
	groundTile.image = LoadImage("gme/ground.png")
	groundTile.size = 32

	player = {}
	player.image = LoadImage("gme/player.png")
	player.size = 32
	player.x = SCREEN_WIDTH / 2 - player.size / 2
	player.xVel = 0
	player.xAcc = 0
	player.yVel = 0
	player.yAcc = 0
	player.accelerationInput = 5000
	player.jumpImpulse = 1100
	player.gravity = 8000
	player.isJumping = false
	player.maxSpeed = 512
	player.drag = 10
	player.y = SCREEN_HEIGHT - groundTile.size - player.size
	
	GROUND_HEIGHT = player.y 
end

function Fire()
	fireTimer = fireTimer + MS_PER_SHOT
	
	bullet = {}
	bullet.x = player.x
	bullet.y = player.y
	
	if player.xVel < 0 then
		bullet.xVel = -BULLET_SPEED
		bullet.rotation = 180
	else
		bullet.xVel = BULLET_SPEED
		bullet.rotation = 0
	end
	
	bullets[#bullets + 1] = bullet
end

function UpdateBullets()
	local dt = GetDeltaTime()

	--check for firing
	if IsKeyDown(SDL_SCANCODE_SPACE) and fireTimer == 0 then
		Fire()
	end
	
	-- fire timer cooldown
	if fireTimer > 0 then
		fireTimer = fireTimer - dt * 1000
		if fireTimer < 0 then
			fireTimer = 0
		end
	end
	
	--update bullets
	for i = #bullets, 1, -1 do
		bullets[i].x = bullets[i].x + bullets[i].xVel * dt
		
		if bullets[i].x > SCREEN_WIDTH then
			table.remove(bullets, i)
		elseif bullets[i].x < -bulletImage.size then
			table.remove(bullets, i)
		end
	end
end

function UpdatePlayer()
	local dt = GetDeltaTime()

	-- check for jump input
	if IsKeyDown(SDL_SCANCODE_UP) and not player.isJumping then
		player.yVel = player.yVel - player.jumpImpulse
		player.isJumping = true
	end
	
	-- update acceleration based on input
	if IsKeyDown(SDL_SCANCODE_LEFT) then
		player.xAcc = player.xAcc - player.accelerationInput * dt
	elseif IsKeyDown(SDL_SCANCODE_RIGHT) then
		player.xAcc = player.xAcc + player.accelerationInput * dt
	else
		player.xAcc = 0
	end
	
	-- apply gravity
	player.yAcc = player.yAcc + player.gravity * dt
	
	-- update velocity and position
	player.xVel = player.xVel + player.xAcc * dt
	player.x = player.x + player.xVel * dt
	
	player.yVel = player.yVel + player.yAcc * dt
	player.y = player.y + player.yVel * dt
	
	-- clamp player max speed
	if player.xVel < -player.maxSpeed then
		player.xVel = -player.maxSpeed
	elseif player.xVel > player.maxSpeed then
		player.xVel = player.maxSpeed
	end
	
	-- apply drag
	player.xVel = player.xVel - (player.xVel * player.drag * dt)
	
	-- check for ground collision
	if player.y > GROUND_HEIGHT then
		player.y = GROUND_HEIGHT
		player.isJumping = false
		player.yAcc = 0
		player.yVel = 0
	end
	
	-- lock player to screen
	if player.x < 0 then
		player.x = 0
	elseif player.x > SCREEN_WIDTH - player.size then
		player.x = SCREEN_WIDTH - player.size
	end
end

function Update()
	UpdatePlayer()
	UpdateBullets()
end

function Draw()
	ClearScreen(68, 136, 204)
	
	for i = 1, #bullets do
		DrawImageRotated(bulletImage.image, 0, bullets[i].x, bullets[i].y, bulletImage.size, bullets[i].rotation)
	end
		
	DrawImage(player.image, 0, player.x, player.y, player.size)
	
	local x = 0;
	while x < SCREEN_WIDTH do
		DrawImage(groundTile.image, 0, x, SCREEN_HEIGHT - groundTile.size, groundTile.size)
		x = x + groundTile.size
	end
end