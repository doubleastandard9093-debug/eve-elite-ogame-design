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

## Jump drives: range, fuel and drive strain (first pass)
Ships carry their own jump drive for system-to-system travel off the gate network. Every number here is first pass. Light ships are long-range explorers; big ships make short, fuel-heavy jumps and are rate-limited.

| Class | Base range | Max (skills/fit) | Fuel |
|---|---|---|---|
| Light (fighters, corvettes, frigates, scouts) | 12 ly | 40 ly | Curve: f = k × (d × m / m_opt)^p, p = 2.0 |
| Medium (destroyers, cruisers, haulers, miners) | 10 ly | 30 ly | Same curve, p = 2.3 |
| Large (battlecruisers, battleships) | 4 ly | 8 ly | 7 t per ly |
| Capital (carriers, dreadnoughts, capital industrials) | 3.5 ly | 7 ly | 30 t per ly |
| Supercapital (flagship-class capitals) | 3 ly | 6 ly | 30 t per ly |
| Jump freighter / capital industrial hauler | 5 ly | 10 ly | 90 t per ly |

- **Light and medium fuel curve.** Fuel per jump rises faster than distance: k is set by drive grade (about 8–12, ×10⁻³ for tonnes), m is the ship's current mass including cargo and fuel, m_opt is the drive's rated mass, and p grows with drive size. Short hops are cheap; a max-range jump drains the tank. A full hold shortens your reach, which ties into the live-mass flight model (doc 02).
- **Large and capital fuel** is linear per light-year. Skills can cut it by up to 50%.
- **Range skill:** +20% range per level, five levels, which doubles base range.
- **Drive strain (large ships and up).** Each jump adds strain: at least 10 × (1 + ly) minutes, or existing strain × (1 + ly), whichever is higher, capped at **5 hours**. Strain decays in real time.
  - **Spool-down:** after a jump the drive can't fire again for 1/10 of the strain before the jump, at least (1 + ly) minutes, capped at **30 minutes**.
  - Haulers, jump freighters and capital industrials count only 10% of the distance for strain.
  - Net effect: a capital can jump out and back quickly once, but a second long jump strands it for a while. Defended gas-giant stations (doc 19) set where fleets can reach.

## Lanes are owned corridors
Lanes are infrastructure, not scenery. Each lane has:
- **Alignment**, **integrity**, **signature damping** and a **toll**.
- **Owner.** NPCs own the core Secured network and it is free. Players only build lanes where the NPC network doesn't reach. That prevents a player monopoly on basic travel.
- **Economic weight.** Cut a Secured lane between two industrial hubs and downstream prices move within the hour, because the gate route is 4–5x slower.
- **Interdiction** can pull ships out of a lane, only in claimed volumes or Frontier (doc 15).
