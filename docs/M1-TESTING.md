# M1 Flight Slice: Testing Guide

## 0. Build
Close the editor, then from `C:\Dev\Helios-Drift` run:
```powershell
& "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" HeliosDriftEditor Win64 Development -Project="C:\Dev\Helios-Drift\HeliosDrift.uproject" -WaitMutex
```
Then open `HeliosDrift.uproject`.

## 1. Backend (optional; the game runs without it and only logs a warning)
```powershell
cd C:\Dev\Helios-Drift\backend
docker compose up --build
```

## 2. Test map (one-time, in the editor)
1. **File → New Level → Empty Level**, then save it as `Content/Maps/M1_Sandbox`.
2. **Window → World Settings → GameMode Override**: `HDGameMode`. Leave the defaults: Pawn `HDShipPawn`, Controller `HDPlayerController`, HUD `HDHUD`.
3. Add light and space:
   - Place an **Directional Light**, a **Sky Atmosphere** (or skip it for black space), and an **Exponential Height Fog** (optional).
   - Optionally add a large **Sphere** mesh far away (scale 500, about 50 km out) as a planet reference.
4. Place **2 Player Start** actors about 20 m apart (X=0 and X=2000), facing +X.
5. Place target drones. In Place Actors, search **HD Target Drone** and drag in:
   - 3 drones about 50–150 m in front of the starts (X = 5000–15000). They orbit their placed spot with a 30 m radius.
   - 1 drone very far away, for example X = 2,000,000 (20 km), to test slipstream.

   Every drone is tagged `SlipTarget`, so slipstream drops you out near the one you're pointing at.
6. **Project Settings → Maps & Modes**: set Editor Startup Map and Game Default Map to `M1_Sandbox`.
7. Save all. The `.umap` is LFS-tracked, so commit and push it.

## 3. Play with 2 clients
Open the Play dropdown, then **Advanced Settings**:
- **Number of Players: 2**
- **Net Mode: Play As Client** (runs a dedicated server in the background)
- Optional: **Multiplayer Options → Run Under One Process** on (faster), with **New Editor Window (PIE)**.

Then press Play and click into a window to capture the mouse. Press Shift+F1 to release it.

For lag testing, open the console with `` ` `` in a client and run `NetEmulation.PktLag 150` and `NetEmulation.PktLoss 2`.

## Controls
| Action | Keyboard/Mouse | Xbox controller |
|---|---|---|
| Thrust forward/back | W / S | Left stick up/down, or RT (forward) / LT (reverse) |
| Strafe left/right | A / D | Left stick left/right |
| Up / down | R / F | Hold L3 (click left stick) = up, hold R3 (click right stick) = down |
| Pitch / yaw | Mouse, or arrow keys | Right stick |
| Roll | Q / E | LB / RB |
| Fire laser | LMB or Space | A |
| Toggle Flight Assist | Z | B |
| Equalize shields | X | X |
| Slipstream (spool, engage, drop) | J | Y |
| Power: Balanced / Attack / Tank / Run | 1 / 2 / 3 / 4 | D-pad up / right / down / left |
| Recenter VR | Home | View |
| Controls overlay (menu placeholder) | Tab | Menu |

**Why this layout:** the triggers and the left stick both drive thrust. When both are used, the one pushed harder wins, so they never fight. Fire is on A so your thumb never has to leave thrust. Vertical thrust is held on the stick clicks, which keeps the whole D-pad free for the four power presets.

**How the inputs feel:**
- **Right stick and arrow keys are rate inputs.** Holding the stick halfway turns at half the ship's max turn rate.
- **The mouse is a delta input.** Each mouse count turns the ship by a fixed number of degrees (`MouseDegreesPerCount`, default 0.15), so frame rate doesn't change how far the ship turns. Mouse and stick add together and are capped at the ship's max turn rate. Neither is scaled twice.
- **Deadzone and curve:** both sticks use a 15% deadzone (radial on the right stick) and a squared response curve for fine aim near the center. The triggers use half that deadzone.

**Settings** (`Config/DefaultGame.ini`, section `[/Script/HeliosDrift.HDPlayerController]`):
- `StickSensitivity` (0.1–3, default 1)
- `StickCurveExponent` (1 = linear, default 2)
- `StickDeadZone` (default 0.15)
- `bInvertStickPitch`
- `MouseDegreesPerCount`
- `bInvertMousePitch`

You can also change sensitivity live from the console (`` ` ``) with `HDStickSensitivity 1.5` and `HDMouseSensitivity 0.1`. Deadzone and curve changes take effect the next time you start PIE.

**HOTAS:** sticks that show up as XInput devices, or that are mapped through Steam Input or the vendor's software, work through the gamepad bindings. Native DirectInput axes need the **RawInput** plugin plus a device config, which is planned for later in M1.

## Test checklist
- [ ] Project compiles; editor opens with no errors in the Output Log (filter `LogHelios`).
- [ ] Spawn in the cockpit. The HUD shows speed, FA, travel, power, shields, and hull.
- [ ] **FA ON:** W accelerates to the cap. Releasing W brakes to 0. Releasing the mouse stops rotation.
- [ ] **FA OFF (Z):** releasing W keeps the ship drifting. Rotation keeps spinning until countered.
- [ ] **Power:** 4 (Run) ramps Engines to 100 in about 1 s, and the speed cap rises to about 450 m/s. 2 (Attack) lowers the cap.
- [ ] **Slipstream:** aim at the far drone and press J. SPOOLING runs for 4 s, then SLIPSTREAM at about 20 km/s, and you drop out near the drone automatically. Pressing J again drops you out manually.
- [ ] **Interdiction:** shooting a spooling or slipstreaming ship drops it out, and it can't re-engage for 10 s.
- [ ] **Laser:** red/orange tracers appear. Hitting a drone drains the facing you hit first, then its hull. The drone disappears, then respawns after 5 s.
- [ ] **Shield facings:** client B shoots client A from behind. A's **A**(ft) value drops while F stays full. X (equalize) spreads the remaining shields evenly. Shields regenerate over time.
- [ ] **Ship death:** at 0 hull the ship respawns at a Player Start.
- [ ] **Multiplayer:** each client sees the other ship move smoothly. With `PktLag 150`, your own ship still responds instantly with only small corrections.
- [ ] **Backend:** with docker running, the log shows `No saved ship for ... (HTTP 404)` on first join. When a client quits, it shows `SaveShip ... -> HTTP 200`. On the next join it shows `Loaded ship`.
- [ ] VR (optional): VR Preview works, the cockpit camera follows the HMD, and Home (or View) recenters.
- [ ] **Xbox controller:** every action in the table works. Releasing the sticks stops input completely (no drift). The triggers give smooth partial thrust. `HDStickSensitivity 2` makes the right stick turn noticeably faster.
- [ ] **Mouse at different frame rates:** turning the same mouse distance at `t.MaxFPS 30` and at `t.MaxFPS 144` turns the ship by about the same angle.
- [ ] **No firing in slipstream:** holding fire while spooling or in slipstream shoots nothing.
