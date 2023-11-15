#if defined player_account_included
    #endinput
#endif
#define player_account_included

#include <YSI_Coding\y_hooks>

static const ACCOUNT_PATH[64] = "/Users/%s.ini";

const pass_length_max = 26;
const pass_length_min = 6;
const login_attempts_max = 3;

const __gType_register = 1,
      __gType_login = 2;

static
    user_password[MAX_PLAYERS][pass_length_max],
    user_gender[MAX_PLAYERS][2],
    user_email[MAX_PLAYERS][32],
    user_country[MAX_PLAYERS][64],
    user_score[MAX_PLAYERS],
    user_skin[MAX_PLAYERS],
    user_money[MAX_PLAYERS],
    user_euro[MAX_PLAYERS],

    server_login_attempts[MAX_PLAYERS];

forward data_load(const playerid, const string: name[], const string: value[]);
public data_load(const playerid, const string: name[], const string: value[])
{
    INI_String("Password", user_password[playerid]);
    INI_String("Sex", user_gender[playerid]);
    INI_String("Email", user_email[playerid]);
    INI_String("Country", user_country[playerid]);
    INI_Int("Score", user_score[playerid]);
    INI_Int("Skin", user_skin[playerid]);
    INI_Int("Money", user_money[playerid]);
    INI_Int("Euro", user_euro[playerid]);

    return 1;
}

stock data_path(playerid)
{
    static string[128];
    format(string, sizeof(string), ACCOUNT_PATH, ReturnPlayerName(playerid));
    return string;
}

stock SetPlayerToSpawn(playerid, type)
{
    if(type == __gType_register)
    {
        SetSpawnInfo(playerid, 0, user_skin[playerid], 2357.8042,-1742.3123,13.5469,9.8522, 0, 0, 0, 0, 0, 0);
        SpawnPlayer(playerid);

        SetPlayerScore(playerid, user_score[playerid]);
        SetPlayerSkin(playerid, user_skin[playerid]);
        GivePlayerMoney(playerid, user_money[playerid]);
    }
    else if(type == __gType_login)
    {
        SetSpawnInfo(playerid, 0, user_skin[playerid], 2357.8042,-1742.3123,13.5469,9.8522, 0, 0, 0, 0, 0, 0);
        SpawnPlayer(playerid);

        SetPlayerScore(playerid, user_score[playerid]);
        SetPlayerSkin(playerid, user_skin[playerid]);
        GivePlayerMoney(playerid, user_money[playerid]);

    }
    return 1;
}

stock IsRpNickname(const nickname[])
{
  static Regex:regex;
  if (!regex) regex = Regex_New("[A-Z][a-z]+_[A-Z][a-z]+");

  return Regex_Check(nickname, regex);
}

stock IsValidEmail(const email[])
{
  static Regex:regex;
  if (!regex) regex = Regex_New("^[\\w-\\.]+@([\\w-]+\\.)+[\\w-]{2,4}$");

  return Regex_Check(email, regex);
}

HOOK__ OnPlayerConnect(playerid)
{
    if(fexist(data_path(playerid)))
    {
        INI_ParseFile(data_path(playerid), "data_load", true, true, playerid);

        Dialog_Show(playerid, "dialog_login", DIALOG_STYLE_INPUT, ""c_server"Warsaw Community",
        "Dobrodosli "c_server"%s "c_white"na server\n\n\
        U prazno polje unesite vasu lozinku.\n\n\
        Imate "c_server"%d/%d "c_white"pokusaja da upisite tacnu lozinku", 
        ""c_server"Login","", ReturnPlayerName(playerid), server_login_attempts[playerid], login_attempts_max);
    }
    else
    {
        Dialog_Show(playerid, "dialog_password", DIALOG_STYLE_INPUT, ""c_server"Warsaw Community",
        "Dobrodosli "c_server"%s "c_white"na nas server\n\n\
        U prazno polje unesite zeljenu lozinku.\n\
        "c_red"NAPOMENA: Sifra moze imati od %d do %d karaktera.", 
        ""c_server"Next","", ReturnPlayerName(playerid), pass_length_min, pass_length_max);
    }
    return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
    
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File, "Score", GetPlayerScore(playerid));
    INI_WriteInt(File, "Skin", GetPlayerSkin(playerid));
    INI_WriteInt(File, "Money", GetPlayerMoney(playerid));
    INI_WriteInt(File, "Euro", user_euro[playerid]);
    INI_Close(File);   

    return 1;
}

HOOK__ OnPlayerSpawn(playerid)
{
    if(!IsRpNickname(ReturnPlayerName(playerid)))
	{
        for(new i; i < 110; i++)
            SendClientMessage(playerid, -1, "");
            
		SendClientMessage(playerid, y_ruby, "( ANTI-CHEAT ) Vase ime nije u formatu Ime_Prezime");
        Kick(playerid);
	}

    return 1;
}
Dialog: dialog_password(playerid, response, listitem, string: inputtext[])
{
    if(!response) return Kick(playerid);

    if(!(pass_length_min <= strlen(inputtext) <= pass_length_max))
        return Dialog_Show(playerid, "dialog_password", DIALOG_STYLE_INPUT, ""c_server"Warsaw Community",
        "Dobrodosli "c_server"%s "c_white"na nas server\n\n\
        U prazno polje unesite zeljenu lozinku.\n\
        "c_red"NAPOMENA: Sifra moze imati od %d do %d karaktera.", 
        ""c_server"Next","", ReturnPlayerName(playerid), pass_length_min, pass_length_max);
    
    strcopy(user_password[playerid], inputtext);

    Dialog_Show(playerid, "dialog_email", DIALOG_STYLE_INPUT, ""c_server"Warsaw Community",
    "U prazno polje unesite vas email.\n\n\
    "c_red"NAPOMENA: Email mora biti u formatu example@gmail.com.",
    ""c_server"Next","");

    return 1;
}

Dialog: dialog_email(playerid, response, listitem, string: inputtext[])
{
    if(!response) return Kick(playerid);

    if(!IsValidEmail(inputtext))
        return Dialog_Show(playerid, "dialog_email", DIALOG_STYLE_INPUT, ""c_server"Warsaw Community",
            "U prazno polje unesite vas email.\n\n\
            "c_red"NAPOMENA: Email mora biti u formatu example@gmail.com.",
            ""c_server"Next","");

    strcopy(user_email[playerid], inputtext);

    Dialog_Show(playerid, "dialog_country", DIALOG_STYLE_LIST, ""c_server"Warsaw Community",
    "Bosna i Hercegovina\n\
    Srbija\n\
    Hrvatska\n\
    Slovenija\n\
    Ostalo",
    ""c_server"Next","");

    return 1;
}

Dialog: dialog_country(playerid, response, listitem, string: inputtext[])
{
    if(!response) return Kick(playerid);

    switch(listitem)
    {
        case 0: strcopy(user_country[playerid], "Bosna i Hercegovina");
        case 1: strcopy(user_country[playerid], "Srbija");
        case 2: strcopy(user_country[playerid], "Hrvatska");
        case 3: strcopy(user_country[playerid], "Slovenija");
        case 4: strcopy(user_country[playerid], "Ostalo");
    }

    Dialog_Show(playerid, "dialog_gender", DIALOG_STYLE_LIST, ""c_server"Warsaw Community",
    "Musko\n\
    Zensko",
    ""c_server"Next","");

    return 1;
}

Dialog: dialog_gender(playerid, response, listitem, string: inputtext[])
{
    if(!response) return Kick(playerid);

    new gender = listitem + 1;
 
	new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File,"data");
	INI_WriteString(File, "Password", user_password[playerid]);
	INI_WriteString(File, "Sex", (gender == 1 ? ("Musko") : ("Zensko")));
    INI_WriteString(File, "Email", user_email[playerid]);
    INI_WriteString(File, "Country", user_country[playerid]);
	INI_WriteInt(File, "Score", 3);
	INI_WriteInt(File, "Skin", 240);
	INI_WriteInt(File, "Money", 20000);
	INI_WriteInt(File, "Euro", 100);
	INI_Close(File);
 
	user_money[playerid] = 20000;
	user_skin[playerid] = 240;
	user_score[playerid] = 3;
    user_euro[playerid] = 100;

    SetPlayerToSpawn(playerid, 1);

    return 1;
}

Dialog: dialog_login(playerid, response, listitem, string: inputtext[])
{
    if(!response) return Kick(playerid);

    if(!strcmp(user_password[playerid], inputtext, false))
        SetPlayerToSpawn(playerid, 2);
    
    else
    {
        if(server_login_attempts[playerid] == login_attempts_max)
            return Kick(playerid);
        
        ++server_login_attempts[playerid];
        Dialog_Show(playerid, "dialog_login", DIALOG_STYLE_INPUT, ""c_server"Warsaw Community",
        "Dobrodosli "c_server"%s "c_white"na server\n\n\
        U prazno polje unesite vasu lozinku.\n\n\
        Imate "c_server"%d/%d "c_white"pokusaja da upisite tacnu lozinku", 
        ""c_server"Login","", ReturnPlayerName(playerid), server_login_attempts[playerid], login_attempts_max);


    }
    return 1;
}