#include <a_samp>
#include <YSI\y_commands>
#include <optp>
    
main(){}

public OnGameModeInit()
{
    // People can spawn with either the CJ skin or The Truth skin.
    AddPlayerClass(0, 1958.33, 1343.12, 15.36, 269.15, 26, 36, 28, 150, 0, 0);
    AddPlayerClass(1, 1958.33, 1343.12, 15.36, 269.15, 26, 36, 28, 150, 0, 0);

 	AddStaticVehicle(520, 2109.1763, 1503.0453, 32.2887, 82.2873, 0, 1);
    return 1;
}

YCMD:spp(playerid, params[], help)
{
	SendClientMessage(playerid, -1, "Command: /spp");
	SetPlayerPos(playerid, 1958.33, 1343.12, 15.36);
	return true;
}

YCMD:ppv(playerid, params[], help)
{
	SendClientMessage(playerid, -1, "Command: /ppv");

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid,x,y,z);
    new vehicle = CreateVehicle(411, x, y, z, 90, 0, 1, 60);
    PutPlayerInVehicle(playerid,vehicle,0);


	return true;
}

YCMD:sppf(playerid, params[], help)
{
	SendClientMessage(playerid, -1, "Command: /sppf");
	SetPlayerPosFindZ(playerid, 1958.33, 1343.12, 15.36);
	return true;
}


public OnPlayerTeleport(playerid)
{
	SendClientMessage(playerid, -1, "You have been banned for teleport hacks!");
	return BanEx(playerid, "Teleport Hack");
}
