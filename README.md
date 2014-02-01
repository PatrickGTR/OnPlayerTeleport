## Introduction ##
This include is created for [San Andreas Multiplayer](www.samp.com) Community, this include detects a player who uses Teleport hacks, you can do what ever you want underneath OnPlayerTeleport callback, OnPlayerTeleport is called when the include detects a player who is using Fly hacks.

## Credits ##
* [IpSBruno](http://forum.sa-mp.com/member.php?u=87608): Hook Method 7.
* [Y_Less](http://forum.sa-mp.com/member.php?u=29176): Hook Method 7 Tutorial and y_bit.

## Callback ##
    public OnPlayerTeleport(playerid, Float:distance)
    {
       SendClientMessage(playerid, -1, "You have been banned for using Teleport Hacks!");
       return Ban(playerid);
    }
