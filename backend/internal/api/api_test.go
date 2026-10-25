package api

import (
	"context"
	"errors"
	"net/http"
	"net/http/httptest"
	"testing"
)

type fakeDB struct{ err error }

func (f fakeDB) PingContext(context.Context) error { return f.err }

func get(h http.Handler, path string) int {
	rr := httptest.NewRecorder()
	h.ServeHTTP(rr, httptest.NewRequest("GET", path, nil))
	return rr.Code
}

func TestHealth(t *testing.T) {
	if c := get(NewRouter(nil, "test"), "/healthz"); c != 200 {
		t.Fatalf("healthz %d", c)
	}
}

func TestReady(t *testing.T) {
	if c := get(NewRouter(fakeDB{}, "t"), "/readyz"); c != 200 {
		t.Fatalf("ready ok %d", c)
	}
	if c := get(NewRouter(fakeDB{errors.New("down")}, "t"), "/readyz"); c != 503 {
		t.Fatalf("ready down %d", c)
	}
}
