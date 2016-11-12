// 2-SAT
// O(n + m)
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
    }

    SCC scc(adj);
    is_satisfiable = true;
    for (int u = 0; u < 2 * m; u += 2)
      if (scc.scc[u] == scc.scc[u + 1])
        is_satisfiable = false;

    if (is_satisfiable) {
      values = vector<bool>(m);
      for (int u = 0; u < m; ++u)
        values[u] = scc.scc[2 * u] > scc.scc[2 * u + 1];
    }
  }
};


