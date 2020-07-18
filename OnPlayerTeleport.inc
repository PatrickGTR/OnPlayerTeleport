#include <a_samp>
#include <YSI\y_bit>
#include <foreach>

static stock
Float:AC_Position[MAX_PLAYERS][3],
pLastPosTick[MAX_PLAYERS],
BitArray:AC_SafeTP<MAX_PLAYERS>
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
	if((gettime( )-pLastPosTick[playerid]) > 0) //updates the player location every 1 second.
	{
		//on-foot
		if( !IsPlayerInRangeOfPoint(playerid, 50.0, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]) && !Bit_Get(AC_SafeTP, playerid) && !IsPlayerNPC(playerid) 
		&& GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
		{
			new Float:dist = GetPlayerDistanceFromPoint(playerid, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
			CallLocalFunction("OnPlayerTeleport", "iffff", playerid, dist, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
			pLastPosTick[playerid] = gettime()+1;
			return 1;
		}
		//on-vehicle
		else if( !IsPlayerInRangeOfPoint(playerid, 300.0, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]) && !Bit_Get(AC_SafeTP, playerid) && !IsPlayerNPC(playerid) 
		&& GetVehicleSpeed( GetPlayerVehicleID(playerid) ) <= 50 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER || GetPlayerState(playerid) == PLAYER_STATE_PASSENGER)
		{
			new Float:dist = GetPlayerDistanceFromPoint(playerid, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
			CallLocalFunction("OnPlayerTeleport", "iffff", playerid, dist, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
			pLastPosTick[playerid] = gettime()+1;
			return 1;
		}
		GetPlayerPos(playerid, AC_Position[playerid][0], AC_Position[playerid][1], AC_Position[playerid][2]);
		Bit_Set(AC_SafeTP, playerid, false);
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
	if(AC_Position[playerid][0] != PosX && AC_Position[playerid][1] != PosY && AC_Position[playerid][2] != PosZ) Bit_Set(AC_SafeTP, playerid, true);
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
			if(AC_Position[i][0] != PosX && AC_Position[i][1] != PosY && AC_Position[i][2] != PosZ) Bit_Set(AC_SafeTP, i, true);
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
	if(AC_Position[playerid][0] != PosX && AC_Position[playerid][1] != PosY && AC_Position[playerid][2] != PosZ) Bit_Set(AC_SafeTP, playerid, true);
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
	if(AC_Position[playerid][0] != pos[0] && AC_Position[playerid][1] != pos[1] && AC_Position[playerid][2] != pos[2]) Bit_Set(AC_SafeTP, playerid, true);
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
