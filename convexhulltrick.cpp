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
      if ((long double)(b0 - b2) * (a1 - a0) < 
          (long double)(b0 - b1) * (a2 - a0))
        break;
      //if ((long double)1 * (b0 - b2) / (a2 - a0) < 
      //    (long double)1 * (b0 - b1) / (a1 - a0))
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

// Inspire de https://github.com/niklasb/contest-algos/
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
    return (long double)(x->b - y->b) * (z->a - y->a) >= 
           (long double)(y->b - z->b) * (y->a - x->a);
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


