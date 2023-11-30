#if defined rank_supporter_included
    #endinput
#endif
#define rank_supporter_included

#include <YSI_Coding\y_hooks>

static
    user_owner[MAX_PLAYERS],
    user_supporter[MAX_PLAYERS],
    user_admin[MAX_PLAYERS],
    user_skin[MAX_PLAYERS];

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Owner", user_owner[playerid]);
    INI_Int("Supporter", user_supporter[playerid]);
    INI_Int("Admin", user_admin[playerid]);
    INI_Int("Skin", user_skin[playerid]);

    return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File,"Supporter", user_supporter[playerid]);
    INI_Close(File);

    return 1;
}

YCMD:sduty(playerid, const string:params[], help) = aduty;

YCMD:shelp(playerid, const string:params[], help) 
{
    if(!user_supporter[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    Dialog_Show(playerid, "dialog_supphelp", DIALOG_STYLE_MSGBOX, 
	"Supporter komande",
	""c_lgreen"SUPPORTER | "c_white"/shelp /sc /goto /slap /freeze /unfreeze /hduty\n\
	"c_lgreen"____________________________________________________\n\
	"c_lgreen"SUPPORTER 2 | "c_white"/makesupp",
	"Potvrdi", "Odustani");

    return 1;
}

YCMD:makesupp(playerid, const string: params[], help) 
{
    static targetid, level;

    if(user_owner[playerid] || user_supporter[playerid] == 2)
    {
        if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Igrac nije na serveru.");

        if(sscanf(params, "ui", targetid, level)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/makesupp [igrac] [level]");

        if(level < 0 || level > 2) return SendClientMessage(playerid, y_blue, "( HELP ) "c_white"Netacan supporter level ( 0-2 ).");

        if(!level)
        {
            user_supporter[targetid] = 0;
            user_skin[targetid] = 240;
            SetPlayerSkin(playerid, 240);

            SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
            va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Vodja %s vam je skinuo poziciju supportera.", ReturnPlayerName(playerid));
            SendClientMessage(targetid, y_green, "( INFO ) "c_white"Ukoliko mislite da je doslo do greske postavite zalbu na forum.");
            SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");

        }
        else
        {
            user_supporter[targetid] = level;
            user_skin[targetid] = 171;
            SetPlayerSkin(playerid, 171);

            SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
            va_SendClientMessage(targetid, y_green, "( INFO ) "c_white"Vodja %s vam je postavio poziciju supportera, level: %d.", ReturnPlayerName(playerid), level);
            SendClientMessage(targetid, y_green, "------------------------------------------------------------------------------------------------");
        }

        new INI:File = INI_Open(data_path(playerid));
        INI_SetTag(File, "data" );
        INI_WriteInt(File, "Supporter", user_supporter[playerid]);
        INI_WriteInt(File, "Skin", GetPlayerSkin(playerid));
        INI_Close(File);
    }
    else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");


    return 1;
}

YCMD:sc(playerid, const string:params[], help) 
{
    if(user_supporter[playerid] || user_admin[playerid])
    {
        if(isnull(params)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/sc [text]");

        static fm_string[256];

        format(fm_string, sizeof(fm_string), "[ SC ][ %s ] %s: %s", GetPlayerRank(playerid), ReturnPlayerName(playerid), params);   

        foreach(new i: Player)
            if(user_supporter[i] || user_admin[i])
                SendClientMessage(i, y_lgreen, fm_string);
    }
    else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    return 1;
}