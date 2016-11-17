// Tire de https://github.com/stjepang/snippets/blob/master/directed_mst.cpp
// Directed minimum spanning tree
//
// Given a directed weighted graph and root node, computes the minimum spanning
// directed tree (arborescence) on it.
//
// Complexity: O(N * M), where N is the number of nodes, and M the number of edges
struct Edge { int x, y, w; };

int dmst(int N, vector<Edge> E, int root) {
  const int oo = 1e9;

  vector<int> cost(N), back(N), label(N), bio(N);
  int ret = 0;

  for (;;) {
    fill(cost.begin(), cost.end(), oo);
    for (auto e : E) {
      if (e.x == e.y) continue;
      if (e.w < cost[e.y]) cost[e.y] = e.w, back[e.y] = e.x;
    }
    cost[root] = 0;
    
    for (int i = 0; i < N; ++i)
      if (cost[i] == oo)
        return -1;
    
    for (int i = 0; i < N; ++i)
      ret += cost[i];

    int K = 0;
    fill(label.begin(), label.end(), -1);
    fill(bio.begin(), bio.end(), -1);

    for (int i = 0; i < N; ++i) {
      int x = i;
      for (; x != root && bio[x] == -1; x = back[x]) bio[x] = i;

      if (x != root && bio[x] == i) {
        for (; label[x] == -1; x = back[x]) label[x] = K;
        ++K;
      }
    }
    if (K == 0) break;

    for (int i = 0; i < N; ++i)
      if (label[i] == -1)
        label[i] = K++;

    for (auto &e : E) {
      int xx = label[e.x];
      int yy = label[e.y];
      if (xx != yy) e.w -= cost[e.y];
      e.x = xx;
      e.y = yy;
    }

    root = label[root];
    N = K;
  }

  return ret;
}


