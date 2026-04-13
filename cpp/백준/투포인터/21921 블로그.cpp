#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, X;
    cin >> N >> X;
    vector<long long> income(N);
    for (int i = 0; i < N; ++i) cin >> income[i];

    long long current_sum = 0;
    for (int i = 0; i < X; ++i) current_sum += income[i];
    long long max_income = current_sum;
    int max_count = 1;

    for (int i = X; i < N; ++i) {
        current_sum += income[i] - income[i - X];
        if (current_sum == max_income) {
            max_count += 1;
        } else if (current_sum > max_income) {
            max_income = current_sum;
            max_count = 1;
        }
    }

    if (max_income == 0) {
        cout << "SAD" << "\n";
    } else {
        cout << max_income << "\n" << max_count << "\n";
    }
    return 0;
}
