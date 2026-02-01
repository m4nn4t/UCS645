#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
    const int N = 1000;

    vector<vector<double>> A(N, vector<double>(N, 1.0));
    vector<vector<double>> B(N, vector<double>(N, 1.0));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    cout << "2D Threading Matrix Multiplication\n";
    cout << "Threads\tExecution Time (seconds)\n";

    for (int threads = 2; threads <= 16; threads *= 2) {

        omp_set_num_threads(threads);
        double start = omp_get_wtime();

        #pragma omp parallel for collapse(2)
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                double sum = 0.0;
                for (int k = 0; k < N; k++) {
                    sum += A[i][k] * B[k][j];
                }
                C[i][j] = sum;
            }
        }

        double end = omp_get_wtime();
        cout << threads << "\t" << (end - start) << endl;
    }

    return 0;
}
