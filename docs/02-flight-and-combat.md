# 02 — Flight & Combat
- **Flight model:** six degrees of freedom with flight assist (FA) on or off. Throttle, boost, and lateral thrusters. You divert power between three buses: Shields, Weapons, and Engines.
- **Travel tiers:** normal space (combat), **Slipstream** (fast travel within a system that can be interrupted), and **Lattice Jumps** (gates between systems; capital ships can jump without gates).
- **Hulls:** Skiff, Corvette, Frigate, Cruiser, Hauler, Miner, and Explorer. Capital ships are planned for after the MVP. Each hull has hardpoints, utility slots, and internal slots.
- **Combat:** shots travel as projectiles, and some weapons fire hitscan beams. Ships have subsystems you can target (drives, power plant, cargo). Heat management matters.
- **Interdiction:** a minigame that pulls a target out of Slipstream.
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
