# Data structure

## Files

| Path | What |
|---|---|
| `src/RZPlayer.cpp` | `checkZoneKeepOut`, `teleportPlayer`, `RZPlayerScript` ("KeepOutPlayerScript"), `RZWorldScript` ("KeepOutWorldScript") |
| `src/RZ_loader.cpp` | `Addfl_restrict_zonesScripts()` -> `AddRZScripts()` |
| `conf/fl_restrict_zones.conf.dist` | `FLRZ_*` keys (below) |
| `data/sql/db-world/updates/flrz_lock.sql` | `CREATE TABLE IF NOT EXISTS restricted_zones_lock` (MyISAM) |
| `data/sql/db-characters/updates/flrz_exploit.sql` | `CREATE TABLE IF NOT EXISTS restricted_zones_exploit` (InnoDB) |
| `data/sql/db-{auth,characters,world}/.gitkeep`, `apps/`, `include.sh`, `pull_request_template.md` | AzerothCore skeleton leftovers |
| `.github/README.md`, `.github/README_ES.md`, `.github/ISSUE_TEMPLATE/` | skeleton template texts, not a module description |
| `.github/workflows/core-build.yml`, `core_codestyle.yml` | CI: AzerothCore's reusable module build + codestyle |

## Tables

| Table | Columns | Content |
|---|---|---|
| `acore_world.restricted_zones_lock` | `mapId` smallint, `zoneID` smallint, `comment` varchar(255); unique (`mapId`, `zoneID`) | the locked maps; empty on the workbench (2026-10-09) |
| `acore_characters.restricted_zones_exploit` | `accountId` int PK, `count` smallint | offence counter per account; the old server's rows came over with the character migration |

The module SQL creates only the tables; no lock rows are shipped.

## Config keys

| Key | Dist | Workbench (deployed) | Meaning |
|---|---|---|---|
| `FLRZ_Announcer` | 1 | 1 | login line "This server is running the Restricted Zones module." |
| `FLRZ_Enabled` | 1 | **0** | the keep-out check on zone change |
| `FLRZ_MaxWarnings` | 3 | 3 | offences that only warn and teleport |
| `FLRZ_TeleportEnabled` | 1 | 1 | teleport on the first offence and, without kick, past the limit |
| `FLRZ_KickPlayerEnabled` | 1 | 1 | kick past the limit |

`FLRZ_Announcer` is read at every login; the other four once at startup (`OnBeforeConfigLoad`
with `reload == false`).
