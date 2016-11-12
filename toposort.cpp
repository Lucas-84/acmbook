// Tri topologique
// O(|E|)
namespace Toposort {
  vector<bool> seen;
  vector<int> order;

  void dfs(const vector<vector<int>>& adj, int u) {
    seen[u] = true;
    for (int v: adj[u])
      if (!seen[v])
        dfs(adj, v);
    order.push_back(u);
  }

  vector<int> make(const vector<vector<int>> adj) {
    int n = (int)adj.size();
    seen = vector<bool>(n);
    order = vector<int>(0);
    for (int u = 0; u < n; ++u)
      if (!seen[u])
        dfs(adj, u);
    reverse(order.begin(), order.end());
    return order;
  }
};


