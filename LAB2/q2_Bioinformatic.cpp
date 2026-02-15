#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

using namespace std;

int score(char a, char b) {
    return (a == b) ? 2 : -1;   // match / mismatch
}

int main() {
    string A = "ACACACTA";
    string B = "AGCACACA";

    int M = A.length();
    int N = B.length();
    int gap = 2;

    vector<vector<int>> H(M + 1, vector<int>(N + 1, 0));
    int max_score = 0;

    cout << "Threads\tExecution Time (seconds)\n";

    for (int threads = 2; threads <= 16; threads *= 2) {

        omp_set_num_threads(threads);
        max_score = 0;

        double start = omp_get_wtime();

        // Wavefront (anti-diagonal traversal)
        for (int d = 1; d <= M + N - 1; d++) {

            #pragma omp parallel for schedule(dynamic) reduction(max:max_score)
            for (int i = 1; i <= M; i++) {
                int j = d - i + 1;

                if (j >= 1 && j <= N) {
                    int diag = H[i-1][j-1] + score(A[i-1], B[j-1]);
                    int up   = H[i-1][j] - gap;
                    int left = H[i][j-1] - gap;

                    H[i][j] = max({0, diag, up, left});
                    max_score = max(max_score, H[i][j]);
                }
            }
        }

        double end = omp_get_wtime();
        cout << threads << "\t" << (end - start) << endl;
    }

    cout << "\nMaximum Local Alignment Score: " << max_score << endl;
    return 0;
}
