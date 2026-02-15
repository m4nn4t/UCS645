#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <cstdlib>


using namespace std;

struct Particle {
    double x, y, z;
    double fx, fy, fz;
};

int main() {
    const int N = 2000;          // number of particles
    const double epsilon = 1.0;
    const double sigma = 1.0;
    const double cutoff = 2.5 * sigma;
    const double cutoff2 = cutoff * cutoff;
    srand(123);


    vector<Particle> p(N);

    // Initialize particle positions
    for (int i = 0; i < N; i++) {
        p[i].x = (double)rand() / RAND_MAX;
        p[i].y = (double)rand() / RAND_MAX;
        p[i].z = (double)rand() / RAND_MAX;

        p[i].fx = p[i].fy = p[i].fz = 0.0;
    }

    cout << "Threads\tExecution Time (s)\tTotal Energy\n";

    for (int threads = 2; threads <= 16; threads *= 2) {

        omp_set_num_threads(threads);

        double total_energy = 0.0;
        double start = omp_get_wtime();

        #pragma omp parallel reduction(+:total_energy)
        {
            // Thread-private force arrays
            vector<double> fx(N, 0.0), fy(N, 0.0), fz(N, 0.0);

            #pragma omp for schedule(dynamic)
            for (int i = 0; i < N; i++) {
                for (int j = i + 1; j < N; j++) {

                    double dx = p[i].x - p[j].x;
                    double dy = p[i].y - p[j].y;
                    double dz = p[i].z - p[j].z;

                    double r2 = dx*dx + dy*dy + dz*dz;

                    if (r2 < cutoff2) {
                        double inv_r2 = 1.0 / r2;
                        double inv_r6 = inv_r2 * inv_r2 * inv_r2;
                        double inv_r12 = inv_r6 * inv_r6;

                        double force = 24.0 * epsilon * inv_r2 *
                                       (2.0 * inv_r12 - inv_r6);

                        fx[i] += force * dx;
                        fy[i] += force * dy;
                        fz[i] += force * dz;

                        fx[j] -= force * dx;
                        fy[j] -= force * dy;
                        fz[j] -= force * dz;

                        total_energy += 4.0 * epsilon *
                                        (inv_r12 - inv_r6);
                    }
                }
            }

            // Safe accumulation into global forces
            #pragma omp critical
            {
                for (int i = 0; i < N; i++) {
                    p[i].fx += fx[i];
                    p[i].fy += fy[i];
                    p[i].fz += fz[i];
                }
            }
        }

        double end = omp_get_wtime();

        cout << threads << "\t" << (end - start)
             << "\t\t" << total_energy << endl;
    }

    return 0;
}
