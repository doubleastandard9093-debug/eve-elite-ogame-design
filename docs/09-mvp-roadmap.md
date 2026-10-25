# 09 — MVP Roadmap (UE5)

## M0 — Foundations (4–6 wks)
UE5 C++ project, CI build, dedicated server target, Large World Coordinates on, EOS login, backend skeleton (Auth + Postgres).

## M1 — Flight Vertical Slice (6–8 wks)
One star system, two flyable hulls, 6DoF flight with assist, Slipstream, one weapon family, NPC pirates, docking, and replicated multiplayer for about 20 players, **4 shield facings, power allocation, and 3 bridge stations with starter abilities**. **This is the first playable target.**

## M1.5 — VR Cockpit (3–4 wks, can overlap M1)
OpenXR on Quest Link, a cockpit camera locked to the HMD with recenter, a virtual stick and throttle on the motion controllers, diegetic shield and power panels, a comfort vignette, and 90 fps in the flight slice.

## M2 — Economy Core (6–8 wks)
Persistent inventory, mining, a regional market order book, one refining and fabrication chain, and ship loss and wrecks.

## M3 — Holo-Command Colonies (6–8 wks)
In-game colony UI, mines and storage, build queue, Colony & Timer service, a starter research tree, Transport and Raid fleet missions, and beginner protection.

## M3.5 — Crew & Story Slice (6–8 wks)
One origin arc (first 2 hours), 2 companions with affinity and a first-chapter personal arc, choice flags stored in the backend, one away-mission instance, and the boarding prototype.

## M4 — Multi-System & Syndicates (8–10 wks)
3–5 systems on separate servers with jump handoff, Syndicates with a shared hangar, one Void system with a Beacon claim and siege.

## MVP exit criteria
About 200 concurrent players in a closed alpha. The full loop works: mine → build → fly → lose → rebuild. Timers survive server restarts, with no duplicated items.

## Out of scope for MVP
Capital ships, planetary landing, the other three origin arcs, full companion roster, ground PvP, Coalitions, most of the research tree, and full EWAR, standalone Quest build.
