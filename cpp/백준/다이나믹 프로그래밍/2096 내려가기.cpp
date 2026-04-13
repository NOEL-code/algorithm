#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    int a, b, c;
    cin >> a >> b >> c;
    int max_a = a, max_b = b, max_c = c;
    int min_a = a, min_b = b, min_c = c;

    for (int i = 0; i < N - 1; ++i) {
        cin >> a >> b >> c;

        int new_max_a = max(max_a, max_b) + a;
        int new_max_b = max({max_a, max_b, max_c}) + b;
        int new_max_c = max(max_b, max_c) + c;

        int new_min_a = min(min_a, min_b) + a;
        int new_min_b = min({min_a, min_b, min_c}) + b;
        int new_min_c = min(min_b, min_c) + c;

        max_a = new_max_a; max_b = new_max_b; max_c = new_max_c;
        min_a = new_min_a; min_b = new_min_b; min_c = new_min_c;
    }

    cout << max({max_a, max_b, max_c}) << " " << min({min_a, min_b, min_c}) << "\n";
    return 0;
}
