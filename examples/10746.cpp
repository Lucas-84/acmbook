// mcmf
// hungarian
// treap
// miller-rabin
// link-cut
// kd-tree
// stoer-wagner
// geometrie
// simplexe
// Gomory-Hu
// Erdos-Gallai
// Aho-Corasick
#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const ll LLINF = 1ll << 53;
const int INF = 1000 * 1000 * 1000;

// Inspire de http://e-maxx.ru/algo/assignment_hungary
// Algorithme hongrois
// Matching parfait de poids min
// Complexite : O(nm)
// Applications :
// - Max matching min weight
// - Max matching max weight (poids < 0)
// - Decomposer un DAG en un nombre min de chemins disjoints
// - Coloriage d'un arbre k-aire avec k couleurs, le coloriage de chaque noeud
// par une certaine couleur a un certain cout -> trouver un coloriage de cout
// min (dp[v][c] = cout min de colorier le sous-arbre en v sachant c(v) = c)
// - Etant donnee une matrice a[1..n][1..m], trouver deux tableaux u[1..n] et
// v[1..m] tq pour tout i,j : u[i] + v[j] <= a[i][j] et la somme des elements
// de u et v est max.
struct Hungarian {
  // p[i] = si 1 <= i <= m, vaut le noeud matche avec i (entre 1 et n)
  // vaut 0 si non matche
  vector<int> p;

  // a doit etre de taille (n + 1) x (m + 1), avec n <= m
  Hungarian(vector<vector<double>> a, int n, int m) {
    vector<double> u(n + 1), v(m + 1);
    vector<int> way(m + 1);
    p = vector<int>(m + 1);
    for (int i = 1; i <= n; ++i) {
      p[0] = i;
      int j0 = 0;
      vector<double> minv(m + 1, INF);
      vector<bool> used(m + 1, false);
      do {
        used[j0] = true;
        int i0 = p[j0], j1;
        double delta = LLINF;
        for (int j = 1; j <= m; ++j)
          if (!used[j]) {
            double cur = a[i0][j] - u[i0] - v[j];
            if (cur < minv[j]) {
              minv[j] = cur;
              way[j] = j0;
            }
            if (minv[j] < delta) {
              delta = minv[j];
              j1 = j;
            }
          }
        for (int j = 0; j <= m; ++j)
          if (used[j]) {
            u[p[j]] += delta;
            v[j] -= delta;
          } else {
            minv[j] -= delta;
          }
        j0 = j1;
      } while (p[j0] != 0);
      do {
        int j1 = way[j0];
        p[j0] = p[j1];
        j0 = j1;
      } while (j0 != 0);
    }
  }
};

int main() {
  int n, m;
  while (scanf("%d%d", &n, &m) == 2 && !(n == 0 && m == 0)) {
    vector<vector<double>> a(n + 1, vector<double>(m + 1));
    for (int i = 1; i <= n; ++i)
      for (int j = 1; j <= m; ++j)
        scanf("%lf", &a[i][j]);
    Hungarian h(a, n, m);
    double ans = 0;
    for (int i = 1; i <= m; ++i) {
      ans += a[h.p[i]][i];
    }
    printf("%.2f\n", round(ans / n * 100) / 100);
  }
  return 0;
}
