// Nombre max de noeuds
const int MAXN = 50 * 1000;
// assert(INF > MAXN)
const int INF = 1000 * 1000 * 1000;

// Hopcroft-Karp
// Max cardinality matching en O(|E| sqrt(|V|))
struct HopcroftKarp {
  const int NIL = MAXN;
  vector<int> adj[MAXN + 1];
  int pairu[MAXN + 1];
  int pairv[MAXN + 1];
  int dist[MAXN + 1];
  int nl, nr;

  HopcroftKarp() {}
  // nl : #noeuds a gauche
  // nr : #noeuds a droite
  HopcroftKarp(int nl, int nr) : nl(nl), nr(nr) {}

  void add_edge(int u, int v) {
    adj[u].push_back(v);
  }

  bool bfs() {
    queue<int> q;

    for (int u = 0; u < nl; ++u) {
      if (pairu[u] == NIL) {
        dist[u] = 0;
        q.push(u);
      } else {
        dist[u] = INF;
      }
    }

    dist[NIL] = INF;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      
      if (dist[u] >= dist[NIL])
        continue;

      for (int v: adj[u]) {
        if (dist[pairv[v]] != INF) continue;
        dist[pairv[v]] = dist[u] + 1;
        q.push(pairv[v]);
      }
    }

    return dist[NIL] != INF;
  }
  
  bool dfs(int u) {
    if (u == NIL)
      return true;

    for (int v: adj[u]) {
      if (dist[pairv[v]] == dist[u] + 1 && dfs(pairv[v])) {
        pairv[v] = u;
        pairu[u] = v;
        return true;
      }
    }

    dist[u] = INF;
    return false;
  }

  int maxmatching() {
    fill(pairu, pairu + nl, NIL);
    fill(pairv, pairv + nr, NIL);
    int ans = 0;
    while (bfs())
      for (int u = 0; u < nl; ++u)
        if (pairu[u] == NIL && dfs(u))
          ++ans;
    return ans;
  }
};

