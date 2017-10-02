#include <algorithm>
#include <cassert>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <queue>
#include <map>
#include <set>
#include <vector>
#include <complex>
using namespace std;
#define pb push_back
#define fst first
#define snd second
#define sz(a) int((a).size())
#define REP(i, n) for (int i = 0; i < n; ++i)
typedef long long ll;
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<vector<int>> vvi;
typedef long double ld;
const int INF = 2000 * 1000 * 1000;
const ll LLINF = 1ll << 53;
template<class T> void relaxmax(T& r, T v) { r = max(r, v); }
template<class T> void relaxmin(T& r, T v) { r = min(r, v); }

const int MAXN = 200 * 1000;

namespace DominatorTree {
    int dfn[MAXN], pre[MAXN], pt[MAXN], sz;
    int semi[MAXN], dsu[MAXN], idom[MAXN], best[MAXN];

    int get(int x) {
        if (x == dsu[x]) return x;
        int y = get(dsu[x]);
        if (semi[best[x]] > semi[best[dsu[x]]]) best[x] = best[dsu[x]];
        return dsu[x] = y;
    }

    void dfs(int u, const vvi& succ) {
        dfn[u] = sz; pt[sz++] = u;
        for (auto &v: succ[u]) if (!~dfn[v]) {
            dfs(v, succ); pre[dfn[v]] = dfn[u];
        }
    }

    void tarjan(const vvi& pred, vvi& dom) {
        for (int j = sz - 1, u; u = pt[j], j > 0; --j) {
            for (auto &tv: pred[u]) if (~dfn[tv]) {
                int v = dfn[tv]; get(v);
                if (semi[best[v]] < semi[j]) semi[j] = semi[best[v]];
            }
            dom[semi[j]].push_back(j);
            int x = dsu[j] = pre[j];
            for (auto &z: dom[x]) {
                get(z);
                if (semi[best[z]] < x) idom[z] = best[z];
                else idom[z] = x;
            }
            dom[x].clear();
        }
        for (int i = 1; i < sz; ++ i) {
            if (semi[i] != idom[i]) idom[i] = idom[idom[i]];
            dom[idom[i]].push_back(i);
        }
    }

    void build(int n, int s, const vvi& succ, const vvi& pred, vvi& dom) {
        for (int i = 0; i < n; ++ i) {
            dfn[i] = -1; dom[i].clear();
            dsu[i] = best[i] = semi[i] = i;
        }
        sz = 0; dfs(s, succ); tarjan(pred, dom);
    }
}

int timec;

void dfs(int u, vi& start, vi& end, vvi& dom) {
    start[u] = timec++;
    for (int v: dom[u]) dfs(v, start, end, dom);
    end[u] = timec++;
}

int dfs(int u, vvi& dom) {
    int ans = 1; 
    for (int v: dom[u]) ans += dfs(v, dom);
    return ans;
}

int main() {
    int n, m, s;
    scanf("%d%d%d", &n, &m, &s);
    s--;
    vector<vector<pii>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        u--; v--;
        adj[u].pb({v, w});
        adj[v].pb({u, w});
    }
    priority_queue<pll, vector<pll>, greater<pll>> q;
    vll dist(n, LLINF);
    q.push({s, 0});
    dist[s] = 0;
    vvi succ(n), pred(n);
    while (!q.empty()) {
        ll u = q.top().fst, d = q.top().snd;
        q.pop();
        for (auto p: adj[u]) {
            ll v = p.fst;
            ll dp = d + p.snd;
            if (dist[v] == LLINF) {
                dist[v] = dp;
                q.push({v, dp});
            }
            if (dist[v] == dp) {
                succ[u].pb(v);
                pred[v].pb(u);
            }
        }
    }
    vvi dom(n);
    DominatorTree::build(n, s, succ, pred, dom);
    int ans = 0;
    for (int u: dom[DominatorTree::dfn[s]]) ans = max(ans, dfs(u, dom));
    printf("%d\n", ans);
    return 0;
}
