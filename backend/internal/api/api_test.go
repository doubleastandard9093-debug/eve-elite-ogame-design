package api

import (
	"context"
	"encoding/json"
	"errors"
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"

	"github.com/doubleastandard9093-debug/eve-elite-ogame-design/backend/internal/ships"
)

type fakeDB struct{ err error }

func (f fakeDB) PingContext(context.Context) error { return f.err }

func do(h http.Handler, method, path, body string, hdr map[string]string) *httptest.ResponseRecorder {
	rr := httptest.NewRecorder()
	req := httptest.NewRequest(method, path, strings.NewReader(body))
	for k, v := range hdr {
		req.Header.Set(k, v)
	}
	h.ServeHTTP(rr, req)
	return rr
}

func router(key string) http.Handler {
	return NewRouter(Config{DB: fakeDB{}, Ships: ships.NewMemStore(), Version: "test", ServerKey: key})
}

func TestHealth(t *testing.T) {
	if c := do(router(""), "GET", "/healthz", "", nil).Code; c != 200 {
		t.Fatalf("healthz %d", c)
	}
}

func TestReady(t *testing.T) {
	if c := do(router(""), "GET", "/readyz", "", nil).Code; c != 200 {
		t.Fatalf("ready ok %d", c)
	}
	down := NewRouter(Config{DB: fakeDB{errors.New("down")}, Ships: ships.NewMemStore()})
	if c := do(down, "GET", "/readyz", "", nil).Code; c != 503 {
		t.Fatalf("ready down %d", c)
	}
}

func TestShipRoundTrip(t *testing.T) {
	h := router("")
	if c := do(h, "GET", "/v1/pilots/p1/ship", "", nil).Code; c != 404 {
		t.Fatalf("want 404, got %d", c)
	}
	body := `{"hull":"skiff","system_id":"sys-0001","location":[1,2,3],"rotation":[0,90,0],"hull_hp":750}`
	if rr := do(h, "PUT", "/v1/pilots/p1/ship", body, nil); rr.Code != 200 {
		t.Fatalf("put %d %s", rr.Code, rr.Body)
	}
	rr := do(h, "GET", "/v1/pilots/p1/ship", "", nil)
	if rr.Code != 200 {
		t.Fatalf("get %d", rr.Code)
	}
	var s ships.Ship
	if err := json.Unmarshal(rr.Body.Bytes(), &s); err != nil {
		t.Fatal(err)
	}
	if s.PilotID != "p1" || s.HullHP != 750 || s.Location != [3]float64{1, 2, 3} {
		t.Fatalf("unexpected ship %+v", s)
	}
}

func TestShipValidation(t *testing.T) {
	h := router("")
	if c := do(h, "PUT", "/v1/pilots/p1/ship", `not json`, nil).Code; c != 400 {
		t.Fatalf("bad json %d", c)
	}
	if c := do(h, "PUT", "/v1/pilots/p1/ship", `{"hull":"","hull_hp":1}`, nil).Code; c != 400 {
		t.Fatalf("missing hull %d", c)
	}
	if c := do(h, "PUT", "/v1/pilots/p1/ship", `{"hull":"skiff","hull_hp":-5}`, nil).Code; c != 400 {
		t.Fatalf("negative hp %d", c)
	}
}

func TestServerKey(t *testing.T) {
	h := router("secret")
	if c := do(h, "GET", "/v1/pilots/p1/ship", "", nil).Code; c != 401 {
		t.Fatalf("want 401, got %d", c)
	}
	if c := do(h, "GET", "/v1/pilots/p1/ship", "", map[string]string{"X-Helios-Server-Key": "secret"}).Code; c != 404 {
		t.Fatalf("want 404 with key, got %d", c)
	}
	if c := do(h, "GET", "/healthz", "", nil).Code; c != 200 {
		t.Fatalf("healthz must not need key, got %d", c)
	}
}
