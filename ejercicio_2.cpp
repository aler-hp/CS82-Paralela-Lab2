#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>

using namespace std;

typedef void (*MatrixMultiplyFunc)(const vector<vector<double>>&,
                                   const vector<vector<double>>&,
                                   vector<vector<double>>&,
                                   int);

typedef void (*BlockedMatrixMultiplyFunc)(const vector<vector<double>>&,
                                          const vector<vector<double>>&,
                                          vector<vector<double>>&,
                                          int, int);


void multiplicationClasica(const vector<vector<double>>& A,
                                   const vector<vector<double>>& B,
                                   vector<vector<double>>& C,
                                   int N) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            C[i][j] = 0.0;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void multiplicacionBloques(const vector<vector<double>>& A,
                                 const vector<vector<double>>& B,
                                 vector<vector<double>>& C,
                                 int N, int b_size) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            C[i][j] = 0.0;

    for (int ii = 0; ii < N; ii += b_size) {
        for (int kk = 0; kk < N; kk += b_size) {
            for (int jj = 0; jj < N; jj += b_size) {
                for (int i = ii; i < min(ii + b_size, N); ++i) {
                    for (int k = kk; k < min(kk + b_size, N); ++k) {
                        for (int j = jj; j < min(jj + b_size, N); ++j) {
                            C[i][j] += A[i][k] * B[k][j];
                        }
                    }
                }
            }
        }
    }
}

double evaluate(MatrixMultiplyFunc func,
                const vector<vector<double>>& A,
                const vector<vector<double>>& B,
                vector<vector<double>>& C, int n) {
    auto start = chrono::high_resolution_clock::now();
    func(A, B, C, n);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    return duration.count();
}

double evaluateB(BlockedMatrixMultiplyFunc func,
                 const vector<vector<double>>& A,
                 const vector<vector<double>>& B,
                 vector<vector<double>>& C, int n, int b_size) {
    auto start = chrono::high_resolution_clock::now();
    func(A, B, C, n, b_size);
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double> duration = end - start;
    return duration.count();
}

void evaluateBSize(int N, const vector<int>& block_sizes) {
    vector<vector<double>> A(N, vector<double>(N, 1.5));
    vector<vector<double>> B(N, vector<double>(N, 2.0));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    cout << "\nEvaluacion por tamano de bloque (N = " << N << ")" << endl;
    cout << left << setw(15) << "b_size"
         << setw(20) << "Tiempo (seg)" << endl;
    cout << string(35, '-') << endl;

    for (int b : block_sizes) {
        double time = evaluateB(multiplicacionBloques, A, B, C, N, b);
        cout << left << setw(15) << b
             << fixed << setprecision(5) << setw(20) << time << endl;
    }
}

int main() {
    vector<int> sizes = {200, 400, 800, 1000, 1500, 2000};
    int b_size = 64;

    cout << left << setw(15) << "N x N"
         << setw(20) << "Clasica (seg)"
         << setw(20) << "Bloques (seg)"
         << setw(15) << "Speedup" << endl;
    cout << string(70, '-') << endl;

    /*
    for (int N : sizes) {
        vector<vector<double>> A(N, vector<double>(N, 1.5));
        vector<vector<double>> B(N, vector<double>(N, 2.0));
        vector<vector<double>> C(N, vector<double>(N, 0.0));


        double time_classical = evaluate(multiplicationClasica, A, B, C, N);

        double time_blocked = evaluateB(multiplicacionBloques, A, B, C, N, b_size);
        double speedup = time_classical / time_blocked;
        cout << left << setw(15) << N
             << fixed << setprecision(5) << setw(20) << time_classical
             << fixed << setprecision(5) << setw(20) << time_blocked
             << fixed << setprecision(2) << speedup << "x" << endl;
    }*/

    vector<int> block_sizes_to_test = {8, 16, 32, 64, 128, 256, 512};
    evaluateBSize(200, block_sizes_to_test);

    return 0;
}