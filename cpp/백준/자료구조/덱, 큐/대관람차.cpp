#include <iostream>
#include <deque>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int room_size = 4;
    deque<int> person = {2, 3, 1, 5};

    int time_elapsed = 0;

    deque<int> queue;
    queue.push_back(person.front());
    person.pop_front();
    for (int i = 0; i < room_size - 1; ++i) queue.push_back(0);

    deque<int> waiting_list(person.begin(), person.end());

    while (!queue.empty()) {
        time_elapsed += 1;

        for (int i = 0; i < (int)queue.size(); ++i) queue[i] -= 1;

        if (queue.front() <= 0) queue.pop_front();

        if (!waiting_list.empty() && (int)queue.size() < room_size) {
            queue.push_back(waiting_list.front());
            waiting_list.pop_front();
        }

        if (!queue.empty()) {
            queue.push_back(queue.front());
            queue.pop_front();
        }
    }

    cout << time_elapsed << "\n";
    return 0;
}
