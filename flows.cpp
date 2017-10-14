#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
#define sz(a) int((a).size())
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<vll> vvll;
typedef vector<vector<int>> vvi;
typedef long double ld;
const int INF = 2000 * 1000 * 1000;
const ll LLINF = 1ll << 53;
template<class T> void relaxmax(T& r, T v) { r = max(r, v); }
template<class T> void relaxmin(T& r, T v) { r = min(r, v); }
template<class T> using max_queue = priority_queue<T>;
template<class T> using min_queue = priority_queue<T, vector<T>, greater<T>>;

struct Dinic {
    vi last, dist, curr;
    vi ne, adj; vll cap;
    int N, M;

    Dinic(int N) : N(N), M(0) {
        last = vi(N, -1); dist = vi(N); curr = vi(N);
    }

    // u -> v (c1), v -> u (c2)
    void add(int u, int v, ll c1, ll c2) {
        adj.pb(v); cap.pb(c1); ne.pb(last[u]); last[u] = M++;
        adj.pb(u); cap.pb(c2); ne.pb(last[v]); last[v] = M++;
    }

    ll push(int u, int t, ll f) {
        if (u == t) return f;
        if (f == 0) return 0;
        for (int& e = curr[u]; e != -1; e = ne[e]) {
            int v = adj[e];
            if (dist[u] + 1 != dist[v]) continue;
            if (ll r = push(v, t, min(f, cap[e]))) {
                cap[e] -= r; cap[e ^ 1] += r;
                return r;
            }
        }
        return 0;
    }

    ll maxflow(int s, int t) {
        ll ans = 0;
        for (;;) {
            fill(dist.begin(), dist.end(), -1);
            copy(last.begin(), last.end(), curr.begin());
            queue<int> q; q.push(s); dist[s] = 0;
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int e = last[u]; e != -1; e = ne[e]) {
                    int v = adj[e];
                    if (cap[e] && dist[v] == -1) {
                        q.push(v);
                        dist[v] = dist[u] + 1;
                    }
                }
            }
            if (dist[t] == -1) break;
            while (ll f = push(s, t, LLINF)) ans += f;
        }
        return ans;
    }
};

struct MCMF_DFS {
    vi last, curr, seen; vll pi;
    vi ne, adj; vll cap, cost;
    int N, M;

    MCMF_DFS(int N) : N(N), M(0) {
        last = vi(N, -1); curr = vi(N); seen = vi(N); pi = vll(N);
    }

    // u -> v (c, w)
    void add(int u, int v, ll c, ll w) {
        adj.pb(v); cap.pb(c); cost.pb(+w); ne.pb(last[u]); last[u] = M++;
        adj.pb(u); cap.pb(0); cost.pb(-w); ne.pb(last[v]); last[v] = M++;
    }

    ll push(int u, int t, ll f) {
        if (u == t) return f;
        if (seen[u]) return 0;
        seen[u] = true;
        for (int& e = curr[u]; e != -1; e = ne[e]) {
            int v = adj[e];
            if (cap[e] && pi[u] == pi[v] + cost[e]) {
                if (ll r = push(v, t, min(f, cap[e]))) {
                    cap[e] -= r; cap[e ^ 1] += r;
                    return r;
                }
            }
        }
        return 0;
    }

    pll mcmf(int s, int t) {
        ll answ = 0, ansf = 0;
        fill(pi.begin(), pi.end(), LLINF);
        pi[t] = 0;
        for (;;) {
            bool done = true;
            for (int u = 0; u < N; ++u)
                for (int e = last[u]; e != -1; e = ne[e]) {
                    if (cap[e] && pi[u] > pi[adj[e]] + cost[e]) {
                        pi[u] = pi[adj[e]] + cost[e];
                        done = false;
                    }
                }
            if (done) break;
        }
        for (;;) {
            fill(seen.begin(), seen.end(), false);
            copy(last.begin(), last.end(), curr.begin());
            while (ll f = push(s, t, LLINF)) {
                answ += pi[s] * f;
                ansf += f;
                fill(seen.begin(), seen.end(), false);
            }
            ll inc = LLINF;
            for (int u = 0; u < N; ++u)
                if (seen[u])
                    for (int e = last[u]; e != -1; e = ne[e]) {
                        int v = adj[e];
                        if (cap[e] && !seen[v])
                            inc = min(inc, pi[v] + cost[e] - pi[u]);
                    }
            if (inc == LLINF) break;
            for (int u = 0; u < N; ++u) if (seen[u]) pi[u] += inc;
        }
        return {answ, ansf};
    }
};

struct MCMF_Dijkstra {
    vi last, how; vll dist, pi;
    vi ne, from, adj; vll cap, cost;
    int N, M;

    MCMF_Dijkstra(int N) : N(N), M(0) {
        last = vi(N, -1); how = vi(N); dist = vll(N); pi = vll(N);
    }

    // u -> v (c, w)
    void add(int u, int v, ll c, ll w) {
        from.pb(u); adj.pb(v); cap.pb(c); cost.pb(+w); ne.pb(last[u]); last[u] = M++;
        from.pb(v); adj.pb(u); cap.pb(0); cost.pb(-w); ne.pb(last[v]); last[v] = M++;
    }

    pll mcmf(int s, int t) {
        ll answ = 0, ansf = 0;
        fill(pi.begin(), pi.end(), LLINF);
        pi[t] = 0;
        for (;;) {
            bool done = true;
            for (int u = 0; u < N; ++u)
                for (int e = last[u]; e != -1; e = ne[e]) {
                    if (cap[e] && pi[u] > pi[adj[e]] + cost[e]) {
                        pi[u] = pi[adj[e]] + cost[e];
                        done = false;
                    }
                }
            if (done) break;
        }
	for (;;) {
            fill(dist.begin(), dist.end(), LLINF);
            set<pii> S;
	    dist[s] = 0;
	    S.insert({0, s});
	    while (!S.empty()) {
		int u = S.begin()->snd; S.erase(S.begin());
		if (u == t) break;
                for (int e = last[u]; e != -1; e = ne[e]) {
                    if (!cap[e]) continue;
                    int v = adj[e];
                    ll val = dist[u] + pi[u] + cost[e] - pi[v];
                    if (val < dist[v]) {
                        S.erase({dist[v], v});
                        dist[v] = val;
                        how[v] = e;
                        S.insert({dist[v], v});
                    }
		}
	    }
            if (dist[t] >= LLINF / 2) break;
            ll inc = cap[how[t]];
            for (int u = t; u != s; u = from[how[u]]) inc = min(inc, cap[how[u]]);
            for (int u = t; u != s; u = from[how[u]]) {
                cap[how[u]] -= inc;
                cap[how[u] ^ 1] += inc;
                answ += cost[how[u]] * inc;
            }
            ansf += inc;
            for (int u = 0; u < N; ++u) pi[u] = min(pi[u] + dist[u], LLINF);
	}

	return {answ, ansf};
    }
};

const int L = 10 * 1000 + 5;
int N;
vector<pii> e;

bool possible(int m) {
    Dinic d(N + L + 2);
    for (int i = 0; i < N; ++i)
        d.add(0, i + 1, m, 0);
    for (pii p: e)
        d.add(p.fst + 1, N + 1 + p.snd, 1, 0);
    for (int i = 0; i < L; ++i)
         d.add(N + 1 + i, N + L + 1, 1, 0);
    return d.maxflow(0, N + L + 1) == N * m;
}

int main() {
    scanf("%d", &N);
    for (int i = 0; i < N; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        for (int j = u; j < v; ++j) e.pb({i, j});
    }
    int l = 0, r = L;
    while (l < r - 1) {
        int m = (l + r) / 2;
        if (possible(m)) l = m;
        else r = m;
    }
    printf("%d\n", N * l);
    return 0;
}
