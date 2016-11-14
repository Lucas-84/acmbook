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
