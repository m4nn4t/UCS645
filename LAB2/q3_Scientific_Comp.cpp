#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;

int main() {
    const int N = 1000;          // grid size NxN
    const int STEPS = 500;       // time steps
    const double alpha = 0.1;    // diffusion coefficient

    vector<vector<double>> T(N, vector<double>(N, 0.0));
    vector<vector<double>> T_new(N, vector<double>(N, 0.0));

    // Initial condition: hot spot in center
    T[N/2][N/2] = 100.0;

    cout << "Threads\tExecution Time (s)\tTotal Heat\n";

    for (int threads = 2; threads <= 16; threads *= 2) {

        omp_set_num_threads(threads);
        double start = omp_get_wtime();

        for (int t = 0; t < STEPS; t++) {

            double total_heat = 0.0;

            #pragma omp parallel for reduction(+:total_heat) schedule(static)
            for (int i = 1; i < N - 1; i++) {
                for (int j = 1; j < N - 1; j++) {
                    T_new[i][j] = T[i][j] + alpha * (
                        T[i+1][j] + T[i-1][j] +
                        T[i][j+1] + T[i][j-1] -
                        4.0 * T[i][j]
                    );
                    total_heat += T_new[i][j];
                }
            }

            // Swap grids
            T.swap(T_new);
        }

        double end = omp_get_wtime();
        cout << threads << "\t" << (end - start)
             << "\t\t" << T[N/2][N/2] << endl;
    }

    return 0;
}
