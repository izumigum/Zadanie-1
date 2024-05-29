#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
private:
    int num_vertices;
    vector<vector<int>> adj_matrix;

public:
    void newgraph() {
        int* points;
        int kolpoints = 0;
        cout << "Количество точек графа: ";
        cin >> kolpoints;
        num_vertices = kolpoints;
        adj_matrix = vector<vector<int>>(num_vertices, vector<int>(num_vertices, 0));

        points = new int[kolpoints];
        for (int i = 0; i < kolpoints; i++) {
            points[i] = i;
            int k;
            cout << "Количество путей из точки " << i << ": ";
            cin >> k;
            for (int j = 0; j < k; j++) {
                int n;
                cout << "Точка, к которой идет путь: ";
                cin >> n;
                adj_matrix[i][n] = 1;
                adj_matrix[n][i] = 1; 
            }
        }
        delete[] points;
    }

    vector<int> bfs(int start) {
        vector<int> distances(num_vertices, -1);  // Не посещенные вершины
        queue<int> q;
        q.push(start);
        distances[start] = 0;  // Расстояние до самой себя равно 0

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            // Проверка всех соседей текущей вершины
            for (int neighbor = 0; neighbor < num_vertices; ++neighbor) {
                if (adj_matrix[current][neighbor] == 1 && distances[neighbor] == -1) {
                    q.push(neighbor);
                    distances[neighbor] = distances[current] + 1;
                }
            }
        }

        return distances;
    }

    int shortestPath(int start, int end) {
        vector<int> distances = bfs(start);
        return distances[end];
    }

    int shortestCycle() {
        int min_cycle_length = num_vertices + 1; 

        for (int start = 0; start < num_vertices; ++start) {
            vector<int> distances(num_vertices, -1);
            vector<int> parents(num_vertices, -1);
            queue<int> q;
            q.push(start);
            distances[start] = 0;

            while (!q.empty()) {
                int current = q.front();
                q.pop();

                for (int neighbor = 0; neighbor < num_vertices; ++neighbor) {
                    if (adj_matrix[current][neighbor] == 1) {
                        if (distances[neighbor] == -1) {
                            q.push(neighbor);
                            distances[neighbor] = distances[current] + 1;
                            parents[neighbor] = current;
                        }
                        else if (parents[current] != neighbor) {
                            int cycle_length = distances[current] + distances[neighbor] + 1;
                            if (cycle_length < min_cycle_length) {
                                min_cycle_length = cycle_length;
                            }
                        }
                    }
                }
            }
        }

        return (min_cycle_length > num_vertices) ? -1 : min_cycle_length;
    }

    vector<vector<int>> connectedComponents() {
        vector<bool> visited(num_vertices, false);
        vector<vector<int>> components;

        for (int start = 0; start < num_vertices; ++start) {
            if (!visited[start]) {
                vector<int> component;
                queue<int> q;
                q.push(start);
                visited[start] = true;

                while (!q.empty()) {
                    int current = q.front();
                    q.pop();
                    component.push_back(current);

                    for (int neighbor = 0; neighbor < num_vertices; ++neighbor) {
                        if (adj_matrix[current][neighbor] == 1 && !visited[neighbor]) {
                            q.push(neighbor);
                            visited[neighbor] = true;
                        }
                    }
                }

                components.push_back(component);
            }
        }

        return components;
    }

    // Метод для поиска точек сочленения
    vector<int> articulationPoints() {
        vector<int> ap;
        for (int u = 0; u < num_vertices; ++u) {
            // Удаляем вершину u и проверяем количество компонент связности
            vector<vector<int>> temp_adj_matrix = adj_matrix;
            for (int v = 0; v < num_vertices; ++v) {
                temp_adj_matrix[u][v] = 0;
                temp_adj_matrix[v][u] = 0;
            }

            vector<bool> visited(num_vertices, false);
            int components = 0;
            for (int v = 0; v < num_vertices; ++v) {
                if (v != u && !visited[v]) {
                    components++;
                    queue<int> q;
                    q.push(v);
                    visited[v] = true;

                    while (!q.empty()) {
                        int current = q.front();
                        q.pop();

                        for (int neighbor = 0; neighbor < num_vertices; ++neighbor) {
                            if (temp_adj_matrix[current][neighbor] == 1 && !visited[neighbor]) {
                                q.push(neighbor);
                                visited[neighbor] = true;
                            }
                        }
                    }
                }
            }

            // Если количество компонент больше 1, то вершина u - точка сочленения
            if (components > 1) {
                ap.push_back(u);
            }
        }

        return ap;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Graph g;
    g.newgraph();
    int a, b;

    cout << "Введите 2 точки, между которыми нужно узнать кратчайшее растояние: ";
    cin >> a >> b;
    cout << "Кратчайший путь от " << a << " до " << b <<  ": " << g.shortestPath(a, b) << endl;

    cout << "Кратчайший цикл в графе: " << g.shortestCycle() << endl;

    auto components = g.connectedComponents();
    cout << "Компоненты связности:" << endl;
    for (const auto& component : components) {
        for (int v : component) {
            cout << v << " ";
        }
        cout << endl;
    }

    auto articulation_points = g.articulationPoints();
    cout << "Точки сочленения в графе:" << endl;
    for (int point : articulation_points) {
        cout << point << " ";
    }
    cout << endl;

    return 0;
}
