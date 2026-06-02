
#include "Globals.h"
#include "ApiDocs.h"

namespace ApiDocs
{
	const std::map<std::string, Class> & GetApiRegistry()
	{
		static std::map<std::string, Class> Registry;
		if (!Registry.empty()) return Registry;

		{
			Class c;
			c.Name = "Globals";
			c.Desc = "Maximum BlockType number used";
			// Extracted 30 metadata entries
			Registry["Globals"] = std::move(c);
		}
		{
			Class c;
			c.Name = "HTTPFormData";
			c.Desc = "";
			// Extracted 5 metadata entries
			Registry["HTTPFormData"] = std::move(c);
		}
		{
			Class c;
			c.Name = "HTTPRequest";
			c.Desc = "HTTP method used for the request (\\";
			// Extracted 6 metadata entries
			Registry["HTTPRequest"] = std::move(c);
		}
		{
			Class c;
			c.Name = "HTTPTemplateRequest";
			c.Desc = "";
			// Extracted 3 metadata entries
			Registry["HTTPTemplateRequest"] = std::move(c);
		}
		{
			Class c;
			c.Name = "TakeDamageInfo";
			c.Desc = "";
			// Extracted 7 metadata entries
			Registry["TakeDamageInfo"] = std::move(c);
		}
		{
			Class c;
			c.Name = "Vector3d";
			c.Desc = "Updates each coord to its absolute value";
			// Extracted 108 metadata entries
			Registry["Vector3d"] = std::move(c);
		}
		{
			Class c;
			c.Name = "Vector3f";
			c.Desc = "Updates each coord to its absolute value";
			// Extracted 108 metadata entries
			Registry["Vector3f"] = std::move(c);
		}
		{
			Class c;
			c.Name = "Vector3i";
			c.Desc = "Updates each coord to its absolute value";
			// Extracted 108 metadata entries
			Registry["Vector3i"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cArrowEntity";
			c.Desc = "Returns true if the specified player can pick the arrow up";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 29 metadata entries
			Registry["cArrowEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBeaconEntity";
			c.Desc = "Calculate the amount of layers the pyramid below the beacon has.";
			c.Inherits.push_back("cBlockEntityWithItems");
			// Extracted 39 metadata entries
			Registry["cBeaconEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBedEntity";
			c.Desc = "Returns the color of the bed";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 12 metadata entries
			Registry["cBedEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBlockArea";
			c.Desc = "Clears the data stored to reclaim memory";
			// Extracted 140 metadata entries
			Registry["cBlockArea"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBlockEntity";
			c.Desc = "Called when a player uses this entity; should open the UI window.\\\n\treturns true if the use was successful, return false to use the block as a \\";
			// Extracted 35 metadata entries
			Registry["cBlockEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBlockEntityWithItems";
			c.Desc = "Returns the ItemGrid used for storing the contents";
			c.Inherits.push_back("cBlockEntity");
			c.Inherits.push_back("cBlockEntityWindowOwner");
			// Extracted 16 metadata entries
			Registry["cBlockEntityWithItems"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBlockInfo";
			c.Desc = "Can a finisher change it?";
			// Extracted 47 metadata entries
			Registry["cBlockInfo"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBoat";
			c.Desc = "Returns the eMaterial of the boat";
			c.Inherits.push_back("cEntity");
			// Extracted 23 metadata entries
			Registry["cBoat"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBoundingBox";
			c.Desc = "Represents two sets of coords, minimum and maximum for each direction.\\\nAll the coords within those limits (inclusive the edges) are considered \\";
			// Extracted 66 metadata entries
			Registry["cBoundingBox"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cBrewingstandEntity";
			c.Desc = "Returns the time until the current items finishes brewing, in ticks";
			c.Inherits.push_back("cBlockEntityWithItems");
			// Extracted 47 metadata entries
			Registry["cBrewingstandEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cChatColor";
			c.Desc = "@deprecated use ChatColor::Delimiter instead";
			// Extracted 26 metadata entries
			Registry["cChatColor"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cChestEntity";
			c.Desc = "";
			c.Inherits.push_back("cBlockEntityWithItems");
			// Extracted 4 metadata entries
			Registry["cChestEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cChunkDesc";
			c.Desc = "Fills the relative cuboid with specified block; allows cuboid out of range of this chunk";
			// Extracted 100 metadata entries
			Registry["cChunkDesc"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cClientHandle";
			c.Desc = "Returns the client brand received in the MC|Brand plugin message or set by a plugin.";
			// Extracted 133 metadata entries
			Registry["cClientHandle"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cColor";
			c.Desc = "Resets the color";
			// Extracted 36 metadata entries
			Registry["cColor"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cCommandBlockEntity";
			c.Desc = "Sets the command block to execute a command in the next tick";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 18 metadata entries
			Registry["cCommandBlockEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cCompositeChat";
			c.Desc = "Container for a single chat message composed of multiple functional parts.\\\nEach part corresponds roughly to the behavior supported by the client messaging:\\\n\t- plain text, optionaly colorized / styled\\\n\t- clickable URLs\\\n\t- clickable commands (run)\\\n\t- clickable commands (suggest)\\\nEach part has a text assigned to it that can be styled. The style is specified using a string,\\\neach character / character combination in the string specifies the style to use:\\\n\t- (char from 0 - 9 or a - f) = color X\\\n\t- k = obfuscated\\\n\t- l = bold\\\n\t- m = strikethrough\\\n\t- n = underlined\\\n\t- o = italic\\\n\t- r = reset\\\nIf the protocol version doesn't support all the features, it degrades gracefully.";
			// Extracted 14 metadata entries
			Registry["cCompositeChat"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cCraftingGrid";
			c.Desc = "Removes items in a_Grid from m_Items[] (used by cCraftingRecipe::ConsumeIngredients())";
			// Extracted 28 metadata entries
			Registry["cCraftingGrid"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cCraftingRecipe";
			c.Desc = "Consumes ingredients from the crafting grid specified";
			// Extracted 33 metadata entries
			Registry["cCraftingRecipe"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cCuboid";
			c.Desc = "Clamps this cuboid, so that it doesn't reach outside of a_Limits in any direction.\\\n\tAssumes both this and a_Limits are sorted.";
			// Extracted 59 metadata entries
			Registry["cCuboid"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cDispenserEntity";
			c.Desc = "Returns a unit vector in the cardinal direction of where the dispenser with the specified meta would be facing.";
			c.Inherits.push_back("cDropSpenserEntity");
			// Extracted 11 metadata entries
			Registry["cDispenserEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cDropSpenserEntity";
			c.Desc = "Sets the dropspenser to dropspense an item in the next tick";
			c.Inherits.push_back("cBlockEntityWithItems");
			// Extracted 11 metadata entries
			Registry["cDropSpenserEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cDropperEntity";
			c.Desc = "";
			c.Inherits.push_back("cDropSpenserEntity");
			// Extracted 2 metadata entries
			Registry["cDropperEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cEnchantments";
			c.Desc = "Class that stores item enchantments or stored-enchantments\\\nThe enchantments may be serialized to a stringspec and read back from such stringspec.\\\nThe format for the stringspec is \\";
			// Extracted 41 metadata entries
			Registry["cEnchantments"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cEnderCrystal";
			c.Desc = "If the EnderCrystal should send it's beam to the client and save it.";
			c.Inherits.push_back("cEntity");
			// Extracted 21 metadata entries
			Registry["cEnderCrystal"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cEntity";
			c.Desc = "Applies damage to the armor after the armor blocked the given amount";
			// Extracted 369 metadata entries
			Registry["cEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cEntityEffect";
			c.Desc = "Returns the potion color (used by the client for visuals), based on the potion's damage value";
			// Extracted 19 metadata entries
			Registry["cEntityEffect"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cExpBottleEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cExpBottleEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cExpOrb";
			c.Desc = "Returns the number of ticks that this entity has existed";
			c.Inherits.push_back("cEntity");
			// Extracted 15 metadata entries
			Registry["cExpOrb"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFallingBlock";
			c.Desc = "Creates a new falling block.\\\n\ta_Position is expected in world coords";
			c.Inherits.push_back("cEntity");
			// Extracted 9 metadata entries
			Registry["cFallingBlock"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFile";
			c.Desc = "Reads the list of ports from the INI file, possibly upgrading from IPv4 / IPv6-specific values into new version-agnostic value.\\\nReads the list of ports from a_PortsValueName. If that value doesn't exist or is empty, the list is combined from values\\\nin a_OldIPv4ValueName and a_OldIPv6ValueName; in this case the old values are removed from the INI file.\\\nIf there is none of the three values or they are all empty, the default is used and stored in the Ports value.";
			// Extracted 8 metadata entries
			Registry["cFile"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFireChargeEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cFireChargeEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFireworkEntity";
			c.Desc = "Returns the item used to create the rocket (has all the firework effects on it)";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 15 metadata entries
			Registry["cFireworkEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFloater";
			c.Desc = "";
			c.Inherits.push_back("cEntity");
			// Extracted 15 metadata entries
			Registry["cFloater"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFlowerPotEntity";
			c.Desc = "Get the item in the flower pot";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 12 metadata entries
			Registry["cFlowerPotEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cFurnaceEntity";
			c.Desc = "Calculates, resets, and returns the experience reward in this furnace";
			c.Inherits.push_back("cBlockEntityWithItems");
			// Extracted 38 metadata entries
			Registry["cFurnaceEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cGhastFireballEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cGhastFireballEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cHangingEntity";
			c.Desc = "Returns the direction in which the entity is facing.";
			c.Inherits.push_back("cEntity");
			// Extracted 12 metadata entries
			Registry["cHangingEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cHopperEntity";
			c.Desc = "";
			c.Inherits.push_back("cBlockEntityWithItems");
			// Extracted 4 metadata entries
			Registry["cHopperEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cIniFile";
			c.Desc = "Adds a header comment";
			c.Inherits.push_back("cSettingsRepositoryInterface");
			// Extracted 165 metadata entries
			Registry["cIniFile"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cInventory";
			c.Desc = "This class represents the player's inventory\\\nThe slots are divided into three areas:\\\n- armor slots     (1 x 4)\\\n- inventory slots (9 x 3)\\\n- hotbar slots    (9 x 1)\\\nThe generic GetSlot(), SetSlot() and HowManyCanFit() functions take the index of the slots,\\\nas if armor slots, inventory slots and then hotbar slots were put one after another.\\\nYou can use the invArmorOffset, invInventoryOffset and invHotbarOffset constants.";
			// Extracted 117 metadata entries
			Registry["cInventory"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cItem";
			c.Desc = "Adds the specified count to this object and returns the reference to self (useful for chaining)";
			// Extracted 81 metadata entries
			Registry["cItem"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cItemFrame";
			c.Desc = "Returns the item in the frame";
			c.Inherits.push_back("cHangingEntity");
			// Extracted 15 metadata entries
			Registry["cItemFrame"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cItemGrid";
			c.Desc = "Used to store loot probability tables";
			// Extracted 98 metadata entries
			Registry["cItemGrid"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cItems";
			c.Desc = "This class bridges a vector of cItem for safe access via Lua. It checks boundaries for all accesses\\\nNote that this class is zero-indexed!";
			// Extracted 40 metadata entries
			Registry["cItems"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cJukeboxEntity";
			c.Desc = "Ejects the currently held record as a pickup. Return false when no record had been inserted.";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 21 metadata entries
			Registry["cJukeboxEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cLeashKnot";
			c.Desc = "";
			c.Inherits.push_back("cHangingEntity");
			// Extracted 2 metadata entries
			Registry["cLeashKnot"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cLuaWindow";
			c.Desc = "A window that has been created by a Lua plugin and is handled entirely by that plugin\\\nThis object needs extra care with its lifetime management:\\\n- It is created by Lua, so Lua expects to garbage-collect it later\\\n- Normal cWindow objects are deleted in their ClosedByPlayer() function if the last player closes them\\\n\tTo overcome this, this object overloads the Destroy functions, which doesn't let the ClosedByPlayer()\\\n\tdelete the window, but rather leaves it dangling, with only Lua having the reference to it.\\\n- Lua could GC the window while a player is still using it\\\n\tThe object creates a Lua reference to itself when opened by a player and\\\n\tremoves the reference when the last player closes the window.";
			c.Inherits.push_back("cWindow");
			// Extracted 9 metadata entries
			Registry["cLuaWindow"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cMap";
			c.Desc = "Encapsulates an in-game world map.";
			// Extracted 52 metadata entries
			Registry["cMap"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cMapManager";
			c.Desc = "Manages the in-game maps of a single world - Thread safe.";
			// Extracted 1 metadata entries
			Registry["cMapManager"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cMobHeadEntity";
			c.Desc = "Returns the player name of the mob head";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 27 metadata entries
			Registry["cMobHeadEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cMobSpawnerEntity";
			c.Desc = "Sets the spawn delay to a new random value.";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 66 metadata entries
			Registry["cMobSpawnerEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cMojangAPI";
			c.Desc = "Returns if a statistic is both present and has nonzero value.";
			// Extracted 1 metadata entries
			Registry["cMojangAPI"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cMonster";
			c.Desc = "Returns whether the mob can be leashed.";
			c.Inherits.push_back("cPawn");
			// Extracted 80 metadata entries
			Registry["cMonster"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cNoteEntity";
			c.Desc = "Creates a new note entity. a_World may be nullptr";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 15 metadata entries
			Registry["cNoteEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cObjective";
			c.Desc = "Adds a_Delta and returns the new score";
			// Extracted 34 metadata entries
			Registry["cObjective"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPainting";
			c.Desc = "Returns the protocol name of the painting";
			c.Inherits.push_back("cHangingEntity");
			// Extracted 6 metadata entries
			Registry["cPainting"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPawn";
			c.Desc = "Applies an entity effect.\\\n\tChecks with plugins if they allow the addition.\\\n\ta_EffectIntensity is the level of the effect (0 = Potion I, 1 = Potion II, etc).\\\n\ta_DistanceModifier is the scalar multiplied to the potion duration (only applies to splash potions).";
			c.Inherits.push_back("cEntity");
			// Extracted 22 metadata entries
			Registry["cPawn"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPickup";
			c.Desc = "Returns whether this pickup is allowed to combine with other similar pickups";
			c.Inherits.push_back("cEntity");
			// Extracted 33 metadata entries
			Registry["cPickup"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPlayer";
			c.Desc = "Adds the specified exhaustion to m_FoodExhaustion. Expects only positive values.";
			c.Inherits.push_back("cPawn");
			// Extracted 337 metadata entries
			Registry["cPlayer"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPlugin";
			c.Desc = "Returns the name of the folder (in the Plugins folder) from which the plugin is loaded.";
			// Extracted 29 metadata entries
			Registry["cPlugin"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPluginLua";
			c.Desc = "";
			c.Inherits.push_back("cPlugin");
			// Extracted 2 metadata entries
			Registry["cPluginLua"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cPluginManager";
			c.Desc = "Executes the command, as if it was requested by a_Player. Checks permissions first. Returns crExecuted if executed.";
			// Extracted 54 metadata entries
			Registry["cPluginManager"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cProjectileEntity";
			c.Desc = "Returns the name of the player that created the projectile\\\n\tWill be empty for non-player creators";
			c.Inherits.push_back("cEntity");
			// Extracted 20 metadata entries
			Registry["cProjectileEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cRoot";
			c.Desc = "The root of the object hierarchy";
			// Extracted 82 metadata entries
			Registry["cRoot"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cScoreboard";
			c.Desc = "Retrieves the objective with the specified name, nullptr if not found";
			// Extracted 37 metadata entries
			Registry["cScoreboard"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cServer";
			c.Desc = "Can login more than once with same username.\\\n\tReturns false if it is not allowed, true otherwise.";
			// Extracted 35 metadata entries
			Registry["cServer"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cSignEntity";
			c.Desc = "Retrieves individual line (zero-based index)";
			c.Inherits.push_back("cBlockEntity");
			// Extracted 12 metadata entries
			Registry["cSignEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cSplashPotionEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 18 metadata entries
			Registry["cSplashPotionEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cTNTEntity";
			c.Desc = "Explode the tnt";
			c.Inherits.push_back("cEntity");
			// Extracted 12 metadata entries
			Registry["cTNTEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cTeam";
			c.Desc = "Adds a new player to the team";
			// Extracted 50 metadata entries
			Registry["cTeam"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cThrownEggEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cThrownEggEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cThrownEnderPearlEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cThrownEnderPearlEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cThrownSnowballEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cThrownSnowballEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cUUID";
			c.Desc = "Lexicographically compare bytes with another UUID.\\\n\tReturns:\\\n\t\t0 when equal to a_Other,\\\n\t\t< 0 when less than a_Other,\\\n\t\t> 0 when greater than a_Other";
			// Extracted 29 metadata entries
			Registry["cUUID"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cWebAdmin";
			c.Desc = "Returns the prefix needed for making a link point to the webadmin root from the given URL (\\";
			// Extracted 17 metadata entries
			Registry["cWebAdmin"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cWindow";
			c.Desc = "Represents a UI window.\\\n\\\nEach window has a list of players that are currently using it\\\nWhen there's no player using a window, it is destroyed.\\\nA window consists of several areas of slots with similar functionality - for example the crafting grid area, or\\\nthe inventory area. Each area knows what its slots are (GetSlot() function) and can handle mouse clicks.\\\nThe window acts only as a top-level container for those areas, redirecting the click events to the correct areas.\\\nInventory painting, introduced in 1.5, is handled by the window, too";
			// Extracted 37 metadata entries
			Registry["cWindow"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cWitherSkullEntity";
			c.Desc = "";
			c.Inherits.push_back("cProjectileEntity");
			// Extracted 2 metadata entries
			Registry["cWitherSkullEntity"] = std::move(c);
		}
		{
			Class c;
			c.Name = "cWorld";
			c.Desc = "Retrieves the world height at the specified coords; returns nullopt if chunk not loaded / generated";
			// Extracted 391 metadata entries
			Registry["cWorld"] = std::move(c);
		}

		return Registry;
	}
}
