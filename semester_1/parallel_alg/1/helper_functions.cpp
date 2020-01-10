#include <iostream>
#include <random>
#include <omp.h>
#include <fstream>


const int NUMBER_KERNELS = 6;


double openMP_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, const int N,
        const int M, int threads, const int hack = false) {
    // by default threads == number of physical kernels
    // i, j, k, s: private for each thread
    // A, B, C: shared variables

    auto dist = hack ? M / NUMBER_KERNELS : 1;

    auto start = omp_get_wtime();

    #pragma omp parallel num_threads(threads)
    {
        #pragma omp for schedule(static, dist)  // number of working threads ~= number of physical kernels
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

double SPMD_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, const int N, const int M, int threads) {
    // by default threads == number of physical kernels
    auto start = omp_get_wtime();

    if (threads > M) {
        threads = M;
    }

    #pragma omp parallel num_threads(threads)
    {
        int n_threads = threads;
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

void renew_vector(std::vector<int>& vector) {
    const auto size = vector.size();
    vector.clear();
    vector.resize(size);
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
