# 41343110 and 41343153
# Homework 2 Graph
# Problem

## 解題說明
本題要求實作一個Graph類別，支援無向圖與有向圖，並整合遍歷、生成樹、最短路徑及網路排程等核心演算法。

在資料儲存上，使用相鄰串列（Adjacency List） 搭配 邊列表（Edge List） 的雙重結構，使 DFS/BFS 遍歷能快速存取鄰居
，同時支援 Kruskal 演算法對邊進行全域排序。

此外，透過 互動式命令列介面（CLI） 模組化各演算法指令，讓使用者能針對同一圖形資料進行多維度的分析與交叉驗證，不需要每次為了測試新功能就重新編譯，這在除錯時非常有幫助。

## 程式實作
```cpp
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

using namespace std;

static const long long INF = (1LL << 60);

struct Edge {
    int to;
    long long w;
};

struct UEdge {
    int u, v;
    long long w;
};

class Graph {
public:
    int n;
    bool directed;
    vector<vector<Edge>> adj;
    vector<UEdge> edges; // one per input line

    Graph(int n_ = 0, bool directed_ = false) : n(n_), directed(directed_), adj(n_) {}

    void addEdge(int u, int v, long long w) {
        adj[u].push_back({ v, w });
        edges.push_back({ u, v, w });
        if (!directed) adj[v].push_back({ u, w });
    }

    bool hasNegativeEdge() const {
        for (int i = 0; i < (int)edges.size(); i++)
            if (edges[i].w < 0) return true;
        return false;
    }

    // ---------- DFS (iterative) ----------
    vector<int> dfsOrder(int s) const {
        vector<int> order;
        if (s < 0 || s >= n) return order;

        vector<char> vis(n, false);
        vector<int> st;
        st.push_back(s);

        while (!st.empty()) {
            int u = st.back();
            st.pop_back();
            if (vis[u]) continue;
            vis[u] = true;
            order.push_back(u);

            // push neighbors in reverse so traversal resembles recursive order
            for (int i = (int)adj[u].size() - 1; i >= 0; i--) {
                int v = adj[u][i].to;
                if (!vis[v]) st.push_back(v);
            }
        }
        return order;
    }

    // ---------- BFS (vector queue) ----------
    vector<int> bfsOrder(int s) const {
        vector<int> order;
        if (s < 0 || s >= n) return order;

        vector<char> vis(n, false);
        vector<int> q;
        q.push_back(s);
        vis[s] = true;

        int head = 0;
        while (head < (int)q.size()) {
            int u = q[head++];
            order.push_back(u);
            for (int i = 0; i < (int)adj[u].size(); i++) {
                int v = adj[u][i].to;
                if (!vis[v]) {
                    vis[v] = true;
                    q.push_back(v);
                }
            }
        }
        return order;
    }

    // ---------- Connected components (undirected) ----------
    vector<vector<int>> connectedComponentsUndirected() const {
        vector<vector<int>> comps;
        if (directed) return comps;

        vector<char> vis(n, false);
        for (int i = 0; i < n; i++) if (!vis[i]) {
            vector<int> comp;
            vector<int> st;
            st.push_back(i);
            vis[i] = true;

            while (!st.empty()) {
                int u = st.back(); st.pop_back();
                comp.push_back(u);
                for (int k = 0; k < (int)adj[u].size(); k++) {
                    int v = adj[u][k].to;
                    if (!vis[v]) {
                        vis[v] = true;
                        st.push_back(v);
                    }
                }
            }

            sort(comp.begin(), comp.end());
            comps.push_back(comp);
        }
        return comps;
    }

    // ---------- MST (undirected) ----------
    pair<long long, vector<pair<int, int>>> mstKruskalUndirected() const {
        if (directed) return { 0, {} };

        vector<UEdge> es = edges;
        sort(es.begin(), es.end(), [](const UEdge& a, const UEdge& b) {
            if (a.w != b.w) return a.w < b.w;
            if (a.u != b.u) return a.u < b.u;
            return a.v < b.v;
            });

        vector<int> p(n), r(n, 0);
        iota(p.begin(), p.end(), 0);

        auto findp = [&](int x) {
            while (p[x] != x) {
                p[x] = p[p[x]];
                x = p[x];
            }
            return x;
            };

        auto unite = [&](int a, int b) {
            a = findp(a);
            b = findp(b);
            if (a == b) return false;
            if (r[a] < r[b]) swap(a, b);
            p[b] = a;
            if (r[a] == r[b]) r[a]++;
            return true;
            };

        long long total = 0;
        vector<pair<int, int>> picked;
        for (int i = 0; i < (int)es.size(); i++) {
            int u = es[i].u, v = es[i].v;
            if (unite(u, v)) {
                total += es[i].w;
                picked.push_back({ u,v });
            }
        }
        return { total, picked };
    }

    // Prim O(n^2) (undirected)
    pair<long long, vector<pair<int, int>>> mstPrimUndirected(int s) const {
        if (directed || s < 0 || s >= n) return { 0, {} };

        vector<long long> key(n, INF);
        vector<int> parent(n, -1);
        vector<char> inMST(n, false);

        key[s] = 0;
        for (int iter = 0; iter < n; iter++) {
            int u = -1;
            long long best = INF;
            for (int i = 0; i < n; i++) {
                if (!inMST[i] && key[i] < best) {
                    best = key[i];
                    u = i;
                }
            }
            if (u == -1) break;
            inMST[u] = true;

            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                long long w = adj[u][k].w;
                if (!inMST[v] && w < key[v]) {
                    key[v] = w;
                    parent[v] = u;
                }
            }
        }

        long long total = 0;
        vector<pair<int, int>> picked;
        for (int v = 0; v < n; v++) {
            if (v == s) continue;
            if (parent[v] != -1) {
                picked.push_back({ parent[v], v });
                total += key[v];
            }
        }
        return { total, picked };
    }

    // ---------- Shortest paths ----------
    // Dijkstra O(n^2) (requires no negative edges)
    vector<long long> dijkstra(int s) const {
        vector<long long> dist(n, INF);
        if (s < 0 || s >= n) return dist;

        vector<char> used(n, false);
        dist[s] = 0;

        for (int iter = 0; iter < n; iter++) {
            int u = -1;
            long long best = INF;
            for (int i = 0; i < n; i++) {
                if (!used[i] && dist[i] < best) {
                    best = dist[i];
                    u = i;
                }
            }
            if (u == -1) break;
            used[u] = true;

            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                long long w = adj[u][k].w;
                if (dist[u] + w < dist[v]) dist[v] = dist[u] + w;
            }
        }
        return dist;
    }

    // Bellman-Ford (detect neg cycle reachable)
    pair<bool, vector<long long>> bellmanFord(int s) const {
        vector<long long> dist(n, INF);
        if (s < 0 || s >= n) return { false, dist };
        dist[s] = 0;

        auto relax = [&](int u, int v, long long w, bool& changed) {
            if (dist[u] >= INF / 2) return;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                changed = true;
            }
            };

        for (int i = 0; i < n - 1; i++) {
            bool changed = false;
            for (int k = 0; k < (int)edges.size(); k++) {
                relax(edges[k].u, edges[k].v, edges[k].w, changed);
                if (!directed) relax(edges[k].v, edges[k].u, edges[k].w, changed);
            }
            if (!changed) break;
        }

        for (int k = 0; k < (int)edges.size(); k++) {
            int u = edges[k].u, v = edges[k].v;
            long long w = edges[k].w;
            if (dist[u] < INF / 2 && dist[u] + w < dist[v]) return { true, dist };
            if (!directed && dist[v] < INF / 2 && dist[v] + w < dist[u]) return { true, dist };
        }
        return { false, dist };
    }

    // Floyd-Warshall
    pair<vector<vector<long long>>, bool> floydWarshall() const {
        vector<vector<long long>> d(n, vector<long long>(n, INF));
        for (int i = 0; i < n; i++) d[i][i] = 0;

        for (int u = 0; u < n; u++) {
            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                d[u][v] = min(d[u][v], adj[u][k].w);
            }
        }

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) if (d[i][k] < INF / 2) {
                for (int j = 0; j < n; j++) if (d[k][j] < INF / 2) {
                    long long nd = d[i][k] + d[k][j];
                    if (nd < d[i][j]) d[i][j] = nd;
                }
            }
        }

        bool negCycle = false;
        for (int i = 0; i < n; i++) if (d[i][i] < 0) negCycle = true;
        return { d, negCycle };
    }

    // Transitive closure
    vector<vector<int>> transitiveClosure() const {
        vector<vector<int>> reach(n, vector<int>(n, 0));
        for (int i = 0; i < n; i++) reach[i][i] = 1;
        for (int u = 0; u < n; u++)
            for (int k = 0; k < (int)adj[u].size(); k++)
                reach[u][adj[u][k].to] = 1;

        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) if (reach[i][k]) {
                for (int j = 0; j < n; j++)
                    reach[i][j] = reach[i][j] || (reach[i][k] && reach[k][j]);
            }
        }
        return reach;
    }

    // AOV topo sort (directed) using vector-queue
    pair<bool, vector<int>> topoSortAOV() const {
        if (!directed) return { false, {} };

        vector<int> indeg(n, 0);
        for (int u = 0; u < n; u++)
            for (int k = 0; k < (int)adj[u].size(); k++)
                indeg[adj[u][k].to]++;

        vector<int> q;
        for (int i = 0; i < n; i++) if (indeg[i] == 0) q.push_back(i);

        int head = 0;
        vector<int> order;
        while (head < (int)q.size()) {
            int u = q[head++];
            order.push_back(u);
            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                indeg[v]--;
                if (indeg[v] == 0) q.push_back(v);
            }
        }
        bool ok = ((int)order.size() == n);
        return { ok, order };
    }

    // AOE critical path (directed DAG)
    struct AOECPResult {
        bool ok;
        long long projectDuration;
        vector<long long> ve, vl;
        vector<UEdge> criticalEdges; // store as (u,v,w)
        vector<int> topo;
    };

    AOECPResult criticalPathAOE() const {
        AOECPResult res;
        res.ok = false;
        res.projectDuration = 0;
        if (!directed) return res;

        auto topoPair = topoSortAOV();
        bool ok = topoPair.first;
        vector<int> topo = topoPair.second;
        res.topo = topo;
        if (!ok) return res;

        vector<long long> ve(n, 0);
        for (int idx = 0; idx < (int)topo.size(); idx++) {
            int u = topo[idx];
            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                long long w = adj[u][k].w;
                ve[v] = max(ve[v], ve[u] + w);
            }
        }

        long long duration = 0;
        for (int i = 0; i < n; i++) duration = max(duration, ve[i]);

        vector<long long> vl(n, duration);
        for (int idx = (int)topo.size() - 1; idx >= 0; idx--) {
            int u = topo[idx];
            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                long long w = adj[u][k].w;
                vl[u] = min(vl[u], vl[v] - w);
            }
        }

        vector<UEdge> critical;
        for (int u = 0; u < n; u++) {
            for (int k = 0; k < (int)adj[u].size(); k++) {
                int v = adj[u][k].to;
                long long w = adj[u][k].w;
                long long ee = ve[u];
                long long el = vl[v] - w;
                if (ee == el) critical.push_back({ u, v, w });
            }
        }

        res.ok = true;
        res.projectDuration = duration;
        res.ve = ve;
        res.vl = vl;
        res.criticalEdges = critical;
        return res;
    }
};

// ---------- Printing helpers ----------
static void printVectorInt(const vector<int>& v) {
    if (v.empty()) { cout << "(empty)\n"; return; }
    for (int i = 0; i < (int)v.size(); i++) {
        if (i) cout << ' ';
        cout << v[i];
    }
    cout << "\n";
}

static void printEdgesPairs(const vector<pair<int, int>>& es) {
    if (es.empty()) { cout << "(empty)\n"; return; }
    for (int i = 0; i < (int)es.size(); i++)
        cout << es[i].first << " " << es[i].second << "\n";
}

static void printDist(const vector<long long>& d) {
    for (int i = 0; i < (int)d.size(); i++) {
        cout << i << ": ";
        if (d[i] >= INF / 2) cout << "INF";
        else cout << d[i];
        cout << "\n";
    }
}

static void printMatrixLL(const vector<vector<long long>>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j) cout << ' ';
            if (a[i][j] >= INF / 2) cout << "INF";
            else cout << a[i][j];
        }
        cout << "\n";
    }
}

static void printMatrix01(const vector<vector<int>>& a) {
    int n = (int)a.size();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (j) cout << ' ';
            cout << a[i][j];
        }
        cout << "\n";
    }
}

static void help() {
    cout << "=== LIST ==="<<endl;
    cout << "  DFS s                     " << endl;
    cout << "  BFS s                     " << endl;
    cout << "  COMPONENTS                " << endl;
    cout << "  MST_KRUSKAL               " << endl;
    cout << "  MST_PRIM s                " << endl;
    cout << "  DIJKSTRA s                " << endl;
    cout << "  BELLMAN_FORD s            " << endl;
    cout << "  FLOYD_WARSHALL            " << endl;
    cout << "  TRANSITIVE_CLOSURE        " << endl;
    cout << "  AOV_TOPO                  " << endl;
    cout << "  AOE_CRITICAL              " << endl;
    cout << "  HELP                      " << endl;
    cout << "  EXIT                      " << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, directedInt;
    if (!(cin >> n >> m >> directedInt)) {
        cerr << "Failed to read: n m directed\n";
        return 1;
    }
    bool directed = (directedInt != 0);

    Graph g(n, directed);

    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cerr << "Edge endpoint out of range at line " << (i + 1) << "\n";
            return 1;
        }
        g.addEdge(u, v, w);
    }

    help();

    string cmd;
    while (cin >> cmd) {
        if (cmd == "EXIT") break;
        if (cmd == "HELP") { help(); continue; }

        if (cmd == "DFS") {
            int s; cin >> s;
            auto order = g.dfsOrder(s);
            cout << "DFS order: ";
            printVectorInt(order);

        }
        else if (cmd == "BFS") {
            int s; cin >> s;
            auto order = g.bfsOrder(s);
            cout << "BFS order: ";
            printVectorInt(order);

        }
        else if (cmd == "COMPONENTS") {
            if (g.directed) {
                cout << "COMPONENTS: only for undirected graphs.\n";
                continue;
            }
            auto comps = g.connectedComponentsUndirected();
            cout << "Connected components = " << comps.size() << "\n";
            for (int i = 0; i < (int)comps.size(); i++) {
                cout << "Component " << i << ": ";
                printVectorInt(comps[i]);
            }

        }
        else if (cmd == "MST_KRUSKAL") {
            if (g.directed) { cout << "MST_KRUSKAL: only for undirected graphs.\n"; continue; }
            auto ans = g.mstKruskalUndirected();
            cout << "Kruskal MST total weight: " << ans.first << "\n";
            cout << "Edges:\n";
            printEdgesPairs(ans.second);

        }
        else if (cmd == "MST_PRIM") {
            int s; cin >> s;
            if (g.directed) { cout << "MST_PRIM: only for undirected graphs.\n"; continue; }
            auto ans = g.mstPrimUndirected(s);
            cout << "Prim MST total weight: " << ans.first << "\n";
            cout << "Edges:\n";
            printEdgesPairs(ans.second);

        }
        else if (cmd == "DIJKSTRA") {
            int s; cin >> s;
            if (g.hasNegativeEdge()) {
                cout << "DIJKSTRA: graph has negative edge; not allowed.\n";
                continue;
            }
            auto dist = g.dijkstra(s);
            cout << "Dijkstra distances from " << s << ":\n";
            printDist(dist);

        }
        else if (cmd == "BELLMAN_FORD") {
            int s; cin >> s;
            auto ans = g.bellmanFord(s);
            if (ans.first) {
                cout << "Bellman-Ford: negative cycle reachable from source " << s << ". Distances invalid.\n";
            }
            else {
                cout << "Bellman-Ford distances from " << s << ":\n";
                printDist(ans.second);
            }

        }
        else if (cmd == "FLOYD_WARSHALL") {
            auto ans = g.floydWarshall();
            if (ans.second) cout << "Floyd-Warshall: negative cycle exists (d[i][i] < 0).\n";
            cout << "All-pairs distances matrix:\n";
            printMatrixLL(ans.first);

        }
        else if (cmd == "TRANSITIVE_CLOSURE") {
            auto reach = g.transitiveClosure();
            cout << "Transitive closure (0/1):\n";
            printMatrix01(reach);

        }
        else if (cmd == "AOV_TOPO") {
            if (!g.directed) { cout << "AOV_TOPO: requires directed graph.\n"; continue; }
            auto ans = g.topoSortAOV();
            if (!ans.first) cout << "AOV_TOPO: graph has a cycle; no topo order.\n";
            else {
                cout << "Topological order:\n";
                printVectorInt(ans.second);
            }

        }
        else if (cmd == "AOE_CRITICAL") {
            if (!g.directed) { cout << "AOE_CRITICAL: requires directed graph.\n"; continue; }
            auto r = g.criticalPathAOE();
            if (!r.ok) {
                cout << "AOE_CRITICAL: graph is not a DAG.\n";
                continue;
            }
            cout << "Project duration: " << r.projectDuration << "\n";
            cout << "ve:\n";
            for (int i = 0; i < g.n; i++) cout << i << ": " << r.ve[i] << "\n";
            cout << "vl:\n";
            for (int i = 0; i < g.n; i++) cout << i << ": " << r.vl[i] << "\n";
            cout << "Critical edges (u v w):\n";
            if (r.criticalEdges.empty()) cout << "(empty)\n";
            for (int i = 0; i < (int)r.criticalEdges.size(); i++) {
                cout << r.criticalEdges[i].u << " " << r.criticalEdges[i].v << " " << r.criticalEdges[i].w << "\n";
            }

        }
        else {
            cout << "Unknown command: " << cmd << "\n";
            cout << "Type HELP.\n";
        }
    }

    return 0;
}
```
## 效能分析
#### 時間複雜度和空間複雜度
| 功能                  | 時間複雜度                  | 空間複雜度                                           |說明|
|-----------------------|-----------------------------|---------------------------|------------------------------------|
|圖形儲存|O(V + E)|O(V + E)|$O(V + E)$使用 adj 串列與 edges 向量雙重儲存|
|DFS / BFS|O(V + E)|O(V)|疊代式 DFS 避免了遞迴過深的問題|
|Kruskal (MST)|O(E \log E)|O(E)|效能瓶頸在於 std::sort 邊的權重|
|Prim / Dijkstra|O(V^2)|O(V)|採點搜尋而非優先權佇列，適合稠密圖|
|Bellman-Ford|O(V X E)|O(V)|可偵測負環，比 Dijkstra 適用範圍更廣|

## 測試與驗證
| 順序 | 輸入參數                |  預期輸出     | 說明                             | 
|----|-------------------------|----------------------------------------|----------------------------------------|
| 一 | 3 3 1 |   | 第一個 3：代表有 3 個點）。第二個 3：代表你接下來要輸入 3 條邊。第三個 1：代表這是 有向圖。 |
| 二 | 0 1 5 |   |點 0 → 點 1，權重（距離）是 5。|
| 三 | 1 2 -10 |      |點 1 → 點 2，權重是 -10（這是負權邊）。|
| 四 |0 2 2 |     |點 0 → 點 2，權重是 2。|
| 五 |      |=== LIST ===|      |
| 六 | | DFS s|從節點 s 開始進行「深度優先搜尋」。|
| 七 | |BFS s|從節點 s 開始進行「廣度優先搜尋」。|
| 八 | |COMPONENTS|找出圖中所有的「連通元件」。|
| 九 | |MST_KRUSKAL|使用 Kruskal 演算法找「最小生成樹」。|
| 十 | |MST_PRIM s|從點 s 開始，用 Prim 演算法找「最小生成樹」。|
| 十一 | | DIJKSTRA s|計算從點 s 到其他所有點的最短距離。|
| 十二| |BELLMAN_FORD s|同樣是算最短距離，但可以處理「負權邊」。|
| 十三 | |FLOYD_WARSHALL|計算「所有點對所有點」的最短路徑。|
| 十四| |TRANSITIVE_CLOSURE|計算「遞移閉包」。|  
| 十五 | |AOV_TOPO|進行「拓撲排序」。|
| 十六| |AOE_CRITICAL|找出「關鍵路徑」。|  
| 十七 | |HELP|重新顯示這份清單（救命清單）|
| 十八| |EXIT|關閉程式並退出。|    
| 十九| DFS s|DFS order: 0 1 2|從節點 s 開始進行「深度優先搜尋」|      
| 二十|按下enter| |關閉程式並退出|       
### 測試輸入
```
3 3 1
(第一行：3個點、3條邊、1為有向圖)
0 1 5
(第一條邊：0→1 權重 5)
1 2 -10
(第二條邊：1→2 權重 -10，此為負權邊)
0 2 2
(第三條邊：0→2 權重 2)
```
### 測試輸出
```
=== LIST ===
DFS s
BFS s
COMPONENTS
MST_KRUSKAL
MST_PRIM s
DIJKSTRA s
BELLMAN_FORD s
FLOYD_WARSHALL
TRANSITIVE_CLOSURE
AOV_TOPO
AOE_CRITICAL
HELP
EXIT

再次輸入:
DFS s
輸出:
DFS order: 0 1 2

再次輸入:
BFS s
輸出:
BFS order: 0 1 2

再次輸入:
MST_PRIM s
輸出:
MST_PRIM: only for undirected graphs.

再次輸入:
DIJKSTRA s
輸出:
DIJKSTRA: graph has negative edge; not allowed.

再次輸入:
BELLMAN_FORD s
輸出:
Bellman-Ford distances from 0:
0: 0
1: 5
2: -5


```
## 申論及開發報告
捨棄了遞迴 DFS，改用  Stack的疊代寫法。這是因為如果點的數量幾萬個，遞迴會讓系統堆疊爆掉導致當機。
在數值處理上，我把權重定為 long long，並計算了 INF 的大小，防止在運算時發生「數字溢位」變成負數的情況。
此外，類別設計上我把儲存跟演算法分開，未來還能加上其他功能。
### 結論
這個做完後有感的地方是，選對資料結構真的會讓寫程式輕鬆很多。之前看教材都覺得很抽象，但把相鄰串列建起來，並處理完正負權重和環的問題後，邏輯就變得非常清楚。
特別是為了 Kruskal 多存了一份 Edge List 雖然多佔了一點點記憶體，但排序起來真的方便很多。
