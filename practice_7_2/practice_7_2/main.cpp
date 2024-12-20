#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <stack>
#include <map>

using namespace std;

struct Graph {
    int cnt_node;  
    vector<vector<int>> matrix;  
    bool isOrientation = false;
    map<int, char> symbol{
        {0, 'a'}, {1, 'b'},{2, 'c'}, {3, 'd'}, {4, 'f'}, {5, 'g'}, {6, 'h'}, {7, 'i'}, {8, 'j'}, {9, 'k'} // конкретно для варианта
    };
    
    Graph() : cnt_node(10), matrix(10, vector<int>(10, 0)) {
        matrix = {
            {0, 1, 0, 0, 1, 0, 0, 0, 0, 0},
            {1, 0, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 1, 0, 1, 0, 0, 0, 0, 1, 0},
            {0, 1, 1, 0, 0, 0, 0, 1, 1, 0},
            {1, 1, 0, 0, 0, 1, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 1, 0, 0, 0},
            {0, 0, 0, 0, 1, 1, 0, 1, 0, 1},
            {0, 0, 0, 1, 0, 0, 1, 0, 1, 1},
            {0, 0, 1, 1, 0, 0, 0, 1, 0, 0},
            {0, 0, 0, 0, 0, 0, 1, 1, 0, 0}
        };
    }

    Graph(int cnt_node) : cnt_node(cnt_node), matrix(cnt_node, vector<int>(cnt_node, 0)) {}

    void inputGraph() {
        cout << "Введите для каждого узла смежные узлы (0 если нет рёбер, 1 если есть):\n";
        for (int i = 0; i < cnt_node; i++) {
            for (int j = 0; j < cnt_node; j++) {
                if (i == j) {
                    continue;
                }
                else {
                    cout << "Есть ли ребро между вершинами " << i << " и " << j << endl;
                    cin >> matrix[i][j];
                }
            }
        }
    }

    void printGraph() {
        cout << "Матрица смежности графа:\n";
        for (int i = 0; i < cnt_node; ++i) {
            cout << "Узел " << i << ": ";
            for (int j = 0; j < cnt_node; ++j) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void checkIsOrientation() {
        for (int i = 0; i < cnt_node; i++) {
            for (int j = 0; j < cnt_node; j++) {
                if (i == j) continue;
                else if (matrix[i][j] != matrix[j][i]) {
                    isOrientation = true;
                    return;
                }
            }
        }
    }

    vector<int> BFS(int start, vector<vector<int>>& graph) {
        vector<bool> visited(cnt_node, false);
        queue<int> q;

        vector<int> result;

        visited[start] = true;
        q.push(start);

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            result.push_back(node);

            for (int i = 0; i < cnt_node; i++) {
                if (graph[node][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }

        return result;
    }

    bool checkNoOrientationGraph() {
        // 1 условие
        vector<int> check_result;
        check_result = BFS(0, matrix);
        for (int i = 0; i < cnt_node; i++) {
            auto item = find(check_result.begin(), check_result.end(), i);
            if (item == check_result.end()) return false;
        }
        // 2 условие
        int cnt_vertices = 0;
        for (int i = 0; i < cnt_node; i++) {
            int cnt_edges = 0;
            for (int j = 0; j < cnt_node; j++) {
                if (matrix[i][j] == 1) cnt_edges++;
            }
            if (cnt_edges % 2 != 0) cnt_vertices++;
            cnt_edges = 0;
        }
        if (cnt_vertices != 2 && cnt_vertices != 0) {
            return false;
        }

        return true;
    }

    bool checkOrientationGraph() {
        // 1 условие
        for (int i = 0; i < cnt_node; i++) {
            for (int j = 0; j < cnt_node; j++) {
                if (i == j) continue;
                if (matrix[i][j] != 1 && matrix[j][i] != 1) {
                    return false;
                }
            }
        }
        // 2 условие
        bool start_node = false;
        bool end_node = false;
        for (int i = 0; i < cnt_node; i++) {
            int cnt_edges_in = 0;
            int cnt_edges_out = 0;
            for (int j = 0; j < cnt_node; j++) {
                if (matrix[i][j] == 1) {
                    cnt_edges_out++;
                }
                else if (matrix[j][i] == 1) {
                    cnt_edges_in++;
                }
            }
            if (cnt_edges_out - cnt_edges_in == 1) {
                if (!start_node) start_node = true;
                else return false;
            }
            else if (cnt_edges_in - cnt_edges_out == 1) {
                if (!end_node) start_node = true;
                else return false;
            }
        }
        return start_node == end_node;
    }

    bool isBridge(int u, int v, vector<vector<int>>& graph, bool isOrientation) {
        int start = 0;
        while (start < cnt_node && (start == u || start == v || !count(graph[start].begin(), graph[start].end(), 1)))
            start++;

        if (start == cnt_node) return false;

        int count_before = BFS(start, graph).size();
        graph[u][v] = 0;
        if (!isOrientation) graph[v][u] = 0;

        int count_after = BFS(start, graph).size();

        graph[u][v] = 1;
        if (!isOrientation) graph[v][u] = 1;

        return count_before > count_after;
    }

    void EulerWay() {
        if (isOrientation) {
            if (checkOrientationGraph()) {
                vector<int> cnt_edges_in(cnt_node, 0);
                vector<int> cnt_edges_out(cnt_node, 0);

                vector<vector<int>> copy_matrix = matrix;
                stack<int> path;
                vector<int> result;

                // Ищем начальную вершину
                int start_node = -1;
                for (int i = 0; i < cnt_node; i++) {
                    for (int j = 0; j < cnt_node; j++) {
                        if (matrix[i][j] == 1) {
                            cnt_edges_out[i]++;
                            cnt_edges_in[j]++;
                        }
                        else if (matrix[j][i] == 1) {
                            cnt_edges_out[j]++;
                            cnt_edges_in[i]++;
                        }
                    }
                    if (cnt_edges_out[i] - cnt_edges_in[i] == 1) {
                        start_node = i;
                        break;
                    }
                }
                
                if (start_node == -1) start_node = 0;

                path.push(start_node);

                while (!path.empty()) {
                    int current = path.top();
                    bool found = false;

                    for (int i = 0; i < cnt_node; i++) {
                        if (copy_matrix[current][i] == 1) {
                            bool isOnlyEdge = true;
                            for (int j = 0; j < cnt_node; j++) {
                                if (j != i && copy_matrix[current][j] == 1) {
                                    isOnlyEdge = false;
                                    break;
                                }
                            }

                            if (!isOnlyEdge && isBridge(current, i, copy_matrix, isOrientation)) {
                                continue;
                            }

                            copy_matrix[current][i] = 0;
                            path.push(i);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        result.push_back(current);
                        path.pop();
                    }
                }

                cout << "Эйлеров путь: ";
                for (int i = result.size() - 1; i >= 0; i--) {
                    cout << symbol[result[i]] << " ";
                }
                cout << endl;
            }
            else {
                cout << "Эйлеров путь не существует\n";
            }
        }
        else {
            if (checkNoOrientationGraph()) {
                vector<vector<int>> copy_matrix = matrix;
                stack<int> path;
                vector<int> result;

                // Ищем начальную вершину
                int start_node = -1;
                for (int i = 0; i < cnt_node; i++) {
                    int cnt_edges = 0;
                    for (int j = 0; j < cnt_node; j++) {
                        if (matrix[i][j] == 1) cnt_edges++;
                    }
                    if (cnt_edges % 2 != 0) {
                        start_node = i;
                        break;
                    }
                }
                if (start_node == -1) start_node = 0; 

                path.push(start_node);

                while (!path.empty()) {
                    int current = path.top();
                    bool found = false;

                    for (int i = 0; i < cnt_node; i++) {
                        if (copy_matrix[current][i] == 1) {
                            bool isOnlyEdge = true;
                            for (int j = 0; j < cnt_node; j++) {
                                if (j != i && copy_matrix[current][j] == 1) {
                                    isOnlyEdge = false;
                                    break;
                                }
                            }

                            if (!isOnlyEdge && isBridge(current, i, copy_matrix, isOrientation)) {
                                continue;
                            }

                            copy_matrix[current][i] = 0;
                            copy_matrix[i][current] = 0;
                            path.push(i);
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        result.push_back(current);
                        path.pop();
                    }
                }

                cout << "Эйлеров путь: ";
                for (int i = result.size() - 1; i >= 0; i--) {
                    cout << symbol[result[i]] << " ";
                }
                cout << endl;
            }
            else {
                cout << "Эйлеров путь не существует\n";
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Graph neor_graph;
    neor_graph.printGraph();  
    neor_graph.checkIsOrientation();
    neor_graph.EulerWay();

    cout << "Обход в ширину для неориентированного графа: ";
    vector<int> bfs_neorGraph = neor_graph.BFS(0, neor_graph.matrix);
    for (int i = 0; i < neor_graph.cnt_node; i++) {
        cout << bfs_neorGraph[i] << " ";
    }
    cout << "\n\n";

    cout << "Введите количество вершин для ориентированного графа: ";
    int cnt_node;
    cin >> cnt_node;

    Graph or_graph(cnt_node);
    or_graph.inputGraph();
    or_graph.printGraph();
    or_graph.checkIsOrientation();
    or_graph.EulerWay();

    cout << "Обход в ширину для ориентированного графа: ";
    vector<int> bfs_orGraph = or_graph.BFS(0, or_graph.matrix);
    for (int i = 0; i < or_graph.cnt_node; i++) {
        cout << bfs_orGraph[i] << " ";
    }
    return 0;
}
