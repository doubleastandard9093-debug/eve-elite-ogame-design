# 15 — Claim Defense, PVE & PVP Rules (§7)
Source: Aaron's design update, 2026-10-27.

## PVE carries the economy
NPCs never sell finished ships. They sell ore and buy components. Every hull traces back to somebody mining. An NPC faucet for finished goods would make player industry, and the PVE layer, decorative.

## PVP is a property of place
Hostility between companies depends on where you are, not who you are.
| Band | PVP rule |
|---|---|
| **Secured** (Sec Space) | None, except under opt-in flags: a duel flag, or the salvage PVP flag at Class 3–4 sites (doc 16). |
| **Restricted** (Rest Space) | Only under a declared war, with 48 h notice. |
| **Borderland** (Bor Space) | Open only inside a claimed volume. The claim announces itself before you enter, including whether its garrison is funded. |
| **Frontier** (The Black) | Open; interdiction allowed. |

Underfunded claims show an **"undefended"** flag, so defending is a logistics problem, not a twitch problem.

**Lane interdiction replaces the gank.** Interdictors pull ships out of a lane, but only in claimed volumes or Frontier. They cost a module charge and show on lane telemetry, and the lane owner can buy **suppression** as counterplay.

## §7 Defense is a spending rate, not a garrison
A claim's **Defense Budget (DB)** is computed from two slow inputs:
- **30-day industrial output**
- **14-day registered residency**

Trade volume also feeds DB (it is one of the three inputs in the cap reference point below). DB becomes a **Defense Rating per unit of claim volume**. The slowness is deliberate: you can't build a defense out of a one-week raid budget. Defense is what you already built.

### Defense ladder
Autonomy and cost rise upward; capability rises faster.
1. **Gun placements and defense platforms**: fully autonomous, cheap per unit, hungry for ammo.
2. **Patrol fighters**: cheap hulls, constant attrition cost.
3. **NPC squadron wings**: standing-gated; a hostile faction may refuse the contract.
4. **Hired players.**
5. **Capital**: players only.

- **NPC hardware has a stat ceiling and is meant to die.** It caps below player-grade fitting and wins on geometry and numbers. An NPC-only defense is a speed bump.
- **NPCs cannot hold a lane exit against players alone.**
- **Escrow:** payment comes out of DB before the fight. If DB can't cover the contract, there is no contract.
- **Bonuses:** the largest bonus goes to defenders for a surviving lane or structure, not for kills.
- **Vetting:** standing minimums only. No refusal list.

### Anti-bloat rules
- Residency capped per structure berth.
- Upkeep paid off the top before hire.
- Diminishing returns per berth past the first few guns.
- Visible **"thin garrison"** flag on approach for claims with high hardware and few residents.

### The cap (curve constants are FIRST PASS)
The constants are a first pass and need an economic model behind them before they are load-bearing. The shape is the design.
1. **Sublinear growth.** DB scales at roughly the **0.45–0.6 power** of its inputs.
   - Reference: 120M output, 400M trade volume, 60 residents → **≈50,000 cr/day**.
   - Double all three → **≈73,000 cr/day** (~1.45x, not 2x).
2. **Ceiling by security band:** **Frontier 150,000**, **Borderland 100,000**, **Restricted 70,000** cr/day. Restricted has NPC enforcement, so its garrison supplements rather than replaces.
3. **Per-tier share limits.** Tier 5 is capped at **50%** of DB. At the Frontier ceiling that is 75,000 cr/day of mercenary work.
4. **Anti-spike window:** DB = **min(24 h rolling average, 7-day rolling average)**. A war chest dumped in leaves the budget unchanged for a week.

**Pressure valve (main balance lever):** destroyed defense hardware drops salvage worth **~40% of build cost**. A raid that chews through a thin garrison partly funds itself; one that bounces off a full grid is a straight loss. Cap plus salvage keeps a strong claim worth attacking and a weak one worth defending.

### Example: corridor waypoint
High trade volume buys a decent budget, but no residents, industry or berths means it can only afford platforms and interdiction suppression. Its defense is the lane.
