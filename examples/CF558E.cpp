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
                     function<void (vector<T>&, int, int, int)> push_up = nullptr,
                     function<int (vector<T>&, int, int, int, T, T)> combine_results = nullptr,
                     function<void (vector<T>&, int, int, int, T)> update_node = nullptr,
                     function<void (vector<T>&, int, int, int, T)> update_push = nullptr,
                     function<void (vector<T>&, vector<T>&, int, int, int)> push_down = nullptr) {
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
      return combine_results(tree, u, l, r, query(ql, qr, l, m, 2 * u), neutral);
    if (ql > m)
      return combine_results(tree, u, l, r, neutral, query(ql, qr, m + 1, r, 2 * u + 1));
    return combine_results(tree, u, l, r, query(ql, qr, l, m, 2 * u), query(ql, qr, m + 1, r, 2 * u + 1));
  }

  T query(int ql, int qr) {
    return query(ql, qr, 0, n - 1, 1); 
  }
};

void push_up(vector<int>& tree, int u, int l, int r) {
  tree[u] = tree[2 * u] + tree[2 * u + 1];
}

int combine_results(vector<int>& tree, int u, int l, int r, int p1, int p2) {
  return p1 + p2;
}

void update_node(vector<int>& tree, int u, int l, int r, int x) {
  if (x > 0)
    tree[u] += r - l + 1;
  else
    tree[u] = 0;
}

void update_push(vector<int>& push_tree, int u, int l, int r, int x) {
  if (x < 0)
    push_tree[u] = -1;
  else if (push_tree[u] >= 0)
    push_tree[u]++;
  else 
    push_tree[u]--;
}

void push_down(vector<int>& tree, vector<int>& push_tree, int u, int l, int r) {
  int size = r - l + 1;
  if (push_tree[u] < 0) {
    tree[2 * u] = (-push_tree[u] - 1) * size / 2;
    tree[2 * u + 1] = (-push_tree[u] - 1) * size / 2;
    push_tree[2 * u] = push_tree[2 * u + 1] = push_tree[u];
  } else if (push_tree[u] > 0) {
    tree[2 * u] += push_tree[u] * size / 2;
    tree[2 * u + 1] += push_tree[u] * size / 2;
  }
  push_tree[u] = 0;
}

const int MAXN = 100 * 1000;
char s[MAXN + 1];
int main() {
  int N, Q;
  scanf("%d%d", &N, &Q);
  scanf("%s", s);
  vector<SegmentTree<int>> ST(26, SegmentTree<int>(N, 0, push_up, combine_results, update_node, update_push, push_down));
  for (int i = 0; i < N; ++i)
    ST[s[i] - 'a'].update(i, i, 1); 
  for (int i = 0; i < Q; ++i) {
    int l, r, inc;
    scanf("%d%d%d", &l, &r, &inc);
    --l, --r;
    vector<int> cnt(26);
    for (int c = 0; c < 26; ++c)
      cnt[c] = ST[c].query(l, r);
    int pos = inc ? l : r;  
    for (int c = 0; c < 26; ++c) {
      if (cnt[c] == 0) continue;
      ST[c].update(l, r, -1);
      if (inc) {
        ST[c].update(pos, pos + cnt[c] - 1, 1);
        pos += cnt[c];
      } else {
        ST[c].update(pos - cnt[c] + 1, pos, 1);
        pos -= cnt[c];
      }
    }
  }
  for (int i = 0; i < N; ++i)
    for (int c = 0; c < 26; ++c)
      if (ST[c].query(i, i) > 0)
        printf("%c", c + 'a');
  puts("");
  return 0;
}
