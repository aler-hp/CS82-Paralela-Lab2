#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

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

int main(int argc, char* argv[]) {
    // Validar argumentos de entrada
    if (argc < 3) {
        cerr << "Uso: " << argv[0] << " <clasica|bloques> <N> [tamano_bloque]" << endl;
        return 1;
    }

    string mode = argv[1];
    int N = stoi(argv[2]);
    int b_size = (argc >= 4) ? stoi(argv[3]) : 64;

    // Inicialización de matrices
    vector<vector<double>> A(N, vector<double>(N, 1.5));
    vector<vector<double>> B(N, vector<double>(N, 2.0));
    vector<vector<double>> C(N, vector<double>(N, 0.0));

    // Ejecución aislada según el parámetro
    if (mode == "clasica") {
        cout << "Ejecutando multiplicacion clasica (N=" << N << ")" << endl;
        multiplicationClasica(A, B, C, N);
    } 
    else if (mode == "bloques") {
        cout << "Ejecutando multiplicacion por bloques (N=" << N << ", b_size=" << b_size << ")" << endl;
        multiplicacionBloques(A, B, C, N, b_size);
    } 
    else {
        cerr << "Error: Modo no reconocido. Usa 'clasica' o 'bloques'." << endl;
        return 1;
    }

    return 0;
}