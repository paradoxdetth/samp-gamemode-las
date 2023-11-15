#if defined player_chat_included
    #endinput
#endif
#define player_chat_included

HOOK__ OnPlayerText(playerid, const string: text[])
{
    static fm_string[128];

    format(fm_string, sizeof(fm_string), ""c_greey"%s "c_lblue"[ ID:%d ] "c_greey"kaze: "c_white"%s", ReturnPlayerName(playerid), playerid, text);

	__proxy(playerid, Float:30.0, y_greey, fm_string);

    return 0;
}