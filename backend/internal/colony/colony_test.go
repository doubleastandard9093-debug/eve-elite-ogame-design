package colony

import (
	"testing"
	"time"
)

func TestMineRate(t *testing.T) {
	if MineRatePerHour(30, 0) != 0 {
		t.Fatal("level 0 should produce nothing")
	}
	if MineRatePerHour(30, 2) <= MineRatePerHour(30, 1) {
		t.Fatal("rate should grow with level")
	}
}

func TestAccrueClampsAndHandlesNegative(t *testing.T) {
	r := Accrue(Resources{100, 0, 0}, Resources{60, 0, 0}, 2*time.Hour, 200)
	if r.Ferrite != 200 {
		t.Fatalf("want clamp 200, got %v", r.Ferrite)
	}
	r = Accrue(Resources{100, 0, 0}, Resources{60, 0, 0}, -time.Hour, 1000)
	if r.Ferrite != 100 {
		t.Fatalf("negative elapsed must not change, got %v", r.Ferrite)
	}
}

func TestBuildTime(t *testing.T) {
	if BuildTime(60, 1) != time.Minute || BuildTime(60, 3) != 135*time.Second {
		t.Fatal("unexpected build time curve")
	}
}
