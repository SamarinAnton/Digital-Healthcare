#include <iostream>
#include <cmath>
#include <omp.h>


// system setting
const int THREADS = 6;  // number of threads

struct IntegralResult {
    double value = 0;
    int n = 0;
    double time = -1;
};

IntegralResult serial_integral(double a, double b, int n);

IntegralResult atomic_integral(double a, double b, int n);

IntegralResult critical_section_integral(double a, double b, int n);

IntegralResult locks_integral(double a, double b, int n);

IntegralResult reduction_integral(double a, double b, int n);

IntegralResult get_result(double a, double b, double epsilon, IntegralResult (*integral)(double, double, int));

double target_func(double x);


int main() {
    // input settings
    const double EPS = 1e-7;  // precision of computing
    const double A = 1;  // start of domain
    const double B = 1000;  // end of domain

    {
        auto result = get_result(A, B, EPS, serial_integral);

        std::cout << "Serial execution:" << std::endl;
        std::cout << "Integral value: " << result.value << std::endl;
        std::cout << "Number of iterations: " << result.n << std::endl;
        std::cout << "Time: " << result.time << std::endl << std::endl;
    }

    {
        auto result = get_result(A, B, EPS, atomic_integral);

        std::cout << "OpenMP atomic execution:" << std::endl;
        std::cout << "Integral value: " << result.value << std::endl;
        std::cout << "Number of iterations: " << result.n << std::endl;
        std::cout << "Time: " << result.time << std::endl << std::endl;
    }

    {
        auto result = get_result(A, B, EPS, critical_section_integral);

        std::cout << "OpenMP critical section execution:" << std::endl;
        std::cout << "Integral value: " << result.value << std::endl;
        std::cout << "Number of iterations: " << result.n << std::endl;
        std::cout << "Time: " << result.time << std::endl << std::endl;
    }

    {
        auto result = get_result(A, B, EPS, locks_integral);

        std::cout << "OpenMP locks execution:" << std::endl;
        std::cout << "Integral value: " << result.value << std::endl;
        std::cout << "Number of iterations: " << result.n << std::endl;
        std::cout << "Time: " << result.time << std::endl << std::endl;
    }

    {
        auto result = get_result(A, B, EPS, reduction_integral);

        std::cout << "OpenMP reduction execution:" << std::endl;
        std::cout << "Integral value: " << result.value << std::endl;
        std::cout << "Number of iterations: " << result.n << std::endl;
        std::cout << "Time: " << result.time << std::endl << std::endl;
    }

    return 0;
}

double target_func(double x) {
    return (1.0 / (x * x) * pow(sin(1 / x), 2));
}

// test integral with A = 0, B = 1, integral == pi
//double target_func(double x) {
//    return (4 / (1 + x * x));
//}

IntegralResult get_result(const double a, const double b, const double epsilon,
        IntegralResult (*integral)(double, double, int)) {

    IntegralResult result;
    const int increment = 100;
    int n = 1000;
    double diff;

    do {
        auto start_time = omp_get_wtime();
        auto new_result = integral(a, b, n);  // spend additional time on call of pointer and to copy of structure
        auto end_time = omp_get_wtime();

        diff = std::abs(new_result.value - result.value);
        result = new_result;
        result.time = end_time - start_time;
        n += increment;

    } while (diff > epsilon);

    for (int i = 0; i < 4; i++) {
        auto start_time = omp_get_wtime();
        integral(a, b, result.n);
        auto end_time = omp_get_wtime();

        result.time += end_time - start_time;
    }

    result.time /= 5;

    return result;
}

IntegralResult serial_integral(const double a, const double b, const int n) {
    IntegralResult result;
    const double delta = (b - a) / n;
    result.n = n;

    double sum = 0;

    for (int i = 1; i < n; i++) {
        double xi = a + i * delta;
        sum += target_func(xi);
    }

    result.value = (sum + target_func(a) / 2 + target_func(b) / 2) * delta;
    return result;
}

IntegralResult atomic_integral(const double a, const double b, const int n) {
    IntegralResult result;
    const double delta = (b - a) / n;
    result.n = n;

    double sum = 0;

    #pragma omp parallel num_threads(THREADS)
    {
        double local_sum = 0;

        #pragma omp for
        for (int i = 1; i < n; i++) {
            double xi = a + i * delta;
            local_sum += target_func(xi);
        }

        #pragma omp atomic
        sum += local_sum;
    }

    result.value = (sum + target_func(a) / 2 + target_func(b) / 2) * delta;
    return result;
}

IntegralResult critical_section_integral(const double a, const double b, const int n) {
    IntegralResult result;
    const double delta = (b - a) / n;
    result.n = n;

    double sum = 0;

    #pragma omp parallel num_threads(THREADS)
    {
        double local_sum = 0;

        #pragma omp for
        for (int i = 1; i < n; i ++) {
            double xi = a + i * delta;
            local_sum += target_func(xi);
        }

        #pragma omp critical(critical)
        {
            sum += local_sum;
        }
    }

    result.value = (sum + target_func(a) / 2 + target_func(b) / 2) * delta;
    return result;
}

IntegralResult locks_integral(const double a, const double b, const int n) {
    IntegralResult result;
    const double delta = (b - a) / n;
    result.n = n;

    double sum = 0;

    omp_lock_t lock;
    omp_init_lock(&lock);

    #pragma omp parallel num_threads(THREADS)
    {
        double local_sum = 0;

        #pragma omp for
        for (int i = 1; i < n; i ++) {
            double xi = a + i * delta;
            local_sum += target_func(xi);
        }

        omp_set_lock(&lock);
        sum += local_sum;
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    result.value = (sum + target_func(a) / 2 + target_func(b) / 2) * delta;
    return result;
}

IntegralResult reduction_integral(const double a, const double b, const int n) {
    IntegralResult result;
    const double delta = (b - a) / n;
    result.n = n;

    double sum = 0;

    #pragma omp parallel for num_threads(THREADS) reduction(+:sum)
    for (int i = 1; i < n; i++) {
        double xi = a + i * delta;
        sum += target_func(xi);
    }

    result.value = (sum + target_func(a) / 2 + target_func(b) / 2) * delta;
    return result;
}