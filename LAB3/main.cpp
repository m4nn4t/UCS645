#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include "functions.h"



using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Usage: ./matrix_mult ny nx\n";
        return 1;
    }

    int ny = atoi(argv[1]);
    int nx = atoi(argv[2]);

    vector<float> data(ny * nx);
    vector<float> result(ny * ny);

    // initialize data
    for (int i = 0; i < ny * nx; i++) {
        data[i] = (float)rand() / RAND_MAX;
    }

    double start = omp_get_wtime();

correlate(ny, nx, data.data(), result.data());

double end = omp_get_wtime();

cout << "Time taken: " << (end - start) << " seconds\n";


    cout << "Computation finished.\n";
    return 0;
}
