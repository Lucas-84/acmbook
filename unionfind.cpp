// Union-Find
// Temps : O(alpha(n)) amorti par requete
// Memoire : O(n)
struct UnionFind {
  vector<int> cc;
  vector<int> ccsz;

  UnionFind() {}

  UnionFind(int n) {
    cc = vector<int>(n);
    ccsz = vector<int>(n);
    for (int i = 0; i < n; ++i)
      cc[i] = i;
  }

  int find(int i) {
    if (cc[i] != i)
      cc[i] = find(cc[i]);
    return cc[i];
  }

  bool merge(int i, int j) {
    i = find(i);
    j = find(j);
    if (i == j) return false;
    if (ccsz[i] < ccsz[j])
      swap(i, j);
    ccsz[i] += ccsz[j];
    cc[j] = i;
    return true;
  }
};


