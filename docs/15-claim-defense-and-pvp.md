# 15 — Claim Defense, PVE & PVP Rules (§7)
Source: Aaron's design update, 2026-10-27.

## PVE carries the economy
NPCs never sell finished ships. They sell ore and buy components. Every hull traces back to somebody mining. An NPC faucet for finished goods would make player industry, and the PVE layer, decorative.

## PVP is a property of place
Hostility between companies depends on where you are, not who you are.
| Band | PVP rule | Defense ceiling |
|---|---|---|
| **Secured** (Sec Space) | No open PVP; duel flag only. NPC enforcement; lanes free. Class 3–4 salvage does not spawn. Encounter anomalies are PVE (doc 20). | Not on the ladder |
| **Restricted** (Rest Space) | Only under a declared war, with 48 h notice. Class 3–4 salvage starts here. | 70,000 cr/day |
| **Borderland** (Bor Space) | Open only inside a claimed volume. The claim announces itself before you enter, including whether its garrison is funded. | 100,000 cr/day |
| **Frontier** (The Black) | Open. Full defense economy. | 150,000 cr/day |

Underfunded claims show an **"undefended"** flag, so defending is a logistics problem, not a twitch problem.

**Lane interdiction replaces the gank.** Interdictors pull ships out of a lane, but only in claimed volumes or Frontier. Because small ships must use gates between systems, gates and the lanes leading to them are natural chokepoints. Beyond the gate network, wormhole mouths are the small-ship chokepoints instead. They cost a module charge and show on lane telemetry, and the lane owner can buy **suppression** as counterplay.

## §7 Defense is a spending rate, not a garrison
A claim's **Defense Budget (DB)** is computed from two slow inputs:
- **30-day industrial output**
- **14-day registered residency**

Trade volume also feeds DB (it is one of the three inputs in the cap reference point below). DB becomes a **Defense Rating per unit of claim volume**. The slowness is deliberate: you can't build a defense out of a one-week raid budget. Defense is what you already built.

### Defense ladder
Autonomy and cost run upward; capability runs the other way.
- **Tier 1 – Gun placements** and **Tier 2 – defense platforms**: fully autonomous, cheap per unit, hungry for ammo.
- **Tier 3 – Capital**: players only.
- **Tier 4 – NPC-flown ships**: patrol fighters (cheap hulls, constant attrition cost) and NPC squadron wings (standing-gated; a hostile faction may refuse the contract). Both are NPC-crewed ships rather than fixed hardware or players, so they share one tier.
- **Tier 5 – Hired players.** The 50% cap applies here, and only here.

- **NPC hardware has a stat ceiling and is meant to die.** It caps below player-grade fitting and wins on geometry and numbers. An NPC-only defense is a speed bump.
- **NPCs cannot hold a lane exit against players alone.**
- **Escrow:** payment comes out of DB before the fight. If DB can't cover the contract, there is no contract.
- **Bonuses:** the largest bonus goes to defenders for a surviving lane or structure, not for kills.
- **Tolls don't count.** Lane tolls and encounter-anomaly tolls are income to the owner but **do not count toward the claim's Defense Budget**. DB comes only from industrial output, residency and trade volume.
- **Vetting:** standing minimums only. A refusal list would let a bloc gate who is allowed to earn, so that option is closed.

### Anti-bloat rules
- Residency capped per structure berth.
- Upkeep paid off the top before hire.
- Diminishing returns per berth past the first few guns.
- Visible **"thin garrison"** flag on approach for claims with high hardware and few residents.

### The cap (curve constants are FIRST PASS)
The curve constants are a first pass, fitted to the reference economy (DB reference point below), and need an economic model behind them before they are load-bearing. The shape is the design.
1. **Sublinear growth.** DB scales at roughly the **0.45–0.6 power** of its inputs.
   - Reference: 120M output, 400M trade volume, 60 residents → **≈50,000 cr/day**.
   - Double all three → **≈73,000 cr/day** (~1.45x, not 2x).
2. **Ceiling by security band:** **Frontier 150,000**, **Borderland 100,000**, **Restricted 70,000** cr/day. **Secured is not on the ladder:** NPC enforcement already holds the volume, so a claim there does not buy a defense economy. A full defense economy is a privilege of Frontier; Borderland is enough to hire a real wing, not enough to match a Frontier grid.
3. **Per-tier share limits.** The 50% cap applies at **tier 5 (hired players) and only at tier 5**: a claim may spend at most half its DB there. At the Frontier ceiling that is **75,000 cr/day**; at the Borderland ceiling, **50,000 cr/day**. Both hire a wing; neither replaces the garrison.
4. **Anti-spike window:** DB = **min(24 h rolling average, 7-day rolling average)**. A war chest dumped in leaves the budget unchanged for a week.

**Pressure valve (main balance lever):** destroyed defense hardware drops salvage worth **~40% of build cost**. A raid that chews through a thin garrison partly funds itself; one that bounces off a full grid is a straight loss. Cap plus salvage keeps a strong claim worth attacking and a weak one worth defending.

### Example: corridor waypoint
High trade volume buys a decent budget, but no residents, industry or berths means it can only afford platforms and interdiction suppression. Its defense is the lane.
