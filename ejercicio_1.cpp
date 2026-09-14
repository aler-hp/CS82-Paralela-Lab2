#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace std;

double medirTiempo(void (*func)(const vector<vector<double>>&, const vector<double>&, vector<double>&),
                   const vector<vector<double>>& A, const vector<double>& x, vector<double>& y) {
    auto start = chrono::high_resolution_clock::now();

    func(A, x, y);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    return duration.count();
}

void filaColumna(const vector<vector<double>>& A, const vector<double>& x, vector<double>& y) {
    int n = A.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

void columnaFila(const vector<vector<double>>& A, const vector<double>& x, vector<double>& y) {
    int n = A.size();
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            y[i] += A[i][j] * x[j];
        }
    }
}

int main() {

    vector<int> tamanos = {1000, 2000, 4000, 8000, 10000};

    cout << setw(10) << "Tamano" << setw(28) << "Tiempo Fila x Fila" << setw(30) << "Tiempo Columna x Columna" << endl;
    cout << string(70, '-') << endl;

    for (int MAX : tamanos) {

        vector<double> x(MAX, 1.0);
        vector<double> y1(MAX, 0.0); //1er bucle
        vector<double> y2(MAX, 0.0); // clean slate para el 2do bucle
        vector<vector<double>> A(MAX, vector<double>(MAX, 1.0));

        double tiempo1 = medirTiempo(filaColumna, A, x, y1);
        double tiempo2 = medirTiempo(columnaFila, A, x, y2);

        cout << setw(10) << MAX
             << setw(22) << tiempo1 << " s"
             << setw(26) << tiempo2 << " s" << endl;
    }

    return 0;
}