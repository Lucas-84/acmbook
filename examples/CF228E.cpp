#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
const ll LLINF = (1ull << 53) - 1;
const int INF = 1000 * 1000 * 1000;

// Tri topologique
namespace Toposort {
  vector<bool> seen;
  vector<int> order;

  void dfs(const vector<vector<int>>& adj, int u) {
    seen[u] = true;
    for (int v: adj[u])
      if (!seen[v])
        dfs(adj, v);
    //printf("node %d => %d\n", u, (int)adj[u].size());
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

// CFC
struct SCC {
  // scc[u] : CFC du noeud u
  vector<int> scc;
  // sccadj[i] : liste d'adjacence de la i-eme CFC
  vector<vector<int>> sccgraph;
  int nb_scc;

  vector<bool> seen;
  vector<vector<int>> adj;
  vector<vector<int>> adjt;

  void mark_scc(int u, int idscc) {
    scc[u] = idscc;
    seen[u] = true;
    for (int v: adjt[u])
      if (!seen[v])
        mark_scc(v, idscc);
  }

  void create_scc_graph(int u) {
    seen[u] = true;
    for (int v: adj[u]) {
      if (scc[v] != scc[u])
        sccgraph[scc[u]].push_back(scc[v]);
      if (!seen[v])
        create_scc_graph(v);
    }
  }

  SCC(vector<vector<int>> adj) {
    int n = (int)adj.size();
    this->adj = adj;
    adjt = vector<vector<int>>(n, vector<int>(0));
    for (int u = 0; u < n; ++u)
      for (int v: adj[u])
        adjt[v].push_back(u);

    vector<int> order = Toposort::make(adj);
    //puts("end toposort");
    scc = vector<int>(n);
    nb_scc = 0;
    seen = vector<bool>(n);
    for (int t = n - 1; t >= 0; --t) {
      int u = order[t];
      //printf("ith-node: %d\n", u);
      if (!seen[u])
        mark_scc(u, nb_scc++);
    }

    //puts("DAG construction");
    // (optionnel) construction du DAG des CFC 
    sccgraph = vector<vector<int>>(nb_scc, vector<int>(0));
    fill(seen.begin(), seen.end(), false);
    for (int u = 0; u < n; ++u)
      if (!seen[u])
        create_scc_graph(u);
    //puts("end SCC");
  }
};

// 2-SAT
struct SAT2 {
  vector<bool> values;
  bool is_satisfiable;

  vector<int> order;
  vector<vector<int>> adj;
  int timer;

  // Formule a satisfaire :
  // (maxterms[0].fst ou maxterms[0].snd) et
  // (maxterms[1].fst ou maxterms[1].snd) et
  // ...
  // (maxterms[p].fst ou maxterms[p].snd)
  // litteral ::= 2*x pour x
  //              2*x+1 pour non(x)
  // 0 <= minterms[0].fst/snd < 2*m
  SAT2(vector<pair<int, int>> maxterms, int m) {
    int n = (int)maxterms.size();
    adj = vector<vector<int>>(2 * m, vector<int>(0));

    for (int i = 0; i < n; ++i) {
      adj[maxterms[i].first ^ 1].push_back(maxterms[i].second);
      adj[maxterms[i].second ^ 1].push_back(maxterms[i].first);
      //printf("node %d %d\n", maxterms[i].first^1, maxterms[i].second);
      //printf("node %d %d\n", maxterms[i].second^1, maxterms[i].first);
    }

    //puts("start SCC");
    SCC scc(adj);
    is_satisfiable = true;
    for (int u = 0; u < 2 * m; u += 2)
      if (scc.scc[u] == scc.scc[u + 1])
        is_satisfiable = false;

    //puts("mini end");
    if (is_satisfiable) {
      //puts("satisfiaible!");
      values = vector<bool>(m);
      for (int u = 0; u < m; ++u)
        values[u] = scc.scc[2 * u] > scc.scc[2 * u + 1];
    }
    //puts("--- end ----");
  }
};

int main() {
  int n, m;
  scanf("%d%d", &n, &m);
  vector<pair<int, int>> maxterms;
  for (int i = 0; i < m; ++i) {
    int u, v, c;
    scanf("%d%d%d", &u, &v, &c);
    --u, --v;
    if (c) {
      maxterms.push_back({2 * u, 2 * v + 1});
      maxterms.push_back({2 * v, 2 * u + 1});
    } else {
      maxterms.push_back({2 * u, 2 * v});
      maxterms.push_back({2 * u + 1, 2 * v + 1});
    }
  }
  SAT2 sat2(maxterms, n);
  if (!sat2.is_satisfiable) {
    puts("Impossible");
    return 0;
  }
  vector<int> ans;
  for (int u = 0; u < n; ++u)
    if (sat2.values[u])
      ans.push_back(u);
  printf("%d\n", (int)ans.size());
  for (int u: ans)
    printf("%d ", u + 1);
  puts("");
  return 0;
}
