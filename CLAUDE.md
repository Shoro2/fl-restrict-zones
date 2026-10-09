# fl-restrict-zones

Keeps non-GM players out of whole maps on Forgotten Land. A map listed in
`acore_world.restricted_zones_lock` is forbidden: on every zone change a player there is teleported
to the Azealia Under entrance (map 741) with a warning, each offence is counted per account, and
past the warning limit the player is kicked. A login line announces the module. The old FL
server's lock table listed maps 0, 530 and 571 (Eastern Kingdoms, Outland, Northrend; old
`update_world`, read 2026-10-09). There is no module README (`.github/README.md` is the
AzerothCore skeleton text).

## Ids and tables

| What | Id / name |
|---|---|
| `acore_world.restricted_zones_lock` | `mapId`, `zoneID` (unused by the code), `comment`; the lock list (module SQL `flrz_lock.sql`) |
| `acore_characters.restricted_zones_exploit` | `accountId`, `count`; offences per account (module SQL `flrz_exploit.sql`) |
| Teleport target | map 741 (Azealia Under) at (-241.954, 2160.460, 78.504) |
| Scripts | PlayerScript `KeepOutPlayerScript`, WorldScript `KeepOutWorldScript` |

## Status and progress

- **Where it runs**: workbench (built from `azerothcore-wotlk/modules/fl-restrict-zones`) and
  host since the first deploy (2026-07-13), both at `700a0ee`. On the workbench the keep-out is
  **off**: the deployed conf has `FLRZ_Enabled = 0` (the dist says 1) and the lock table is
  empty; only the login announcement shows. The host's conf value and lock rows are not
  verified from here. `restricted_zones_exploit` holds the old server's rows, carried over by the
  character migration.
- **Evidence**: module test pass incl. restrict-zones T2 (user-confirmed 2026-07-13).
- **Done**: port to the current AzerothCore API (2026-07-09); the placeholder teleport to map 13
  rebound to the map-741 entrance. No work since.

## Next steps

No open work known; last change 2026-07-09.

1. (suggestion) Decide whether FL still wants map locks. If yes: fill `restricted_zones_lock`
   through share-public `python_scripts/fl_content/sql/` with a MIG entry and set
   `FLRZ_Enabled = 1`; if no: also turn `FLRZ_Announcer` off so players are not told about an
   inactive module.
2. (suggestion) Check the host's `fl_restrict_zones.conf` and lock rows read-only before any
   change; the workbench value must not be assumed there.

Open points in full: [todo.md](todo.md).

## Working here

- Branch `claude/<topic>-<sessionId>`, merge into `master` and push (project rule: no pull requests).
- C++ changes: rebuild worldserver (`C:\wowstuff\dcore_bin`). The `FLRZ_*` keys except
  `FLRZ_Announcer` are read once at startup (not on `.reload config`), so a conf change needs a
  restart with `C:\wowstuff\ForgottenLand2.0\scripts\worldserver_restart.ps1`.
- The deployed conf is `C:\wowstuff\dcore\configs\modules\fl_restrict_zones.conf`; it differs from
  the dist, so read it before stating a runtime value.
- Host-relevant changes (code, lock rows, conf keys) need a MIG entry in share-public
  `docs/World of Warcraft/forgotten-land/15-host-migration-log.md`.
- Vault: `forgotten-land/06-fl-modules.md`.
- Doc set: [INDEX.md](INDEX.md), CLAUDE.md, [data_structure.md](data_structure.md),
  [functions.md](functions.md), [log.md](log.md) (newest first), [todo.md](todo.md).
