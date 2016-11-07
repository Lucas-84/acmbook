// Fonction Z : s[0 .. n-1] -> z[0 .. n-1]
// z[i] = longueur du plus grand prefixe commun de s[0 .. n-1] et s[i .. n-1]
// Calcul en O(n)
// Applications
// - Trouver toutes les occurrences d'un motif dans une chaine en O(n+p).
// - Nombre de sous-chaines distinctes quand on ajoute un caractere en tete.
// - Factorisation 
vector<int> compute_z(string s) {
  int n = (int)s.length();
  vector<int> z(n);
  int l = 1, r = 0;
  for (int i = 1; i < n; ++i) {
    if (i <= r)
      z[i] = min(r - i + 1, z[i - l]);
    while (i + z[i] < n && s[z[i]] == s[i + z[i]])
      ++z[i];
    if (i + z[i] - 1 > r) {
      l = i;
      r = i + z[i] - 1;
    }
  }
  z[0] = n;
  return z;
}

