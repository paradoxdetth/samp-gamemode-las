#if defined label_included
    #endinput
#endif
#define label_included

stock Create3DTextPickup(const string:text[], Float:vXU, Float:vYU, Float:vZU, vInt, vVW, pickupid, Float:radius ) 
{
	CreateDynamic3DTextLabel(text, 0x0059FFAA, vXU, vYU, vZU, radius, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, vVW, vInt, -1, 20.0);
	CreateDynamicPickup(pickupid, 1, vXU, vYU, vZU, vVW, vInt);
}