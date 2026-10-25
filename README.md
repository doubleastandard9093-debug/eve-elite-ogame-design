# HELIOS DRIFT — Game Design (working title)

A UE5 space sandbox where you **fly your own ship in the cockpit**, trade and fight in **one persistent shared galaxy with a player-run economy**, and grow a **planetary empire** whose mines, build queues, and fleet timers keep running while you're away.

## Pillars
1. **Hands on the stick.** Every ship is flown in first person, with Newtonian-assisted flight.
2. **One galaxy, real stakes.** A single shard. Almost everything is player-built, and ships you lose stay lost.
3. **Your empire never sleeps.** Colonies, research, and fleets run on real-time timers.
4. **Groups shape the map.** Syndicates claim systems, run logistics, and go to war.

## How the inspirations map in
| Inspiration | What we borrow (as mechanics, not content) |
|---|---|
| Elite Dangerous | Cockpit flight, exploration scanning, trade and bounty loops, supercruise-style travel |
| EVE Online | Single-shard economy, industry chains, corporations, sovereignty, full-loot risk |
| OGame | Colonies, resource mines, build queues, research tree, timed fleet missions, raids |

All names, lore, and assets are original.

## Platform
Built in **Unreal Engine 5** (C++ plus Blueprints) on dedicated servers, backed by a persistent backend service. Colony management is an **in-game UI layer** (Holo-Command), not a separate client.

## Docs
1. [Core Loop](docs/01-core-loop.md)
2. [Flight & Combat](docs/02-flight-and-combat.md)
3. [Economy & Industry](docs/03-economy-and-industry.md)
4. [Planets & Colonies](docs/04-planets-and-colonies.md)
5. [Research Tree](docs/05-research-tree.md)
6. [Syndicates & Territory](docs/06-corporations-and-territory.md)
7. [Progression & Risk](docs/07-progression-and-risk.md)
8. [Tech Architecture (UE5)](docs/08-tech-architecture.md)
9. [MVP Roadmap](docs/09-mvp-roadmap.md)
