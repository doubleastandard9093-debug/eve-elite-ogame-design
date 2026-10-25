// Package colony holds pure colony economy math (lazy evaluation per doc 08).
package colony

import (
	"math"
	"time"
)

type Resources struct{ Ferrite, Crystal, Volatiles float64 }

// MineRatePerHour returns base production for a mine level (grows ~1.1^level).
func MineRatePerHour(base float64, level int) float64 {
	if level <= 0 {
		return 0
	}
	return base * float64(level) * math.Pow(1.1, float64(level))
}

// Accrue returns stored resources after elapsed time, clamped to capacity.
func Accrue(r Resources, rate Resources, elapsed time.Duration, capacity float64) Resources {
	h := elapsed.Hours()
	if h < 0 {
		h = 0
	}
	c := func(v, r float64) float64 { return math.Min(capacity, v+r*h) }
	return Resources{c(r.Ferrite, rate.Ferrite), c(r.Crystal, rate.Crystal), c(r.Volatiles, rate.Volatiles)}
}

// BuildTime returns build duration for upgrading to level (cost grows 1.5^level).
func BuildTime(baseSeconds float64, level int) time.Duration {
	return time.Duration(baseSeconds*math.Pow(1.5, float64(level-1))) * time.Second
}
