#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
const ll LLINF = (1ull << 53) - 1;
const int INF = 1000 * 1000 * 1000;

// Suffix Array & LCP
// Construction en O(n log^2 n) 
struct SuffixArray {
  // sa[i] = pos du premier caractere du i-eme suffixe dans l'ordre lexicographique
  vector<int> sa;
  // pos[i] a l'issue du constructeur : position de s[i .. n-1] dans sa
  vector<int> pos;
  // lcp[i] : plus grand prefixe commun a lcp[i] et lcp[i+1]
  vector<int> lcp;
  int gap;
  int n;

  struct Compare {
    const SuffixArray& s;
    Compare(const SuffixArray& s) : s(s) {}

    bool operator () (int i, int j) const {
      if (s.pos[i] != s.pos[j]) return s.pos[i] < s.pos[j];
      i += s.gap;
      j += s.gap;
      return i < s.n && j < s.n ? s.pos[i] < s.pos[j] : i > j;
    }
  };

  SuffixArray(string s) {
    n = (int)s.length();
    sa = vector<int>(n);
    vector<int> tmp = vector<int>(n);
    pos = vector<int>(n);

    // Construction de sa et pos
    for (int i = 0; i < n; ++i) {
      sa[i] = i;
      pos[i] = s[i];
    }

    for (gap = 1; gap <= n; gap *= 2) {
      sort(sa.begin(), sa.end(), Compare(*this));
      for (int i = 1; i < n; ++i) tmp[i] = tmp[i - 1] + Compare(*this)(sa[i - 1], sa[i]);
      for (int i = 0; i < n; ++i) pos[sa[i]] = tmp[i];
    }
/*
    // Construction de lcp (peut etre supprime si non necessaire)
    int k = 0;
    for (int i = 0; i < n; ++i)
      if (pos[i] != n - 1) {
        int j = sa[pos[i] + 1]; 
        while (s[i + k] == s[j + k]) ++k;
        lcp[pos[i]] = k;
        if (k > 0) --k;
      }
*/
  }

  
};

const int MAXN = 100 * 1000;
char s[MAXN + 1];
int main() {
  scanf("%s", s);
  SuffixArray sa(s);
  for (int i = 0; i < (int)sa.sa.size(); ++i)
    printf("%d\n", sa.sa[i]);
  return 0;
}
