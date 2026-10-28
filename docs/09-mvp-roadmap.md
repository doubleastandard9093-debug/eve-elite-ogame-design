# 09 — MVP Roadmap (UE5)

## M0 — Foundations (4–6 wks)
UE5 C++ project, CI build, dedicated server target, Large World Coordinates on, EOS login, backend skeleton (Auth + Postgres).

## M1 — Flight Vertical Slice (6–8 wks)
One star system, two flyable hulls, 6DoF flight with assist, Slipstream, one weapon family, NPC pirates, docking, and replicated multiplayer for about 20 players, **4 shield facings, power allocation, and 3 bridge stations with starter abilities**. **This is the first playable target.**

## M1.5 — VR Cockpit (3–4 wks, can overlap M1)
OpenXR on Quest Link, a cockpit camera locked to the HMD with recenter, a virtual stick and throttle on the motion controllers, diegetic shield and power panels, a comfort vignette, and 90 fps in the flight slice.

## M1.7 — Sim Flight & Lanes (4–6 wks)
Four control modes (assisted, decoupled, lane, cruise), the mass/thrust/heat/signature/inertia tick with live cargo and fuel mass, controller and HOTAS bindings, the autopilot order set, one NPC-owned trade lane, and visible module hardpoints on two hulls (doc 02, 13, 14).

## M2 — Economy Core (6–8 wks)
Persistent inventory, the §7A mining deposit types (belts and ice first), a regional market order book, one refining and fabrication chain, bare NPC baseline hulls, ship loss and wrecks, §7B salvage grades with Class 1 Solo hulk interiors (doc 16), and Secured-space encounter anomalies: signal fires and contraband drifts first (doc 20).

## M3 — Landing & Planet Building (8–10 wks)
LZ landing with Administered corridors and one free-descent world (§7C), Holo-Command planet building in Direct and Governor modes (§7D.1a), structure upkeep and decay into derelicts (§7D), a starter research tree, Transport fleet missions, and beginner protection (doc 17, 19).

## M3.5 — Crew & Story Slice (6–8 wks)
One origin arc (first 2 hours), 2 companions with affinity and a first-chapter personal arc, choice flags stored in the backend, one away-mission instance, the boarding prototype, one planetoid interior (worked mine) with personnel loot (§7C.5), and rogues and pirates with economic spawn pressure (§7E).

## M4 — Multi-System & Syndicates (8–10 wks)
3–5 systems on separate servers with jump handoff, Syndicates with a shared hangar, jump gates as the only way small ships cross systems, one player-built lane with toll, one Frontier claim with the §7 Defense Budget, escrowed contracts and the 40% salvage valve, lane interdiction, Open/Corp hulk flags, and one gas-giant fuel station (§7D.1b).

## MVP exit criteria
About 200 concurrent players in a closed alpha. The full loop works: mine → build → fly → lose → rebuild. Timers survive server restarts, with no duplicated items.

## Out of scope for MVP
Capital ships and capital self-jumps, free-descent landing beyond one world, galactic gates, shadow factions and sentinel vaults, pirate kitbash stations, the other three origin arcs, full companion roster, ground PvP, Coalitions, most of the research tree, and full EWAR, standalone Quest build.
