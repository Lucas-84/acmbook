// Plus petit ancetre commun
// Pretraitement : O(n log n)
// Requete : O(log n)
// Memoire : O(n)
struct LCA {
  vector<vector<int>> adj;
  vector<vector<int>> prev;
  int h;
  vector<int> tin, tout;
  int timer;

  void dfs(int u, int p = 0) {
    tin[u] = ++timer;
    prev[u][0] = p;
    for (int i = 1; i < h; ++i)
      prev[u][i] = prev[prev[u][i - 1]][i - 1];
    for (int v: adj[u])
      if (v != p)
        dfs(v, u);
    tout[u] = ++timer;
  }

  // adj : adjacency list of an undirected acyclic graph 
  LCA(vector<vector<int>> adj, int root) {
    int n = (int)adj.size();
    this->adj = adj;
    for (h = 0; (1 << h) <= n; ++h)
      ;
    prev = vector<vector<int>>(n, vector<int>(h)); 
    timer = 0;
    tin = vector<int>(n);
    tout = vector<int>(n);
    dfs(root);
  }

  bool is_ancestor(int u, int v) {
    return tin[u] <= tin[v] && tout[v] <= tout[u];
  }

  int lca(int u, int v) {
    if (is_ancestor(u, v)) return u;
    if (is_ancestor(v, u)) return v;
    for (int i = h - 1; i >= 0; --i) {
      if (!is_ancestor(prev[u][i], v))
        u = prev[u][i];
    }
    return prev[u][0];
  }
};


