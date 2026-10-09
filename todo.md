# Todo

No open work known; last change 2026-07-09 (port). The keep-out is off on the workbench
(`FLRZ_Enabled = 0`, empty lock table).

- (low) (suggestion) Decide whether FL still wants map locks. Yes: ship the lock rows through
  share-public `python_scripts/fl_content/sql/` with a MIG entry and enable the check (read the
  host's conf and rows first). No: also set `FLRZ_Announcer = 0` so players are not told about an
  inactive module.
