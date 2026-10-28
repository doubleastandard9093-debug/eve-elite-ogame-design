# 19 — Building (§7D), Stations (§7D.0), Planets (§7D.1a) & Gas Giants (§7D.1b)
Source: Aaron's design update, 2026-10-27.

## §7D Building and decay loop
- Structures anchor to **belts, surfaces or dead space**.
- Upkeep is **fuel and ammo**.
- Unmaintained structures **decay into derelicts**, which feed salvage (§7B), which feeds industry. Abandoned industry → salvage → industry.
- **Population coupling:** more berths and jobs → more population → bigger Defense Budget → more defense. Building is defending.
- **Surface structures can't be reached from orbit**, so an attacker has to land. Planetary industry is safer than orbital industry but harder to supply. The counter is an **orbital blockade**, currently drafted as a soft counter.

## §7D.0 Stations
Stations differ by faction and type and are slightly modifiable. Four families:
| Family | Customization |
|---|---|
| **Accord civic** | Color and beacons only. |
| **Faction industrial** | Silhouettes matching the faction's hulls, with prefab add-ons. |
| **Corporate modular** | Prefab sections placed within limits. |
| **Pirate kitbash** | Clip modules at any attachment point for a unique build. |

- Everything routes through four systems, **power, hardpoints, support points and infrastructure**, plus **integrity**.
- Modules can be destroyed without killing the station, so station fights degrade outside-in, not one health bar.
- **Kitbash has no structural floor:**
  - Each non-standard attachment adds a stress penalty; integrity drains faster the further you deviate from a standard plan.
  - Improvised joins lose **~15% routing efficiency**.
  - A **stress event** can shear a module off: it drops as salvage and punches a hole in support coverage.
  - Trade: more modules, more hardpoints, a unique design, against integrity, efficiency and the risk of losing a section.

## §7D.1a Planet building
City-building on planetoids, in two modes:
- **Direct:** you place every building.
- **Governor:** appoint an NPC, set the world's focus and budget, and review an editable **weekly build queue**. Delegated, not automated.
  - Focuses: **Extraction, Refining, Industry, Trade, Residency, Defense, Research, Balanced**.
  - Governor quality matters: a cheap one misses the geology and overbuilds one sector; a great one develops a world better than most players.
  - Governors are hireable specialists. A rescued or captured engineer from an interior run (§7C.5.3) can be installed as governor.
- **Building types are gated by world type plus resources actually present.**
  - A metal-rich airless rock can be a superb industrial world but can never feed anyone.
  - A gas giant can't be settled, but its moons can.
  - A water world is a residency and agriculture paradise with almost no minerals.

## §7D.1b Gas giants
The one body type outside the planetoid model.

### Scooping is a flying task
- Enter the column on a descent profile and fill tanks from the gas.
- Entry angle, altitude band, thermal load and time-on-station all matter. Greed on tank volume risks heat damage.
- Thin upper atmosphere is slow and cool; deep is fast and hot. A bigger scoop module fills faster and runs hotter.
- **Cloud density tiers.** Each gas giant has a cloud density tier: **Rare**, **Consistent** or **Abundant**. The tier sets scoop yield and fuel-station yield.
- **Weekly re-roll.** Each planet re-rolls its tier at random once a week (weekly respawn). Exact odds per tier are not set yet.
- Density and weather combine: an Abundant calm giant is a strategic asset, an Abundant violent one is a high-skill site, and a Rare calm one is where you learn.

### Fuel mining station
- Anchored in the **upper atmosphere** (there is no surface).
- Produces continuously at a rate set by the giant's current cloud density tier and scoop quality.
- **A station's value shifts week to week** as its giant re-rolls. An Abundant week makes it a prize, and a Rare week makes it a cost center.
- Consumes power, spare intakes (they wear out in storms), and ballast and tether upkeep.

### Gas-giant defense: anchored, five layers
Orbital batteries can't cover a platform inside an atmosphere, and the platform can't shoot out through its own storm layer.
1. Buoyancy and tether array
2. Weather screening
3. Cloud picket dispersed through the column
4. Interceptor cage that can climb out of the atmosphere
5. Ascent denial at the atmospheric boundary

An attacker descending from orbit fights the boundary layer, then weather, then the picket, while the interceptor cage climbs past to hit their exit vectors. A gas-giant assault is the inverse of a station assault.

### Safe refueling
- **Jump fuel is the projection limiter:** how far a ship can operate is bounded by the jump fuel it carries, and gas-giant fuel stations are where that fuel is refilled. How much fuel a station supplies follows its giant's cloud density tier, so its reach changes with the weekly re-roll. (TODO: jump range and fuel-per-jump numbers.) So a defended station extends every friendly ship's operational radius by its full jump range.
- Access: **open** (toll), **standing-gated**, or **company-only**. An open station in dangerous space is a public good and an obvious target, the most contested structure type.
- **Refueling is a real stop:** docking means a stationary window. Under threat you choose full tank, half, or leave dry and jump short.
- **Counter to lane interdiction:** a ship pulled from a corridor can run for a defended giant and use it as a hard point while its drive charges.
