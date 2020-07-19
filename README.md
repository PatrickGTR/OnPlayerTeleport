## Introduction ##
This include is for detect players using teleport hacks. Maybe fly hack and airbreak can be detected.

## Requeriments ##
* [Foreach](https://github.com/karimcambridge/samp-foreach/releases) - Put the include file in pawno/include folder
* [SAfull.hmap](http://www.mediafire.com/file/2b19ske1h8jd44f/SAfull.hmap) - Put the map file in scriptfiles folder

## Credits ##
* [PatrickGRT](https://github.com/PatrickGTR): Original code creator
* [Walter-Correa](https://github.com/Walter-Correa): Improvments and fixes
* [Pottus](https://github.com/Pottus): GetVehicleSpeed
* [karimcambridge](https://github.com/karimcambridge): Foreach include
* [RyDeR](https://forum.sa-mp.com/showthread.php?t=273263): GetPointZPos

## Callback ##
You can try reset to the old position or/and warn/kick/ban

    public OnPlayerTeleport(playerid, Float:distance, Float:oldx, Float:oldy, Float:oldz)
    {
    	if(IsPlayerInAnyVehicle(playerid) && GetPlayerState(playerid) == PLAYER_STATE_DRIVER) SetVehiclePos(GetPlayerVehicleID(playerid), oldx, oldy, oldz);
    	else SetPlayerPos(playerid, oldx, oldy, oldz);
    	return 1;
    }
