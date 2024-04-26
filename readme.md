## PDC Project

Group Members:

- Muhammad Sohail Shahbaz - 21I1356
- Mubeen Qaiser           - 21I0788
- Muneeb Ur Rehman        - 21I0392

## Compilation and Execution Instructions

### Serial

The serial program will be compiled and executed using the following commands:

```bash
g++ -std=c++11 serial.cpp -o serial && ./serial
```

### Parallel

The parallel program will be compiled and executed using 5 MPI processes using the following commands:

```bash
mpic++ -o parallel parallel.cpp
mpirun -np 5 ./parallel
```