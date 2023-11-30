#if defined player_activity_included
    #endinput
#endif
#define player_activity_included

#include <YSI_Coding\y_hooks>

static
    user_respects[MAX_PLAYERS],
    user_needrespects[MAX_PLAYERS],
    user_activity[MAX_PLAYERS],
    user_score[MAX_PLAYERS],
    user_money[MAX_PLAYERS],
    user_vip[MAX_PLAYERS],
    user_viptime[MAX_PLAYERS];

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Respects", user_respects[playerid]);
    INI_Int("NeedRespects", user_needrespects[playerid]);
    INI_Int("Activity", user_activity[playerid]);
    INI_Int("Score", user_score[playerid]);
    INI_Int("Money", user_money[playerid]);
    INI_Int("Vip", user_vip[playerid]);
    INI_Int("Viptime", user_viptime[playerid]);


    return 1;
}

ptask Activity[3600000](playerid)
{
    user_respects[playerid]++;
    user_activity[playerid]++;

    Dialog_Show(playerid, "dialog_payday", DIALOG_STYLE_MSGBOX, 
		""c_green"PayDay", 
		""c_white"Primili ste platu u iznosu od 1000$\n\
		"c_white"Imate %d/%d respekata",
		"Uredu", "", user_respects[playerid], user_needrespects[playerid]
    );
    
    GivePlayerMoney(playerid, 1000);

    if(user_viptime[playerid] != 0)
    {
        user_viptime[playerid]--;
    }

    if(user_vip[playerid] >= 1 && user_viptime[playerid] == 0)
    {
        user_vip[playerid] = 0;
        user_viptime[playerid] = 0;

        SendClientMessage(playerid, y_green, "( PAYDAY ): "c_white"Vas vip je istekao.");
    }

    if(user_respects[playerid] >= user_needrespects[playerid])
    {
        user_respects[playerid] = 0;
        user_score[playerid]++;
        user_needrespects[playerid] = user_score[playerid]*2+4;

        va_SendClientMessage(playerid, y_green, "( PAYDAY ): "c_white"Cestitamo sada si level %d! Potrebno ti je %d respekata za iduci !", user_score[playerid], user_needrespects[playerid]);
    }
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File, "Score", GetPlayerScore(playerid));
    INI_WriteInt(File, "Respects", user_respects[playerid]);
    INI_WriteInt(File, "NeedRespects", user_needrespects[playerid]);
    INI_WriteInt(File, "Money", GetPlayerMoney(playerid));
    INI_WriteInt(File, "Activity", user_activity[playerid]);
    INI_WriteInt(File, "Vip", user_vip[playerid]);
    INI_WriteInt(File, "Viptime", user_viptime[playerid]);
    INI_Close(File);

    SetPlayerScore(playerid, user_score[playerid]);

    return 1;
}