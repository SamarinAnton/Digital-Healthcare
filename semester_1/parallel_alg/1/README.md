## Task 1

g++ version 5.4.0

compile computing engine for write result to files

```bash
g++ -std=c++14 -O2 -fopenmp main.cpp helper_functions.cpp -o out.out
```

Files "number_threads.txt" and "size_data.txt" must be here. "number_threads.txt" will set number threads that engine will use.
"size_data.txt" will set size matrices.

for execution

```bash
./out.out
```

After execution you will receve "time_threads.txt" that consist time(seconds) of excution in one thread, speedup for SPMD, speedup for OpenMP, speedup for OpenMP with hack and
"time_size.txt" that consist time(seconds) of excution in one thread, speedup for SPMD, speedup for OpenMP, speedup for OpenMP with hack.

Analysis is done using Python and results of computing.

Python 3.6

create virtual environment and activate it

```bash
virtualenv .venv -p python3.6
source .venv/bin/activate
```

install necessary dependencies

```bash
pip install -r requirements.txt
```

You must have "number_threads.txt", "size_data.txt", "time_size.txt" and "time_threads.txt" for showing dependencies.

launch of script

```bash
python analytics.py
```

You will see images with dependencies

____________________

compile computing engine for executing one time and write result to console

```bash
g++ -std=c++14 -O2 -fopenmp test_start.cpp helper_functions.cpp -o out.out
```

for execution

```bash
./out.out
```
____________________

Also you can use Cython with OpenMP

____________________

### TO DO

- [ ] use CMake for build
