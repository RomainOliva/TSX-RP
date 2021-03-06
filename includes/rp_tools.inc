/*
 * Cette oeuvre, création, site ou texte est sous licence Creative Commons Attribution
 * - Pas d’Utilisation Commerciale 
 * - Partage dans les Mêmes Conditions 4.0 International. 
 * Pour accéder à une copie de cette licence, merci de vous rendre à l'adresse suivante
 * http://creativecommons.org/licenses/by-nc-sa/4.0/ .
 *
 * Merci de respecter le travail fourni par le ou les auteurs 
 * https://www.ts-x.eu/ - kossolax@ts-x.eu
 */
#if defined _rp_tools_included_
  #endinput
#endif
#define _rp_tools_included_

#include <sdkhooks>
// ----------------------------------------------------------------------------
public int Math_GetRandomPow(int min, int max) {
	return RoundToFloor(float(max - min) * Pow(GetURandomFloat(), 2.0)) + min;
}
// ----------------------------------------------------------------------------
public void PrintToChatClientArea(int client, const char[] message, any ...) {
	char buffer[250];
	VFormat(buffer, sizeof(buffer), message, 3);
	
	for(int i=1; i<=MaxClients; i++) {
		if( !IsValidClient(i) )
			continue;
		if( !IsPlayerAlive(i) )
			continue;

		if( rp_IsTargetHear(i, client) ) {
			CPrintToChat(i, "{lightblue}[TSX-RP]{default} %s", buffer);
		}
	}
}
public void PrintToChatZone(int zoneID, const char[] message, any ...) {
	char buffer[MAX_MESSAGE_LENGTH];
	VFormat(buffer, sizeof(buffer), message, 3);
	
	for(int i=1; i<=MaxClients; i++) {
		if( !IsValidClient(i) )
			continue;
		if( !IsPlayerAlive(i) )
			continue;
		
		if( rp_GetPlayerZone(i) == zoneID ) {
			CPrintToChat(i, "{lightblue}[TSX-RP]{default} %s", buffer);
		}
	}
}
// ----------------------------------------------------------------------------
public float GetCmdArgFloat(int index) {
	char arg[32];
	GetCmdArg(index, arg, sizeof(arg));
	return StringToFloat(arg);
}
public int GetCmdArgInt(int index) {
	char arg[32];
	GetCmdArg(index, arg, sizeof(arg));
	return StringToInt(arg);
}
public bool IsValidClient(int client) {
	if( client <= 0 || client > MaxClients )
		return false;
	
	if( !IsValidEdict(client) || !IsClientConnected(client) )
		return false;
	
	return true;
}
public bool FilterToOne(int entity, int mask, any data) {
	static char classname[64];
	if( entity == data )
		return false;
	GetEdictClassname(entity, classname, sizeof(classname));
	
	if( StrEqual(classname, "notsolid") )
		return false;
		
	return true;
}
public void SQL_QueryCallBack(Handle owner, Handle handle, const char[] error, any data) {
	if( strlen(error) >= 1  ) {
		LogError("[SQL] [ERROR] %s - %f - %i", error, data, data);
	}
}
public int rp_GetItemInt(int itemID, int typeID) {
	char tmp[32];
	rp_GetItemData(itemID, view_as<rp_item_type>(typeID), tmp, sizeof(tmp));
	return StringToInt(tmp);
}
public int rp_GetJobInt(int jobID, int typeID) {
	char tmp[32];
	rp_GetJobData(jobID, view_as<rp_job_type>(typeID), tmp, sizeof(tmp));
	return StringToInt(tmp);
}
public int rp_GetGroupInt(int groupID, int typeID) {
	char tmp[32];
	rp_GetGroupData(groupID, view_as<rp_group_type>(typeID), tmp, sizeof(tmp));
	return StringToInt(tmp);
}
public int rp_GetZoneInt(int zoneID, int typeID) {
	char tmp[32];
	rp_GetZoneData(zoneID, view_as<zone_type>(typeID), tmp, sizeof(tmp));
	return StringToInt(tmp);
}
public float rp_GetZoneFloat(int zoneID, int typeID) {
	char tmp[32];
	rp_GetZoneData(zoneID, view_as<zone_type>(typeID), tmp, sizeof(tmp));
	return StringToFloat(tmp);
}
public int rp_GetLocationInt(int zoneID, int typeID) {
	char tmp[32];
	rp_GetLocationData(zoneID, view_as<location_type>(typeID), tmp, sizeof(tmp));
	return StringToInt(tmp);
}
public int rp_GetServerInt(rp_serv typeID) {
	char tmp[64];
	rp_GetServerString(typeID, tmp, sizeof(tmp));
	return StringToInt(tmp);
}

public bool rp_IsRemoveAble(int target) {
	#if defined DEBUG
	PrintToServer("IsRemoveAble");
	#endif
	if( !IsValidEdict(target) )
		return false;
	if( !IsValidEntity(target) )
		return false;
	
	char classname[128];
	GetEdictClassname(target, classname, sizeof(classname));
	
	if( StrEqual(classname, "prop_dynamic") ) {
		if( Entity_GetParent(target) >= 1 ) {
			return false;
		}
		return true;
	}
	if( StrEqual(classname, "rp_mine") ) {
		return true;
	}
	if( StrContains(classname, "weapon_") == 0 ) {
		return true;
	}
	if( StrContains(classname, "item_") == 0 ) {
		return true;
	}
	if( StrContains(classname, "rp_cashmachine_") == 0 ) {
		return true;
	}
	if( StrContains(classname, "rp_bigcashmachine_") == 0 ) {
		return true;
	}
	if( StrContains(classname, "rp_plant_") == 0 ) {
		return true;
	}
	if( StrContains(classname, "prop_physics") == 0 ) {
		return true;
	}
	if( StrContains(classname, "rp_grave") == 0 ) {
		return true;
	}
	
	return false;
}
public bool rp_IsMoveAble(int ent) {
	static char classname[64];
	
	if( ent <= 0 )
		return false;
	if( !IsValidEdict(ent) ) 
		return false;
	if( !IsValidEntity(ent) )
		return false;
	
	if( GetEntityMoveType(ent) == MOVETYPE_NOCLIP )
		return false;
	
	if( IsValidClient(ent) )  {
		if( IsPlayerAlive(ent) ) {
			return true;
		}
		else {
			return false;
		}
	}
	
	
	GetEdictClassname(ent, classname, 63);
	
	
	if( StrContains(classname, "door", false) != -1 )
		return false;
	if( StrContains(classname, "player", false) == 0 )
		return true;
	if( StrContains(classname, "prop_p", false) == 0 )
		return true;
	if( StrContains(classname, "func_phys", false) == 0 )
		return true;
	if( StrContains(classname, "weapon_", false) == 0 )
		return true;
	if( StrContains(classname, "item_", false) == 0 )
		return true;
	if( StrContains(classname, "chicken", false) == 0 )
		return true;
	if( StrContains(classname, "bonbon_entity_2", false) == 0 )
		return true;
	
	return false;
}
stock void rp_ClientSwitchWeapon(int client, int wepid, int target=-1) {
	if( target == -1 )
		target = client;
	
	char wepname[64];
	int g, ammo, ammo1, index;
	enum_ball_type wep_type = rp_GetWeaponBallType(wepid);
	bool s = rp_GetWeaponStorage(wepid);
	
	g = rp_GetWeaponGroupID(wepid);
	ammo = Weapon_GetPrimaryClip(wepid);
	Client_GetWeaponPlayerAmmoEx(client, wepid, ammo1);
	index = GetEntProp(wepid, Prop_Send, "m_iItemDefinitionIndex");
	CSGO_GetItemDefinitionNameByIndex(index, wepname, sizeof(wepname));
	
	RemovePlayerItem(client, wepid);
	RemoveEdict( wepid );
	
	wepid = GivePlayerItem(target, wepname);
	
	rp_SetWeaponBallType(wepid, wep_type);
	rp_SetWeaponGroupID(wepid, g);
	Weapon_SetPrimaryClip(wepid, ammo);
	Client_SetWeaponPlayerAmmoEx(target, wepid, ammo1);
	rp_SetWeaponStorage(wepid, s);
	
	rp_SetClientWeaponSkin(target, wepid);
}

public Action rp_ClientRemoveProp(int client, int target, int item_id) {
		
	if( target > 0 && Entity_GetDistance(client, target) < 300.0 ) {
		if( rp_IsRemoveAble(target) ) {
				
			char name[128];
			GetEdictClassname(target, name, sizeof(name));
			if( StrContains(name, "rp_cashmachine_") == 0 ) {
					
				int owner = GetEntPropEnt(target, Prop_Send, "m_hOwnerEntity");
				if( IsValidClient(owner) ) {
					CPrintToChat(client, "{lightblue}[TSX-RP]{default} Vous avez detruit la machine de %N", owner);
					CPrintToChat(owner, "{lightblue}[TSX-RP]{default} Une de vos machines a faux billet a été detruite.");
				}
				
			}
			if( StrContains(name, "rp_bigcashmachine_") == 0 ) {
					
				int owner = GetEntPropEnt(target, Prop_Send, "m_hOwnerEntity");
				if( IsValidClient(owner) ) {
					CPrintToChat(client, "{lightblue}[TSX-RP]{default} Vous avez detruit la grosse machine de %N", owner);
					CPrintToChat(owner, "{lightblue}[TSX-RP]{default} Votre grosse machine a faux billet a été detruite.");
				}
			}
			else if( StrContains(name, "rp_plant_") == 0 ) {
				int owner = GetEntPropEnt(target, Prop_Send, "m_hOwnerEntity");
				if( IsValidClient(owner) && owner != client ) {
					CPrintToChat(client, "{lightblue}[TSX-RP]{default} Vous avez detruit un plant de drogue de %N", owner);
					CPrintToChat(owner, "{lightblue}[TSX-RP]{default} Une de vos plantations a été detruite.");
				}
				else if( owner == client ) {
					char tmp2[64];
					Format(tmp2, sizeof(tmp2), "rp_plant_%i_", client);
						
					ReplaceString(name, sizeof(name), tmp2, "");
					ReplaceString(name, sizeof(name), "_", "");
						
					int sub = StringToInt(name);
					rp_ClientGiveItem(client, sub, rp_GetBuildingData(target, BD_count));
					
					if( rp_GetBuildingData(target, BD_started)+180 < GetTime() ) {
						rp_ClientGiveItem(client, sub, Math_GetRandomInt(1, 5) );
					}
				}
			}
			
			rp_Effect_Tazer(client, target);
				
				
			if( StrContains(name, "prop_physics") == 0 ) {
				int owner = rp_GetBuildingData(target, BD_owner);
				
				if( IsValidClient(owner) ) {
					
					if( owner == client ) {
						int i = rp_GetBuildingData(target, BD_item_id);
						if( i > 0 ) {
							rp_ClientGiveItem(client, i);
						}
						AcceptEntityInput(target, "Kill");
					}
					else {
						// DealDamage(target, 1000, client);
						SDKHooks_TakeDamage(target, client, client, 1000.0);
					}
				}
				else {
					AcceptEntityInput(target, "Kill");
					CPrintToChat(client, "{lightblue}[TSX-RP]{default} Le props a été supprime.");
				}
			}
			else {
				AcceptEntityInput(target, "Kill");
				CPrintToChat(client, "{lightblue}[TSX-RP]{default} Le props a été supprime.");
			}
		}
		else if( rp_IsValidVehicle(target) && rp_IsInPVP(client) && rp_IsInPVP(target) ) {
			int heal = rp_GetVehicleInt(target, car_health) - 500;
			int owner = rp_GetVehicleInt(target, car_owner);
			rp_SetVehicleInt(target, car_health, heal);
				
			if( heal <= 0 && IsValidClient(owner) ) {
				CPrintToChat(owner, "{lightblue}[TSX-RP]{default} Votre voiture a été détruite en PvP.");
			}
			rp_Effect_Tazer(client, target);
		}
		else {				
			ITEM_CANCEL(client, item_id);
			CPrintToChat(client, "{lightblue}[TSX-RP]{default} Il n'y a rien à supprimer.");
		}
	}
	else {
		ITEM_CANCEL(client, item_id);
	}
	
	return Plugin_Continue;
}
stock int rp_GetWeaponPrice(int target) {
	char classname[65], tmp[255], tmp2[255];
	GetEdictClassname(target, classname, sizeof(classname));
	
	for (int i = 1; i < MAX_ITEMS; i++) {
		rp_GetItemData(i, item_type_extra_cmd, tmp, sizeof(tmp));
		Format(tmp2, sizeof(tmp2), "rp_giveitem %s", classname);
		if( StrEqual(tmp, tmp2) ) {
			rp_GetItemInt(i, item_type_prix);
			return rp_GetItemInt(i, item_type_prix);
		}
	}
	
	ReplaceString(classname, sizeof(classname), "weapon_", "");
	int owner = GetEntPropEnt(target, Prop_Send, "m_hPrevOwner");
	
	if( owner > 0 && view_as<int>(CS_AliasToWeaponID(classname)) > 0 )
		return CS_GetWeaponPrice(owner, CS_AliasToWeaponID(classname));
	// fallback:
	return 500; 
}
