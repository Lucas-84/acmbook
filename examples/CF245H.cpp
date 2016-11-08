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

// Hash
// P premier de l'ordre de la taille de l'alphabet
// P = 31 pour les minuscules, 53 pour tous les caracteres alphabetiques
//
// (Quelques) applications :
// - Recherche de motifs (Rabin-Karp)
// - Nombre de sous-chaines distinctes 
// - Requetes : est-ce que la sous-chaine est un palindrome ?
//
// /!\ Ne pas utiliser les hash modulo 2^64 http://codeforces.com/blog/entry/4898(
// Utiliser un modulo suffit generalement (/!\ paradoxe des anniversaires)
// Sinon utiliser deux mod
// Quelques premiers de l'ordre de 10^9 : 10^9 + 7 ; 10^9 + 9 ; 10^9 + 123
// Trouver le reste entre 0 et M - 1 de a/M : (a % M + M) % M
typedef long long ll;
ll posmod(ll a, ll M) { return (a % M + M) % M; }

struct StringHash {
  ll mod;
  vector<ll> ppows;
  // hashsuff[i] = s[i] + s[i + 1] * P + s[i + 2] * P^2 + ... + s[n - 1] * P^(n-1-i) [mod]
  vector<ll> hashsuff;

  StringHash() {}

  StringHash(string s, ll base, ll mod) {
    this->mod = mod;
    int n = (int)s.length();
    ppows = vector<ll>(n);
    hashsuff = vector<ll>(n + 1);
    ppows[0] = 1;
    for (int i = 1; i < n; ++i)
      ppows[i] = posmod(ppows[i - 1] * base, mod);
    hashsuff[n] = 0;
    for (int i = n - 1; i >= 0; --i) 
      hashsuff[i] = posmod(hashsuff[i + 1] * base + s[i], mod);
  }
  
  // [l, r]
  // 0 <= l <= r <= n
  ll get_hash(int l, int r) {
    return posmod(hashsuff[l] - hashsuff[r + 1] * ppows[r + 1 - l], mod);
  }
};

const int BASE = 31;
const int NB_MODS = 2;
const ll MOD[NB_MODS] = {1000 * 1000 * 1000 + 9, 1000 * 1000 * 1000 + 7};

const int MAXN = 5000;
bool is_pal[MAXN][MAXN];
int dp[MAXN][MAXN];
char s[MAXN + 1];

int main() {
  scanf("%s", s);
  int n = (int)strlen(s);
  int q;
  scanf("%d", &q);
  StringHash h[NB_MODS], ht[NB_MODS];
  string t = string(s);
  reverse(t.begin(), t.end());
  for (int k = 0; k < NB_MODS; ++k) {
    h[k] = StringHash(string(s), BASE, MOD[k]);
    ht[k] = StringHash(t, BASE, MOD[k]);
  }
  for (int i = 0; i < n; ++i)
    for (int j = i; j < n; ++j) {
      bool equal = true;
      for (int k = 0; k < 2; ++k)
        equal &= h[k].get_hash(i, j) == ht[k].get_hash(n - j - 1, n - i - 1);
      is_pal[i][j] = equal;
    } 
  for (int i = n - 1; i >= 0; --i) {
    dp[i][i] = 1;
    for (int j = i + 1; j < n; ++j)
      dp[i][j] = dp[i][j - 1] + dp[i + 1][j] - dp[i + 1][j - 1] + is_pal[i][j];
  }
  for (int i = 0; i < q; ++i) {
    int l, r;
    scanf("%d%d", &l, &r);
    --l, --r;
    printf("%d\n", dp[l][r]);
  }
  return 0;
}
