# 16 — Mining (§7A) & Salvage (§7B)
Source: Aaron's design update, 2026-10-27.

## §7A Mining
Four deposit types, each a different flying problem:
| Deposit | How it plays |
|---|---|
| **Asteroid belts** | Need a stable orbit, so a miner is a sitting target by design. |
| **Ice fields** | Long stationary holds. |
| **Gas clouds** | Siphoned while flying slowly with no cover. |
| **Deep-core anomalies** | Mix combat with extraction. |

- Ore is **finite per rock**. Belt respawn scales to **regional consumption**, not the system's, so a heavily mined region feels mined out.
- Mining hulls are their own line (doc 14).
- **Industrial command ships** boost the fleet's rate, not their own, so mining is social.
- Large asteroids can be landed on and built on (doc 17, doc 19).

## §7B Salvage
### Site tiers
Fresh field wreck → cooled hulk → ancient derelict.

### Damage grades
- **G-A** (intact, powered down): yields **80–95%** of fitted modules. The real prize.
- Grade drops **one step per 24 h** untouched, and **one step each time it's worked**. A hulk looted twice is a worse hulk.
- Explosive kills grade lower; precise damage grades higher, so clean kills pay.

### Resale and rebuild
- A good hull can be **towed and rebuilt at ~30% of new-build cost at G-A**.
- Rebuilt hulls carry a **"recovered" provenance tag**.
- This is the only faucet of existing hulls, a route into ship ownership for non-industrial players. A destroyed fleet has residual value someone will contract to collect.
- **Station derelicts** yield station modules (habitat blocks, dock sections); they are not rebuildable themselves.

### §7B.2 Hulk flags
Hulk interiors are **shared**, not solo-instanced. You pick a flag on breach:
- **Solo:** nobody else may enter. The flag holds for the decay window so a solo run can't be jumped.
- **Open:** anyone enters; contested wrecks and fights. Compartments are knife-range, so a well-flown salvage frigate can beat a bigger ship inside a hull.
- **Corp:** your company only.

The flag is **broadcast at scanner range** before you breach.

### Site classes
| Class | Examples | Allowed flags |
|---|---|---|
| **1 Small** | Frigate/destroyer hulks, rogue dens | Solo or Corp only. PVE by rule; the entry profession. |
| **2 Mid** | Cruiser hulks, pirate hideouts, worked mines | Solo, Open or Corp. |
| **3 Large** | Battleship hulks, warrens | Open or Corp only. |
| **4 Massive** | Capital and station derelicts, sentinel vaults | Open only, no flag. Contested objectives, not instances: anyone who scans the site can breach the same wreck. Endgame of salvage. |

### Open pays ~2x Solo
Paid four ways:
1. Full interior room count, instead of a truncated one.
2. Access to sealed and locked containers.
3. Longer decay window, instead of a short stability clock.
4. If you own the wreck's claim volume, a **salvage-rights toll** on other entrants' yield.

**Class 3–4 sites in Secured space.** They still spawn in Secured space. There, breaching under the Open/salvage PVP flag is an opt-in exception to Secured's no-hostility rule, like the duel flag. Choosing not to flag is allowed but gives reduced rewards.

**Solo must never be a trap.** It gets a shorter clock but a guaranteed room. No NPC spawn scales with another player's presence, and nothing inside is designed to make a solo run unwinnable.
