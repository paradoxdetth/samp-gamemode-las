#if defined global_port_included
    #endinput
#endif
#define global_port_included

#include <YSI_Coding\y_hooks>

static const PORT_PATH[64] = "/Ports/%d.ini";

const max_ports = 200;

static 
    user_admin[MAX_PLAYERS],
    user_supporter[MAX_PLAYERS],
    user_vip[MAX_PLAYERS];

static
    Float: port_position[max_ports][3],
    string: port_name[max_ports][24],
    port_type[max_ports];

new selected_port[MAX_PLAYERS];

static vip_time_limit[MAX_PLAYERS];

HOOK__ data_load(playerid, const string: name[], const string: value[])
{
    INI_Int("Admin", user_admin[playerid]);
    INI_Int("Supporter", user_supporter[playerid]);
    INI_Int("Vip", user_vip[playerid]);
    
    return 1;
}

forward ports_load(const portid, const string:name[], const string:value[]);
public ports_load(const portid, const string:name[], const string:value[])
{
    INI_Float("Position_X", port_position[portid][0]);
    INI_Float("Position_Y", port_position[portid][1]);
    INI_Float("Position_Z", port_position[portid][2]);
    INI_String("Name", port_name[portid]);
    INI_Int("Type", port_type[portid]);

    return 1;
}

stock port_path(const portid)
{
    new path[64];
    format(path, sizeof(path), PORT_PATH, portid);
    return path;
}

stock port_save(const portid)
{
    new INI:File = INI_Open(port_path(portid));
    INI_WriteFloat(File, "Position_X", port_position[portid][0]);
    INI_WriteFloat(File, "Position_Y", port_position[portid][1]);
    INI_WriteFloat(File, "Position_Z", port_position[portid][2]);
    INI_WriteString(File, "Name", port_name[portid]);
    INI_WriteInt(File, "Type", port_type[portid]);
    INI_Close(File);
    return 1;
}

stock NextPortID(const len) 
{
    new id = (-1);
    for( new loop = ( 0 ), provjera = ( -1 ), Data_[ 64 ] = "\0"; loop != len; ++ loop ) {
       provjera = ( loop+1 );
       format( Data_, ( sizeof Data_ ), PORT_PATH, provjera );
       if(!fexist(Data_)) {
          id = ( provjera );
          break; } }
    return ( id );
}

GetNearestPort(playerid)
{
    for(new i = 1; i < max_ports; i++)
    {
        if(IsPlayerInRangeOfPoint(playerid, 3.0, port_position[i][0], port_position[i][1], port_position[i][2])) return i;
    }
    return -1;
}

HOOK__ OnGameModeInit()
{
    for(new i = 0; i < max_ports; i++)
    {
        if(fexist(port_path(i)))
        {
            INI_ParseFile(port_path(i), "ports_load", true, true, i);
        }
    }
    return 1;
}

YCMD:createport(playerid, const string: params[], help) 
{
    if(user_admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    Dialog_Show(playerid, "dialog_createport", DIALOG_STYLE_LIST, "Los Angeles Side", "Los Santos\nPoslovi\nllegalno\nOrganizacije", "Potvrdi", "Izlaz");

    return 1;
}

YCMD:deleteport(playerid, string: params[], help)
{
    if(user_admin[playerid] < 5) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");

    new portid = GetNearestPort(playerid);

    if(portid == -1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nisi blizu nijednog porta.");

    new name[64];

    if(fexist(data_path(portid))) { fremove(data_path(portid)); }

    va_SendClientMessage(playerid, y_green, "( INFO ) "c_white"Port %s uspesno obrisan!", name);
    return 1;
}

YCMD:port(playerid, const string: params[], help) 
{
    if(user_admin[playerid] >= 1) 
    {
        Dialog_Show(playerid, "dialog_port", DIALOG_STYLE_LIST, "Los Angeles Side", "Los Santos\nPoslovi\nllegalno\nOrganizacije", "Potvrdi", "Izlaz");
    }
    else if(user_supporter[playerid] >= 1)
    {
        Dialog_Show(playerid, "dialog_port", DIALOG_STYLE_LIST, "Los Angeles Side", "Los Santos\nPoslovi\nllegalno\nOrganizacije", "Potvrdi", "Izlaz");
    }
    else if(user_vip[playerid] >= 1)
    {
        Dialog_Show(playerid, "dialog_port", DIALOG_STYLE_LIST, "Los Angeles Side", "Los Santos\nPoslovi\nllegalno\nOrganizacije", "Potvrdi", "Izlaz");
        
        if(vip_time_limit[playerid] > 0) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Mozete se portati za %d sekundi", vip_time_limit[playerid]);

        vip_time_limit[playerid] = 60;
    }
    else return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Niste Ovlasceni.");
    return 1;
}

Dialog: dialog_createport(const playerid, response, listitem, string: inputtext[])
{
    selected_port[playerid] = listitem + 1;
    
    Dialog_Show(playerid, "dialog_createport_name", DIALOG_STYLE_INPUT, "Los Angeles Side", "Upisite ime porta koji kreirate", "Potvrdi", "Izlaz");

    return 1;
}

Dialog: dialog_createport_name(const playerid, response, listitem, string: inputtext[])
{
    if(!response) return selected_port[playerid] = -1;
    if(response)
    {
        new portid = NextPortID(max_ports);

        if(portid == -1) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Max broj portova je kreiran");

        new name[64];
        if(sscanf(inputtext, "s[64]", name)) return Dialog_Show(playerid, "dialog_createport_name", DIALOG_STYLE_INPUT, "Los Angeles Side", "Upisite ime porta koji kreirate", "Potvrdi", "Izlaz");

        static 
            Float: pos_X, 
            Float: pos_Y, 
            Float: pos_Z;

        GetPlayerPos(playerid, pos_X, pos_Y, pos_Z);

        port_position[portid][0] = pos_X;
        port_position[portid][1] = pos_Y;
        port_position[portid][2] = pos_Z;
        port_type[portid] = selected_port[playerid];

        strcopy(port_name[portid], name);

        port_save(portid);

        SendClientMessage(playerid, y_green, "( INFO ) "c_white"Kreirali ste port %s", port_name[portid]);

        return 1;
    }
    return 1;
}

Dialog: dialog_port(const playerid, response, listitem, string: inputtext[])
{
    if(listitem == 0)
    {
        new string[128], file_string[2048];
        for(new i = 0; i < max_ports; i++)
        {
            if(fexist(port_path(i)))
            {
                if(port_type[i] == 1) 
                {
                    format(string, sizeof(string), "%s\n", port_name[i]);
                    strcat(file_string, string);
                }
            }
        }
        if(isnull(file_string)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nema kreiranih portova.");

        Dialog_Show(playerid, "dialog_port_select", DIALOG_STYLE_LIST, "Odaberi Port", file_string, "Potvrdi", "Izlaz");

        return 1;
    }
    if(listitem == 1)
    {
        new string[128], file_string[2048];
        for(new i = 0; i < max_ports; i++)
        {
            if(fexist(port_path(i)))
            {
                if(port_type[i] == 2)
                {
                    format(string, sizeof(string), "%s\n", port_name[i]);
                    strcat(file_string, string);
                }
            }
        }
        if(isnull(file_string)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nema kreiranih portova.");

        Dialog_Show(playerid, "dialog_port_select", DIALOG_STYLE_LIST, "Odaberi Port", file_string, "Potvrdi", "Izlaz");

        return 1;
    }
    if(listitem == 2)
    {
        new string[128], file_string[2048];
        for(new i = 0; i < max_ports; i++)
        {
            if(fexist(port_path(i)))
            {
                if(port_type[i] == 3)
                {
                    format(string, sizeof(string), "%s\n", port_name[i]);
                    strcat(file_string, string);
                }
            }
        }
        if(isnull(file_string)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nema kreiranih portova.");

        Dialog_Show(playerid, "dialog_port_select", DIALOG_STYLE_LIST, "Odaberi Port", file_string, "Potvrdi", "Izlaz");

        return 1;
    }
    if(listitem == 3)
    {
        new string[128], file_string[2048];
        for(new i = 0; i < max_ports; i++)
        {
            if(fexist(port_path(i)))
            {
                if(port_type[i] == 4)
                {
                    format(string, sizeof(string), "%s\n", port_name[i]);
                    strcat(file_string, string);
                }
            }
        }
        if(isnull(file_string)) return SendClientMessage(playerid, y_red, "( ERROR ) "c_white"Nema kreiranih portova.");

        Dialog_Show(playerid, "dialog_port_select", DIALOG_STYLE_LIST, "Odaberi Port", file_string, "Potvrdi", "Izlaz");

        return 1;
    }
    return 1;
}

Dialog: dialog_port_select(const playerid, response, listitem, string: inputtext[])
{
    new vehicleid = GetPlayerVehicleID(playerid);

    for(new i = 1; i < max_ports; i++)
    {
        if(fexist(port_path(i)))
        {

            if(!strcmp(port_name[i], inputtext, true)) 
            {
                if(GetPlayerState(playerid) == 2 ) 
                {
                    SetVehiclePos(vehicleid, port_position[i][0], port_position[i][1], port_position[i][2]);
                    GameTextForPlayer(playerid, "Ucitavanje...", 3000, 4);
                    defer Loading_Timer(playerid);
                    SetCameraBehindPlayer(playerid);
                    TogglePlayerControllable(playerid, 0);
                }
                else 
                {
                    SetPlayerPos( playerid, port_position[i][0], port_position[i][1], port_position[i][2]);
                }
                SetPlayerInterior(playerid, 0);
                SetPlayerVirtualWorld(playerid, 0);
                SendClientMessage(playerid, y_green, "( INFO ) "c_white"Teleportovao si kod %s.", port_name[i]);


                return 1;
            }
        }
    }
    return 1;
}