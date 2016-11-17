// Segment tree
template <typename T>
struct SegmentTree {
  function<void (vector<T>&, int, int, int)> push_up;
  function<int (vector<T>&, int, int, int, T, T)> combine_results;
  function<void (vector<T>&, int, int, int, T)> update_node;
  function<void (vector<T>&, int, int, int, T)> update_push;
  function<void (vector<T>&, vector<T>&, int, int, int)> push_down;
  vector<T> tree;
  vector<T> push_tree;
  T neutral;
  int n;
 
  SegmentTree(int n, T neutral = T(),
                     function<void (vector<T>&, int, int, int)> 
                       push_up = nullptr,
                     function<int (vector<T>&, int, int, int, T, T)>
                       combine_results = nullptr,
                     function<void (vector<T>&, int, int, int, T)>
                       update_node = nullptr,
                     function<void (vector<T>&, int, int, int, T)>
                       update_push = nullptr,
                     function<void (vector<T>&, vector<T>&, int, int, int)>
                       push_down = nullptr) {
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
    if (ql <= l && r <= qr)
      return tree[u]; 
    if (push_down) push_down(tree, push_tree, u, l, r);
    int m = (l + r) / 2;
    if (qr <= m)
      return combine_results(tree, u, l, r, 
                             query(ql, qr, l, m, 2 * u), neutral);
    if (ql > m)
      return combine_results(tree, u, l, r, neutral, 
                             query(ql, qr, m + 1, r, 2 * u + 1));
    return combine_results(tree, u, l, r, 
                           query(ql, qr, l, m, 2 * u),
                           query(ql, qr, m + 1, r, 2 * u + 1));
  }

  T query(int ql, int qr) {
    return query(ql, qr, 0, n - 1, 1); 
  }
};


