# Nullpoint — Game Design

A UE5 **space western** sandbox (PVE-focused) where you work the frontier with sim flight, trade lanes, landable planets and a player-built economy. You **fly your own ship in the cockpit**, trade and fight in **one persistent shared galaxy with a player-run economy**, and grow a **planetary empire** whose mines, build queues, and fleet timers keep running while you're away.

## Pillars
1. **Hands on the stick.** Every ship is flown in first person, with Newtonian-assisted flight.
2. **One galaxy, real stakes.** A single shard. Almost everything is player-built, and ships you lose stay lost.
3. **Your empire never sleeps.** Colonies, research, and fleets run on real-time timers.
4. **Groups shape the map.** Syndicates claim systems, run logistics, and go to war.
5. **Your crew, your story.** Officers crew your stations, companions have arcs, and your choices are remembered.
6. **Sit in the cockpit.** Optional seated PC VR (Quest Link/Air Link via OpenXR) with diegetic cockpit UI.

## How the inspirations map in
| Inspiration | What we borrow (as mechanics, not content) |
|---|---|
| Elite Dangerous | Cockpit flight, exploration scanning, trade and bounty loops, supercruise-style travel |
| EVE Online | Single-shard economy, industry chains, corporations, sovereignty, full-loot risk |
| OGame | Colonies, resource mines, build queues, research tree, timed fleet missions, raids |
| Firefly (TV) | Space-western tone, a small crew on a working ship, salvage and smuggling on the frontier |
| No Man's Sky | Landable planets, surface building, onboard social space |
| Freelancer | Trade lanes as in-system highways, mouse-and-keyboard flight |
| Star Trek Online | Bridge-officer stations, ship consoles, live power levels, four shield facings, away missions |
| Star Wars: The Old Republic | Origin story arcs, companions with affinity, choices with consequences, instanced story and ground layer |

All names, lore, and assets are original.

## Platform
Built in **Unreal Engine 5** (C++ plus Blueprints) on dedicated servers, backed by a persistent backend service. Colony management is an **in-game UI layer** (Holo-Command), not a separate client.

## Docs
0. [Inspiration Analysis](docs/00-inspiration-analysis.md)
1. [Core Loop](docs/01-core-loop.md)
2. [Flight & Combat](docs/02-flight-and-combat.md)
3. [Economy & Industry](docs/03-economy-and-industry.md)
4. [Planets & Colonies](docs/04-planets-and-colonies.md)
5. [Research Tree](docs/05-research-tree.md)
6. [Syndicates & Territory](docs/06-corporations-and-territory.md)
7. [Progression & Risk](docs/07-progression-and-risk.md)
8. [Tech Architecture (UE5)](docs/08-tech-architecture.md)
9. [MVP Roadmap](docs/09-mvp-roadmap.md)
10. [Crew & Story](docs/10-crew-and-story.md)
11. [Ground & Boarding](docs/11-ground-and-boarding.md)
12. [VR](docs/12-vr.md)
13. [Scale, Galaxy & Travel](docs/13-scale-galaxy-and-travel.md)
14. [Ships: Supply, Modules & Interiors](docs/14-ships-supply-and-interiors.md)
15. [Claim Defense, PVE & PVP (§7)](docs/15-claim-defense-and-pvp.md)
16. [Mining & Salvage (§7A, §7B)](docs/16-mining-and-salvage.md)
17. [Landing & Interiors (§7C)](docs/17-landing-and-interiors.md)
18. [Antagonists (§7E)](docs/18-antagonists.md)
19. [Building, Stations, Planets & Gas Giants (§7D)](docs/19-building-stations-and-planets.md)
