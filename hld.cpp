// A adapter au probleme (par exemple avec des poids)
struct Edge {
  int u, v;

  Edge() {}
  Edge(int u, int v) : u(u), v(v) {}

  void read() {
    scanf("%d%d", &u, &v);
    --u, --v;
  }

  int next(int x) {
    return x == u ? v : u;
  }

  bool operator == (const Edge& e) const {
    return u == e.u && v == e.v;
  }
};

// HLD
struct HLD {
  vector<vector<Edge>> adj;
  vector<int> subtreesz;
  vector<int> nodechain;
  vector<int> nodepos;
  vector<int> head;
  vector<int> tail;
  vector<int> length;
  vector<Edge> prev;
  vector<Edge> next;
  int nb_chains;

  void compute_subtree(int u, int p = -1) {
    subtreesz[u]++;
    for (Edge e: adj[u]) {
      int v = e.next(u);
      if (v != p) {
        prev[v] = e;
        compute_subtree(v, u);
        subtreesz[u] += subtreesz[v];
      }
    }
  }

  void construct(int u, int p = -1) {
    nodechain[u] = nb_chains - 1;
    nodepos[u] = length[nb_chains - 1]++;
    if (nodepos[u] == 0)
      head[nb_chains - 1] = u;
    tail[nb_chains - 1] = u;
    if (adj[u].size() == 1 && adj[u][0].next(u) == p) return;
      Edge maxe = adj[u][0].next(u) == p ? adj[u][1] : adj[u][0];
    int maxv = maxe.next(u);
    for (Edge e: adj[u]) {
      int v = e.next(u);
      if (v != p && subtreesz[v] > subtreesz[maxv]) {
        maxv = v;
        maxe = e;
      }
    }
    next[u] = maxe;
    construct(maxv, u);
    for (Edge e: adj[u]) {
      int v = e.next(u);
      if (v != p && v != maxv) {
        nb_chains++;
        construct(v, u);
      }
    }
  }

  HLD(vector<vector<Edge>> adj, int root) {
    int n = (int)adj.size();
    this->adj = adj;
    subtreesz = vector<int>(n);
    prev = vector<Edge>(n);
    compute_subtree(root);
    nodechain = vector<int>(n);
    nodepos = vector<int>(n);
    tail = vector<int>(n);
    head = vector<int>(n);
    length = vector<int>(n);
    next = vector<Edge>(n);
    nb_chains = 1;
    construct(root);
  }
};


