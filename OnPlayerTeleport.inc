#include <a_samp>
#include <foreach>

static stock
Float:AC_Position[MAX_PLAYERS][3],
pLastPosTick[MAX_PLAYERS],
AC_SafeTP[MAX_PLAYERS]
;

forward OnPlayerTeleport(playerid, Float:distance, Float:oldx, Float:oldy, Float:oldz);

public OnPlayerSpawn(playerid)
{
	GetPlayerPos(playerid, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
	#if defined pds_OnPlayerSpawn
	pds_OnPlayerSpawn(playerid);
	#endif
	return true;
}
#if defined _ALS_OnPlayerSpawn
#undef OnPlayerSpawn  
#else
#define _ALS_OnPlayerSpawn
#endif
#define OnPlayerSpawn pds_OnPlayerSpawn    
#if defined pds_OnPlayerSpawn
forward pds_OnPlayerSpawn(playerid);
#endif

public OnPlayerUpdate(playerid)
{  
	if((gettime()-pLastPosTick[playerid]) > 0 && !IsPlayerNPC(playerid)) //updates the player location every 1 second.
	{
		new Float:dist = GetPlayerDistanceFromPoint(playerid, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
		//on-foot
		if(dist > 50 && !AC_SafeTP[playerid] && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && !IsPlayerFalling(playerid) && !IsPlayerParachuting(playerid))
		{
			CallLocalFunction("OnPlayerTeleport", "iffff", playerid, dist, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
			pLastPosTick[playerid] = gettime()+1;
			return 1;
		}
		//on-vehicle
		else if((GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER) && !AC_SafeTP[playerid])
		{
			new Float:speed = GetVehicleSpeed(GetPlayerVehicleID(playerid));
			if((dist > 300 && speed <= 50) || (dist > 50 && speed <= 10))
			{
				CallLocalFunction("OnPlayerTeleport", "iffff", playerid, dist, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
				pLastPosTick[playerid] = gettime()+1;
				return 1;
			}
		}
		GetPlayerPos(playerid, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
		AC_SafeTP[playerid] = 0;
		pLastPosTick[playerid] = gettime()+1;
	}
	#if defined pds_OnPlayerUpdate
	pds_OnPlayerUpdate(playerid);
	#endif
	return true;
}
#if defined _ALS_OnPlayerUpdate
#undef OnPlayerUpdate
#else
#define _ALS_OnPlayerUpdate
#endif
#define OnPlayerUpdate pds_OnPlayerUpdate  
#if defined pds_OnPlayerUpdate
forward pds_OnPlayerUpdate(playerid);
#endif

stock pds_SetPlayerPos(playerid, Float:PosX, Float:PosY, Float:PosZ)
{
	if(AC_Position[playerid][0] != PosX && AC_Position[playerid][1] != PosY && AC_Position[playerid][2] != PosZ) AC_SafeTP[playerid] = 1;
	AC_Position[playerid][0] = PosX,AC_Position[playerid][1] = PosY,AC_Position[playerid][2] = PosZ;
	return SetPlayerPos(playerid, PosX, PosY, PosZ);
}
#if defined _ALS_SetPlayerPos
#undef SetPlayerPos
#else
#define _ALS_SetPlayerPos
#endif
#define SetPlayerPos pds_SetPlayerPos

stock pds_SetVehiclePos(vehicleid, Float:PosX, Float:PosY, Float:PosZ)
{   
	foreach(new i : Player) 
	{
		if(IsPlayerInVehicle(i,vehicleid))
		{
			if(AC_Position[i][0] != PosX && AC_Position[i][1] != PosY && AC_Position[i][2] != PosZ) AC_SafeTP[i] = 1;
			AC_Position[i][0] = PosX,AC_Position[i][1] = PosY,AC_Position[i][2] = PosZ;
		}
	}
	return SetVehiclePos(vehicleid, PosX, PosY, PosZ);
}
#if defined _ALS_SetVehiclePos
#undef SetVehiclePos
#else
#define _ALS_SetVehiclePos
#endif
#define SetVehiclePos pds_SetVehiclePos

stock pds_SetPlayerPosFindZ(playerid, Float:PosX, Float:PosY, Float:PosZ) 
{
	if(AC_Position[playerid][0] != PosX && AC_Position[playerid][1] != PosY && AC_Position[playerid][2] != PosZ) AC_SafeTP[playerid] = 1;
	AC_Position[playerid][0] = PosX,AC_Position[playerid][1] = PosY,AC_Position[playerid][2] = PosZ;
	return SetPlayerPosFindZ(playerid, PosX, PosY, PosZ);
}
#if defined _ALS_SetPlayerPosFindZ
#undef SetPlayerPosFindZ  
#else
#define _ALS_SetPlayerPosFindZ
#endif
#define SetPlayerPosFindZ pds_SetPlayerPosFindZ

stock pds_PutPlayerInVehicle(playerid, vehicleid, seatid) 
{
	new Float:pos[3];
	GetVehiclePos(vehicleid, pos[0], pos[1], pos[2]);
	if(AC_Position[playerid][0] != pos[0] && AC_Position[playerid][1] != pos[1] && AC_Position[playerid][2] != pos[2]) AC_SafeTP[playerid] = 1;
	AC_Position[playerid][0] = pos[0],AC_Position[playerid][1] = pos[1],AC_Position[playerid][2] = pos[2];
	return PutPlayerInVehicle(playerid, vehicleid, seatid);
}
#if defined _ALS_PutPlayerInVehicle
#undef PutPlayerInVehicle  
#else
#define _ALS_PutPlayerInVehicle
#endif
#define PutPlayerInVehicle pds_PutPlayerInVehicle

forward Float:GetVehicleSpeed(vehicleid);
stock Float:GetVehicleSpeed(vehicleid)//Km/h
{
	new Float:SPos[3],Float:speed;
	GetVehicleVelocity(vehicleid, SPos[0], SPos[1], SPos[2]);
	speed = floatmul(floatsqroot(floatadd(floatadd(floatpower(SPos[0], 2), floatpower(SPos[1], 2)),  floatpower(SPos[2], 2))), 181.5);
	return speed;
}

stock IsPlayerFalling(playerid)
{
	new animlib[32], animname[32];
	GetAnimationName(GetPlayerAnimationIndex(playerid),animlib,32,animname,32);
	if(IsSameStr(animlib, "PED"))
	{
	    if(IsSameStr(animname, "FALL_back") || IsSameStr(animname, "FALL_collapse") ||
	    IsSameStr(animname, "FALL_fall") || IsSameStr(animname, "FALL_front") ||
	    IsSameStr(animname, "FALL_glide")) return 1;
	}
	return 0;
}

stock IsPlayerParachuting(playerid)
{
	new animlib[32], animname[32];
	GetAnimationName(GetPlayerAnimationIndex(playerid),animlib,32,animname,32);
	if((IsSameStr(animlib, "PARACHUTE") || (IsSameStr(animlib, "PED") &&
	IsSameStr(animname, "FALL_skyDive"))) && GetPlayerWeapon(playerid) == 46) return 1;
	return 0;
}

stock IsSameStr(string[], compare[], bool:ignorecase=true, length=cellmax)
{
    if(!strcmp(string, compare,  ignorecase, length)) return 1;
    return 0;
}
