# 13 — Scale, Galaxy & Travel
Source: Aaron's design update, 2026-10-27.

## Scale and galaxy
- **One massive galaxy** on the scale of a full simulated galaxy (the update refers to a 5,000-system universe) with planned expansion into **more galaxies** later.
- Every system can hold planet infrastructure, missions, treasures, claims and building.
- **Space stations** in every settled system.
- **Trade lanes** in main secured space, giving increased speed inside a system.
- **Jump gates** between systems, and **instance jump lanes** that jump you to instanced sites.
- **Pirate factions** and **pirate bases you can take over**.
- **Derelicts** you can salvage; some can be repaired and sold (doc 16, §7B).
- **NPCs sell limited goods and ships** (bare hulls only; see doc 14 and doc 03).
- **Hidden wormholes and other signatures** that must be scanned down.
- **PVE focus with limited PVP** (doc 15).

## Flight and autopilot
Flight is either in the cockpit or AI-driven while you do other things. Autopilot orders are limited to:
- Orbit a location
- Defend self
- Mine
- Hold stationary
- Travel to location, then auto dock/land

Defend self, mine and hold can run while orbiting or stationary.

## Travel-speed ladder (slowest → fastest)
1. **Sub Space**: throttle controlled. Used for fights, docking, landing and salvage.
2. **Slipstream**: in-system travel.
3. **Instance jump lane**: jumps to instanced sites.
4. **Trade lane**: travel between settled locations within a system.
5. **Wormholes / jump gates / stellar anomalies**: long jumps, such as to another system. Odd distortions that throw a ship a long way, often to another system, sometimes at random. Not to be confused with encounter anomalies (doc 20).
6. **Galactic gates** (incomplete / ancient ruins): will eventually jump to other galaxies.

## Interstellar travel: gates for small ships, drives for capitals (first pass)
**Who needs gates.** Ships from fighters through battleships have **no independent interstellar jump drive**. To leave a system they use **jump gates, instanced jump lanes, wormholes or stellar anomalies**. Inside a system they still use sub-space, slipstream and trade lanes as normal.
- Battleships sit in the gated group (confirmed by Aaron 2026-10-28).

**Capital-size ships jump on their own drives.** That covers capitals, supercapitals, jump freighters and capital industrials.

| Class | Base range | Max (skills) | Fuel |
|---|---|---|---|
| Capital (carriers, dreadnoughts) | 3.5 ly | 7 ly | 30 t per ly |
| Supercapital (flagship-class capitals) | 3 ly | 6 ly | 30 t per ly |
| Jump freighter / capital industrial | 5 ly | 10 ly | 90 t per ly |

- **Fuel** is linear per light-year. Skills cut it by up to 50%.
- **Range skill:** +20% range per level, five levels, which doubles base range.
- **Drive strain.** Each jump adds strain: at least 10 × (1 + ly) minutes, or existing strain × (1 + ly), whichever is higher, capped at **5 hours**. Strain decays in real time.
  - **Spool-down:** after a jump the drive can't fire again for 1/10 of the strain before the jump, at least (1 + ly) minutes, capped at **30 minutes**.
  - Jump freighters and capital industrials count only 10% of the distance for strain.
  - Net effect: a capital can jump out and back quickly once, but a second long jump strands it for a while.
- **Capitals and gates (default in place; not explicitly confirmed):** capitals are **too large for standard gates**. A few **large gates** (rare, high-value, usually on major routes or player-built) let them through. So a capital chooses between a fuel-and-strain jump and a detour to a large gate, and large gates become strategic sites.

### Ripple effects
- **Gates are chokepoints** for all small-ship traffic between systems. That raises the value of gate control, gate and lane tolls, and interdiction near gates and lanes (doc 15).
- **Gas-giant fuel stations** matter mainly for **capital projection** and for local fuel. Small ships burn fuel in-system (sub-space, slipstream); a gate trip costs them no jump fuel.
- **Exploration (decided 2026-10-28):** small ships (fighters through battleships) reach Frontier space beyond the gate network **only through wormholes**. **Scanning down wormholes is the explorer's core skill.** Explorers find and chart the routes everyone else's small ships depend on.
- **Frontier logistics depend on both.** Capitals self-jump into The Black, but every small hull (haulers, miners, escorts, salvagers) arrives through a wormhole. A Frontier supply chain needs jump freighters and capitals for bulk and projection, and live wormhole routes for everything else. Losing a wormhole route strands small ships on one side.

## Lanes are owned corridors
Lanes are infrastructure, not scenery. Each lane has:
- **Alignment**, **integrity**, **signature damping** and a **toll**.
- **Owner.** NPCs own the core Secured network and it is free. Players only build lanes where the NPC network doesn't reach. That prevents a player monopoly on basic travel.
- **Economic weight.** Cut a Secured lane between two industrial hubs and downstream prices move within the hour, because the gate route is 4–5x slower.
- **Interdiction** can pull ships out of a lane, only in claimed volumes or Frontier (doc 15).
