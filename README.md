*This project has been created as part of the 42 curriculum by gacattan.*

# Codexion

## Description

Codexion is a C simulation of concurrent coders sharing USB dongles in a circular workspace. Each coder runs in a POSIX thread and alternates between compiling, debugging and refactoring. Compiling requires both neighbouring dongles. Released dongles must complete a cooldown before they can be used again.

A separate monitor checks compilation deadlines and completed compilation counts. The simulation is intended to stop on the first burnout or when every coder has completed the requested number of compilations. The subject requires burnout announcements within 10 ms of the deadline.

The implementation uses a custom two-entry priority queue per dongle. FIFO preserves insertion order; EDF prioritizes the earliest deadline (`last_compile_start + time_to_burnout`). Equal EDF deadlines preserve insertion order. Initial requests are registered for odd coder IDs, then even IDs, before coder threads are created. Each dongle has only two neighbouring requesters, so two slots suffice while each coder has at most one pending request per dongle.

The local subject is available in [English](docs/subject.md) and [French](docs/subject_fr.md). This README describes the implementation; passing a few runs does not establish full timing or starvation guarantees.

## Instructions

### Build

Use a POSIX environment with a C compiler, Make and pthread support. Linux and Ubuntu under WSL are suitable environments.

```sh
make
```

The Makefile uses `cc -Wall -Wextra -Werror -pthread` and produces `codexion`.

```sh
make clean   # Remove object files
make fclean  # Remove objects and executable
make re      # Rebuild
```

No libft or other external library is required. Source and header lists are explicit in the Makefile.

### Run

All eight arguments are mandatory:

```sh
./codexion number_of_coders time_to_burnout time_to_compile \
    time_to_debug time_to_refactor number_of_compiles_required \
    dongle_cooldown scheduler
```

| Argument | Meaning |
| --- | --- |
| `number_of_coders` | Number of coder threads and dongles; must be greater than zero. |
| `time_to_burnout` | Maximum elapsed time since the start of the last compilation, or the shared simulation start before the first compilation. |
| `time_to_compile` | Duration of a compilation while holding both dongles. |
| `time_to_debug` | Duration of the debugging phase. |
| `time_to_refactor` | Duration of the refactoring phase. |
| `number_of_compiles_required` | Completed compilations required for each coder. The current parser requires a positive value. |
| `dongle_cooldown` | Minimum time after release before a dongle can be reserved again. |
| `scheduler` | Exactly `fifo` or `edf`, in lowercase. |

All durations are expressed in milliseconds. Numeric inputs must contain decimal digits only and fit in the range 0–2147483647, with the additional positive-value rules above. Negative numbers, signs, whitespace, fractions and trailing characters are rejected. Errors are printed to stderr with an identifier and, where applicable, an argument name.

Examples:

```sh
./codexion 5 2000 200 200 200 10 0 fifo
./codexion 5 2000 200 200 200 7 0 edf
./codexion 1 800 200 200 200 10 0 fifo
```

A single coder cannot acquire two distinct dongles and must eventually burn out. A burnout is a simulation event, not a parsing error.

Logs use timestamps relative to the shared start time:

```text
0 1 has taken a dongle
0 1 has taken a dongle
0 1 is compiling
200 1 is debugging
400 1 is refactoring
```

A burnout line has the form `800 1 burned out`. Scheduling affects the order and timestamps of actual output.

### Checks

```sh
norminette
make re CFLAGS="-Wall -Wextra -Werror -pthread -g"
valgrind --leak-check=full --show-leak-kinds=all \
    ./codexion 5 10000 60 60 60 3 60 edf
valgrind --tool=helgrind \
    ./codexion 5 10000 60 60 60 3 60 edf
```

Valgrind changes execution speed. Measure the 10 ms burnout requirement on native executions, separately from memory and concurrency instrumentation. An error-free report only covers the paths exercised by that run.

## Blocking cases handled

- **Mutex deadlock prevention:** neighbouring dongle pointers are ordered by their positions in the shared array. Both dongle mutexes are acquired in this order. This breaks the circular-wait condition among dongle locks, one of Coffman's necessary conditions for deadlock. Mutual exclusion remains necessary; mutexes are not held during compilation sleeps.
- **Resource contention:** a coder reserves both dongles only when both are available, its requests have priority in both queues, and both cooldowns have elapsed. The queue operations and reservation are protected by the dongle mutexes.
- **Waiting:** unavailable resources lead to a condition-variable wait. Releases and global stop broadcast a notification. Known cooldown delays use an interruptible timed pause before retrying.
- **Cooldown:** both release timestamps are recorded, and the next reservation checks the remaining delay. The initial zero release timestamps allow unused dongles to be taken immediately.
- **Burnout:** the monitor polls approximately every millisecond. Phase waits check the stop flag between short sleeps. A coder checks its previous deadline under the monitor mutex before updating its compilation start.
- **Single coder:** acquisition rejects identical dongles, preventing a double lock of the same non-recursive mutex. The stop broadcast wakes the waiting coder.
- **Starvation policy:** EDF orders waiting requests by deadline, with stable insertion order for ties. This policy is implemented, but a universal liveness guarantee under all feasible workloads requires more than a successful sample run.
- **Log serialization:** a shared log mutex prevents line interleaving. The two acquisition messages and compilation message are emitted together under one lock. While holding the log mutex, log functions check the shared stop flag and suppress normal messages after stop. The burnout message remains allowed, so no normal message can be printed after its announcement.

## Thread synchronization mechanisms

| Primitive | Protected data or purpose |
| --- | --- |
| One `pthread_mutex_t` per dongle | Availability, release timestamp and that dongle's request queue. |
| `monitoring.mutex` | Shared stop flag, compilation-start updates, completed-count updates and monitor reads of these fields. |
| `monitoring.resource_mutex` with `resource_cond` | Coordinates resource waiting and release/stop notifications. `pthread_cond_wait` releases the resource mutex while sleeping and reacquires it before returning; the acquisition condition is checked again in a loop. |
| Shared log mutex | Serializes output across coders and the monitor. |
| `pthread_join` | On the normal path, waits for the monitor and all coders before shared arrays are freed and synchronization objects are destroyed. |

For example, a coder updates `nb_compile` under `monitoring.mutex`; the monitor locks the same mutex while checking the counters. Dongle reservation takes both dongle locks before changing either availability flag, preventing two coders from reserving the same resource concurrently.

Release code drops the dongle locks before acquiring `resource_mutex` for its broadcast. Stop code drops `monitoring.mutex` before acquiring `resource_mutex`. Preserving this separation matters because waiting code holds the resource mutex while checking stop and attempting acquisition.

The project's boolean convention is deliberate: `TRUE = 0`, `FALSE = -1`. Use explicit comparisons rather than ordinary C truth tests.

## Source layout

- `main.c`, `parser.c`, `config.h`: entry point and argument validation.
- `simulation.c`, `init.c`, `threads.c`: setup, thread lifecycle and cleanup.
- `coder.c`: compilation/debug/refactor cycle.
- `dongle.c`, `dongle2.c`: reservation, release, cooldown and waiting.
- `requests.c`, `heap.c`: request registration and FIFO/EDF priority queues.
- `monitoring.c`, `monitoring_checks.c`: stop flag, quota checks and burnout monitoring.
- `log.c`, `time_utils.c`: output and millisecond timing.
- `memory_manager.c`, `error.c`, `types.h`: allocation, diagnostics and shared boolean type.

## Resources

- [Codexion subject, version 1.5](docs/subject.md).
- POSIX/Linux manual pages: `man pthread_create`, `man pthread_join`, `man pthread_mutex_lock`, `man pthread_cond_wait`, `man gettimeofday`.
- [The Open Group POSIX reference](https://pubs.opengroup.org/onlinepubs/9799919799/).
- [Valgrind Helgrind manual](https://valgrind.org/docs/manual/hg-manual.html), for races, lock ordering and pthread misuse.
- [Valgrind Memcheck manual](https://valgrind.org/docs/manual/mc-manual.html), for invalid memory access and leaks.

### Use of AI

AI assistance was used to explain C pointers, structures, allocation, pthreads, mutexes and priority queues; discuss parsing and error messages; review the monitoring and stop logic; suggest and sometimes apply explicitly requested file reorganization and code changes; produce documentation and this README; and prepare and run functional, timing, memory and concurrency audits. The implementation was developed iteratively with the author. AI-generated suggestions are not proof of correctness and must be understood and checked against the subject and peer review.
