#include <omp.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "helper_functions.h"


int main() {
    std::cout << "Number of logical processors: " << omp_get_num_procs() << std::endl << std::endl;

    // A: M*N
    // B: N*M
    // C: M*M

    const int SIZE = 1000;
    const int THREADS = 6;

    auto size_data = std::vector<int>();
    read_data("size_data.txt", size_data);

    auto number_threads = std::vector<int>();
    read_data("number_threads.txt", number_threads);

    // to size matrices
    std::ofstream time_size("time_size.txt", std::ios::out);

    for (int size : size_data) {
        int N = size;
        int M = size;

        auto A = std::vector<int>();
        auto B = std::vector<int>();
        auto C = std::vector<int>(M * M);

        generate_data(A, B, N * M);

        auto serial = (serial_multiplication(A, B, C, N, M) +
                       serial_multiplication(A, B, C, N, M) +
                       serial_multiplication(A, B, C, N, M)) / 3;
        renew_vector(C);

        auto spdm = (SPMD_multiplication(A, B, C, N, M, THREADS) +
                     SPMD_multiplication(A, B, C, N, M, THREADS) +
                     SPMD_multiplication(A, B, C, N, M, THREADS)) / 3;
        renew_vector(C);

        auto openMP = (openMP_multiplication(A, B, C, N, M, THREADS) +
                       openMP_multiplication(A, B, C, N, M, THREADS) +
                       openMP_multiplication(A, B, C, N, M, THREADS)) / 3;
        renew_vector(C);

        auto openMP_hack = (openMP_multiplication(A, B, C, N, M, THREADS, true) +
                            openMP_multiplication(A, B, C, N, M, THREADS, true) +
                            openMP_multiplication(A, B, C, N, M, THREADS, true)) / 3;

        time_size << serial << "," << serial / spdm << "," << serial / openMP << "," << serial / openMP_hack << "\n";
    }

    time_size.close();

    // to number threads
    std::ofstream time_threads("time_threads.txt", std::ios::out);

    double serial = 0;

    for (int n_threads: number_threads) {
        auto A = std::vector<int>();
        auto B = std::vector<int>();
        auto C = std::vector<int>(SIZE * SIZE);

        generate_data(A, B, SIZE * SIZE);

        if (serial == 0) {
            serial = (serial_multiplication(A, B, C, SIZE, SIZE) +
                      serial_multiplication(A, B, C, SIZE, SIZE) +
                      serial_multiplication(A, B, C, SIZE, SIZE)) / 3;
            renew_vector(C);
        }

        auto spdm = (SPMD_multiplication(A, B, C, SIZE, SIZE, n_threads) +
                     SPMD_multiplication(A, B, C, SIZE, SIZE, n_threads) +
                     SPMD_multiplication(A, B, C, SIZE, SIZE, n_threads)) / 3;
        renew_vector(C);

        auto openMP = (openMP_multiplication(A, B, C, SIZE, SIZE, n_threads) +
                       openMP_multiplication(A, B, C, SIZE, SIZE, n_threads) +
                       openMP_multiplication(A, B, C, SIZE, SIZE, n_threads)) / 3;
        renew_vector(C);

        auto openMP_hack = (openMP_multiplication(A, B, C, SIZE, SIZE, n_threads, true) +
                            openMP_multiplication(A, B, C, SIZE, SIZE, n_threads, true) +
                            openMP_multiplication(A, B, C, SIZE, SIZE, n_threads, true)) / 3;


        time_threads << serial << "," << serial / spdm << "," << serial / openMP << "," << serial / openMP_hack << "\n";
    }

    time_threads.close();

    std::cout << "Done!" << std::endl;

    return 0;
}
