## Task 1

g++ version 5.4.0

compile computing engine

```bash
g++ -std=c++14 -O2 -fopenmp main.cpp -o out.out
```

Files "number_threads.txt" and "size_data.txt" must be here. "number_threads.txt" will set number threads that engine will use.
"size_data.txt" will set size matrices.

for execution

```bash
./out.out
```

After execution you will receve "time_threads.txt" that consist time(seconds) of excution in one thread, speedup for SPMD, speedup for OpenMP and
"time_size.txt" that consist time(seconds) of excution in one thread, speedup for SPMD, speedup for OpenMP.

Analysis is done using Python and results of computing.

- [ ] use CMake for build
