
#include "Globals.h"
#include "RankManager.h"
#include "FasterRankManager.h"
#include "Protocol/MojangAPI.h"

cRankManager::cRankManager(void) :
	m_IsInitialized(false),
	m_FasterStore(new FasterDB::cFasterRankManager("Ranks"))
{
}

cRankManager::~cRankManager()
{
}

void cRankManager::Initialize(cMojangAPI & a_MojangAPI)
{
	ASSERT(!m_IsInitialized);
	m_IsInitialized = true;
	a_MojangAPI.SetRankManager(this);

	if (AreTablesEmpty())
	{
		CreateDefaults();
	}

	if (!m_FasterStore->GetDefaultRank(m_DefaultRank))
	{
		m_DefaultRank = "Default";
		m_FasterStore->SetDefaultRank(m_DefaultRank);
	}
}

AString cRankManager::GetPlayerRankName(const cUUID & a_PlayerUUID)
{
	AString Rank, Player;
	if (m_FasterStore->GetPlayerRank(a_PlayerUUID, Rank, Player))
	{
		return Rank;
	}
	return "";
}

AString cRankManager::GetPlayerName(const cUUID & a_PlayerUUID)
{
	AString Rank, Player;
	if (m_FasterStore->GetPlayerRank(a_PlayerUUID, Rank, Player))
	{
		return Player;
	}
	return "";
}

AStringVector cRankManager::GetPlayerGroups(const cUUID & a_PlayerUUID)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		Rank = m_DefaultRank;
	}
	return GetRankGroups(Rank);
}

AStringVector cRankManager::GetPlayerPermissions(const cUUID & a_PlayerUUID)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		Rank = m_DefaultRank;
	}
	return GetRankPermissions(Rank);
}

AStringVector cRankManager::GetPlayerRestrictions(const cUUID & a_PlayerUUID)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		Rank = m_DefaultRank;
	}
	return GetRankRestrictions(Rank);
}

AStringVector cRankManager::GetRankGroups(const AString & a_RankName)
{
	AStringVector Groups;
	m_FasterStore->GetRankGroups(a_RankName, Groups);
	return Groups;
}

AStringVector cRankManager::GetGroupPermissions(const AString & a_GroupName)
{
	AStringVector Perms;
	m_FasterStore->GetGroupPermissions(a_GroupName, Perms);
	return Perms;
}

AStringVector cRankManager::GetGroupRestrictions(const AString & a_GroupName)
{
	AStringVector Rests;
	m_FasterStore->GetGroupRestrictions(a_GroupName, Rests);
	return Rests;
}

AStringVector cRankManager::GetRankPermissions(const AString & a_RankName)
{
	AStringVector Groups = GetRankGroups(a_RankName);
	AStringVector AllPerms;
	for (const auto & Group : Groups)
	{
		AStringVector GroupPerms = GetGroupPermissions(Group);
		AllPerms.insert(AllPerms.end(), GroupPerms.begin(), GroupPerms.end());
	}
	// Remove duplicates
	std::sort(AllPerms.begin(), AllPerms.end());
	AllPerms.erase(std::unique(AllPerms.begin(), AllPerms.end()), AllPerms.end());
	return AllPerms;
}

AStringVector cRankManager::GetRankRestrictions(const AString & a_RankName)
{
	AStringVector Groups = GetRankGroups(a_RankName);
	AStringVector AllRests;
	for (const auto & Group : Groups)
	{
		AStringVector GroupRests = GetGroupRestrictions(Group);
		AllRests.insert(AllRests.end(), GroupRests.begin(), GroupRests.end());
	}
	// Remove duplicates
	std::sort(AllRests.begin(), AllRests.end());
	AllRests.erase(std::unique(AllRests.begin(), AllRests.end()), AllRests.end());
	return AllRests;
}

std::vector<cUUID> cRankManager::GetAllPlayerUUIDs(void)
{
	return m_FasterStore->GetAllPlayerUUIDs();
}

AStringVector cRankManager::GetAllRanks(void)
{
	return m_FasterStore->GetAllRanks();
}

AStringVector cRankManager::GetAllGroups(void)
{
	return m_FasterStore->GetAllGroups();
}

AStringVector cRankManager::GetAllPermissions(void)
{
	AStringVector res;
	AStringVector Groups = GetAllGroups();
	for (const auto & g : Groups)
	{
		AStringVector p = GetGroupPermissions(g);
		res.insert(res.end(), p.begin(), p.end());
	}
	std::sort(res.begin(), res.end());
	res.erase(std::unique(res.begin(), res.end()), res.end());
	return res;
}

AStringVector cRankManager::GetAllRestrictions(void)
{
	AStringVector res;
	AStringVector Groups = GetAllGroups();
	for (const auto & g : Groups)
	{
		AStringVector r = GetGroupRestrictions(g);
		res.insert(res.end(), r.begin(), r.end());
	}
	std::sort(res.begin(), res.end());
	res.erase(std::unique(res.begin(), res.end()), res.end());
	return res;
}

AStringVector cRankManager::GetAllPermissionsRestrictions(void)
{
	AStringVector res = GetAllPermissions();
	AStringVector r = GetAllRestrictions();
	res.insert(res.end(), r.begin(), r.end());
	std::sort(res.begin(), res.end());
	res.erase(std::unique(res.begin(), res.end()), res.end());
	return res;
}

bool cRankManager::GetPlayerMsgVisuals(const cUUID & a_PlayerUUID, AString & a_MsgPrefix, AString & a_MsgSuffix, AString & a_MsgNameColorCode)
{
	AString Rank = GetPlayerRankName(a_PlayerUUID);
	if (Rank.empty())
	{
		Rank = m_DefaultRank;
	}
	return GetRankVisuals(Rank, a_MsgPrefix, a_MsgSuffix, a_MsgNameColorCode);
}

void cRankManager::AddRank(const AString & a_RankName, const AString & a_MsgPrefix, const AString & a_MsgSuffix, const AString & a_MsgNameColorCode)
{
	if (RankExists(a_RankName))
	{
		return;
	}
	m_FasterStore->SetRankDetails(a_RankName, a_MsgPrefix, a_MsgSuffix, a_MsgNameColorCode);
}

void cRankManager::AddGroup(const AString & a_GroupName)
{
	if (GroupExists(a_GroupName))
	{
		return;
	}
	m_FasterStore->SetGroupPermissions(a_GroupName, {});
}

void cRankManager::AddGroups(const AStringVector & a_GroupNames)
{
	for (const auto & g : a_GroupNames)
	{
		AddGroup(g);
	}
}

bool cRankManager::AddGroupToRank(const AString & a_GroupName, const AString & a_RankName)
{
	if (!RankExists(a_RankName) || !GroupExists(a_GroupName))
	{
		return false;
	}
	AStringVector Groups = GetRankGroups(a_RankName);
	if (std::find(Groups.begin(), Groups.end(), a_GroupName) != Groups.end())
	{
		return false;
	}
	Groups.push_back(a_GroupName);
	m_FasterStore->SetRankGroups(a_RankName, Groups);
	return true;
}

bool cRankManager::AddPermissionToGroup(const AString & a_Permission, const AString & a_GroupName)
{
	if (!GroupExists(a_GroupName))
	{
		return false;
	}
	AStringVector Perms = GetGroupPermissions(a_GroupName);
	if (std::find(Perms.begin(), Perms.end(), a_Permission) != Perms.end())
	{
		return false;
	}
	Perms.push_back(a_Permission);
	m_FasterStore->SetGroupPermissions(a_GroupName, Perms);
	return true;
}

bool cRankManager::AddRestrictionToGroup(const AString & a_Restriction, const AString & a_GroupName)
{
	if (!GroupExists(a_GroupName))
	{
		return false;
	}
	AStringVector Rests = GetGroupRestrictions(a_GroupName);
	if (std::find(Rests.begin(), Rests.end(), a_Restriction) != Rests.end())
	{
		return false;
	}
	Rests.push_back(a_Restriction);
	m_FasterStore->SetGroupRestrictions(a_GroupName, Rests);
	return true;
}

bool cRankManager::AddPermissionsToGroup(const AStringVector & a_Permissions, const AString & a_GroupName)
{
	bool res = true;
	for (const auto & p : a_Permissions)
	{
		res = AddPermissionToGroup(p, a_GroupName) && res;
	}
	return res;
}

bool cRankManager::AddRestrictionsToGroup(const AStringVector & a_Restrictions, const AString & a_GroupName)
{
	bool res = true;
	for (const auto & r : a_Restrictions)
	{
		res = AddRestrictionToGroup(r, a_GroupName) && res;
	}
	return res;
}

void cRankManager::RemoveRank(const AString & a_RankName, const AString & a_ReplacementRankName)
{
	if (!RankExists(a_RankName))
	{
		return;
	}

	// Reassign players
	AString Replacement = a_ReplacementRankName;
	if (Replacement.empty() || !RankExists(Replacement))
	{
		Replacement = "";
	}

	std::vector<cUUID> AllPlayers = GetAllPlayerUUIDs();
	for (const auto & UUID : AllPlayers)
	{
		if (GetPlayerRankName(UUID) == a_RankName)
		{
			if (Replacement.empty())
			{
				RemovePlayerRank(UUID);
			}
			else
			{
				SetPlayerRank(UUID, GetPlayerName(UUID), Replacement);
			}
		}
	}

	if (m_DefaultRank == a_RankName)
	{
		if (!Replacement.empty())
		{
			SetDefaultRank(Replacement);
		}
	}

	m_FasterStore->RemoveRank(a_RankName);
}

void cRankManager::RemoveGroup(const AString & a_GroupName)
{
	if (!GroupExists(a_GroupName))
	{
		return;
	}

	// Remove from all ranks
	AStringVector Ranks = GetAllRanks();
	for (const auto & Rank : Ranks)
	{
		RemoveGroupFromRank(a_GroupName, Rank);
	}

	m_FasterStore->RemoveGroup(a_GroupName);
}

void cRankManager::RemoveGroupFromRank(const AString & a_GroupName, const AString & a_RankName)
{
	AStringVector Groups = GetRankGroups(a_RankName);
	Groups.erase(std::remove(Groups.begin(), Groups.end(), a_GroupName), Groups.end());
	m_FasterStore->SetRankGroups(a_RankName, Groups);
}

void cRankManager::RemovePermissionFromGroup(const AString & a_Permission, const AString & a_GroupName)
{
	AStringVector Perms = GetGroupPermissions(a_GroupName);
	Perms.erase(std::remove(Perms.begin(), Perms.end(), a_Permission), Perms.end());
	m_FasterStore->SetGroupPermissions(a_GroupName, Perms);
}

void cRankManager::RemoveRestrictionFromGroup(const AString & a_Restriction, const AString & a_GroupName)
{
	AStringVector Rests = GetGroupRestrictions(a_GroupName);
	Rests.erase(std::remove(Rests.begin(), Rests.end(), a_Restriction), Rests.end());
	m_FasterStore->SetGroupRestrictions(a_GroupName, Rests);
}

bool cRankManager::RenameRank(const AString & a_OldRankName, const AString & a_NewRankName)
{
	if (!RankExists(a_OldRankName) || RankExists(a_NewRankName))
	{
		return false;
	}

	AString Pre, Suf, Col;
	GetRankVisuals(a_OldRankName, Pre, Suf, Col);
	m_FasterStore->SetRankDetails(a_NewRankName, Pre, Suf, Col);

	AStringVector Groups = GetRankGroups(a_OldRankName);
	m_FasterStore->SetRankGroups(a_NewRankName, Groups);

	// Reassign players
	std::vector<cUUID> AllPlayers = GetAllPlayerUUIDs();
	for (const auto & UUID : AllPlayers)
	{
		if (GetPlayerRankName(UUID) == a_OldRankName)
		{
			SetPlayerRank(UUID, GetPlayerName(UUID), a_NewRankName);
		}
	}

	if (m_DefaultRank == a_OldRankName)
	{
		SetDefaultRank(a_NewRankName);
	}

	m_FasterStore->RemoveRank(a_OldRankName);
	return true;
}

bool cRankManager::RenameGroup(const AString & a_OldGroupName, const AString & a_NewGroupName)
{
	if (!GroupExists(a_OldGroupName) || GroupExists(a_NewGroupName))
	{
		return false;
	}

	AStringVector Perms = GetGroupPermissions(a_OldGroupName);
	m_FasterStore->SetGroupPermissions(a_NewGroupName, Perms);

	AStringVector Rests = GetGroupRestrictions(a_OldGroupName);
	m_FasterStore->SetGroupRestrictions(a_NewGroupName, Rests);

	// Update all ranks
	AStringVector Ranks = GetAllRanks();
	for (const auto & Rank : Ranks)
	{
		AStringVector Groups = GetRankGroups(Rank);
		bool changed = false;
		for (auto & g : Groups)
		{
			if (g == a_OldGroupName)
			{
				g = a_NewGroupName;
				changed = true;
			}
		}
		if (changed)
		{
			m_FasterStore->SetRankGroups(Rank, Groups);
		}
	}

	m_FasterStore->RemoveGroup(a_OldGroupName);
	return true;
}

void cRankManager::SetPlayerRank(const cUUID & a_PlayerUUID, const AString & a_PlayerName, const AString & a_RankName)
{
	m_FasterStore->SetPlayerRank(a_PlayerUUID, a_PlayerName, a_RankName);
}

void cRankManager::RemovePlayerRank(const cUUID & a_PlayerUUID)
{
	m_FasterStore->RemovePlayerRank(a_PlayerUUID);
}

void cRankManager::SetRankVisuals(const AString & a_RankName, const AString & a_MsgPrefix, const AString & a_MsgSuffix, const AString & a_MsgNameColorCode)
{
	if (!RankExists(a_RankName))
	{
		return;
	}
	m_FasterStore->SetRankDetails(a_RankName, a_MsgPrefix, a_MsgSuffix, a_MsgNameColorCode);
}

bool cRankManager::GetRankVisuals(const AString & a_RankName, AString & a_MsgPrefix, AString & a_MsgSuffix, AString & a_MsgNameColorCode)
{
	return m_FasterStore->GetRankDetails(a_RankName, a_MsgPrefix, a_MsgSuffix, a_MsgNameColorCode);
}

bool cRankManager::RankExists(const AString & a_RankName)
{
	AString p, s, c;
	return m_FasterStore->GetRankDetails(a_RankName, p, s, c);
}

bool cRankManager::GroupExists(const AString & a_GroupName)
{
	AStringVector p;
	return m_FasterStore->GetGroupPermissions(a_GroupName, p);
}

bool cRankManager::IsPlayerRankSet(const cUUID & a_PlayerUUID)
{
	return !GetPlayerRankName(a_PlayerUUID).empty();
}

bool cRankManager::IsGroupInRank(const AString & a_GroupName, const AString & a_RankName)
{
	AStringVector Groups = GetRankGroups(a_RankName);
	return std::find(Groups.begin(), Groups.end(), a_GroupName) != Groups.end();
}

bool cRankManager::IsPermissionInGroup(const AString & a_Permission, const AString & a_GroupName)
{
	AStringVector Perms = GetGroupPermissions(a_GroupName);
	return std::find(Perms.begin(), Perms.end(), a_Permission) != Perms.end();
}

bool cRankManager::IsRestrictionInGroup(const AString & a_Restriction, const AString & a_GroupName)
{
	AStringVector Rests = GetGroupRestrictions(a_GroupName);
	return std::find(Rests.begin(), Rests.end(), a_Restriction) != Rests.end();
}

void cRankManager::NotifyNameUUID(const AString & a_PlayerName, const cUUID & a_UUID)
{
	UpdatePlayerName(a_UUID, a_PlayerName);
}

bool cRankManager::SetDefaultRank(const AString & a_RankName)
{
	if (!RankExists(a_RankName))
	{
		return false;
	}
	m_DefaultRank = a_RankName;
	m_FasterStore->SetDefaultRank(a_RankName);
	return true;
}

AString cRankManager::GetDefaultRank(void)
{
	return m_DefaultRank;
}

void cRankManager::ClearPlayerRanks(void)
{
	std::vector<cUUID> Players = GetAllPlayerUUIDs();
	for (const auto & UUID : Players)
	{
		RemovePlayerRank(UUID);
	}
}

bool cRankManager::UpdatePlayerName(const cUUID & a_PlayerUUID, const AString & a_NewPlayerName)
{
	AString Rank, OldName;
	if (m_FasterStore->GetPlayerRank(a_PlayerUUID, Rank, OldName))
	{
		m_FasterStore->SetPlayerRank(a_PlayerUUID, a_NewPlayerName, Rank);
		return true;
	}
	return false;
}

bool cRankManager::AreTablesEmpty(void)
{
	return GetAllRanks().empty();
}

void cRankManager::CreateDefaults(void)
{
	AddRank("Default", "", "", "");
	AddGroup("Default");
	AddGroupToRank("Default", "Default");
	SetDefaultRank("Default");
}
