#if defined player_commands_included
    #endinput
#endif
#define player_commands_included

public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success)
{
    if(success != COMMAND_OK) 
	{
		va_SendClientMessage(playerid, y_red, "( ERROR ) "c_white"`%s` je ne postojeca komanda, za listu komandi upisite ( /commands )", cmdtext);

        return COMMAND_OK;
    }

    return COMMAND_OK;
}

YCMD:b(playerid, const string:params[], help) 
{
	if(isnull(params)) 
		return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/b [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), ""c_blue"( OOC | %s: "c_white"%s "c_blue")", ReturnPlayerName(playerid), params);

	__proxy(playerid, 30.0, -1, fm_string);
    
	return 1;
}

YCMD:d(playerid, const string:params[], help) 
{
	if(isnull(params)) 
		return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/d [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), ""c_orange"%s se dere: "c_white"%s", ReturnPlayerName(playerid), params);

	__proxy(playerid, 40.0, -1, fm_string);

	ApplyAnimation(playerid, "PED", "IDLE_CHAT", 4.0, 0, 0, 0, 0, 1000, 1);
	return 1;
}

YCMD:t(playerid, const string:params[], help) 
{
	if(isnull(params)) 
		return SendClientMessage(playerid, y_lblue, "( ? ) "c_white"/t [text]");

	static fm_string[256];

	format(fm_string, sizeof(fm_string), ""c_orange"%s kaze tiho: "c_white"%s", ReturnPlayerName(playerid), params);

	__proxy(playerid, 10.0, -1, fm_string);

	return 1;
}

YCMD:commands(playerid, const string:params[], help) 
{
	Dialog_Show(playerid, "dialog_commands", DIALOG_STYLE_MSGBOX, 
		"Player Commands", 
		""c_blue"CHAT | "c_white"/b /d /t\n\
		"c_blue"HOUSE | "c_white"/buyhouse /sellhouse /houseinfo",
		"Potvrdi",""
	);

	return 1;
}
