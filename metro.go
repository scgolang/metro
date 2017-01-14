// metronome suitable for applications that require
// the smallest amount of clock jitter possible
package metro

// #include "metro.h"
// #cgo linux LDFLAGS: -lrt
import "C"
import "fmt"
import "sync/atomic"

// Metro is the interface to a bpm-based metronome.
type Metro interface {
	Start() error
	Stop() error
	SetTempo(bpm float32) float32
	Ticks() chan uint64
}

// do we need to cover Metro_free? [bps]

type metro struct {
	c       chan uint64
	handle  C.Metro
	counter uint64
}

func (self *metro) Start() error {
	swapped := atomic.CompareAndSwapUint64(&self.counter, self.counter, uint64(0))
	if !swapped {
		return fmt.Errorf("Could not set counter to 0")
	}
	if rc := C.Metro_start(self.handle); rc != 0 {
		return fmt.Errorf("Could not start metro %d", rc)
	}
	return nil
}

func (self *metro) Stop() error {
	if rc := C.Metro_stop(self.handle); rc != 0 {
		return fmt.Errorf("Could not stop metro %d", rc)
	}
	return nil
}

func (self *metro) SetTempo(bpm float32) float32 {
	return float32(C.Metro_set_bpm(self.handle, C.Bpm(bpm)))
}

func (self *metro) Ticks() chan uint64 {
	return self.c
}

func New(bpm float32) Metro {
	m := new(metro)
	m.c = make(chan uint64)
	m.handle = C.Metro_create(C.Bpm(bpm))
	m.counter = 0
	e := C.Metro_tick(m.handle)
	go metroTick(m, e)
	return m
}

func metroTick(m *metro, e C.MetroEvent) {
	for {
		C.MetroEvent_wait(e)
		m.c <- m.counter
		m.counter = atomic.AddUint64(&m.counter, uint64(1))
	}
}
