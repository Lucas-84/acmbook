// Nombre max de noeuds
const int MAXN = 10 * 1000;
// assert(INF > maxflow)
const ll INF = 1ll << 53;

// Dinic 
// Flot max en O(V^2 * E)
struct Dinic {
  struct Edge { int u, v; ll cap, flow; };

  vector<int> adj[MAXN];
  vector<Edge> edges;
  int dist[MAXN];
  int idnext[MAXN];
  int n;

  Dinic() {}
  Dinic(int n) : n(n) {}

  // ajoute l'arete u -> v de capacite c
  void add_edge(int u, int v, int c) {
    edges.push_back({u, v, c, 0});
    adj[u].push_back((int)edges.size() - 1);
    edges.push_back({v, u, 0, 0});
    adj[v].push_back((int)edges.size() - 1);
  }

  bool bfs(int s, int t) {
    queue<int> q; 
    fill(dist, dist + n, -1);
    dist[s] = 0;
    q.push(s);

    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int e: adj[u]) {
        int v = edges[e].v;
        if (dist[v] == -1 && edges[e].flow < edges[e].cap) {
          dist[v] = dist[u] + 1;
          q.push(v);
        }
      }
    }

    return dist[t] != -1;
  }

  int dfs(int u, int t, ll flow) {
    if (flow == 0) return 0;
    if (u == t)    return flow;

    for (; idnext[u] < (int)adj[u].size(); ++idnext[u]) {
      int e = adj[u][idnext[u]];
      int v = edges[e].v;
      if (dist[v] != dist[u] + 1) continue;
      ll pushed = dfs(v, t, min(flow, edges[e].cap - edges[e].flow));
      if (pushed > 0) {
        edges[e].flow += pushed;
        edges[e ^ 1].flow -= pushed;
        return pushed;
      }
    }

    return 0;
  }

  // maxflow entre s et t
  ll maxflow(int s, int t) {
    ll ans = 0;
    while (bfs(s, t)) {
      fill(idnext, idnext + n, 0);
      ll pushed = 0;
      while ((pushed = dfs(s, t, INF)) > 0)
        ans += pushed;
    }
    return ans;
  }
};
