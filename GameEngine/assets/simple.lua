spriteSheetId = 0
spriteId = 781
spriteSize = 32
spritePosX = 32
spritePosY = 400
speed = 1

sound = 0
font = 0

function Start()
	spriteSheetId = LoadImage("tiles1.bmp")
	sound = LoadSound("victory.ogg")
	font = LoadFont("8_bit_pusab.ttf", 24)
	spritePosX = 32
	spritePosY = 300
end

function Update()
	spritePosX = spritePosX + speed
	if spritePosX > 800 then
		speed = 0
		spritePosX = 32
		PlaySound(sound)
	end
end

function Draw()
	ClearScreen(0, 0, 0)
	DrawImage(spriteSheetId, spriteId, spritePosX, spritePostY, spriteSize)
	DrawText("G.Man runs then teleports the game.", 10, 100, font, 255, 255, 255)
end