# Functions

## Scripts

| Script | Hook | What |
|---|---|---|
| `KeepOutWorldScript` (`RZWorldScript`) | `OnBeforeConfigLoad(bool reload)` | on the first load only, reads `FLRZ_MaxWarnings`, `FLRZ_Enabled`, `FLRZ_TeleportEnabled`, `FLRZ_KickPlayerEnabled` into the global struct `flrz` |
| `KeepOutPlayerScript` (`RZPlayerScript`) | `OnPlayerLogin` | sends the announcement when `FLRZ_Announcer` is on |
| | `OnPlayerUpdateZone` | calls `checkZoneKeepOut(player)` when `FLRZ_Enabled` |

Both register with bare constructors, so all hooks of their script type are enabled.

## `checkZoneKeepOut(Player* player)`

1. Accounts with security `SEC_MODERATOR` or higher are skipped.
2. `SELECT * FROM restricted_zones_lock WHERE mapId = <current map>` (world DB, synchronous, on
   every zone change). No row -> done. `zoneID` is not consulted: a row locks the whole map.
3. Looks up the account in `restricted_zones_exploit` (characters DB, synchronous):
   - no row: inserts `count = 1`; teleports only if `FLRZ_TeleportEnabled`.
   - row: `count + 1`; up to `FLRZ_MaxWarnings` it updates the row and teleports (regardless of
     `FLRZ_TeleportEnabled`). Past the limit the row is no longer updated, and the player is
     kicked ("FLRZ:: Entering a place not allowed.") if `FLRZ_KickPlayerEnabled`, else teleported
     if `FLRZ_TeleportEnabled`, else only warned.

## `teleportPlayer(Player* player)`

`TeleportTo(741, -241.953995, 2160.459961, 78.504204, 2.407250)` (Azealia Under entrance) and the
message "You have gone to a forbidden place your actions have been logged." The source still
carries the old comment "todo insert teleport location for azealia".
