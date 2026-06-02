#include "Globals.h"
#include "NativeCore.h"
#include "Root.h"
#include "Bindings/ApiDocs.h"
#include "Entities/Player.h"
#include "ChatColor.h"
#include "RankManager.h"

cNativeCore::cNativeCore()
{
}

cNativeCore::~cNativeCore()
{
}

void cNativeCore::Initialize()
{
	auto pm = cPluginManager::Get();
	auto self = std::shared_ptr<cNativeCore>(this, [](cNativeCore*){}); // No-op deleter since Root owns us

	pm->BindCommand("/help",  nullptr, self, "core.help", "- Shows available commands and API info");
	pm->BindCommand("/tp",    nullptr, self, "core.tp",   "<player> [target] - Teleports players");
	pm->BindCommand("/rank",  nullptr, self, "core.rank", "<player> [rank] - Manages player ranks");
	
	LOG("Native C++ Core initialized with ApiDocs support.");
}

bool cNativeCore::ExecuteCommand(
	const AStringVector & a_Split,
	cPlayer * a_Player,
	const AString & a_Command,
	cCommandOutputCallback * a_Output
)
{
	if (a_Split.empty()) return false;

	if (a_Split[0] == "/help") return HandleHelp(a_Split, a_Player);
	if (a_Split[0] == "/tp")   return HandleTeleport(a_Split, a_Player);
	if (a_Split[0] == "/rank") return HandleRank(a_Split, a_Player);

	return false;
}

bool cNativeCore::HandleHelp(const AStringVector & a_Split, cPlayer * a_Player)
{
	if (a_Player == nullptr) return false;

	if (a_Split.size() == 1)
	{
		a_Player->SendMessage(AString(cChatColor::Green) + "--- Native Core Help ---");
		a_Player->SendMessage(AString(cChatColor::Yellow) + "/help [class] " + cChatColor::White + "- Shows API documentation");
		a_Player->SendMessage(AString(cChatColor::Yellow) + "/tp <player> " + cChatColor::White + "- Teleport to player");
		a_Player->SendMessage(AString(cChatColor::Yellow) + "/rank <player> [rank] " + cChatColor::White + "- View or set rank");
		return true;
	}

	// Dynamic API help from ApiDocs
	const auto & registry = ApiDocs::GetApiRegistry();
	auto it = registry.find(a_Split[1]);
	if (it != registry.end())
	{
		const auto & cls = it->second;
		a_Player->SendMessage(AString(cChatColor::Green) + "API Class: " + cls.Name);
		a_Player->SendMessage(AString(cChatColor::Italic) + cls.Desc);
		if (!cls.Inherits.empty())
		{
			a_Player->SendMessage(AString(cChatColor::Gray) + "Inherits: " + cls.Inherits[0]);
		}
		return true;
	}

	a_Player->SendMessage(AString(cChatColor::Red) + "Unknown API class: " + a_Split[1]);
	return true;
}

bool cNativeCore::HandleTeleport(const AStringVector & a_Split, cPlayer * a_Player)
{
	if (a_Player == nullptr) return false;
	if (a_Split.size() < 2) return false;

	AString TargetName = a_Split[1];
	cRoot::Get()->DoWithPlayer(TargetName, [a_Player](cPlayer & a_Target)
	{
		a_Player->TeleportToEntity(a_Target);
		a_Player->SendMessage(AString(cChatColor::Green) + "Teleported to " + a_Target.GetName());
		return true;
	});

	return true;
}

bool cNativeCore::HandleRank(const AStringVector & a_Split, cPlayer * a_Player)
{
	if (a_Player == nullptr) return false;
	if (a_Split.size() < 2) return false;

	auto rm = cRoot::Get()->GetRankManager();
	AString PlayerName = a_Split[1];

	if (a_Split.size() == 2)
	{
		// View rank
		cRoot::Get()->DoWithPlayer(PlayerName, [a_Player, rm](cPlayer & a_Target)
		{
			AString Rank = rm->GetPlayerRankName(a_Target.GetUUID());
			a_Player->SendMessage(AString(cChatColor::Yellow) + a_Target.GetName() + " is rank: " + (Rank.empty() ? "Default" : Rank));
			return true;
		});
	}
	else
	{
		// Set rank
		AString NewRank = a_Split[2];
		if (!rm->RankExists(NewRank))
		{
			a_Player->SendMessage(AString(cChatColor::Red) + "Rank does not exist!");
			return true;
		}

		cRoot::Get()->DoWithPlayer(PlayerName, [a_Player, rm, NewRank](cPlayer & a_Target)
		{
			rm->SetPlayerRank(a_Target.GetUUID(), a_Target.GetName(), NewRank);
			a_Player->SendMessage(AString(cChatColor::Green) + "Rank set successfully.");
			return true;
		});
	}

	return true;
}
