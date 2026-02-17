#include "functions.h"
#include <cmath>
#include <omp.h>

void correlate(int ny, int nx,
               const float* data,
               float* result)
{
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < ny; i++) {

        for (int j = 0; j <= i; j++) {

            double dot = 0.0;
            double norm_i = 0.0;
            double norm_j = 0.0;

            // inner loop vectorization-friendly
            for (int k = 0; k < nx; k++) {
                double xi = data[k + i*nx];
                double xj = data[k + j*nx];

                dot    += xi * xj;
                norm_i += xi * xi;
                norm_j += xj * xj;
            }

            double corr = dot / (sqrt(norm_i) * sqrt(norm_j));
            result[i + j*ny] = (float)corr;
        }
    }
}

