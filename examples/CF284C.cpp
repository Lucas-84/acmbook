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

// Max flow en O(V^3))
struct PushRelabel {
  // utiliser flow[u][v] a la fin de l'algo
  vector<vector<ll>> flow;
  
  // cap[1 .. n][1 .. n]
  // cap[u][v] = capacite entre les noeuds u et v
  // sur un graphe non complet, prendre cap[u][v] = 0 si not [(u, v) in E]
  // Calcule le flot max entre s et t avec les capacites cap
  PushRelabel(vector<vector<ll>> cap, int s, int t) {
    int n = cap.size();
    
    // sans perte de generalite 
    for (int u = 0; u < n; ++u) cap[u][u] = 0;
    
    flow = vector<vector<ll>>(n, vector<ll>(n));
    vector<ll> e(n);
    vector<int> h(n);
    h[s] = n - 1;
    for (int i = 0; i < n; ++i) {
      flow[s][i] = cap[s][i];
      flow[i][s] = -flow[s][i];
      e[i] = cap[s][i];
    }
  
    vector<int> maxh(h);
    int sz = 0;
    while (true) {
      if (sz == 0)
        for (int i = 0; i < n; ++i)
          if (i != s && i != t && e[i] > 0) {
            if (sz > 0 && h[i] > h[maxh[0]]) sz = 0;
            if (sz == 0 || h[i] == h[maxh[0]]) maxh[sz++] = i;
          }

      if (sz == 0) break;

      while (sz > 0) {
        int i = maxh[sz - 1];
        bool pushed = false;
        for (int j = 0; j < n && e[i] > 0; ++j)
          if (cap[i][j] > flow[i][j] && h[i] == h[j] + 1) {
            pushed = true;
            ll addf = min(cap[i][j] - flow[i][j], e[i]);
            flow[i][j] += addf;
            flow[j][i] -= addf;
            e[i] -= addf;
            e[j] += addf;
            if (e[i] == 0) --sz;
          }

        if (!pushed) {
          h[i] = INF;
          for (int j = 0; j < n; ++j)
            if (cap[i][j] > flow[i][j] && h[i] > h[j] + 1)
              h[i] = h[j] + 1;
          if (h[i] > h[maxh[0]]) {
            sz = 0;
            break;
          }
        }
      }
    }
  }
};

// Calcule la liste des nombres premiers dans [2, n]
// Stocke egalement pour tout i lp[i] le plus petit diviseur premier de i
// (permet de factoriser en temps lineaire)
// Complexite : O(n)
struct PrimeList {
  // primes[i] = i-eme premier de [2, n]
  vector<int> primes;
  // lp[i] = plus petit diviseur premier de i
  vector<int> lp;

  PrimeList(int n) {
    lp = vector<int>(n + 1);
    for (int i = 2; i <= n; ++i) {
      if (lp[i] == 0) {
        lp[i] = i;
        primes.push_back(i);
      }
      for (int j = 0; j < (int)primes.size() && primes[j] <= lp[i] && i * primes[j] <= n; ++j)
        lp[i * primes[j]] = primes[j];
    }
  }

  // n > 0
  // retourne la liste des facteurs premiers de n
  // (ex: factorize(12) = [2, 2, 3])
  vector<int> factorize(int n) {
    vector<int> ans;
    while (n > 1) {
      ans.push_back(lp[n]);
      n /= lp[n];
    }
    return ans;
  }
};

// factorise n en produit de ses facteurs premiers
// Complexite : O(sqrt(n))
vector<int> factorize(int n) {
  vector<int> ans;
  for (int i = 2; i * i <= n; ++i)
    while (n % i == 0) {
      ans.push_back(i);
      n /= i;
    }
  if (n > 1)
    ans.push_back(n);
  return ans;
}

int main() {
  int N, M;
  scanf("%d%d", &N, &M);
  vector<int> a(N);
  map<int, vector<int>> m;
  for (int i = 0; i < N; ++i) {
    scanf("%d", &a[i]);
    vector<int> fact = factorize(a[i]);
    for (int v: fact) {
      if (m.find(v) == m.end()) m[v] = vector<int>(N);
      ++m[v][i];
      //printf("add %d %d\n",v,i);
    }
  }
  vector<pair<int, int>> couples;
  for (int i = 0; i < M; ++i) {
    int l, r;
    scanf("%d%d", &l, &r);
    --l, --r;
    if (r % 2 == 0) swap(l, r);
    couples.push_back({l, r});
  }
  ll ans = 0;
  for (pair<int, vector<int>> p: m)  {
    vector<vector<ll>> cap = vector<vector<ll>>(N + 2, vector<ll>(N + 2));
    for (pair<int, int> c: couples) {
      cap[c.first + 1][c.second + 1] = LLINF;
      //printf("between %d and %d\n", c.first+1, c.second+1);
    }
    for (int u = 0; u < N; u += 2) cap[0][u + 1] = p.second[u];
    for (int u = 1; u < N; u += 2) cap[u + 1][N + 1] = p.second[u]; 
    PushRelabel pr(cap, 0, N + 1);
    //ll ans = 0;
    for (int u = 0; u <= N + 1; ++u) ans += pr.flow[0][u];
    //printf("%lld\n", ans);
  }
  printf("%lld\n", ans);
  return 0;
}
