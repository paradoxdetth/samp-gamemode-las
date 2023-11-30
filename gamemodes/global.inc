#define YSI_YES_HEAP_MALLOC
#define YSI_NO_VERSION_CHECK
#define YSI_NO_MODE_CACHE
#define YSI_NO_OPTIMISATION_MESSAGE

#define CGEN_MEMORY 90000

#include <a_samp>
#include <YSI_Storage\y_ini>
#include <YSI_Visual\y_commands>
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>
#include <YSI_Coding\y_va>
#include <easy-dialog>
#include <Pawn.regex>
#include <streamer>
#include <sscanf2>

#include <colors>

// ----------------------------------- //

main() { print("Gamemode Started."); }

// ----------------------------------- //

#define key_pressed(%0) (((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))

// ----------------------------------- //

static 
	user_owner[MAX_PLAYERS],
	user_supporter[MAX_PLAYERS],
	user_admin[MAX_PLAYERS],
	user_vip[MAX_PLAYERS];

static 
    vip_time_limit[MAX_PLAYERS];
	
HOOK__ data_load(playerid, const string: name[], const string: value[])
{
	INI_Int("Admin", user_admin[playerid]);
	INI_Int("Owner", user_owner[playerid]);
	INI_Int("Supporter", user_supporter[playerid]);
	INI_Int("Vip", user_vip[playerid]);

	return 1;
}
// ----------------------------------- //

// Timers

task Global_Timer[1000]() 
{
	foreach(new i : Player)
	{
		if(user_owner[i] >= 1) SetPlayerChatBubble(i, "[ VLASNIK ]", y_red, 40.0, 10000);
		else if(user_admin[i] < 4) SetPlayerChatBubble(i, "[ ADMIN ]", y_red, 40.0, 10000);
		else if(user_admin[i] == 4) SetPlayerChatBubble(i, "[ HEAD ADMIN ]", y_red, 40.0, 10000);
		else if(user_admin[i] == 5) SetPlayerChatBubble(i, "[ DIREKTOR ]", y_red, 40.0, 10000);
		else if(user_admin[i] == 6) SetPlayerChatBubble(i, "[ MENAGER ]", y_red, 40.0, 10000);
		else if(user_admin[i] == 7) SetPlayerChatBubble(i, "[ SUVLASNIK ]", y_red, 40.0, 10000);
		else if(user_supporter[i] >= 1) SetPlayerChatBubble(i, "[ SUPPORTER ]", y_lgreen, 40.0, 10000);
		else if(user_vip[i] >= 1) SetPlayerChatBubble(i, "[ VIP ]", y_yellow, 40.0, 10000);
	}
	return 1;
}

task Vip_timer[1000]() 
{
    foreach(new i : Player)
    {
        if(vip_time_limit[i] > 0) vip_time_limit[i] --;
    }

    return 1;
}
timer Loading_Timer[3000](playerid)
{
	TogglePlayerControllable(playerid, true);
}

timer Kick_Timer[5000](playerid)
{
	Kick(playerid);
}


// Remote Functions

HOOK__ OnGameModeInit()
{
    DisableInteriorEnterExits();
	ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
	SetNameTagDrawDistance(20.0);
	LimitGlobalChatRadius(20.0);
	AllowInteriorWeapons(1);
	EnableVehicleFriendlyFire();
	EnableStuntBonusForAll(0);
    return 1;
}