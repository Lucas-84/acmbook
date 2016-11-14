#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
const int INF = 1000 * 1000 * 1000;

// File monotone (min/max sur une fenetre glissante)
// Par defaut : file MIN
template<class T>
struct MonotonicQueue {
  deque<pair<T, int>> q;

  // ajoute elt a la file au "temps" t
  // t doit etre croissant au fur et a mesure des appels a add
  void add(T elt, int t) {
    while (!q.empty() && q.back().first > elt)
      q.pop_back();
    q.push_back({elt, t});
  }

  // supprime l'element issu du temps t
  // retourne vrai ssi. il y a effectivement un tel element
  // les appels successifs a remove doivent etre les memes qu'a add
  // (et dans le meme ordre) 
  bool remove(int t) {
    if (!q.empty() && q.front().second == t) {
      q.pop_front();
      return true;
    }
    return false;
  }

  T get() {
    return q.front().first;
  }
};

// Convex hull trick "statique"
// Recurrence de la forme : dp[i] = min(dp[j] + b[j] * a[i], j < i)
// Hypotheses : a[i] <= a[i+1] (requetes croissantes)
//              b[i] >= b[i+1] (coefficients directeurs decroissants)
const int MAX_NB_LINES = 100 * 1000;
struct ConvexHullTrick {
  ll a[MAX_NB_LINES], b[MAX_NB_LINES];
  ll hd, tl; 

  ConvexHullTrick() {hd = tl = 0;}

  // ajoute y = a0 * x + b0
  void add(ll a0, ll b0) {
    while (tl - hd >= 2) {
      ll a1 = a[tl - 1], b1 = b[tl - 1];
      ll a2 = a[tl - 2], b2 = b[tl - 2];
      if ((long double)(b0 - b2) * (a1 - a0) < (long double)(b0 - b1) * (a2 - a0))
        break;
      //if ((long double)1 * (b0 - b2) / (a2 - a0) < (long double)1 * (b0 - b1) / (a1 - a0))
      //  break;
      tl--;
    }
    a[tl] = a0;
    b[tl++] = b0;
  }

  // valeur de l'enveloppe min sur la droite d'equation x = x0
  ll query(ll x0) {
    while (tl - hd >= 2) {
      if (x0 * a[hd] + b[hd] < x0 * a[hd + 1] + b[hd + 1])
        break;
      ++hd;
    }
    return x0 * a[hd] + b[hd];
  }
};

// Inspire de https://github.com/niklasb/contest-algos/blob/master/convex_hull/dynamic.cpp
// Variante du precedent sans hypothese sur les a[i] et les b[i] 
const ll QUERY = -(1LL << 62);
struct Line {
  ll a, b;
  mutable function<const Line *()> succ;

  bool operator < (const Line& other) const {
    if (other.b != QUERY)
      return a > other.a;
    const Line *s = succ();
    if (s == NULL) return false;
    return b - s->b > (s->a - a) * other.a;
  }
};

struct DynamicConvexHullTrick : public multiset<Line> {
  bool bad(iterator y) {
    auto z = next(y);
    if (y == begin()) {
      if (z == end()) return false;
      return y->a == z->a && y->b >= z->b;
    }
    auto x = prev(y);
    if (z == end())
      return y->a == x->a && y->b >= x->b;
    return (long double)(x->b - y->b) * (z->a - y->a) >= (long double)(y->b - z->b) * (y->a - x->a);
  }

  void add(ll a, ll b) {
    auto y = insert({a, b, nullptr});
    y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
    
    if (bad(y)) {
      erase(y);
      return;
    }

    while (next(y) != end() && bad(next(y)))
      erase(next(y));

    while (y != begin() && bad(prev(y)))
      erase(prev(y));
  }

  ll query(ll x) {
    auto l = *lower_bound((Line){x, QUERY, nullptr});
    return l.a * x + l.b;
  }
};

int main() {
  int n, s, l;
  scanf("%d%d%d", &n, &s, &l);
  vector<int> a(n);
  for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
  MonotonicQueue<int> qmin;
  MonotonicQueue<int> qmax;
  vector<int> last(n + 1); 
  int j = 0;
  for (int i = 0; i <= n; ++i) {
    while (i > 0 && -qmax.get() - qmin.get() > s) {
      qmin.remove(j);
      qmax.remove(j);
      ++j;
    }
    last[i] = j;
    assert(last[i] <= j);
    if (i < n) {
      qmin.add(a[i], i);
      qmax.add(-a[i], i);
    }
  }
  MonotonicQueue<int> m;
  vector<int> dp(n + 1);
  j = 0;
  m.add(0, 0);
  for (int i = 0; i <= n; ++i) {
    while (j < last[i]) { 
      m.remove(j);
      ++j;
    }
    dp[i] = INF;
    if (last[i] <= i - l) {
      m.add(dp[i - l], i - l);
      if (!m.q.empty() && m.get() != INF)
        dp[i] = m.get() + 1;
    }
  }
  printf("%d\n", dp[n] == INF ? -1 : dp[n]);
  return 0;
}
