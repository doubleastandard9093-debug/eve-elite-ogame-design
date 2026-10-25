-- M1: one active ship per pilot (expands to hangars in M2)
CREATE TABLE IF NOT EXISTS pilot_ships (
  pilot_id   TEXT PRIMARY KEY,
  data       JSONB NOT NULL,
  updated_at TIMESTAMPTZ NOT NULL DEFAULT now()
);
