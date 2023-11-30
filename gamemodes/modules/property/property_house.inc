#if defined property_house_included
    #endinput
#endif
#define property_house_included

#include <YSI_Coding\y_hooks>

static const HOUSE_PATH[64] = "/Property/Houses/%d.ini";

const MAX_HOUSES = 500;

static
    user_admin[MAX_PLAYERS],
    user_money[MAX_PLAYERS],
    user_score[MAX_PLAYERS],
    user_house[MAX_PLAYERS];

static 
    Float: house_enterence[MAX_HOUSES][3],
    Float: house_exit[MAX_HOUSES][3],

    house_interier[MAX_HOUSES],
    house_virtualworld[MAX_HOUSES],
    house_id[MAX_HOUSES],
    house_owned[MAX_HOUSES],
    house_price[MAX_HOUSES],
    house_level[MAX_HOUSES],
    house_lock[MAX_HOUSES],

    string:house_type[MAX_HOUSES][24],
    string:house_owner[MAX_HOUSES][MAX_PLAYER_NAME];

static
    house_pickup[MAX_HOUSES],
    Text3D:house_label[MAX_HOUSES];


HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", user_admin[playerid]);
    INI_Int("Money", user_money[playerid]);
    INI_Int("Score", user_score[playerid]);
    INI_Int("House", user_house[playerid]);

    return 1;
}
forward house_load(id, const string:name[], const string:value[]);
public house_load(id, const string:name[], const string:value[])
{
    INI_Float("Enterence_X", house_enterence[id][0]);
    INI_Float("Enterence_Y", house_enterence[id][1]);
    INI_Float("Enterence_Z", house_enterence[id][2]);
    INI_Float("Exit_X", house_exit[id][0]);
    INI_Float("Exit_Y", house_exit[id][1]);
    INI_Float("Exit_Z", house_exit[id][2]);
    INI_Int("Interier", house_interier[id]);
    INI_Int("Virtual_World", house_virtualworld[id]);
    INI_Int("ID", house_id[id]);
    INI_Int("Owned", house_owned[id]);
    INI_Int("Price", house_price[id]);
    INI_Int("Level", house_level[id]);
    INI_Int("Lock", house_lock[id]);
    INI_String("Type", house_type[id]);
    INI_String("Owner", house_owner[id]);
    
    return 1;
}

stock NextHouseID(const len)
{
    new id = (-1);
    for( new loop = ( 0 ), provjera = ( -1 ), Data_[ 64 ] = "\0"; loop != len; ++ loop ) {
       provjera = ( loop+1 );
       format( Data_, ( sizeof Data_ ), HOUSE_PATH, provjera );
       if(!fexist(Data_)) {
          id = ( provjera );
          break; } }
    return ( id );
}

stock house_path(const id)
{
    new tmp_fmt[64];
    format(tmp_fmt, sizeof(tmp_fmt), HOUSE_PATH, id);
    return tmp_fmt;
}

stock house_save(id)
{
    new INI:File = INI_Open(house_path(id));
    INI_SetTag(File,"data");
    INI_WriteFloat(File,"Enterence_X", house_enterence[id][0]);
    INI_WriteFloat(File,"Enterence_Y", house_enterence[id][1]);
    INI_WriteFloat(File,"Enterence_Z", house_enterence[id][2]);
    INI_WriteFloat(File,"Exit_X", house_exit[id][0]);
    INI_WriteFloat(File,"Exit_Y", house_exit[id][1]);
    INI_WriteFloat(File,"Exit_Z", house_exit[id][2]);
    INI_WriteInt(File,"Interier", house_interier[id]);
    INI_WriteInt(File,"Virtual_World", house_virtualworld[id]);
    INI_WriteInt(File,"ID", house_id[id]);
    INI_WriteInt(File,"Owned", house_owned[id]);
    INI_WriteInt(File,"Price", house_price[id]);
    INI_WriteInt(File,"Level", house_level[id]);
    INI_WriteInt(File,"Lock", house_lock[id]);
    INI_WriteString(File,"Type", house_type[id]);
    INI_WriteString(File,"Owner", house_owner[id]);
    INI_Close(File);

    return 1;
}

GetNearestHouse(playerid)
{
    for(new i = 1; i < MAX_HOUSES; i++)
    {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2])) return i;
    }
    return -1;
}

HOOK__ OnGameModeInit()
{
    for(new i = 0; i < MAX_HOUSES; i++)
    {
        if(fexist(house_path(i)))
        {
            INI_ParseFile(house_path(i), "house_load", true, true, i);

            static fm_string[256];
            if(house_owned[i] >= 1)
            {
                format(fm_string, sizeof(fm_string), ""c_yellow"Kuca [ID: %d] | Vrsta: "c_white"%s "c_yellow"\nLevel: "c_white"%d "c_yellow"\nVlasnik: "c_white"%s", house_id[i], house_type[i], house_level[i], house_owner[i]);

                house_pickup[i] = CreateDynamicPickup(19523, 1, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2]);

                house_label[i] = CreateDynamic3DTextLabel(fm_string, -1, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2], 30.0);
            }
            else
            {
                format(fm_string, sizeof(fm_string), ""c_yellow"Kuca [ID: %d] | Vrsta: "c_white"%s "c_yellow"\nLevel: "c_white"%d "c_yellow"\nCena: "c_white"%d$\n"c_white"Kuca na prodaju `/buyhouse`", house_id[i], house_type[i], house_level[i], house_price[i]);

                house_pickup[i] = CreateDynamicPickup(1273, 1, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2]);

                house_label[i] = CreateDynamic3DTextLabel(fm_string, -1, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2], 30.0);
            }
        }
    }
    return 1;
}

HOOK__ OnPlayerDisconnect(playerid, reason)
{
    
    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag(File,"data");
    INI_WriteInt(File, "House", user_house[playerid]);
    INI_Close(File);   

    return 1;
}

YCMD:createhouse(playerid, const string:params[], help) 
{
    if(user_admin[playerid] < 6) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    new id = NextHouseID(MAX_HOUSES),
        type;
    
    if(sscanf(params, "i", type)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/createhouse [vrsta]");

    static fm_string[256], 
        Float:pos_X, Float:pos_Y, Float:pos_Z;

    GetPlayerPos(playerid, pos_X, pos_Y, pos_Z);

    if(type == 0)
    {
        house_exit[id][0] = 446.626;
        house_exit[id][1] = 1397.738;
        house_exit[id][2] = 1084.3047;
        house_interier[id] = 2;
        house_price[id] = 100000;
        house_level[id] = 5;
        strcopy(house_type[id], "Mala Kuca");
        strcopy(house_owner[id], "Drzava");
    }
    else if(type == 1)
    {
        house_exit[id][0] = 234.2826;
        house_exit[id][1] = 1065.229;
        house_exit[id][2] = 1084.2101;
        house_interier[id] = 6;
        house_price[id] = 250000;
        house_level[id] = 7;
        strcopy(house_type[id], "Srednja Kuca");
        strcopy(house_owner[id], "Drzava");
    }
    house_enterence[id][0] = pos_X;
    house_enterence[id][1] = pos_Y;
    house_enterence[id][2] = pos_Z;
    house_virtualworld[id] = id;
    house_id[id] = id;
    house_owned[id] = 0;
    house_lock[id] = 0;
                
    format(fm_string, sizeof(fm_string), ""c_yellow"Kuca [ID: %d] | Vrsta: "c_white"%s "c_yellow"\nLevel: "c_white"%d "c_yellow"\nCena: "c_white"%d$\n"c_white"Kuca na prodaju `/buyhouse`", house_id[id], house_type[id], house_level[id], house_price[id]);

    house_pickup[id] = CreateDynamicPickup(1273, 1, house_enterence[id][0], house_enterence[id][1], house_enterence[id][2]);

    house_label[id] = CreateDynamic3DTextLabel(fm_string, -1, house_enterence[id][0], house_enterence[id][1], house_enterence[id][2], 30.0);

    house_save(id);

    va_SendClientMessage(playerid, y_green, "( INFO ): "c_white"Kreirali ste kucu, ( ID : %d ).", id);

    return 1;
}


YCMD:buyhouse(playerid, const string:params[], help) 
{
    for(new i; i < MAX_HOUSES; i++)
    {

        if(IsPlayerInRangeOfPoint(playerid, 3.0, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2]))
        {
            if(house_owned[i] >= 1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Ova kuca nije na prodaju.");

            if(GetPlayerMoney(playerid) < house_price[i]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nemate dovoljno novca.");

            if(user_house[playerid] >= 1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Vec posedujete kucu.");

            GivePlayerMoney(playerid, -house_price[i]);
            user_house[playerid] = i;

            house_owned[i] = 1;
            strcopy(house_owner[i], ReturnPlayerName(playerid));

            house_save(i);

            DestroyDynamic3DTextLabel(house_label[i]);
            DestroyDynamicPickup(house_pickup[i]);

            static fm_string[256];

            format(fm_string, sizeof(fm_string), ""c_yellow"Kuca [ID: %d] | Vrsta: "c_white"%s "c_yellow"\nLevel: "c_white"%d "c_yellow"\nVlasnik: "c_white"%s", house_id[i], house_type[i], house_level[i], house_owner[i]);

            house_pickup[i] = CreateDynamicPickup(19523, 1, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2]);

            house_label[i] = CreateDynamic3DTextLabel(fm_string, -1, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2], 30.0);

            new INI:File = INI_Open(data_path(playerid));
            INI_SetTag( File, "data");
            INI_WriteInt(File,"Money", user_money[playerid]);
            INI_WriteInt(File,"House", user_house[playerid]);
            INI_Close(File); 
        }
    }
    return 1;
}

YCMD:sellhouse(playerid, const string:params[], help) 
{
    new id = user_house[playerid];

    if(user_house[playerid] <= 0) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Vi ne posjedujete kucu.");

    if(GetNearestHouse(playerid) != user_house[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste u blizini vase kuce.");

    GivePlayerMoney(playerid, house_price[id]);
    user_house[playerid] = 0;

    house_owned[id] = 0;
    strcopy(house_owner[id], "Drzava");

    house_save(id);

    DestroyDynamic3DTextLabel(house_label[id]);
    DestroyDynamicPickup(house_pickup[id]);

    static fm_string[256];

    format(fm_string, sizeof(fm_string), ""c_yellow"Kuca [ID: %d] | Vrsta: "c_white"%s "c_yellow"\nLevel: "c_white"%d "c_yellow"\nCena: "c_white"%d$\n"c_white"Kuca na prodaju `/buyhouse`", house_id[id], house_type[id], house_level[id], house_price[id]);

    house_pickup[id] = CreateDynamicPickup(1273, 1, house_enterence[id][0], house_enterence[id][1], house_enterence[id][2]);

    house_label[id] = CreateDynamic3DTextLabel(fm_string, -1, house_enterence[id][0], house_enterence[id][1], house_enterence[id][2], 30.0);

    new INI:File = INI_Open(data_path(playerid));
    INI_SetTag( File, "data");
    INI_WriteInt(File,"Money", GetPlayerMoney(playerid));
    INI_WriteInt(File,"House", user_house[playerid]);
    INI_Close(File); 

    return 1;
}

YCMD:deletehouse(playerid, const string:params[], help)
{  
    if(user_admin[playerid] < 6) return SendClientMessage(playerid, y_red, "( ! ) "c_white"Niste Ovlasceni");

    new id;

    if(sscanf(params, "i", id)) return SendClientMessage(playerid, y_lblue, "( USAGE ) "c_white"/deletehouse [id]");

    if(fexist(house_path(id)))
    {
        fremove(house_path(id));

        DestroyDynamicPickup(house_pickup[id]);
        DestroyDynamic3DTextLabel(house_label[id]);
    }
    else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"ID kuce je nepostojeci.");
    
    return 1;
}

YCMD:houseinfo(playerid, const string:params[], help)
{
    if(GetNearestHouse(playerid) != user_house[playerid]) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste u blizini vase kuce.");
    
    Dialog_Show(playerid, "dialog_housemenu", DIALOG_STYLE_LIST, 
    "House Info", 
    "Info\n\
    Lock\n\
    Unlock\n",
    "Potvrdi", "Izlaz");

    return 1;
}

Dialog:dialog_housemenu(const playerid, response, listitem, string: inputtext[])
{
    if(!response) return 0;
    
    if(listitem == 0) { Dialog_Show(playerid, "dialog_houseinfo", DIALOG_STYLE_MSGBOX, "House Info", ""c_server"Level: "c_white"%d\n"c_server"Price: "c_white"%d\n"c_server"Owner: "c_white"%s", "Potvrdi", "Izlaz", house_level[user_house[playerid]], house_price[user_house[playerid]], house_owner[user_house[playerid]]); }
    if(listitem == 1) 
    { 
        if(house_lock[user_house[playerid]] == 0)
        {
            house_lock[user_house[playerid]] = 1; 
            house_save(user_house[playerid]);
            SendClientMessage(playerid, y_yellow, "( HOUSE ) "c_white"Zakljucali ste vasu kucu");
        }
        else SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Kuca je vec zakljucana.");
    }
    if(listitem == 2)
    {
        if(house_lock[user_house[playerid]] == 1)
        {
            house_lock[user_house[playerid]] = 0;
            house_save(user_house[playerid]);
            SendClientMessage(playerid, y_yellow, "( HOUSE ) "c_white"Otkljucali ste vasu kucu");
        }
        else SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Kuca je vec otkljucana.");
    }
    return 1;
}

HOOK__ OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(key_pressed(KEY_SECONDARY_ATTACK))
    {
        for(new i; i < MAX_HOUSES; i++)
        {
            if(IsPlayerInRangeOfPoint(playerid, 2.0, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2]))
            {
                if(house_lock[i] >= 1) return SendClientMessage(playerid, y_yellow, "( HOUSE ) "c_white"Kuca je zakljucana");

                GameTextForPlayer(playerid, "Ucitavanje...", 4000, 4);
                defer Loading_Timer(playerid);
                TogglePlayerControllable(playerid, false);
                SetPlayerPos(playerid, house_exit[i][0], house_exit[i][1], house_exit[i][2]);
                SetPlayerInterior(playerid, house_interier[i]);
                SetPlayerVirtualWorld(playerid, house_virtualworld[i]);
            }
            else if(IsPlayerInRangeOfPoint(playerid, 2.0, house_exit[i][0], house_exit[i][1], house_exit[i][2]) && GetPlayerVirtualWorld(playerid) == house_virtualworld[i])
            {
                GameTextForPlayer(playerid, "Ucitavanje...", 4000, 4);
                defer Loading_Timer(playerid);
                TogglePlayerControllable(playerid, false);
                SetPlayerPos(playerid, house_enterence[i][0], house_enterence[i][1], house_enterence[i][2]);
                SetPlayerInterior(playerid, 0);
                SetPlayerVirtualWorld(playerid, 0);

                return 1;
            }
        }   
    }
    return 1;
}