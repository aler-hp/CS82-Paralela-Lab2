#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

typedef void (*MatrixMultiplyFunc)(const vector<vector<double>>&,
                                  const vector<vector<double>>&,
                                  vector<vector<double>>&, int);


void multiplicacionClasica(const vector<vector<double>>& A,
                      const vector<vector<double>>& B,
                      vector<vector<double>>& C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            C[i][j] = 0.0;
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


double measureTime(MatrixMultiplyFunc func,
                            const vector<vector<double>>& A,
                            const vector<vector<double>>& B,
                            vector<vector<double>>& C, int n) {
    auto start = chrono::high_resolution_clock::now();

    func(A, B, C, n);

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = end - start;
    return duration.count();
}

void evaluate(MatrixMultiplyFunc func, const vector<int>& sizes) {
    cout << left << setw(15) << "Tamaño (N)" << setw(20) << "Tiempo (ms)" << endl;
    cout << "----------------------------------" << endl;

    for (int n : sizes) {
        vector<vector<double>> A(n, vector<double>(n, 2.0));
        vector<vector<double>> B(n, vector<double>(n, 2.0));
        vector<vector<double>> C(n, vector<double>(n, 0.0));

        cout << "Calculando N = " << n << "... " << flush;

        double elapsed_ms = measureTime(func, A, B, C, n);

        cout << fixed << setprecision(3) << elapsed_ms << " ms" << endl;
    }
}

int main() {
    vector<int> testSizes = {500, 1000, 1500, 2000, 2500};

    cout << "=== Evaluando Multiplicacion de Matrices (O(n^3)) ===\n\n";
    evaluate(multiplicacionClasica, testSizes);

    return 0;
}