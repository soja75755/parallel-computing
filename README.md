## Directory Structure
Here is an outline of the main files and directories:
```ccs
├── src
│   ├── naive.c
│   ├── unrolling.c
│   ├── reordering.c
│   ├── tiling.c
│   ├── multithreading.c
│   ├── simd.c
│   └── overall.c
├── include
│   └── matmul.h
├── benchmark.c
└── Makefile
```

### Compilation
To compile the code, navigate to the repository root and execute:

```bash
make all
```
This will produce an executable named `benchmark`.

### Running the Benchmarks
To run the benchmark, execute:

```bash
./benchmark
```
### Change method

Available arguments are: 
- NAIVE
- UNROLLING
- REORDERING
- TILING
- MULTITHREADING
- SIMD
- OVERALL

```c
// benchmark.c
int main(int argc, char **argv)
{
    ...

    enum IMP_TYPE imp = OVERALL;
    evaluate(imp, &params);

    return 0;
}
```

### Benchmark result

| Name              | 13th Gen Intel(R) Core(TM) i9-13900K  |
| -----------       | -----------                           |
| Cores             | 32                                    |
| Cache line size   | 64 byte                               |


| Method            | (ms)          |
| -----------       | -----------   |
| NAIVE             | 10338         |
| UNROLLING         | 2671          |
| REORDERING        | 3280          |
| TILING            | 3323          |
| MULTITHREADING    | 665           |
| SIMD              | 1302          |
| OVERALL           | 81            |



