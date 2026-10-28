// Package ships stores a pilot's active ship (position, hull, HP).
package ships

import (
	"context"
	"database/sql"
	"encoding/json"
	"errors"
	"fmt"
	"math"
	"sync"
)

type Ship struct {
	PilotID  string     `json:"pilot_id"`
	Hull     string     `json:"hull"`
	SystemID string     `json:"system_id"`
	Location [3]float64 `json:"location"`
	Rotation [3]float64 `json:"rotation"`
	HullHP   float64    `json:"hull_hp"`
}

// Validate checks a ship record before persisting.
func (s Ship) Validate() error {
	if s.PilotID == "" || len(s.PilotID) > 128 {
		return errors.New("pilot_id must be 1-128 chars")
	}
	if s.Hull == "" || len(s.Hull) > 64 {
		return errors.New("hull must be 1-64 chars")
	}
	if len(s.SystemID) > 64 {
		return errors.New("system_id must be <= 64 chars")
	}
	vals := append(append([]float64{s.HullHP}, s.Location[:]...), s.Rotation[:]...)
	for _, v := range vals {
		if math.IsNaN(v) || math.IsInf(v, 0) {
			return errors.New("numbers must be finite")
		}
	}
	if s.HullHP < 0 {
		return errors.New("hull_hp must be >= 0")
	}
	return nil
}

type Store interface {
	Get(ctx context.Context, pilotID string) (Ship, bool, error)
	Put(ctx context.Context, s Ship) error
}

// MemStore is an in-memory Store for tests and local dev.
type MemStore struct {
	mu sync.RWMutex
	m  map[string]Ship
}

func NewMemStore() *MemStore { return &MemStore{m: map[string]Ship{}} }

func (m *MemStore) Get(_ context.Context, id string) (Ship, bool, error) {
	m.mu.RLock()
	defer m.mu.RUnlock()
	s, ok := m.m[id]
	return s, ok, nil
}

func (m *MemStore) Put(_ context.Context, s Ship) error {
	m.mu.Lock()
	defer m.mu.Unlock()
	m.m[s.PilotID] = s
	return nil
}

// PGStore persists ships in Postgres (pilot_ships).
type PGStore struct{ DB *sql.DB }

func (p PGStore) Get(ctx context.Context, id string) (Ship, bool, error) {
	var raw []byte
	err := p.DB.QueryRowContext(ctx, `SELECT data FROM pilot_ships WHERE pilot_id=$1`, id).Scan(&raw)
	if errors.Is(err, sql.ErrNoRows) {
		return Ship{}, false, nil
	}
	if err != nil {
		return Ship{}, false, err
	}
	var s Ship
	if err := json.Unmarshal(raw, &s); err != nil {
		return Ship{}, false, fmt.Errorf("decode ship: %w", err)
	}
	return s, true, nil
}

func (p PGStore) Put(ctx context.Context, s Ship) error {
	raw, err := json.Marshal(s)
	if err != nil {
		return err
	}
	_, err = p.DB.ExecContext(ctx, `INSERT INTO pilot_ships (pilot_id, data, updated_at) VALUES ($1, $2, now())
ON CONFLICT (pilot_id) DO UPDATE SET data = EXCLUDED.data, updated_at = now()`, s.PilotID, raw)
	return err
}
