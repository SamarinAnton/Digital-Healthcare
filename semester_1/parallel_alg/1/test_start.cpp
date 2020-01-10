#include <iostream>
#include <omp.h>
#include "helper_functions.h"


int main() {
    std::cout << "Number of logical processors: " << omp_get_num_procs() << std::endl << std::endl;

    // A: M*N
    // B: N*M
    // C: M*M

    const int N = 1000;  // number of columns
    const int M = 1000;  // number of rows

    const int threads = 6;

    auto A = std::vector<int>();
    auto B = std::vector<int>();
    auto C = std::vector<int>(M * M);

    generate_data(A, B, N * M);

    std::cout << "serial: " << serial_multiplication(A, B, C, N, M) << std::endl;
    renew_vector(C);

    std::cout << "SPMD: " << SPMD_multiplication(A, B, C, N, M, threads) << std::endl;
    renew_vector(C);

    std::cout << "OpenMP: " << openMP_multiplication(A, B, C, N, M, threads) << std::endl;
    renew_vector(C);

    std::cout << "OpenMP hack: " << openMP_multiplication(A, B, C, N, M, threads, true) << std::endl;

    return 0;
}
