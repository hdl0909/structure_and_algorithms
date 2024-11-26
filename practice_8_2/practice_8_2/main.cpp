#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int N, M;
vector<vector<pair<int, int>>> time_to;

int brute_force(int i, int j) {
    if (i == 0 && j == M - 1) {
        return 0; 
    }

    int up = INT_MAX, right = INT_MAX;

    if (i > 0) {
        up = time_to[i][j].first + brute_force(i - 1, j);
    }

    if (j < M - 1) {
        right = time_to[i][j].second + brute_force(i, j + 1);
    }

    return min(up, right);
}

int dynamic_programming() {
    vector<vector<int>> minTable(N, vector<int>(M, INT_MAX));

    minTable[N - 1][0] = 0;

    for (int i = N - 1; i >= 0; i--) {
        for (int j = 0; j < M; j++) {
            if (i < N - 1) {
                minTable[i][j] = min(minTable[i][j], minTable[i + 1][j] + time_to[i + 1][j].first);
            }

            if (j > 0) {
                minTable[i][j] = min(minTable[i][j], minTable[i][j - 1] + time_to[i][j - 1].second);
            }
        }
    }

    return minTable[0][M - 1];
}


int main() {
    setlocale(LC_ALL, "ru");
    cout << "Введите размер поля\n";
    cin >> N >> M;

    time_to = vector<vector<pair<int, int>>>(N, vector<pair<int, int>>(M));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                time_to[i][j].first = 0; 
            }
            else {
                cout << "Введите время движения вверх до клетки " << i << " " << j << endl;
                cin >> time_to[i][j].first;
            }

            if (j == M - 1) {
                time_to[i][j].second = 0;   
            }
            else {
                cout << "Введите время движения вправо до клетки " << i << " " << j << endl;
                cin >> time_to[i][j].second;
            }
        }
    }

    cout << "Поле: {вверх, вправо}\n";
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << "{" << time_to[i][j].first << ", " << time_to[i][j].second << "} ";
        }
        cout << endl;
    }

    cout << "Минимальное время с помощью грубой силы: " << brute_force(N - 1, 0) << endl;
    cout << "Минимальное время с помощью динамического программирования: " << dynamic_programming() << endl;

    return 0;
}
