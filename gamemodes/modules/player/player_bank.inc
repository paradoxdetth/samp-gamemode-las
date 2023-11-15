#if defined player_bank_included
    #endinput
#endif
#define player_bank_included

#include <YSI_Coding\y_hooks>

static
    user_BankAccount[MAX_PLAYERS],
    user_BankMoney[MAX_PLAYERS],
    user_Money[MAX_PLAYERS],
    user_Score[MAX_PLAYERS];

YCMD:otvoriracun(playerid, const string:params[], help) = createaccount;
YCMD:createaccount(playerid, const string:params[], help) 
{

	if(user_BankAccount[playerid]) 
        return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Vec imate otvoren racun");

	//if(!IsPlayerInRangeOfPoint(playerid, 3.0, )) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Morate biti u banci na salteru.");

	user_BankAccount[playerid] = true;
	
    new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File,"data");
	INI_WriteInt(File, "BankAccount", user_BankAccount[playerid]);
	INI_Close(File);

	SendClientMessage(playerid, y_green, "( MAZE BANK ) "c_white"Uspesno ste otvorili bankovni racun. CESTITAMO!");

	return 1;
}

YCMD:stanje(playerid, const string:params[], help) = balance;
YCMD:balance(playerid, const string:params[], help) 
{

    if(!user_BankAccount[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemas bankovni racun.");

    //if(!IsPlayerInRangeOfPoint(playerid, 3.0, )) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Morate biti u banci na salteru.");

	va_SendClientMessage(playerid, y_green, "( MAZE BANK ) "c_white"Imate $%d na vasem bankovnom racunu.", user_BankMoney[playerid]);

	return 1;
}

YCMD:podigni(playerid, const string:params[], help) = withdraw;
YCMD:withdraw(playerid, const string:params[], help)  
{
    if(help)
    {
        SendClientMessage(playerid, y_blue, "[ HELP ] "c_white"Ova komanda sluzi kako bi ste podignuli novac sa vaseg bankovnog racuna");
    }

    if(!user_BankAccount[playerid])
        return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemas bankovni racun.");

    //if(!IsPlayerInRangeOfPoint(playerid, 3.0, )) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Morate biti u banci na salteru.");
    
    static 
        withdraw;
	
    if(sscanf(params, "i", withdraw))
		return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/withdraw [iznos]");
	
    if(withdraw > user_BankMoney[playerid] || withdraw < 1) return va_SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemate dovoljno novca na racunu. Ukupno: $%d", user_BankMoney[playerid]);
	
    GivePlayerMoney(playerid, withdraw);

	user_BankMoney[playerid] -= withdraw;

    va_SendClientMessage(playerid, y_green, "( MAZE BANK ) "c_white"Podigli ste $%d sa vaseg racuna. Ukupno: $%d ", withdraw, user_BankMoney[playerid]);
	
    new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File,"data");
    INI_WriteInt(File, "BankMoney", user_BankMoney[playerid]);
	INI_Close(File);

	return 1;
}

YCMD:deposit(playerid, const string:params[], help) 
{

	if(!user_BankAccount[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemas bankovni racun.");

    //if(!IsPlayerInRangeOfPoint(playerid, 3.0, )) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Morate biti u banci na salteru.");

    new 
        curfunds = user_BankMoney[playerid],
        deposit;

    if(sscanf(params, "i", deposit))
		return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/deposit [iznos]");

	if(deposit > user_Money[playerid] || deposit < 1) 
        return va_SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemate dovoljno novca na racunu. Ukupno: $%d", user_BankMoney[playerid]);

	GivePlayerMoney(playerid, -deposit);

	user_BankMoney[playerid] += deposit;
    
	va_SendClientMessage(playerid, y_greey, "|___ BANKOVNI RACUN ___|");
	va_SendClientMessage(playerid, y_greey, "  Stari iznos: $%d", curfunds);
	va_SendClientMessage(playerid, y_greey, "  Pohrana: $%d",deposit);
	va_SendClientMessage(playerid, y_greey, "|-----------------------------------------|");
	va_SendClientMessage(playerid, y_greey, "  Novi iznos: $%d", user_BankMoney[playerid]);

	new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File,"data");
    INI_WriteInt(File, "BankMoney", user_BankMoney[playerid]);
    INI_WriteInt(File, "Money", user_Money[playerid]);
	INI_Close(File);

	return 1;
}

YCMD:transfer(playerid, const string:params[], help) 
{

    if(!user_BankAccount[playerid])return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemas bankovni racun.");

    //if(!IsPlayerInRangeOfPoint(playerid, 3.0, 0)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Morate biti u banci na salteru.");
        
    static 
        targetid, 
        transfer;

	if(sscanf(params, "ui", targetid, transfer)) 
        return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/transfer [id] [iznos]");
        
	if(transfer > user_Money[playerid] || transfer < 1) return va_SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemate dovoljno novca na racunu. Ukupno: $%d", user_BankMoney[playerid]);

	if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Taj igrac nije na serveru.");

    if(!user_BankAccount[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Igrac nema bankovni racun");

	if(user_Score[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Morate biti level 5+.");

	if(user_Score[targetid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Igrac mora biti level 5+.");

	if(transfer > 500000 || transfer < 1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Transfer novca ne moze biti veci od $500.000 i $1");

	user_BankAccount[playerid] -= transfer;
    user_BankAccount[targetid] += transfer;

    va_SendClientMessage(playerid, y_green, "( MAZE BANK )" c_white"Prebacili ste %s sa svog racuna na racun igraca %s.", transfer, ReturnPlayerName(targetid));
    va_SendClientMessage(targetid, y_green, "( MAZE BANK )" c_white"Primili ste %s na racun od igraca %s.", transfer, ReturnPlayerName(playerid));

	return 1;
}

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("BankAccount", user_BankAccount[playerid]);
	INI_Int("BankMoney", user_BankMoney[playerid]);
	INI_Int("Money", user_Money[playerid]);
	INI_Int("Score", user_Score[playerid]);

	return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
	new INI:File = INI_Open(data_path(playerid));
	INI_SetTag(File,"data");
    INI_WriteInt(File, "BankAccount", user_BankAccount[playerid]);
    INI_WriteInt(File, "BankMoney", user_BankMoney[playerid]);
	INI_Close(File);

	return 1;
}