package metro

import "fmt"
import "math"
import "testing"
import "time"

func TestMetro(t *testing.T) {
	m := New(480)

	err := m.Start()
	if err != nil {
		t.Fatal(err)
	}
	
	done := make(chan error)
	dur1 := time.Duration(125000000)
	dur2 := time.Duration(166000000)
	margin := float64(10000000)

	go func(m Metro) {
		// expect some ticks
		var start, end time.Time
		for count := range m.Ticks() {
			if count == uint64(8) {
				end = time.Now()
				diff := math.Abs(float64(end.Sub(start) - dur1))
				if diff > margin {
					done <-fmt.Errorf("clock ticks were %fns off", diff)
					return
				}
				break
			} else if count > uint64(8) {
				done <-fmt.Errorf("count should have stopped at 8")
				return
			} else {
				start = time.Now()
			}
		}
		// change tempo
		old := m.SetTempo(360)
		if old != float32(480) {
			done <-fmt.Errorf("expected old tempo to be 480")
			return
		}
		// expect another 4 ticks
		for count := range m.Ticks() {
			if count == uint64(16) {
				end = time.Now()
				diff := math.Abs(float64(end.Sub(start) - dur2))
				if diff > margin {
					done <-fmt.Errorf("clock ticks were %fns off", diff)
					return
				}
				break
			} else if count > uint64(16) {
				done <-fmt.Errorf("count should have stopped at 16")
				return
			} else {
				start = time.Now()
			}
		}
		// exit
		done <-m.Stop()
	}(m)

	err = <-done
	if err != nil {
		t.Fatal(err)
	}
}
