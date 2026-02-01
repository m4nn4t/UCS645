#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
    const int N = 1 << 16;  // 2^16
    double a = 2.5;

    vector<double> X(N, 1.0);
    vector<double> Y(N, 2.0);

    cout << "Threads\tExecution Time (seconds)" << endl;

    // Test from 2 threads up to 16 threads
    for (int threads = 2; threads <= 256; threads *= 2) {

        omp_set_num_threads(threads);

        double start = omp_get_wtime();

        #pragma omp parallel for
        for (int i = 0; i < N; i++) {
            X[i] = a * X[i] + Y[i];
        }

        double end = omp_get_wtime();

        cout << threads << "\t" << (end - start) << endl;
    }

    return 0;
}
