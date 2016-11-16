// Calcule la liste des nombres premiers dans [2, n]
// Stocke egalement pour tout i lp[i] le plus petit diviseur premier de i
// (permet de factoriser en temps lineaire)
// Complexite : O(n)
struct PrimeList {
  // primes[i] = i-eme premier de [2, n]
  vector<int> primes;
  // lp[i] = plus petit diviseur premier de i
  vector<int> lp;

  PrimeList(int n) {
    lp = vector<int>(n + 1);
    for (int i = 2; i <= n; ++i) {
      if (lp[i] == 0) {
        lp[i] = i;
        primes.push_back(i);
      }
      for (int j = 0; j < (int)primes.size() && primes[j] <= lp[i] && i * primes[j] <= n; ++j)
        lp[i * primes[j]] = primes[j];
    }
  }

  // n > 0
  // retourne la liste des facteurs premiers de n
  // (ex: factorize(12) = [2, 2, 3])
  vector<int> factorize(int n) {
    vector<int> ans;
    while (n > 1) {
      ans.push_back(lp[n]);
      n /= lp[n];
    }
    return ans;
  }
};


