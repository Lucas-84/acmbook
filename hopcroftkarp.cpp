// Hopcroft-Karp
// Max matching en O(|E| sqrt(|V|))
namespace HopcroftKarp {
  // Nombre max de noeuds
  const int MAXN = 10 * 1000;
  // assert(INF > MAXN)
  const int INF = 1000 * 1000 * 1000;
  vector<int> adj[MAXN + 1];

  const int NIL = MAXN;
  int pairu[MAXN + 1];
  int pairv[MAXN + 1];
  int dist[MAXN + 1];
  int N;

  bool bfs() {
    queue<int> q;

    for (int u = 0; u < N; ++u) {
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
        //break;
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

  int compute() {
    fill(pairu, pairu + N, NIL);
    fill(pairv, pairv + N, NIL);
    int ans = 0;
    while (bfs())
      for (int u = 0; u < N; ++u)
        if (pairu[u] == NIL && dfs(u))
          ++ans;
    return ans;
  }
}


