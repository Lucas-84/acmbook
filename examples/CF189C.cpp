#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

// File monotone (min/max sur une fenetre glissante)
// Ici : file MIN
template<class T>
struct MonotoneQueue {
  deque<pair<T, int>> q;

  // ajoute elt a la file au "temps" t
  // t doit etre croissant au fur et a mesure des appels a add
  void add(T elt, int t) {
    // changer > par < pour une file max
    while (!q.empty() && q.back().first > elt)
      q.pop_back();
    q.push_back({elt, t});
  }

  // supprime l'element issu du temps t
  // retourne vrai ssi. il y a effectivement un tel element
  // les appels successifs a remove doivent etre les memes qu'a add
  // (et dans le meme ordre) 
  bool remove(int t) {
    if (!q.empty && q.front().second == t) {
      q.pop_front();
      return true;
    }
    return false;
  }

  // idem pour max
  T min() {
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

int main() {
  int n;
  scanf("%d", &n);
  vector<int> a(n), b(n);
  for (int i = 0; i < n; ++i) scanf("%d", &a[i]);
  for (int i = 0; i < n; ++i) scanf("%d", &b[i]);
  ConvexHullTrick ch;
  ch.add(b[0], 0);
  ll ans = 0;
  for (int i = 1; i < n; ++i) {
    ans = ch.query(a[i]);
    ch.add(b[i], ans);
  }
  printf("%lld\n", ans);
  return 0;
}
