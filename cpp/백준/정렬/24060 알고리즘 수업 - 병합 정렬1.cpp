#include <iostream>
#include <vector>
using namespace std;

int K;
long long count_ = 0;
long long result = -1;
vector<int> arr;
vector<int> tmp;

void merge(int left, int mid, int right) {
    int i = left;
    int j = mid + 1;
    int t = left;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            tmp[t] = arr[i];
            i += 1;
        } else {
            tmp[t] = arr[j];
            j += 1;
        }
        count_ += 1;
        if (count_ == K) result = tmp[t];
        t += 1;
    }

    while (i <= mid) {
        tmp[t] = arr[i];
        i += 1;
        count_ += 1;
        if (count_ == K) result = tmp[t];
        t += 1;
    }

    while (j <= right) {
        tmp[t] = arr[j];
        j += 1;
        count_ += 1;
        if (count_ == K) result = tmp[t];
        t += 1;
    }

    for (int k = left; k <= right; ++k) {
        arr[k] = tmp[k];
    }
}

void merge_sort(int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;
        merge_sort(left, mid);
        merge_sort(mid + 1, right);
        merge(left, mid, right);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N >> K;

    arr.assign(N, 0);
    tmp.assign(N, 0);
    for (int i = 0; i < N; ++i) cin >> arr[i];

    merge_sort(0, N - 1);

    cout << result << "\n";
    return 0;
}
