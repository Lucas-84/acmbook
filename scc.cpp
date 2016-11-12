// CFC
// O(|E|)
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
    scc = vector<int>(n);
    nb_scc = 0;
    seen = vector<bool>(n);
    for (int t = n - 1; t >= 0; --t) {
      int u = order[t];
      //printf("ith-node: %d\n", u);
      if (!seen[u])
        mark_scc(u, nb_scc++);
    }

    // (optionnel) construction du DAG des CFC 
    sccgraph = vector<vector<int>>(nb_scc, vector<int>(0));
    fill(seen.begin(), seen.end(), false);
    for (int u = 0; u < n; ++u)
      if (!seen[u])
        create_scc_graph(u);
  }
};


