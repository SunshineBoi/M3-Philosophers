*This project has been created as part of the 42 curriculum by kong.*

# philosophers

## Description

This project is an implementation of the **Dining Philosophers Problem**, originally formulated by Edsger Dijkstra in 1965 as a thought experiment to illustrate challenges in concurrent programming.

N philosophers sit at a round table with one fork between each pair of neighbours. To eat, a philosopher must hold both their left and right fork simultaneously. A philosopher dies if they go `time_to_die` milliseconds without starting a meal. The goal is to keep all philosophers alive indefinitely — or until an optional meal quota is met — without deadlock, starvation, or data races.

Each philosopher runs as a POSIX thread (`pthread`). A dedicated watcher thread independently monitors each philosopher's last meal timestamp and declares death when the deadline is exceeded. All shared state is protected by mutexes.

---

## Instructions

### Compilation

```bash
cd philo
make
```

Produces the `philo` binary in the project root. `make clean` removes objects, `make fclean` removes objects and binary, `make re` rebuilds from scratch.

### Execution

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
```

| Argument | Type | Description |
|---|---|---|
| `number_of_philosophers` | int ≥ 1 | Number of philosophers and forks |
| `time_to_die` | int ≥ 0 (ms) | Time before a philosopher dies without eating |
| `time_to_eat` | int ≥ 0 (ms) | Duration of each meal |
| `time_to_sleep` | int ≥ 0 (ms) | Duration of each sleep |
| `number_of_times_each_philosopher_must_eat` | int ≥ 1 (optional) | Program exits once all reach this count |

### Example runs

```bash
./philo 1 800 200 200          # single philosopher — must die, never eats
./philo 5 800 200 200          # all survive indefinitely (Ctrl+C to stop)
./philo 5 800 200 200 7        # stops cleanly after all eat 7 times
./philo 4 310 200 200          # must produce a death (worst-case gap 400ms > t_die 310ms)
./philo 5 610 200 200 10       # tight timing — stagger + think-time required to survive
./philo 200 800 200 200        # scale test — 200 threads
```

### Output format

```
[timestamp_ms] [philosopher_id] [action]
```

Actions: `has taken a fork` · `is eating` · `is sleeping` · `is thinking` · `died`

No two lines are interleaved. Death is reported within 10ms of the actual deadline.

---

## Technical Overview

### Mutex layout

| Mutex | Protects |
|---|---|
| `mutex_forks[N]` | One per fork — the mutex IS the fork |
| `mutex_lasteaten[N]` | `t_lasteaten` and `n_eaten` per philosopher |
| `mutex_print` | stdout output |
| `mutex_flag` | `flag_stop` stop signal |

### Concurrency challenges

#### Deadlock
With N philosophers and N forks arranged in a circle, the naive approach (every philosopher picks up their left fork first) causes deadlock: all philosophers simultaneously hold one fork and wait for another, forming a circular dependency that never resolves.

**Solution — alternating pickup order:** even-indexed philosophers pick up their left fork first; odd-indexed pick up their right fork first. This breaks the circular wait because at least two adjacent philosophers always contend for the same fork as their first grab — one proceeds while the other blocks, so the cycle cannot close.

#### Starvation
Deadlock prevention alone is insufficient. With 5 philosophers (3 even, 2 odd), the unequal split means one even philosopher is always the "odd one out" — it shares a first-grab fork with a neighbour and gets delayed by a full `t_eat` window on every cycle. If this delay cascades into the odd philosophers' window, a philosopher can go `t_die` ms without eating even with no deadlock.

**Solution — startup stagger + per-cycle think time:**
- Odd philosophers sleep `(t_eat + t_sleep) / 2` at startup, placing them half a cycle behind even philosophers. When even philosophers are grabbing forks, odd philosophers are sleeping, and vice versa.
- For cases where `t_eat > t_sleep`, the shorter sleep means philosophers return to fork contention too quickly, causing phase re-lock over successive cycles. A per-cycle think time of `t_eat - t_sleep + 1` ms (applied after sleeping, odd philosophers only) pads the rest period back to approximately `t_eat`, keeping the half-cycle offset stable.
- When `t_eat ≤ t_sleep`, the sleep itself provides sufficient separation and no extra think time is needed. `t_sleep` is guaranteed to be equals to `t_eat + buffer` - this allows the odd out philo grab the fork during the buffer duration.

The minimum viable `t_die` for 5 philosophers is approximately `2 * t_eat + t_sleep` — any tighter and OS scheduling jitter alone can cause a death regardless of implementation.

#### Data races
Every field read or written by more than one thread is protected by a dedicated mutex:
- `t_lasteaten` and `n_eaten` share `mutex_lasteaten[i]` — the watcher reads both while philosopher threads write them during eating.
- `flag_stop` is protected by `mutex_flag` — set once by the watcher, polled by all threads.
- stdout is serialised by `mutex_print`.

**Stop-flag race in `printf_action`:** a naive implementation checks the stop flag, then acquires the print mutex — another thread can print `died` between the check and the lock, causing a non-die message to appear after death. The fix is to acquire `mutex_print` first, then check the flag inside the lock, making the check-and-print decision atomic.

#### Timestamp precision
`gettimeofday` provides microsecond resolution. All internal timestamps are stored as milliseconds elapsed since simulation start (`t_start_app`). Sleep precision is achieved with a busy-wait loop using `usleep(100)` (0.1 ms granularity), which exits early if the stop flag is set, preventing threads from oversleeping after a death is declared.

---

## Resources

### Foundational references

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/) — Comprehensive pthreads reference covering thread creation, mutexes, and synchronisation.
- `man pthread_create`, `man pthread_mutex_init`, `man gettimeofday` — POSIX man pages used throughout.
- [Threads, Mutexes and Concurrent Programming in C — codequoi](https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/) — Practical walkthrough of pthreads and mutex usage in C, directly applicable to this project.

### AI usage

Claude (Anthropic) was used throughout this project as a learning and review tool via the 42 student's personal claude.ai account. Specific uses:

- **Conceptual explanation** — thread vs process memory model, mutex visibility guarantees, deadlock vs starvation distinction, memory barriers and why `volatile` alone is insufficient for cross-thread visibility.
- **Architecture review** — struct design decisions (`t_app` / `t_philo` split, mutex array layout, watcher thread design), reviewed iteratively before implementation.
- **Timing analysis** — traced starvation scenarios cycle by cycle to diagnose phase-lock drift; derived the `think_time = t_eat - t_sleep` formula to maintain fixed cycle length.

All core logic, implementation, and final decisions were written and verified manually. Claude was used to ask questions, review drafts, and reason through edge cases.
