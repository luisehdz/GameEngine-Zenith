SCREEN_WIDTH = GetClientWidth()
SCREEN_HEIGHT = GetClientHeight()

blocks = {}

--Variable declarations and initalized
function Start()
	SetWindowTitle("Breakout")

	player = {}
	player.image = LoadImage("brk/paddle.png")
	player.sizeX = 64
	player.sizeY = 32
	player.speed = 512
	player.xPos = SCREEN_WIDTH / 2 - player.sizeX / 2
	player.yPos = SCREEN_HEIGHT - player.sizeY
	
	ball = {}
	ball.image = LoadImage("brk/ball.png")
	ball.size = 32
	ball.xSpeed = 208
	ball.ySpeed = -208
	ball.xPos = SCREEN_WIDTH / 2 - ball.size / 2
	ball.yPos = SCREEN_HEIGHT - player.sizeY - ball.size
	ball.startYPos = SCREEN_HEIGHT - player.sizeY - ball.size
	ball.speedIncrease = 1.0
	ball.rotation = 1.0
	
	blockImage = {}
	blockImage.image = LoadImage("brk/block.png")
	blockImage.size = 32
	
	font = LoadFont("8_bit_pusab.ttf", 24)
	
	hitBlockSfx = LoadSound("brk/brick_hit.wav")
	hitWallSfx = LoadSound("brk/wall_hit.wav")
	hitPaddleSfx = LoadSound("brk/paddle_hit.wav")
	confirmSfx = LoadSound("brk/confirm.wav");
	bgMusic = LoadMusic("brk/music.wav")
	
	gameStart = false
	gameWon = false
	PlayMusic(bgMusic)
	PlaceBlocks()
end

--Adds a block at an x and y position
function AddBlock(x, y)
	block = {}
	block.xPos = x
	block.yPos = y
	blocks[#blocks + 1] = block
end

--Places the blocks using a while loop
function PlaceBlocks()
	local y = SCREEN_HEIGHT / 4
	
	while y >= blockImage.size do
		local x = blockImage.size
		while x < SCREEN_WIDTH - blockImage.size do
			AddBlock(x, y)
			x = x + blockImage.size
		end
		y = y - blockImage.size
	end
end

--Updates the player location
function UpdatePlayer()
	local dt = GetDeltaTime()
	
	if IsKeyDown(SDL_SCANCODE_LEFT) or IsKeyDown(SDL_SCANCODE_A) then
		player.xPos = player.xPos - player.speed * dt
	elseif IsKeyDown(SDL_SCANCODE_RIGHT) or IsKeyDown(SDL_SCANCODE_D) then
		player.xPos = player.xPos + player.speed * dt
	end
	
	if player.xPos < 0 then
		player.xPos = 0
	elseif player.xPos > SCREEN_WIDTH - player.sizeX then
		player.xPos = SCREEN_WIDTH - player.sizeX
	end
end

--Updates ball movement. If it touches something it turns its speed negative
function UpdateBall()
	local dt = GetDeltaTime()
	
	--Starting the game
	if IsKeyDown(SDL_SCANCODE_UP) or IsKeyDown(SDL_SCANCODE_W) then
		gameStart = true
	end
	
	--If we have not started, then we don't update movement
	if gameStart and not gameWon then
		--Checks hitting the right and left wall
		if ball.xPos > SCREEN_WIDTH - ball.size then
			ball.xSpeed = -ball.xSpeed
			PlaySound(hitWallSfx)
		elseif ball.xPos < 0 then
			ball.xSpeed = -ball.xSpeed
			PlaySound(hitWallSfx)
		end
		
		--Checks hitting the ceiling. If it goes under the floor then we reset the ball
		if ball.yPos < 0 then
			ball.ySpeed = - ball.ySpeed
			PlaySound(hitWallSfx)
		elseif ball.yPos > SCREEN_HEIGHT then
			Reset()
		end
		
		--Checks hitting a block and removes it from the table
		for i = #blocks, 1, -1 do
			if BoxesOverlap(ball.xPos, ball.yPos, ball.size, ball.size, blocks[i].xPos, blocks[i].yPos, blockImage.size, blockImage.size) then
			--if CircleBoxOverlap(ball, blocks[j]) then

				ball.ySpeed = -ball.ySpeed
				--This is so the ball immediately bounces off the block instead of waiting until the end so that the ball doesn't go through multiple blocks
				ball.xPos = ball.xPos + ball.xSpeed * dt
				ball.yPos = ball.yPos + ball.ySpeed * dt
				
				table.remove(blocks, i)
				PlaySound(hitBlockSfx)
			end
		end
		
		--Checks if it hits the paddle/player and then increases speed
		if BoxesOverlap(ball.xPos, ball.yPos, ball.size, ball.size, player.xPos, player.yPos, player.sizeX, player.sizeY) then
		--if CircleBoxOverlap(ball, player) then
			ball.ySpeed = -ball.ySpeed
			PlaySound(hitPaddleSfx)
			if ball.speedIncrease < 2.0 then
				ball.speedIncrease = ball.speedIncrease + 0.2
			end
		end
		
		--Updates the ball location
		ball.xPos = ball.xPos + ball.xSpeed * ball.speedIncrease * dt
		ball.yPos = ball.yPos + ball.ySpeed * ball.speedIncrease * dt
		ball.rotation = ball.rotation + ball.speedIncrease * dt
		
	else
		Reset()
	end
end

--Resets the ball onto the paddle
function Reset()
	ball.xPos = player.xPos
	ball.yPos = ball.startYPos
	ball.ySpeed = -208
	ball.speedIncrease = 1.0
	gameStart = false
end

--Updates the ball and player and checks for user Input
function Update()
	UpdatePlayer()
	UpdateBall()
	
	if IsKeyPressed(SDL_SCANCODE_SPACE) and gameWon then
		Reset()
		PlaySound(confirmSfx)
		PlaceBlocks()
		gameWon = false
	end
	
	if IsKeyPressed(SDL_SCANCODE_R) and not gameWon then
		Reset()
	end
end

--Draws blocks, player, and text to the screen
function Draw()
	ClearScreen(144, 123, 138)
	
	for i = 1, #blocks do
		DrawImage(blockImage.image, 0, blocks[i].xPos, blocks[i].yPos, blockImage.size)
	end
	
	--DrawImage(player.image, 0, player.xPos, player.yPos, player.size)
	DrawImageUneven(player.image, 0, player.xPos, player.yPos, player.sizeX, player.sizeY)
	
	--Checks if there are any blocks left
	if next(blocks) == nil then
		DrawTextWhite("You won!", 158, SCREEN_HEIGHT / 2, font)
		DrawTextWhite("Press ESC to quit", 64, SCREEN_HEIGHT / 2 + 30, font)
		DrawTextWhite("Press Space to", 98, SCREEN_HEIGHT / 2 + 60, font)
		DrawTextWhite("Play Again", 146, SCREEN_HEIGHT / 2 + 90, font)
		gameWon = true
	else
		DrawImageRotated(ball.image, 0, ball.xPos, ball.yPos, ball.size, ball.rotation)
	end
	
	if not gameStart and not gameWon then
		DrawTextWhite("Press UP/W to Begin", 36, SCREEN_HEIGHT / 2, font)
	end
end