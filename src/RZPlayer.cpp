/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "AccountMgr.h"
#include "Chat.h"
#include "Configuration/Config.h"
#include "Creature.h"
#include "Define.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptMgr.h"

struct FLRZ
{
    uint32 maxWarnings;
    bool keepOutEnabled;
    bool teleportEnabled;
    bool kickEnabled;
};

FLRZ flrz;

void teleportPlayer(Player* player)
{
	//todo insert teleport location for azealia
	player->TeleportTo(0, -8833.38f, 628.628f, 94.0066f, 1.06535f);
    ChatHandler(player->GetSession()).PSendSysMessage("You have gone to a forbidden place your actions have been logged.");
}

void checkZoneKeepOut(Player* player)
{
    if (player->GetSession()->GetSecurity() >= SEC_MODERATOR)
        return;

    uint32 mapId = player->GetMapId();
    uint32 zoneId = player->GetZoneId();

    QueryResult result = WorldDatabase.Query("SELECT * FROM `restricted_zones_lock` WHERE `mapId`={} AND `zoneID`={}", mapId, zoneId);

    if (!result)
        return;

    uint32 accountId = player->GetSession()->GetAccountId();
    uint8 countWarnings = 1;

    QueryResult playerWarning = CharacterDatabase.Query("SELECT * FROM `restricted_zones_exploit` WHERE `accountId`={}", accountId);

    if (!playerWarning)
    {
        CharacterDatabase.Execute("INSERT INTO `restricted_zones_exploit` (`accountId`, `count`) VALUES ({}, {})", accountId, countWarnings);

        if (flrz.teleportEnabled)
            teleportPlayer(player);
    }
    else
    {
        countWarnings = (*playerWarning)[1].Get<uint8>() + 1;

        if (countWarnings <= flrz.maxWarnings)
        {
            CharacterDatabase.Execute("UPDATE `restricted_zones_exploit` SET `count`={} WHERE `accountId`={}", countWarnings, accountId);
            teleportPlayer(player);
        }
        else
        {
            if (flrz.teleportEnabled && !flrz.kickEnabled)
                teleportPlayer(player);
            else if (flrz.kickEnabled)
                player->GetSession()->KickPlayer("FLRZ:: Entering a place not allowed.", true);
            else
                ChatHandler(player->GetSession()).PSendSysMessage("You have gone to a forbidden place your actions have been logged.");
        }
    }
}

class RZPlayerScript : public PlayerScript
{
public:
    RZPlayerScript() : PlayerScript("KeepOutPlayerScript"){ }

    void OnPlayerLogin(Player* player) override
    {
        if (sConfigMgr->GetOption<bool>("FLRZ_Announcer", true))
            ChatHandler(player->GetSession()).PSendSysMessage("This server is running the |cff4CFF00Restricted Zones |rmodule.");
    }

    void OnPlayerUpdateZone(Player* player, uint32 /*newZone*/,  uint32 /*newArea*/) override
    {
        if (flrz.keepOutEnabled)
            checkZoneKeepOut(player);
    }
};

class RZWorldScript : public WorldScript
{
public:
    RZWorldScript() : WorldScript("KeepOutWorldScript") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload)
        {
            flrz.maxWarnings = sConfigMgr->GetOption<int>("FLRZ_MaxWarnings", 3);
            flrz.keepOutEnabled = sConfigMgr->GetOption<bool>("FLRZ_Enabled", true);
            flrz.teleportEnabled = sConfigMgr->GetOption<bool>("FLRZ_TeleportEnabled", true);
            flrz.kickEnabled = sConfigMgr->GetOption<bool>("FLRZ_KickPlayerEnabled", true);
        }
    }
};

void AddRZScripts()
{
    new RZWorldScript();
    new RZPlayerScript();
}