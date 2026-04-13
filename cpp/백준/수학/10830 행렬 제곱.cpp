#include <iostream>
#include <vector>
using namespace std;

using Matrix = vector<vector<long long>>;

int N;
const long long MOD = 1000;

Matrix multiply(const Matrix& A, const Matrix& B) {
    Matrix C(N, vector<long long>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            long long sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum % MOD;
        }
    }
    return C;
}

Matrix power(Matrix A, long long b) {
    Matrix result(N, vector<long long>(N, 0));
    for (int i = 0; i < N; ++i) result[i][i] = 1;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            A[i][j] %= MOD;

    while (b > 0) {
        if (b & 1) result = multiply(result, A);
        A = multiply(A, A);
        b >>= 1;
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long B;
    cin >> N >> B;

    Matrix A(N, vector<long long>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> A[i][j];

    Matrix result = power(A, B);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << result[i][j];
            if (j + 1 < N) cout << ' ';
        }
        cout << "\n";
    }
    return 0;
}
