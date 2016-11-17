// Tire de https://github.com/stjepang/snippets/blob/master/mcmf_dijkstra.cpp
// Min-cost max-flow (uses DFS)
//
// Given a directed weighted graph, source, and sink, computes the minimum cost
// of the maximum flow from source to sink.
// This version uses DFS to find shortest paths and gives good performance on
// very "shallow" graphs: graphs which have very short paths between source
// and sink (e.g. at most 10 edges).
// In such cases this algorithm can be orders of magnitude faster than the
// Dijkstra version.
//
// To use, call init(n), then add edges using edge(x, y, c, w), and finally
// call run(src, sink).
//
// Functions:
// - init(n) initializes the algorithm with the given number of nodes
// - edge(x, y, c, w) adds an edge x->y with capacity c and weight w
// - run(src, sink) runs the algorithm and returns {total_cost, total_flow}
//
// Time complexity: O(V * E^3)
//
// Constants to configure:
// - MAXV is the maximum number of vertices
// - MAXE is the maximum number of edges (i.e. twice the calls to function edge)
// - oo is the "infinity" value
namespace Mcmf_dfs {
  const int MAXV = 1000100;
  const int MAXE = 1000100;
  const ll oo = 1e18;

  int V, E;
  int last[MAXV], curr[MAXV], bio[MAXV];
  ll pi[MAXV];
  int next[MAXE], adj[MAXE];
  ll cap[MAXE], cost[MAXE];

  void init(int n) {
    V = n;
    E = 0;
    fill(last, last + V, -1);
    fill(pi, pi + V, 0);
  }

  void edge(int x, int y, ll c, ll w) {
    adj[E] = y; cap[E] = c; cost[E] = +w; next[E] = last[x]; last[x] = E++;
    adj[E] = x; cap[E] = 0; cost[E] = -w; next[E] = last[y]; last[y] = E++;
  }

  ll push(int x, int sink, ll flow) {
    if (x == sink) return flow;
    if (bio[x]) return 0;
    bio[x] = true;

    for (int &e = curr[x]; e != -1; e = next[e]) {
      int y = adj[e];

      if (cap[e] && pi[x] == pi[y] + cost[e])
        if (ll f = push(y, sink, min(flow, cap[e])))
          return cap[e] -= f, cap[e ^ 1] += f, f;
    }
    return 0;
  }

  pair<ll, ll> run(int src, int sink) {
    ll total = 0;
    ll flow = 0;
    pi[src] = oo;

    for (;;) {
      fill(bio, bio + V, false);
      for (int i = 0; i < V; ++i) curr[i] = last[i];

      while (ll f = push(src, sink, oo)) {
        total += pi[src] * f;
        flow += f;
        fill(bio, bio + V, false);
      }

      ll inc = oo;
      for (int x = 0; x < V; ++x)
        if (bio[x])
          for (int e = last[x]; e != -1; e = next[e]) {
            int y = adj[e];
            if (cap[e] && !bio[y]) inc = min(inc, pi[y] + cost[e] - pi[x]);
          }

      if (inc == oo) break;

      for (int i = 0; i < V; ++i)
        if (bio[i])
          pi[i] += inc;
    }
    return {total, flow};
  }
}

// Min-cost max-flow (uses Dijkstra's algorithm)
//
// Given a directed weighted graph, source, and sink, computes the minimum cost
// of the maximum flow from source to sink.
// This version uses Dijkstra's algorithm and gives good performance on all
// kinds of graphs.
//
// To use, call init(n), then add edges using edge(x, y, c, w), and finally
// call run(src, sink).
//
// Functions:
// - init(n) initializes the algorithm with the given number of nodes
// - edge(x, y, c, w) adds an edge x->y with capacity c and weight w
// - run(src, sink) runs the algorithm and returns {total_cost, total_flow}
//
// Time complexity: O(V * E^2 log E)
//
// Constants to configure:
// - MAXV is the maximum number of vertices
// - MAXE is the maximum number of edges (i.e. twice the calls to function edge)
// - oo is the "infinity" value
namespace Mcmf_dijkstra {
  const int MAXV = 1000100;
  const int MAXE = 1000100;
  const ll oo = 1e18;

  int V, E;
  int last[MAXV], how[MAXV]; ll dist[MAXV];
  int next[MAXE], from[MAXE], adj[MAXE]; ll cap[MAXE], cost[MAXE];

  struct cmpf {
    bool operator () (int a, int b) {
      if (dist[a] != dist[b]) return dist[a] < dist[b];
      return a < b;
    }
  };
  set<int, cmpf> S;

  void init(int n) {
    V = n;
    E = 0;
    fill(last, last + V, -1);
  }

  void edge(int x, int y, ll c, ll w) {
    from[E] = x; adj[E] = y; cap[E] = c; cost[E] = +w;
    next[E] = last[x]; last[x] = E++;
    from[E] = y; adj[E] = x; cap[E] = 0; cost[E] = -w;
    next[E] = last[y]; last[y] = E++;
  }

  pair<ll, ll> run(int src, int sink) {
    ll total = 0;
    ll flow = 0;

    for (;;) {
      fill(dist, dist + V, oo);
      dist[src] = 0;

      for (;;) {
        bool done = true;
        for (int x = 0; x < V; ++x)
          for (int e = last[x]; e != -1; e = next[e]) {
            if (cap[e] == 0) continue;

            int y = adj[e];
            ll val = dist[x] + cost[e];

            if (val < dist[y]) {
              dist[y] = val;
              how[y] = e;
              done = false;
            }
          }
        if (done) break;
      }

      if (dist[sink] >= oo / 2) break;

      ll aug = cap[how[sink]];
      for (int i = sink; i != src; i = from[how[i]])
        aug = min(aug, cap[how[i]]);

      for (int i = sink; i != src; i = from[how[i]]) {
        cap[how[i]] -= aug;
        cap[how[i] ^ 1] += aug;
        total += cost[how[i]] * aug;
      }
      flow += aug;
    }
    return {total, flow};
  }
}
