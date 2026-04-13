#include <iostream>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    int count = 0;

    for (int i = 1; i <= n; ++i) {
        string s = to_string(i);
        if (s.size() == 1) {
            count++;
        } else {
            int dif = (s[0] - '0') - (s[1] - '0');
            bool flag = true;
            for (int j = 1; j < (int)s.size(); ++j) {
                if ((s[j - 1] - '0') - (s[j] - '0') != dif) {
                    flag = false;
                    break;
                }
            }
            if (flag) count++;
        }
    }

    cout << count << "\n";
    return 0;
}
