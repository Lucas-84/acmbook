// Remarques sur max flow :
// - probleme avec une borne inf :
//  1) trouver un flot arbitraire entre S et T tq
//        cmin[u][v] <= flot[u][v] <= cmax[u][v]
// Ajouter une nouvelle source S' et un nouveau puits T'
// Poser c[u][v] := cmax[u][v] - cmin[u][v]
//       c[S'][v] := sum(cmin[u][v], u in V)
//       c[u][T'] := sum(cmin[u][v], v in V)
//       c[T][S] := INF
// (Theoreme) L'ancien graphe a un flot qui verifie les conditions ssi. le
// nouveau graphe a un flot saturant, ie. si sa valeur est exactement
//     sum(cmin[u][v], u, v in V)
// (et si c'est le cas, c'est forcement un flot max dans le nouveau graphe)
//  2) trouver le flot min verifiant ces conditions : dichotomie sur la valeur
//  de INF ?
const int INF = 1000 * 1000 * 1000;

// Max flow en O(V^3))
struct PushRelabel {
  // utiliser flow[u][v] a la fin de l'algo
  vector<vector<ll>> flow;
  
  // cap[1 .. n][1 .. n]
  // cap[u][v] = capacite entre les noeuds u et v
  // sur un graphe non complet, prendre cap[u][v] = 0 si not [(u, v) in E]
  // Calcule le flot max entre s et t avec les capacites cap
  PushRelabel(vector<vector<ll>> cap, int s, int t) {
    int n = cap.size();
    
    // sans perte de generalite 
    for (int u = 0; u < n; ++u) cap[u][u] = 0;
    
    flow = vector<vector<ll>>(n, vector<ll>(n));
    vector<ll> e(n);
    vector<int> h(n);
    h[s] = n - 1;
    for (int i = 0; i < n; ++i) {
      flow[s][i] = cap[s][i];
      flow[i][s] = -flow[s][i];
      e[i] = cap[s][i];
    }
  
    vector<int> maxh(h);
    int sz = 0;
    while (true) {
      if (sz == 0)
        for (int i = 0; i < n; ++i)
          if (i != s && i != t && e[i] > 0) {
            if (sz > 0 && h[i] > h[maxh[0]]) sz = 0;
            if (sz == 0 || h[i] == h[maxh[0]]) maxh[sz++] = i;
          }

      if (sz == 0) break;

      while (sz > 0) {
        int i = maxh[sz - 1];
        bool pushed = false;
        for (int j = 0; j < n && e[i] > 0; ++j)
          if (cap[i][j] > flow[i][j] && h[i] == h[j] + 1) {
            pushed = true;
            ll addf = min(cap[i][j] - flow[i][j], e[i]);
            flow[i][j] += addf;
            flow[j][i] -= addf;
            e[i] -= addf;
            e[j] += addf;
            if (e[i] == 0) --sz;
          }

        if (!pushed) {
          h[i] = INF;
          for (int j = 0; j < n; ++j)
            if (cap[i][j] > flow[i][j] && h[i] > h[j] + 1)
              h[i] = h[j] + 1;
          if (h[i] > h[maxh[0]]) {
            sz = 0;
            break;
          }
        }
      }
    }
  }
};
