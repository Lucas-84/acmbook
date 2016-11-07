// Fonction prefixe : s[0 .. n-1] -> pref[0 .. n-1]
// pref[i] = max(0 <= k <= i | s[0 .. k-1] = s[i-k+1 .. i])
// O(n)
// Applications :
// - Trouver toutes les occurrences d'un motif (KMP)
// - Compter le nombre d'occurrences de chaque prefixe de s dans s
// - Nombre de sous-chaines distinctes dans s
// - Factorisation
vector<int> compute_prefix(string s) {
  int n = (int)s.length();
  vector<int> pref(n);
  for (int i = 1; i < n; ++i) {
    int j = pref[i - 1];
    while (j > 0 && s[i] != s[j])
      j = pref[j - 1];
    if (s[i] == s[j])
      ++j;
    pref[i] = j;
  }
  return pref;
}


