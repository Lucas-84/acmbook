// Fenwick tree - sommes sur intervalle
// Generalisation a d'autres operations inversibles
// Requetes de sommes/mises a jour : O(log n)
// Memoire : O(n)
struct FenwickTree {
  vector<ll> tree;
  // a = [0 .. n-1]
  FenwickTree(int n) {
    tree = vector<ll>(n);
  }
  // a[x] += v
  void add(int x, ll v) {
    for (; x < (int)tree.size(); x |= (x + 1))
      tree[x] += v;
  }
  // a[0] + ... + a[r]
  ll sum(int r) {
    ll ans = 0;
    for (; r >= 0; r = (r & (r + 1)) - 1)
      ans += tree[r];
    return ans;
  }
  // a[l] + ... + a[r]
  ll sum(int l, int r) {
    return sum(r) - sum(l - 1);
  }
};

// Fenwick Tree 2D classique - Sommes sur rectangles
// Temps : O(log^2 n) par requete
// Memoire : O(n^2)
struct FenwickTree2D {
  vector<vector<int>> tree;
  // a = [1 .. n][1 .. m]
  FenwickTree2D(int n, int m) {
    tree = vector<vector<int>>(n);
    for (int i = 0; i < n; ++i)
      tree[i] = vector<int>(m);
  }
  // a[x][y] += v
  void add(int x, int y, int v) {
    for (; x < (int)tree.size(); x |= (x + 1))
      for (int j = y; j < (int)tree[x].size(); j |= (j + 1))
        tree[x][j] += v;
  }
  // sum (0 <= i <= x; 0 <= j <= y) a[i][j]
  int sum(int x, int y) {
    int ans = 0;
    for (; x >= 0; x = (x & (x + 1)) - 1) 
      for (int j = y; j >= 0; j = (j & (j + 1)) - 1)
        ans += tree[x][j];
    return ans;
  }
  // sum (x1 <= x <= x2; y1 <= y <= y2) a[x][y]
  int sum(int x1, int y1, int x2, int y2) {
    return sum(x2, y2) - sum(x1 - 1, y2) - sum(x2, y1 - 1) + sum(x1 - 1, y1 - 1);
  }
};

// Fenwick tree 2D compresse 
// Necessite deux passes sur les requetes
// Temps : O(log^2 n) par requete
// Memoire : O(n log n)
// /!\ type
struct FenwickTree2DCompressed {
  vector<vector<int>> tree;
  vector<vector<int>> nodes;
  // a = [1 .. n][1 .. m]
  FenwickTree2DCompressed(int n) {
    tree = vector<vector<int>>(n);
    nodes = vector<vector<int>>(n);
  }
  // premiere passe : a appeler sur toutes les requetes prevues 
  void init_add(int x, int y) {
    for (; x < (int)tree.size(); x |= (x + 1))      
      nodes[x].push_back(y);
  }
  void init_sum(int x, int y) {
    for (; x >= 0; x = (x & (x + 1)) - 1) 
      nodes[x].push_back(y);
  }
  void init_sum(int x1, int y1, int x2, int y2) {
    init_sum(x2, y2);
    init_sum(x1 - 1, y2);
    init_sum(x2, y1 - 1);
    init_sum(x1 - 1, y1 - 1);
  }
  // a appeler a la fin de la premiere passe
  void init() {
    for (int x = 0; x < (int)tree.size(); ++x) {
      sort(nodes[x].begin(), nodes[x].end());
      tree[x] = vector<int>(nodes[x].size());
    }
  }
  // a[x][y] += v
  void add(int x, int y, int v) {
    for (; x < (int)tree.size(); x |= (x + 1))
      for (int j = lower_bound(nodes[x].begin(), nodes[x].end(), y) - nodes[x].begin(); j < (int)nodes[x].size(); j |= (j + 1))
        tree[x][j] += v;
  }
  // sum (0 <= i <= x; 0 <= j <= y) a[i][j]
  int sum(int x, int y) {
    int ans = 0;
    for (; x >= 0; x = (x & (x + 1)) - 1) 
      for (int j = lower_bound(nodes[x].begin(), nodes[x].end(), y) - nodes[x].begin(); j >= 0; j = (j & (j + 1)) - 1)
        ans += tree[x][j];
    return ans;
  }
  // sum (x1 <= x <= x2; y1 <= y <= y2) a[x][y]
  int sum(int x1, int y1, int x2, int y2) {
    return sum(x2, y2) - sum(x1 - 1, y2) - sum(x2, y1 - 1) + sum(x1 - 1, y1 - 1);
  }
};

