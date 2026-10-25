-- HELIOS DRIFT persistent schema (M0)
CREATE TABLE IF NOT EXISTS accounts (
  id          BIGSERIAL PRIMARY KEY,
  eos_user_id TEXT UNIQUE NOT NULL,
  display_name TEXT NOT NULL,
  created_at  TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE TABLE IF NOT EXISTS star_systems (
  id   TEXT PRIMARY KEY,
  name TEXT NOT NULL,
  security TEXT NOT NULL CHECK (security IN ('core','fringe','void'))
);
CREATE TABLE IF NOT EXISTS colonies (
  id          BIGSERIAL PRIMARY KEY,
  account_id  BIGINT NOT NULL REFERENCES accounts(id),
  system_id   TEXT NOT NULL REFERENCES star_systems(id),
  planet_type TEXT NOT NULL,
  ferrite DOUBLE PRECISION NOT NULL DEFAULT 500,
  crystal DOUBLE PRECISION NOT NULL DEFAULT 300,
  volatiles DOUBLE PRECISION NOT NULL DEFAULT 0,
  rates_updated_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
CREATE TABLE IF NOT EXISTS buildings (
  colony_id BIGINT NOT NULL REFERENCES colonies(id) ON DELETE CASCADE,
  kind  TEXT NOT NULL,
  level INT NOT NULL DEFAULT 0,
  PRIMARY KEY (colony_id, kind)
);
CREATE TABLE IF NOT EXISTS timers (
  id          BIGSERIAL PRIMARY KEY,
  kind        TEXT NOT NULL,  -- build | research | fleet
  owner_id    BIGINT NOT NULL REFERENCES accounts(id),
  payload     JSONB NOT NULL,
  completes_at TIMESTAMPTZ NOT NULL,
  lease_until TIMESTAMPTZ,
  completed_at TIMESTAMPTZ,
  idempotency_key TEXT UNIQUE NOT NULL
);
CREATE INDEX IF NOT EXISTS timers_due ON timers (completes_at) WHERE completed_at IS NULL;
CREATE TABLE IF NOT EXISTS items (
  id         BIGSERIAL PRIMARY KEY,
  owner_id   BIGINT NOT NULL REFERENCES accounts(id),
  type_id    TEXT NOT NULL,
  quantity   BIGINT NOT NULL CHECK (quantity >= 0),
  location   TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS market_orders (
  id        BIGSERIAL PRIMARY KEY,
  region    TEXT NOT NULL,
  owner_id  BIGINT NOT NULL REFERENCES accounts(id),
  side      TEXT NOT NULL CHECK (side IN ('buy','sell')),
  type_id   TEXT NOT NULL,
  price     NUMERIC(20,2) NOT NULL,
  quantity  BIGINT NOT NULL,
  created_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
