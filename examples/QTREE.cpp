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

// Segment tree
template <typename T>
struct SegmentTree {
  void (*push_up)(vector<T>&, int, int, int);
  int  (*combine_results)(vector<T>&, int, int, int, T, T);
  void (*update_node)(vector<T>&, int, int, int, T);
  void (*update_push)(vector<T>&, int, int, int, T);
  void (*push_down)(vector<T>&, vector<T>&, int, int, int);
  vector<T> tree;
  vector<T> push_tree;
  T neutral;
  int n;
 
  SegmentTree() {}
  SegmentTree(int n, T neutral = T(),
                      void (*push_up)(vector<T>&, int, int, int) = NULL,
                      int  (*combine_results)(vector<T>&, int, int, int, T, T) = NULL,
                      void (*update_node)(vector<T>&, int, int, int, T) = NULL,
                      void (*update_push)(vector<T>&, int, int, int, T) = NULL,
                      void (*push_down)(vector<T>&, vector<T>&, int, int, int) = NULL) {
    this->neutral = neutral;
    this->push_up = push_up;
    this->combine_results = combine_results;
    this->update_node = update_node;
    this->update_push = update_push;
    this->push_down = push_down;
    int h;
    for (h = 0; (1 << h) < n; ++h)
      ;
    this->n = n = 1 << h;
    tree = vector<T>(2 * n, neutral);
    push_tree = vector<T>(2 * n, neutral);
  }

  void update(int ql, int qr, int l, int r, int u, T val) {
    if (ql > qr) return; // en pratique, mieux de le gerer ici
    if (ql <= l && r <= qr) {
      update_node(tree, u, l, r, val);
      if (update_push) update_push(push_tree, u, l, r, val);
      return;
    }
    if (push_down) push_down(tree, push_tree, u, l, r);
    int m = (l + r) / 2;
    if (ql <= m)
      update(ql, qr, l, m, 2 * u, val);
    if (qr > m)
      update(ql, qr, m + 1, r, 2 * u + 1, val);
    push_up(tree, u, l, r);
  }

  void update(int ql, int qr, T val) {
    update(ql, qr, 0, n - 1, 1, val);
  }

  T query(int ql, int qr, int l, int r, int u) {
    if (ql > qr) return neutral;
    if (ql <= l && r <= qr)
      return tree[u]; 
    if (push_down) push_down(tree, push_tree, u, l, r);
    int m = (l + r) / 2;
    if (qr <= m)
      return combine_results(tree, u, l, r, query(ql, qr, l, m, 2 * u), neutral);
    if (ql > m)
      return combine_results(tree, u, l, r, neutral, query(ql, qr, m + 1, r, 2 * u + 1));
    return combine_results(tree, u, l, r, query(ql, qr, l, m, 2 * u), query(ql, qr, m + 1, r, 2 * u + 1));
  }

  T query(int ql, int qr) {
    return query(ql, qr, 0, n - 1, 1); 
  }
};

// Plus petit ancetre commun
// Pretraitement : O(n log n)
// Requete : O(log n)
// Memoire : O(n)
struct LCA {
  vector<vector<int> > adj;
  vector<vector<int> > prev;
  int h;
  vector<int> tin, tout;
  int timer;

  void dfs(int u, int p = 0) {
    tin[u] = ++timer;
    prev[u][0] = p;
    for (int i = 1; i < h; ++i)
      prev[u][i] = prev[prev[u][i - 1]][i - 1];
    for (int i = 0; i < (int)adj[u].size(); ++i) {
      int v = adj[u][i];
      if (v != p)
        dfs(v, u);
    }
    tout[u] = ++timer;
  }

  // adj : liste d'adjacence de l'arbre
  // peut contenir eventuellement des aretes vers un parent
  LCA(vector<vector<int> > adj, int root) {
    int n = (int)adj.size();
    this->adj = adj;
    for (h = 0; (1 << h) <= n; ++h)
      ;
    prev = vector<vector<int> >(n, vector<int>(h)); 
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

struct Edge {
  int u, v;
  int w;

  Edge() {}
  Edge(int u, int v, int w) : u(u), v(v), w(w) {}

  void read() {
    scanf("%d%d%d", &u, &v, &w);
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
  vector<vector<Edge> > adj;
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
    for (int i = 0; i < (int)adj[u].size(); ++i) {
      Edge e = adj[u][i];
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
    for (int i = 0; i < (int)adj[u].size(); ++i) {
      Edge e = adj[u][i];
      int v = e.next(u);
      if (v != p && subtreesz[v] > subtreesz[maxv]) {
        maxv = v;
        maxe = e;
      }
    }
    next[u] = maxe;
    construct(maxv, u);
    for (int i = 0; i < (int)adj[u].size(); ++i) {
      Edge e = adj[u][i];
      int v = e.next(u);
      if (v != p && v != maxv) {
        nb_chains++;
        construct(v, u);
      }
    }
  }

  HLD(vector<vector<Edge> > adj, int root) {
    int n = (int)adj.size();
    this->adj = adj;
    subtreesz = vector<int>(n);
    prev = vector<Edge>(n);
    compute_subtree(root);
    //puts("subtree completed");
    nodechain = vector<int>(n);
    nodepos = vector<int>(n);
    tail = vector<int>(n);
    head = vector<int>(n);
    length = vector<int>(n);
    next = vector<Edge>(n);
    nb_chains = 1;
    construct(root);
    //for (int i = 0; i < n; ++i)
    //  printf("[%d] = %d %d\n", i+1, nodechain[i], subtreesz[i]);
    //for (int i = 0; i < n; ++i)
    //  printf("chaine %d | taille %d | head %d | tail %d |\n", i+1, length[i], head[i], tail[i]);
    //puts("construction done");
  }
};

void push_up(vector<int>& tree, int u, int l, int r) {
  tree[u] = max(tree[2 * u], tree[2 * u + 1]);
}

int combine_results(vector<int>& tree, int u, int l, int r, int p1, int p2) {
  return max(p1, p2);
}

void update_node(vector<int>& tree, int u, int l, int r, int val) {
  tree[u] = val;
}

// u ancestor of v
int get_maxedge_parenthood(HLD& hld, vector<SegmentTree<int> >& st, int u, int v) {
  int chainu = hld.nodechain[u], chainv = hld.nodechain[v];
 
  if (chainu == chainv)
    return st[chainu].query(hld.nodepos[u] + 1, hld.nodepos[v]);

  int ans = st[chainv].query(0, hld.nodepos[v]);
  Edge e = hld.prev[hld.head[chainv]];
  v = e.next(hld.head[chainv]);

  while (hld.nodechain[v] != chainu) {
    int chainv = hld.nodechain[v];
    ans = max(ans, st[chainv].query(0, hld.nodepos[v]));
    Edge e = hld.prev[hld.head[chainv]];
    v = e.next(hld.head[chainv]);
  }

  return max(ans, st[chainu].query(hld.nodepos[u] + 1, hld.nodepos[v]));
}

int main() {
  int T;
  scanf("%d", &T);
  for (int t = 1; t <= T; ++t) {
    int N;
    scanf("%d", &N);
    vector<vector<Edge> > adj(N);
    vector<vector<int> > adjproj(N);
    vector<Edge> edges(N - 1);

    for (int i = 0; i < N - 1; ++i) {
      edges[i].read();
      adj[edges[i].u].push_back(edges[i]);
      adj[edges[i].v].push_back(edges[i]);
      adjproj[edges[i].u].pb(edges[i].v);
      adjproj[edges[i].v].pb(edges[i].u);
    }

    HLD hld(adj, 0); 
    vector<SegmentTree<int> > st(hld.nb_chains);

    for (int i = 0; i < hld.nb_chains; ++i)
      st[i] = SegmentTree<int>(hld.length[i], 0, push_up, combine_results, update_node);

    for (int i = 0; i < hld.nb_chains; ++i) {
      int u = hld.head[i];
      for (int k = 0; k < hld.length[i]; ++k) {
        st[i].update(k, k, hld.prev[u].w);
        u = hld.next[u].next(u);
      }
    }
    LCA lca(adjproj, 0);
    char cmd[16];
    while (scanf("%s", cmd) == 1 && strcmp(cmd, "DONE") != 0) {
      int i, j;
      scanf("%d%d", &i, &j);
      if (strcmp(cmd, "QUERY") == 0) {
        --i, --j;
        int l = lca.lca(i, j);
        printf("%d\n", max(get_maxedge_parenthood(hld, st, l, i), get_maxedge_parenthood(hld, st, l, j)));
      } else {
        assert(strcmp(cmd, "CHANGE") == 0);
        Edge& e = edges[i - 1];
        int u = e.u, v = e.v;
        if (e == hld.prev[v]) swap(u, v);
        assert(e == hld.prev[u]);
        int chainu = hld.nodechain[u];
        st[chainu].update(hld.nodepos[u], hld.nodepos[u], j);
        e.w = j;
      }
    }
  }
  return 0;
}
