//
// Created by anton on 10.01.2020.
//

#ifndef TESTPLUSPLUS_HELPER_FUNCTIONS_H
#define TESTPLUSPLUS_HELPER_FUNCTIONS_H

#include <vector>

double openMP_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, int N, int M, int threads, int hack = false);
double SPMD_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, int N, int M, int threads);
double serial_multiplication(const std::vector<int>& A, const std::vector<int>& B, std::vector<int>& C, int N, int M);
void generate_data(std::vector<int>& a, std::vector<int>& b, int size);
void renew_vector(std::vector<int>& vector);
void read_data(const std::string& file, std::vector<int>& data);

#endif //TESTPLUSPLUS_HELPER_FUNCTIONS_H
