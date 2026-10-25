# 08 — Tech Architecture (Unreal Engine 5)

## Engine & code
- **UE5** (5.4+). Core systems are written in **C++**: flight physics, replication, inventory, combat, and backend clients. **Blueprints** handle content, UI wiring, VFX/SFX hooks, and designer tuning.
- **Gameplay Ability System (GAS):** weapons, modules, power distribution, and effects.
- **Enhanced Input:** HOTAS, gamepad, and keyboard/mouse profiles.
- **Common UI + UMG** for every UI, including the **Holo-Command** colony layer: an in-game, data-driven UI that talks to the backend over REST/WebSocket through a C++ subsystem.

## Space scale
- Turn on **Large World Coordinates** (double precision) so star systems fit in one level.
- **Floating origin rebasing** per client keeps rendering precise. The server simulates in double precision.
- **World Partition** streams station interiors, asteroid fields, and planets as data layers. Planets are procedural or tiled meshes with impostors at a distance.
- Lattice jumps between systems trigger a **server travel/handoff** with a loading tunnel.

## Networking
- **Dedicated servers** (Linux), with one or more per star system. A **Zone Director** service gives systems their own servers and spins up servers for busy systems.
- **Replication:** **Iris** replication with relevancy based on distance and sensor range, plus a Replication Graph-style spatial grid. Ships are server-authoritative, with client-side movement prediction and reconciliation (a custom movement component, not CharacterMovement).
- **Big battles:** network update frequency drops based on distance, AI escorts are simplified, and a time dilation fallback can kick in.
- **OnlineSubsystem:** EOS (or Steam) for identity, friends, presence, and voice. Platform login is exchanged for a backend session token.

## Persistent backend (authoritative for the long-running state)
UE servers own what happens in real time in space. The backend owns everything that persists.
- **Services** (Go or C#/.NET, containerized on Kubernetes):
  - Account/Auth
  - Inventory & Assets
  - Market (order matching)
  - Industry/Fabrication
  - Colony & Timer service (build queues, research, fleet missions)
  - Syndicate/Territory
  - Chat bridge
- **Timers:** a durable job scheduler (a DB-backed queue with leases) settles completion times exactly once. Colonies are lazy-evaluated: resources = rate × elapsed time, computed on read.
- **Database:** **PostgreSQL**, the source of truth with transactional asset moves. **Redis** handles sessions, caches, and pub/sub. Events (kills, trades, completed timers) are streamed through NATS/Kafka to analytics and to UE servers so fleet interceptions can be spawned.
- **Integrity:** every item transfer runs as a DB transaction with an idempotency key. UE servers call the backend with service credentials, and clients never write directly.

## Pipeline & ops
- Unreal Horde or GitHub Actions + BuildGraph for CI. Builds are cooked per platform.
- Dedicated servers are hosted with Agones on Kubernetes, or GameLift.
- Telemetry: OpenTelemetry for services, Unreal Insights for profiling.
