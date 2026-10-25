# 10 — Crew & Story

## Crew stations (ship)
Each hull has seats: **Helm, Tactical, Engineering, Science, Ops**. Bigger hulls get more seats and higher seat ranks.
- Each officer in a seat contributes **abilities** up to the seat's rank (I–III). Examples:
  - Helm: Evasive Burn
  - Tactical: Focus Fire
  - Engineering: Emergency Power Reroute
  - Science: Shield Harmonics, Sensor Ghost
  - Ops: Boarding Party, Tractor Lock
- Abilities have cooldowns and share an activation queue with your modules.

## Crew members
- Recruited at stations, earned through story, or rescued during missions.
- Each crew member has **traits** (passive), **an ability pool**, and a **morale** value.
- Your long-running characters are **companions**, 6–8 handcrafted crew with personalities. Each has **affinity** that goes up or down with your choices, plus a **personal arc** in 3 chapters. Max affinity unlocks a signature ability.
- **Risk:** if your ship is destroyed, regular crew can be **wounded** (temporary debuff) or **captured** (rescue mission). Companions are never permanently lost.

## Origin arcs
You choose one at character creation:
- **Drifter-Born** (hauler family)
- **Academy Washout** (ex-navy)
- **Syndicate Ward** (raised by a crime cartel)
- **Survey Scion** (exploration guild)

Each is an instanced story of about 6–10 hours. It onboards you into flight, the economy, and colony systems, then hands you off into the sandbox.

## Choices & consequences
- Choices set **flags** (stored in the backend) that affect faction reputation, which crew you can recruit, NPC dialogue, and access to some regions.
- **Echoes:** later content, and events in the shared galaxy, check those flags. Example: an NPC you spared shows up as a market contact.
- No choice locks you out of core sandbox systems.

## Production
Dialogue is built as modular scenes in UE (Dialogue Graph + data tables). Only the main arc beats are voiced, and lip sync is procedural, which keeps costs bounded.
