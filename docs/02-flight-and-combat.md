# 02 — Flight & Combat
- **Flight model:** simulation, not arcade (see below). Six degrees of freedom with flight assist (FA) on or off. Throttle, boost, and lateral thrusters. You divert power between three buses: Shields, Weapons, and Engines.
- **Travel tiers:** see the full travel-speed ladder in doc 13 (Sub Space, Slipstream, instance jump lanes, trade lanes, gates/wormholes/anomalies, galactic gates).
- **Hulls:** fighters up to capitals, plus a mining line. Supply tiers and visible modules are in doc 14. Capital ships are planned for after the MVP. Each hull has hardpoints, utility slots, and internal slots.
- **Combat:** shots travel as projectiles, and some weapons fire hitscan beams. Ships have subsystems you can target (drives, power plant, cargo). Heat management matters.
- **Between systems:** fighters through battleships must use jump gates, jump lanes, wormholes or anomalies; only capital-size ships jump on their own drives (doc 13).
- **Interdiction:** pulls ships out of a lane, only in claimed volumes or Frontier (doc 15).
- **Fleets:** you can captain a ship while AI escorts and wingmates follow your commands. Large battles use the server load tactics described in doc 08.

## Power allocation
- Four subsystems: **Weapons, Shields, Engines, Auxiliary**. Each takes 0–100 from a shared reactor budget.
- Presets (Attack, Tank, Run, Balanced) and live fine-tuning. Changes ramp in over about 1 second so you can't swap instantly.
- Auxiliary powers crew abilities and science effects.

## Shield facings
- Four facings: **Fore, Aft, Port, Starboard**. Damage hits the facing toward the attacker.
- **Rebalance:** move capacity between facings, either manually or by holding a key to auto-equalize. Hull damage starts once a facing is down, so positioning and turning your ship matter.

## Consoles
- Console slots by type (Tactical, Engineering, Science, Universal) hold passive or clickable stat modules. They are separate from weapon hardpoints.

## Crew abilities
- Station officers grant abilities (doc 10) on a hotbar. They draw Auxiliary power and share cooldown groups with modules. Wingmate and AI escort ships use their crew too.
- **Boarding:** disable a target's drives, then use the Ops ability Boarding Party to start an instanced ground fight (doc 11).

## VR cockpit
Seated VR is a first-class way to fly (doc 12). Motion controllers grab a virtual stick and throttle, or you can use HOTAS or a gamepad. Shield facings, power levels and crew abilities show on diegetic cockpit panels, and head-look aiming is available for turrets.

## Flight modes and the tick model (2026-10 update)
Flight is simulation, not arcade, and works on mouse and keyboard, joystick and throttle, or controller. No click-to-move: you fly the ship. Cockpits are customizable.

Four control modes:
1. **Assisted:** flight assist on.
2. **Decoupled:** flight assist off; full Newtonian drift.
3. **Lane:** locked to a trade lane corridor.
4. **Cruise:** high-speed in-system travel (Slipstream).

The tick model is built on **mass, thrust, heat, signature and rotational inertia**.
- **Mass changes in real time with cargo and fuel.** A hauler that dumps its load mid-fight becomes agile, so trading, fighting and escaping are one conversation.
- Module loadout affects ship **stability** (doc 14).
