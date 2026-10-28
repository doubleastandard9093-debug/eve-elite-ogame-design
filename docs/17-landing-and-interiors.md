# 17 — Landing (§7C) & Planetoid Interiors (§7C.5)
Source: Aaron's design update, 2026-10-27.

## §7C Landing tiers
### Seamless descent (2026-10-28 decision)
Landing is **seamless, onto full-size bodies, with no loading screen**. A pilot comes in on **orbital cruise**, drops into a **glide** through the upper approach, then takes over for **surface flight** down to the ground. The same ship, the same session, one continuous flight from orbit to touchdown and back.

**What "scale rules" meant.** The earlier brief said full 1:1 atmospheric planets would "break the scale rules". That referred to engine limits, not a game rule: rendering, streaming and simulating full-size planets costs a lot of performance, and positions far from the origin need UE5 Large World Coordinates (LWC) to stay precise. Seamless descent needs streamed terrain and LWC-safe coordinates on every landable body. That is a tech cost to budget for (doc 08), not a reason to avoid it.

### Descent numbers (first pass)
- **Orbital cruise:** around a landable body, down to a floor of **25 km** altitude. Holding a near-level attitude (about ±5°) gives the best orbital speed.
- **Glide:** starts at 25 km if you arrive slow enough and pitched between **−5° and −60°** (about −45° is ideal). It is unpowered, holds **2.5 km/s**, and ends at **3 km**. Too fast or too steep and you emergency-drop with damage; too shallow and you bounce back to orbital cruise.
- **Surface flight:** below 3 km, normal thrusters take over. Heavy ships with weak thrusters struggle at high gravity.
- **Gravity:** landable generated bodies go up to **2.0 g**. Authored worlds may go higher, up to **5 g**. On-foot disembarking is disabled above **2.7 g**.
- **Body size:** landable bodies run from small moons and large asteroids up to about **29,000 km** radius.

### How the tiers fit together
- **Seamless descent is the default** for landable airless bodies (moons, rocks, large asteroids) and authored atmospheric worlds, with drag, entry heat, gravity and terrain collision where there is an atmosphere.
- **Developed worlds** are still governed by landing zones and corridors (§7C.2a below): the descent is seamless, but where you may come down is controlled.
- **Gas giants stay orbital-only.**

| Body | Landing |
|---|---|
| **Gas giants** | Orbital only (fuel scooping, doc 19 §7D.1b). |
| **Developed / settled worlds** | Seamless descent, but into a landing zone (LZ) through a corridor. |
| **Authored atmospheric worlds** | Seamless free descent with drag, entry heat, gravity and terrain collision. |
| **Airless bodies and large asteroids** | Seamless free descent, minus atmosphere. |

**Draft: LZs on about 20% of inhabited worlds. STILL OPEN.** If LZs are everywhere, planets stop mattering; if they're rare, they become campable chokepoints.

## §7C.2a Landing corridors: development decides the entry
| State | Entry |
|---|---|
| **Undeveloped** | Land anywhere, open sky. |
| **Administered** (LZ or settlement) | Rotating corridors on a published cycle, inbound and outbound separated. You have to know the window. |
| **Hardened** (player-built planetoid defense, or a Secured core world) | Single entry point, gated and monitored, with coverage on every other vector. |

Developing a planetoid brings industry, residency and a bigger Defense Budget, and costs you the open sky. Keep a world undeveloped and not worth taking, or build it into something valuable that can only be approached through a kill zone.

**Corridors are structures.** Destroy one and a Hardened world drops to Administered until it's rebuilt: a raid with a strategic point, not a body count.

## §7C.5 Planetoid interiors
Entered **on foot from a landed ship**, never warped into from orbit. Three kinds, rising in danger:
1. **Worked mines**: shallow, industrial loot.
2. **Warrens and hideouts**: contraband, crew, bounty leaders.
3. **Vault / deep sites**: artifact caches, ancient sentinel guardians.

### §7C.5.3 Loot: four categories
The update names personnel as the key row; the other three categories are implied by the site list (industrial goods, contraband, artifacts).
- **Personnel:**
  - A captured pirate leader is intel and a bounty.
  - A rescued mining engineer gives **+N% output** on one surface structure.
  - A scientist gives blueprint research progress.
  - Engineers can be installed as planet governors (doc 19).

An interior run is how a small company buys capability it can't otherwise afford, and how non-industrial players matter to a claim.
