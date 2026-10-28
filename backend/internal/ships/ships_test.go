package ships

import (
	"math"
	"strings"
	"testing"
)

func TestValidate(t *testing.T) {
	ok := Ship{PilotID: "p1", Hull: "skiff", SystemID: "sys-0001", HullHP: 100}
	if err := ok.Validate(); err != nil {
		t.Fatalf("valid ship rejected: %v", err)
	}
	bad := []Ship{
		{Hull: "skiff"},
		{PilotID: "p", Hull: ""},
		{PilotID: "p", Hull: strings.Repeat("h", 65)},
		{PilotID: "p", Hull: "skiff", SystemID: strings.Repeat("s", 65)},
		{PilotID: "p", Hull: "skiff", HullHP: -1},
		{PilotID: "p", Hull: "skiff", HullHP: math.NaN()},
		{PilotID: "p", Hull: "skiff", Location: [3]float64{math.Inf(1), 0, 0}},
	}
	for i, s := range bad {
		if s.Validate() == nil {
			t.Errorf("case %d: expected error", i)
		}
	}
}
