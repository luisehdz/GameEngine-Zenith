baseAssetDirectory = GetBaseAssetDirectory()

--core scripts
dofile(baseAssetDirectory .. "scanCodes.lua")
dofile(baseAssetDirectory .. "gameMath.lua")

--per-project scripts
dofile(baseAssetDirectory .. "breakout.lua")