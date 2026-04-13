#include <iostream>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    string base_string;
    cin >> base_string;

    for (int k = 0; k < n - 1; ++k) {
        string compare_string;
        cin >> compare_string;
        for (int i = 0; i < (int)base_string.size(); ++i) {
            if (base_string[i] != compare_string[i]) {
                base_string[i] = '?';
            }
        }
    }

    cout << base_string << "\n";
    return 0;
}
