#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

// Euclide etendu
// Retourne PGCD(a, b)
// et u, v contiennent a l'issue de l'algo des bons couples de Bezout
// et |u| + |v| minimal, u <= v en cas d'egalite
ll gcd(ll a, ll b, ll& u, ll& v) {
  if (b == 0) {
    u = 1;
    v = 0;
    return a;
  }
  ll d = gcd(b, a % b, u, v);
  ll oldu = u;
  u = v;
  v = oldu - v * ll(a / b);
  return d;
}

int main() {
  ll a, b;
  while (scanf("%lld%lld", &a, &b) == 2) {
    ll u, v;
    ll d = gcd(a, b, u, v);
    printf("%lld %lld %lld\n", u, v, d);
  }
  return 0;
}
