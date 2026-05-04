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
