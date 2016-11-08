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


