#include <omp.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>

// A: M*N
// B: N*M
// C: M*M

double serial_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, const int N, const int M) {
    // serial multiplying matrices
    auto start = omp_get_wtime();

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < M; j++) {
            int s = 0;
            for (int k = 0; k < N; k++) {
                s += A[i * N + k] * B[j + k * M];
            }
            C[i * M + j] = s;
        }
    }

    return omp_get_wtime() - start;
}


double SPMD_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, const int N, const int M, int threads=omp_get_num_procs()) {
    // by default threads == logical processes
    auto start = omp_get_wtime();

    #pragma omp parallel num_threads(threads)  // if (M / number_threads) < 1 then only one thread work
    {
        int n_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int items_per_thread = M / n_threads;
        int lb = thread_id * items_per_thread;
        int ub = (thread_id == n_threads - 1) ? (M - 1) : (lb + items_per_thread - 1);

        for (int i = lb; i <= ub; i++) {
            for (int j = 0; j < M; j++) {
                int s = 0;
                for (int k = 0; k < N; k++) {
                    s += A[i * N + k] * B[j + k * M];
                }
                C[i * M + j] = s;
            }
        }
    }

    return omp_get_wtime() - start;
}


double openMP_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, const int N, const int M, int threads=omp_get_num_procs()) {
    // by default threads == logical processes
    // i, j, k, s: private for each thread
    // A, B, C: shared variables
    auto start = omp_get_wtime();

    #pragma omp parallel num_threads(threads)
    {
    #pragma omp for schedule(static, 200)
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < M; j++) {
                int s = 0;
                for (int k = 0; k < N; k++) {
                    s += A[i * N + k] * B[j + k * M];
                }
                C[i * M + j] = s;
            }
        }
    }

    return omp_get_wtime() - start;
}


void read_data(const std::string& file, std::vector<int>& data) {
    // read size and number of threads
    std::ifstream data_file(file, std::ios::in);
    int number;

    while (!data_file.eof()) {
        data_file >> number;
        data.push_back(number);
    }

    data_file.close();
}

void renew_vector(std::vector<int>& vector, const int size) {
    vector.clear();
    vector.resize(size);
}

void generate_data(std::vector<int>& a, std::vector<int>& b, const int size) {
    // for random data (C++11)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < size; i++) {
        a.push_back(dis(gen));
        b.push_back(dis(gen));
    }
}

int main() {
    std::cout << "Number of logical processors: " << omp_get_num_procs() << std::endl << std::endl;

    const int SIZE = 1400;

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

        auto serial = (serial_multiplication(A, B, C, N, M) + serial_multiplication(A, B, C, N, M) + serial_multiplication(A, B, C, N, M)) / 3;
        renew_vector(C, M * M);

        auto spdm = (SPMD_multiplication(A, B, C, N, M) + SPMD_multiplication(A, B, C, N, M) + SPMD_multiplication(A, B, C, N, M)) / 3;
        renew_vector(C, M * M);

        auto openMP = (openMP_multiplication(A, B, C, N, M) + openMP_multiplication(A, B, C, N, M) + openMP_multiplication(A, B, C, N, M)) / 3;

        time_size << serial << "," << serial / spdm << "," << serial / openMP << "\n";
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
            serial = (serial_multiplication(A, B, C, SIZE, SIZE) + serial_multiplication(A, B, C, SIZE, SIZE) + serial_multiplication(A, B, C, SIZE, SIZE)) / 3;
            renew_vector(C, SIZE * SIZE);
        }

        auto spdm = (SPMD_multiplication(A, B, C, SIZE, SIZE, n_threads) + SPMD_multiplication(A, B, C, SIZE, SIZE, n_threads) + SPMD_multiplication(A, B, C, SIZE, SIZE, n_threads)) / 3;
        renew_vector(C, SIZE * SIZE);

        auto openMP = (openMP_multiplication(A, B, C, SIZE, SIZE, n_threads) + openMP_multiplication(A, B, C, SIZE, SIZE, n_threads) + openMP_multiplication(A, B, C, SIZE, SIZE, n_threads)) / 3;

        time_threads << serial << "," << serial / spdm << "," << serial / openMP << "\n";
    }

    time_threads.close();

    std::cout << "Done!" << std::endl;

    return 0;
}
