// Package api exposes the HTTP surface used by UE dedicated servers and the Holo-Command UI.
package api

import (
	"context"
	"crypto/subtle"
	"encoding/json"
	"net/http"
	"time"

	"github.com/doubleastandard9093-debug/eve-elite-ogame-design/backend/internal/ships"
)

// Pinger is satisfied by *sql.DB.
type Pinger interface {
	PingContext(ctx context.Context) error
}

type Config struct {
	DB        Pinger
	Ships     ships.Store
	Version   string
	ServerKey string // if set, /v1 requires X-Helios-Server-Key
}

func NewRouter(cfg Config) http.Handler {
	mux := http.NewServeMux()
	mux.HandleFunc("GET /healthz", func(w http.ResponseWriter, r *http.Request) {
		writeJSON(w, http.StatusOK, map[string]string{"status": "ok", "version": cfg.Version})
	})
	mux.HandleFunc("GET /readyz", func(w http.ResponseWriter, r *http.Request) {
		ctx, cancel := context.WithTimeout(r.Context(), 2*time.Second)
		defer cancel()
		if cfg.DB == nil || cfg.DB.PingContext(ctx) != nil {
			writeJSON(w, http.StatusServiceUnavailable, map[string]string{"status": "db_unavailable"})
			return
		}
		writeJSON(w, http.StatusOK, map[string]string{"status": "ready"})
	})
	mux.HandleFunc("GET /v1/pilots/{id}/ship", cfg.auth(cfg.getShip))
	mux.HandleFunc("PUT /v1/pilots/{id}/ship", cfg.auth(cfg.putShip))
	return mux
}

func (cfg Config) auth(h http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if cfg.ServerKey != "" && subtle.ConstantTimeCompare([]byte(r.Header.Get("X-Helios-Server-Key")), []byte(cfg.ServerKey)) != 1 {
			writeJSON(w, http.StatusUnauthorized, map[string]string{"error": "unauthorized"})
			return
		}
		h(w, r)
	}
}

func (cfg Config) getShip(w http.ResponseWriter, r *http.Request) {
	s, ok, err := cfg.Ships.Get(r.Context(), r.PathValue("id"))
	if err != nil {
		writeJSON(w, http.StatusInternalServerError, map[string]string{"error": "store"})
		return
	}
	if !ok {
		writeJSON(w, http.StatusNotFound, map[string]string{"error": "not_found"})
		return
	}
	writeJSON(w, http.StatusOK, s)
}

func (cfg Config) putShip(w http.ResponseWriter, r *http.Request) {
	var s ships.Ship
	if err := json.NewDecoder(http.MaxBytesReader(w, r.Body, 1<<16)).Decode(&s); err != nil {
		writeJSON(w, http.StatusBadRequest, map[string]string{"error": "bad_json"})
		return
	}
	s.PilotID = r.PathValue("id")
	if err := s.Validate(); err != nil {
		writeJSON(w, http.StatusBadRequest, map[string]string{"error": err.Error()})
		return
	}
	if err := cfg.Ships.Put(r.Context(), s); err != nil {
		writeJSON(w, http.StatusInternalServerError, map[string]string{"error": "store"})
		return
	}
	writeJSON(w, http.StatusOK, s)
}

func writeJSON(w http.ResponseWriter, code int, v any) {
	w.Header().Set("Content-Type", "application/json")
	w.WriteHeader(code)
	_ = json.NewEncoder(w).Encode(v)
}
