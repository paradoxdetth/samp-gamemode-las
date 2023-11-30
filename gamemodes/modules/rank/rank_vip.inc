#if defined rank_vip_included
    #endinput
#endif
#define rank_vip_included

#include <YSI_Coding\y_hooks>

static
    user_admin[MAX_PLAYERS],
    user_owner[MAX_PLAYERS],
    user_supporter[MAX_PLAYERS],
    user_vip[MAX_PLAYERS],
    user_viptime[MAX_PLAYERS];

static 
    vip_time_limit[MAX_PLAYERS];

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", user_admin[playerid]);
    INI_Int("Owner", user_owner[playerid]);
    INI_Int("Supporter", user_supporter[playerid]);
    INI_Int("Vip", user_vip[playerid]);
    INI_Int("Viptime", user_viptime[playerid]);

    return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File,"Vip", user_vip[playerid]);
    INI_WriteInt(File,"Viptime", user_viptime[playerid]);
    INI_Close(File);

    vip_time_limit[playerid] = 0;

    return 1;
}

YCMD:makevip(playerid, const string:params[], help) 
{
    if(user_admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
    
    static targetid, time, level;

    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Igrac nije na serveru.");

    if(sscanf(params, "iii", targetid, time, level)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/makevip [igrac] [vreme(sati)] [level]");

    if(level < 0 || level > 4) return SendClientMessage(playerid, y_blue, "( HELP ) "c_white"Netacan vip level ( 0-4 ).");

    if(!level)
    {
        user_vip[playerid] = 0;
        user_viptime[playerid] = 0;

        SendClientMessage(targetid, y_green, "( INFO ) "c_white"Skinuta vam je pozicija Vipa.");
        SendClientMessage(playerid, y_green, "( INFO ) "c_white"Postavili ste poziciju vipa igracu %s.", ReturnPlayerName(targetid));
    }
    else
    {
        user_vip[playerid] = level;
        user_viptime[playerid] = time;

        SendClientMessage(targetid, y_green, "( INFO ) "c_white"Postavljena vam je pozicija Vipa na %d sati.", time);
        SendClientMessage(playerid, y_green, "( INFO ) "c_white"Postavili ste poziciju vipa igracu %s.", ReturnPlayerName(targetid));
    }

    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File,"Vip", user_vip[playerid]);
    INI_WriteInt(File,"Viptime", user_viptime[playerid]);
    INI_Close(File);

    return 1;
}

YCMD:vgoto(playerid, const string:params[], help) 
{
    static
		targetid,
		Float:Position_X, 
		Float:Position_Y, 
		Float:Position_Z;

    new interiorid = GetPlayerInterior(targetid);
        
    if(!user_vip[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
    
    if(sscanf(params, "u", targetid)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/vgoto [targetid]");

    if(vip_time_limit[playerid] > 0) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Mozete se portati do igraca za %d sekundi", vip_time_limit[playerid]);

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

    if(user_admin[targetid] >= 1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ne mozete se portati do admina.");
    if(user_owner[targetid] >= 1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ne mozete se portati do ownera.");
    if(user_supporter[targetid] >= 1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ne mozete se portati do supportera.");

    vip_time_limit[playerid] = 60;
    
	GetPlayerPos(targetid, Position_X, Position_Y, Position_Z);

	SetPlayerPos(playerid, Position_X, Position_Y+1, Position_Z+1);

	SetPlayerInterior(playerid, interiorid);
    
    return 1;
}
