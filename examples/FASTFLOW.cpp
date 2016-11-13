#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

typedef long long ll;
const ll LLINF = (1ull << 53) - 1;
const int INF = 1000 * 1000 * 1000;

const int MAXN = 5000;

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

int main() {
  int N, M;
  scanf("%d%d", &N, &M);
  Dinic D(N);
  for (int i = 0; i < M; ++i) {
    int u, v, c;
    scanf("%d%d%d", &u, &v, &c);
    --u, --v;
    D.add_edge(u, v, c);
    D.add_edge(v, u, c);
  }
  printf("%lld\n", D.maxflow(0, N - 1));
  return 0;
}
