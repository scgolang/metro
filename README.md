# metro
metronome suitable for music applications

[![Build Status](https://drone.io/github.com/lightning/metro/status.png)](https://drone.io/github.com/lightning/metro/latest)

# build

```shell
$ make
```

# install

```shell
$ sudo make install
```

# usage

Look at examples

### link flag

`-lmetro`

## API

### C

```c

#include <metro/event.h>
#include <metro/metro.h>

static void *
tick(void *arg)
{
    Event ticks = (Event) arg;
}

// Initialize some data you want to use in your ticker function
Metro metro = Metro_create(120);

int fail = Metro_start(metro);
if (fail != 0) {
   // could not start
}

// ...

fail = Metro_stop(metro);
if (fail != 0) {
   // could not stop
}



Metro_free(&metro);
```

### Go

```go

import "github.com/lightning/metro"

func main() {
    m := metro.New(120)
    go handleTicks(m.Ticks())
    err := m.Start()
    // handle error
}

func handleTicks(ticks chan uint64) {
     for tick := range ticks {
         fmt.Printf("tick %d\n", tick)
     }
}

```