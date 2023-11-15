#if defined rank_staff_included
    #endinput
#endif
#define rank_staff_included


#include <YSI_Coding\y_hooks>

static
    user_Admin[MAX_PLAYERS],
    user_Skin[MAX_PLAYERS],
    user_Owner[MAX_PLAYERS],
    user_Duty[MAX_PLAYERS];

new bool:admin_Duty[MAX_PLAYERS];
new admin_Vehicle[MAX_PLAYERS] = { INVALID_VEHICLE_ID, ... };

timer DutyTimer[60000](playerid)
{
	if(admin_Duty[playerid])
	{
		user_Duty[playerid]++;
		defer DutyTimer(playerid);
	}
    return 1;
}

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", user_Admin[playerid]);
    INI_Int("Owner", user_Owner[playerid]);
    INI_Int("DutyTime", user_Duty[playerid]);

    return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File,"Admin", user_Admin[playerid]);
    INI_WriteInt(File,"Owner", user_Owner[playerid]);
    INI_WriteInt(File,"DutyTime", user_Duty[playerid]);
    INI_Close(File);   

    DestroyVehicle(admin_Vehicle[playerid]);
	admin_Vehicle[playerid] = INVALID_PLAYER_ID;
    return 1;
}

HOOK__ OnPlayerConnect(playerid)
{
	admin_Vehicle[playerid] = INVALID_VEHICLE_ID;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

HOOK__ OnVehicleDeath(vehicleid, killerid)
{
	DestroyVehicle(admin_Vehicle[vehicleid]);
	admin_Vehicle[vehicleid] = INVALID_PLAYER_ID;
    return Y_HOOKS_CONTINUE_RETURN_1;
}

stock GetPlayerRank(playerid)
{

    static rank_Name[46];
    
    if(user_Owner[playerid]) rank_Name = "OWNER";
    
    else
    {
        if(user_Admin[playerid] < 4) rank_Name = "ADMINISTRATOR";
        else if(user_Admin[playerid] == 4) rank_Name = "GLAVNI ADMIN";
        else if(user_Admin[playerid] == 5) rank_Name = "Z-VODJE ADMINA";
        else if(user_Admin[playerid] == 6) rank_Name = "VODJA ADMINA";
        else if(user_Admin[playerid] == 7) rank_Name = "DIREKTOR";
    }
    
    return rank_Name;
}

YCMD:ah(playerid, const string:params[], help) = acommands;
YCMD:acommands(playerid, const string:params[], help) 
{
    if(!user_Admin[playerid] || !user_Owner[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	Dialog_Show(playerid, "dialog_adminhelp", DIALOG_STYLE_MSGBOX, 
	"Administrator komande",
	""c_red"ADMINISTRATOR 1 | "c_white"/a /ao /cc /aduty\n\
	"c_red"____________________________________________________\n\
	"c_red"ADMINISTRATOR 2 | "c_white"\n\
	"c_red"____________________________________________________\n\
	"c_red"ADMINISTRATOR 3 | "c_white"\n\
	"c_red"____________________________________________________\n\
	"c_red"GLAVNI ADMIN | "c_white"\n\
	"c_red"____________________________________________________\n\
	"c_red"Z-VODJE ADMINA | "c_white"/makeadmin\n\
	"c_red"____________________________________________________\n\
	"c_red"VODJA ADMINA | "c_white"/makeadmin\n\
	"c_red"____________________________________________________\n\
	"c_red"DIREKTOR | "c_white"\n\
	"c_red"____________________________________________________\n\
	"c_red"OWNER | "c_white"/makeadmin",
	"Potvrdi", "Odustani");

	return 1;
}

YCMD:makeadmin(playerid, const string: params[], help) 
{
    static targetid, level;

    if(!user_Owner[playerid] || user_Admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Igrac nije na serveru.");

    if(sscanf(params, "ui", targetid, level)) return SendClientMessage(playerid, y_lblue, "( USSAGE ) "c_white"/makeadmin [igrac] [level]");

    if(level < 0 || level > 7) return SendClientMessage(playerid, y_blue, "( HELP ) "c_white"Netacan admin level ( 0-7 ).");

    if(!level)
    {
        user_Admin[targetid] = 0;
        user_Skin[targetid] = 240;
        user_Duty[targetid] = 0;
        SetPlayerSkin(playerid, 240);

        SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
        va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Owner %s vam je skinuo poziciju admina.", ReturnPlayerName(playerid));
        SendClientMessage(targetid, y_green, "( INFO ) "c_white"Ukoliko mislite da je doslo do greske postavite zalbu na forum.");
        SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");

    }
    else
    {
        user_Admin[targetid] = level;
        user_Skin[targetid] = 294;
        user_Duty[targetid] = 0;
        SetPlayerSkin(playerid, 294);

        SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
        va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Owner %s vam je postavio poziciju admina, level: %d.", ReturnPlayerName(playerid), level);
        SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
    }

    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File, "data" );
    INI_WriteInt(File, "Admin", user_Admin[playerid]);
    INI_WriteInt(File, "Skin", GetPlayerSkin(playerid));
    INI_WriteInt(File, "DutyTime", user_Duty[playerid]);
    INI_Close(File);

    return 1;
}

YCMD:a(playerid, const string: params[], help) 
{
    if(!user_Admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(isnull(params)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/a [text]");

    static fm_string[256];

    format(fm_string, sizeof(fm_string), "[ A ][ %s ] %s: %s", GetPlayerRank(playerid), ReturnPlayerName(playerid), params);

    foreach(new i: Player)
		if(user_Admin[i])
			SendClientMessage(i, y_red, fm_string);

    return 1;
}

YCMD:ao(playerid, const string: params[], help) 
{
    if(!user_Admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(isnull(params)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/ao [text]");

    va_SendClientMessageToAll(-1, ""c_red"(( %s: "c_white"%s "c_red"))", GetPlayerRank(playerid), params);

    return 1;
}

YCMD:cc(playerid, const string: params[], help) 
{
    if(!user_Admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    for(new i; i < 110; i++)
	{
		SendClientMessageToAll(-1, "");
	}

	va_SendClientMessageToAll(-1, ""c_server"Chat je ociscen od strane admina %s", ReturnPlayerName(playerid));
	SendClientMessageToAll(-1, ""c_server"www.warsaw-community.detth");

    return 1;
}

YCMD:adminduty(playerid, const string:parms[], help) = aduty;
YCMD:aduty(playerid, const string:params[], help)
{
	if(!user_Admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

	if(admin_Duty[playerid] == false )
 	{
		SetPlayerHealth(playerid, 100);
		SetPlayerArmour(playerid, 99);
		
		admin_Duty[playerid] = true;
		defer DutyTimer(playerid);

	  	va_SendClientMessageToAll(-1, ""c_red"( DUTY ) "c_white"Admin %s "c_red"(Duty: %d min) "c_white"je sada na duznosti /askq", ReturnPlayerName(playerid), user_Duty[playerid]);

	}
	else if(admin_Duty[playerid] == true)
	{
	 	admin_Duty[playerid] = false;
		va_SendClientMessageToAll(-1, ""c_red"( DUTY ) "c_white"Admin %s "c_red"(Duty: %d min) "c_white"vise nije na duznosti", ReturnPlayerName(playerid), user_Duty[playerid]);	
	}
	new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File, "data");
	INI_WriteInt(File, "DutyTime", user_Duty[playerid]);
	INI_Close(File);

    return 1;
}

YCMD:adminvozilo(playerid, const string:params[], help) = aveh;
YCMD:aveh(playerid, const string:params[], help) 
{
	if(!user_Admin[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    if(admin_Vehicle[playerid] == INVALID_VEHICLE_ID) 
	{
        Dialog_Show(playerid, "dialog_adminvehicle", DIALOG_STYLE_LIST, 
		"Admin Vozila", 
		"Infernus\nNrg-500\nSultan\nHuntley\nCheetah\nSandKing", 
		"Potvrdi", "Odustani");
    }
    else
	{
        va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Vozilo [ ID: %d ] je unisteno.", admin_Vehicle[playerid]);
        DestroyVehicle(admin_Vehicle[playerid]);
        admin_Vehicle[playerid] = INVALID_VEHICLE_ID;
    }

    return 1;
}

Dialog:dialog_adminvehicle(const playerid, response, listitem, string: inputtext[])
{
	if (!response)
		return 0;
	
	if(admin_Vehicle[playerid] == INVALID_VEHICLE_ID) 
	{
        new vehicleid;

        if(listitem == 0) { vehicleid = 411; }
        if(listitem == 1) { vehicleid = 522; }
        if(listitem == 2) { vehicleid = 560; }
        if(listitem == 3) { vehicleid = 579; }
        if(listitem == 4) { vehicleid = 415; }
        if(listitem == 5) { vehicleid = 451; }
        if(listitem == 6) { vehicleid = 490; }
        if(listitem == 7) { vehicleid = 495; }

        new 
			Float:Position_X,
			Float:Position_Y,
			Float:Position_Z;
			
        GetPlayerPos(playerid, Position_X, Position_Y, Position_Z);
        DestroyVehicle(admin_Vehicle[playerid]);
        admin_Vehicle[playerid] = CreateVehicle(vehicleid, Position_X, Position_Y, Position_Z, 0.0, 0, 0, -1);
        PutPlayerInVehicle(playerid, admin_Vehicle[playerid], 0);
        
        va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Vozilo [ ID: %d ] je stvoreno.", admin_Vehicle[playerid]);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}