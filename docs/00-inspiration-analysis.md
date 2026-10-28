# 00 — Inspiration Analysis
This doc names the source games only so we can analyze them. Nullpoint's game content uses none of their names, factions, or assets.

| Game | Does well | Borrow | Avoid |
|---|---|---|---|
| **Elite Dangerous** | Cockpit feel, sense of scale, flight physics | 6DoF flight, power buses (Shields/Weapons/Engines), scanning, fast travel inside a system | Empty grind, long dead-air travel, shallow multiplayer |
| **EVE Online** | Single shard, player economy, meaningful loss, sovereignty | Industry chains, regional markets, groups claiming territory, siege timers | Spreadsheet barrier to entry, nights of tedious structure grinding, brutal onboarding |
| **OGame** | Async empire play, "check back later" timers | Mines, build queues, research tree, timed fleet missions, debris fields | Getting farmed offline, pay-to-skip timers, wiped empires |
| **Firefly** (TV series) | Frontier space-western tone, a small crew on a working ship, scraping by between jobs | Lived-in ships, smuggling and salvage work, frontier vs core-world contrast, the ship as home | Any names, characters, ship designs or dialogue from the show |
| **No Man's Sky** | Landable planets, sense of discovery, base building | Planet landing, surface building, resource-gated world types, onboard social space | Procedural sameness, shallow economy |
| **Freelancer** | Trade lanes, accessible mouse flight, readable sector map | **Trade lanes** as in-system highways, mouse-and-keyboard flight that still feels like flying | Linear story gating, small universe |
| **Star Trek Online** | Ships as characters, crew roles, tactical facing combat | **Bridge officers** in seats that slot abilities; **ship consoles** that boost stats; **power levels** you shift live; **four shield facings** you can rebalance; **ground away missions** with a squad | Too much grind for gear, too many currencies, lockbox monetization, power creep |
| **Star Wars: The Old Republic** | Voiced class stories, companions, choices | **Origin story arcs** per background; **companions** with affinity, personal quests, and combat roles; **choices with consequences** that are remembered; **instanced story zones** layered over shared hubs | Hard to replay as a solo-themepark game, story locked to class, expensive linear content pipeline |

Star Trek Online and Star Wars: The Old Republic remain earlier inspirations for crew and story.

## Reference values: landing and jump drives
Aaron asked Nullpoint's landing ratios and jump distances to be modeled on these. Nullpoint's own values are in docs 13 and 17.

| Game | Mechanic | Reference value | Source |
|---|---|---|---|
| Elite Dangerous | Orbital cruise floor | 25 km | [Orbital Cruise](https://elite-dangerous.fandom.com/wiki/Orbital_Cruise) |
| Elite Dangerous | Glide | 2,500 m/s, from 25 km down to 3 km; entry pitch −5° to −60° | [Glide](https://elite-dangerous.fandom.com/wiki/Glide) |
| Elite Dangerous | Landable gravity | No hard limit; generated planets reported capped near 1.99 g; walking disabled above 2.69 g | [Gravity](https://elite-dangerous.fandom.com/wiki/Gravity), [forum](https://forums.frontier.co.uk/threads/super-earths-in-ed.481416/) |
| Elite Dangerous | Largest landable body | 28,878 km radius (4.64 g) | [Planetary Landing](https://elite-dangerous.fandom.com/wiki/Planetary_Landing) |
| Elite Dangerous | Jump range | Sidewinder 7.6 → 24.4 ly; Diamondback Explorer 14.2 → 41.6 ly; Anaconda 9.4 → 41.5 ly (default → optimized), ~80 ly engineered | [Jump range](https://elite-dangerous.fandom.com/wiki/Jump_range) |
| Elite Dangerous | Fuel per jump | f = r × (d × m_ship / m_opt)^p × 10⁻³; r 8–12 by rating; p 2.00 (class 2) to 2.90 (class 8) | [Frame Shift Drive](https://elite-dangerous.fandom.com/wiki/Frame_Shift_Drive) |
| EVE Online | Capital jump range | Dreadnought/carrier 3.5 → 7 ly; supercarrier/titan 3 → 6 ly; Black Ops 4 → 8 ly; jump freighter/Rorqual 5 → 10 ly (base → max skills) | [EVE Uni: Jump drives](https://wiki.eveuniversity.org/Jump_drives) |
| EVE Online | Fuel per ly | Capitals 3,000 isotopes; Black Ops 700; jump freighters 8,200–10,000; −10% per level of Jump Fuel Conservation | same |
| EVE Online | Jump fatigue | min 10 × (1 + ly) min, else × (1 + ly); cap 5 h. Activation cooldown 1/10 of fatigue, min (1 + ly) min, cap 30 min. Industrials get −90% effective distance | same |

## Synthesis (2026-10 update)
- **Space western first:** Firefly's tone over a PVE-focused sandbox. Work, salvage and survival on the frontier, not empire-scale war.
- **Lanes shape the map:** Freelancer-style trade lanes become owned corridors (doc 13).
- **Planets are places:** No Man's Sky-style landing and building, constrained by tiered landing (doc 17) and governors (doc 19).
- **Elite + EVE stay the spine:** sim flight and scale from Elite; player-built economy, claims and scanning from EVE.
- Aaron's interior minigame references (Asteroids, Peggle, and EVE's citizen-science research minigame) are inspirations only; in-game minigames are original.

## Earlier synthesis
- **Ship = character sheet:** Elite's stick feel, STO-style crew stations, and EVE's fitting.
- **Crew = people, not stats:** STO's officer slots combined with SWTOR's companion affinity and personal arcs.
- **Story in a sandbox:** short origin and crew arcs, written to be cheap to produce in a modular way, sit beside a player-driven world instead of replacing it. Choices change your reputation, unlock crew, and affect your standing with factions.
- **Ground layer:** away missions and ship boarding give a reason to leave the cockpit, but stay instanced and small in scope.
