# Setup (Windows, UE 5.8)

## 1. Prerequisites (you already have most)
- **Unreal Engine 5.8** at `C:\Program Files\Epic Games\UE_5.8` (Epic Games Launcher).
- **Visual Studio** (folder `18`). In the Visual Studio Installer, make sure these are ticked:
  - Workload **Game development with C++** (include the *Unreal Engine installer/IDE support* option)
  - Workload **Desktop development with C++**
  - The Windows 10/11 SDK and the MSVC v14.x toolset that the UE 5.8 release notes recommend
- **Git for Windows** plus **Git LFS**: run `git lfs install` once.
- **Docker Desktop**, to run the backend database. Optional: **Go 1.23+** to run backend tests locally.

## 2. Clone
```powershell
cd $HOME\Documents
git clone https://github.com/doubleastandard9093-debug/eve-elite-ogame-design.git
cd eve-elite-ogame-design
git lfs pull
```

## 3. Generate project files & open
- Right-click `HeliosDrift.uproject` and choose **Generate Visual Studio project files**. If that option is missing, run:
  ```powershell
  & "C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat" -projectfiles -project="$PWD\HeliosDrift.uproject" -game -engine
  ```
- Open `HeliosDrift.sln`, set the configuration to **Development Editor | Win64**, set the startup project to **HeliosDrift**, and press **F5**. The editor opens.
- If it asks to rebuild modules on a double-click, say **Yes**.
- First time in the editor:
  1. Create a new empty level and save it as `Content/Maps/Sandbox`.
  2. Under Project Settings → Maps & Modes, set it as the Editor Startup Map and the Game Default Map.
  3. Check that the GameMode is `HDGameMode`.
  4. Commit the `.umap` file. LFS handles it automatically.

## 4. Test multiplayer in the editor
In the Play dropdown, set Number of Players to 2 and Net Mode to **Play As Client**. This runs a dedicated server in the background. You should see two `HDShipPawn` ships.

## 5. Dedicated server build (later in M0)
The `HeliosDriftServer` target needs a **source-built engine**, because the Launcher engine cannot build Server targets. Clone Unreal Engine from GitHub (it needs Epic account linking), then build it and package with `-server`. Until then, test in the editor using Play As Client.

## 6. Backend
```powershell
cd backend
docker compose up --build
# in another terminal:
curl http://localhost:8080/healthz   # {"status":"ok",...}
curl http://localhost:8080/readyz    # {"status":"ready"} once Postgres is up
go test ./...                        # optional, needs Go
```
Migrations run automatically on startup.

## 7. EOS (Epic Online Services), later in M0
1. Create a product in the Epic Developer Portal.
2. Create a client policy and a client.
3. Put the IDs in Project Settings → Online Subsystem EOS (artifact settings). Don't commit secrets: use `Config/Windows/` or a local ini override that git ignores.
