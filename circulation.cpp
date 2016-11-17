// Tire de https://github.com/stjepang/snippets/blob/master/circulation.cpp
// Circulation
//
// Given a directed weighted graph, computes the minimum cost to run the maximum
// amount of circulation flow through the graph.
//
// Configure: MAXV
// Configure: MAXE (at least 2 * calls_to_edge)
//
// Functions:
// - init(n) initializes the algorithm with the given number of nodes
// - edge(x, y, c, w) adds an edge x->y with capacity c and weight w
// - run() runs the algorithm and returns total cost
//
// Time complexity: No idea, but it should be fast enough to solve any problem
// where V and E are up to around 1000.
//
// Constants to configure:
// - MAXV is the maximum number of vertices
// - MAXE is the maximum number of edges (i.e. twice the calls to function edge)

namespace Circu {
  const int MAXV = 1000100;
  const int MAXE = 1000100;

  int V, E;
  int how[MAXV], good[MAXV], bio[MAXV], cookie = 1; ll dist[MAXV];
  int from[MAXE], to[MAXE]; ll cap[MAXE], cost[MAXE];

  void init(int n) { V = n; E = 0; }

  void edge(int x, int y, ll c, ll w) {
    from[E] = x; to[E] = y; cap[E] = c; cost[E] = +w; ++E;
    from[E] = y; to[E] = x; cap[E] = 0; cost[E] = -w; ++E;
  }

  void reset() {
    fill(dist, dist + V, 0);
    fill(how, how + V, -1);
  }

  bool relax() {
    bool ret = false;
    for (int e = 0; e < E; ++e)
      if (cap[e]) {
        int x = from[e];
        int y = to[e];

        if (dist[x] + cost[e] < dist[y]) {
          dist[y] = dist[x] + cost[e];
          how[y] = e;
          ret = true;
        }
      }
    return ret;
  }

  ll cycle(int s, bool flip = false) {
    int x = s;
    ll c = cap[how[x]];
    do {
      int e = how[x];
      c = min(c, cap[e]);
      x = from[e];
    } while (x != s);

    ll sum = 0;
    do {
      int e = how[x];
      if (flip) {
        cap[e] -= c;
        cap[e ^ 1] += c;
      }
      sum += cost[e] * c;
      x = from[e];
    } while (x != s);
    return sum;
  }

  ll push(int x) {
    for (++cookie; bio[x] != cookie; x = from[how[x]]) {
      if (!good[x] || how[x] == -1 || cap[how[x]] == 0) return 0;
      bio[x] = cookie;
      good[x] = false;
    }
    return cycle(x) >= 0 ? 0 : cycle(x, true);
  }

  ll run() { 
    reset();
    ll ret = 0;
    for (int step = 0; step < 2 * V; ++step) {
      if (step == V) reset();
      if (!relax()) continue;

      fill(good, good + V, true);
      for (int i = 0; i < V; ++i)
        if (ll w = push(i)) {
          ret += w;
          step = 0;
        }
    }
    return ret;
  }
}


