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

// From https://github.com/stjepang/snippets  
//
// Fast Fourier transform
//
// Caling mult(a, b, c, len) is identical to:
//   REP(i, 2*len) tmp[i] = 0
//   REP(i, len) REP(j, len) tmp[i+j] += a[i] * b[j];
//   REP(i, 2*len) c[i] = tmp[i];
//
// There is also a variant with modular arithmetic: mult_mod.
//
// Common use cases:
// - big integer multiplication
// - convolutions in dynamic programming
//
// Time complexity: O(N log N), where N is the length of arrays
//
// Constants to configure:
// - MAX must be at least 2^ceil(log2(2 * len))
#define MY_PI     3.14159265358979323846
#define REP(i, n) for (int i = 0; i < (n); ++i)

namespace FFT {
  const int MAX = 1 << 20;

  typedef ll value;
  typedef complex<double> comp;

  int N;
  comp omega[MAX];
  comp a1[MAX], a2[MAX];
  comp z1[MAX], z2[MAX];

  void fft(comp *a, comp *z, int m = N) {
    if (m == 1) {
      z[0] = a[0];
    } else {
      int s = N / m;
      m /= 2;

      fft(a, z, m);
      fft(a + s, z + m, m);

      for (int i = 0; i < m; ++i) {
        comp c = omega[s * i] * z[m + i];
        z[m + i] = z[i] - c;
        z[i] += c;
      }
    }
  }

  // len = longueur de a et b 
  // c = a * b
  void mult(value *a, value *b, value *c, int len) {
    N = 2 * len;

    while (N & (N - 1))
      ++N;

    assert(N <= MAX);
    fill(a1, a1 + N, 0);
    fill(a2, a2 + N, 0);
    for (int i = 0; i < len; ++i) {
      a1[i] = a[i];
      a2[i] = b[i];
    }

    for (int i = 0; i < N; ++i) 
      omega[i] = polar(1., 2 * MY_PI / N * i);

    fft(a1, z1, N);
    fft(a2, z2, N);

    for (int i = 0; i < N; ++i) {
      omega[i] = comp(1, 0) / omega[i];
      a1[i] = z1[i] * z2[i] / comp(N, 0);
    }

    fft(a1, z1, N);
    for (int i = 0; i < 2 * len; ++i)
      c[i] = round(z1[i].real());
  }

  // len = longueur de a et b
  // c = a * b [mod]
  void mult_mod(value *a, value *b, value *c, int len, int mod) {
    static value a0[MAX], a1[MAX];
    static value b0[MAX], b1[MAX];
    static value c0[MAX], c1[MAX], c2[MAX];

    for (int i = 0; i < len; ++i) {
      a0[i] = a[i] & 0xFFFF;
      a1[i] = a[i] >> 16;
      b0[i] = b[i] & 0xFFFF;
      b1[i] = b[i] >> 16;
    }
    
    FFT::mult(a0, b0, c0, len);
    FFT::mult(a1, b1, c2, len);

    for (int i = 0; i < len; ++i) {
      a0[i] += a1[i];
      b0[i] += b1[i];
    }

    FFT::mult(a0, b0, c1, len);

    for (int i = 0; i < 2 * len; ++i) {
      c1[i] -= c0[i] + c2[i];
      c1[i] %= mod;
      c2[i] %= mod;
      c[i] = (c0[i] + (c1[i] << 16) + (c2[i] << 32)) % mod;
    }
  }
}

const int MAXN = 300 * 1000;
ll s[MAXN];
int ns;
ll t[MAXN];
int nt;
ll ans[2 * MAXN];

int main() {
  int n;
  scanf("%d ", &n);
  for (int i = 0; i < n; ++i) {
    fill(s, s + MAXN, 0);
    fill(t, t + MAXN, 0);
    int c;
    ns = 0;
    nt = 0;
    while (isdigit(c = getchar()))
      s[ns++] = c - '0';
    reverse(s, s + ns);
    while (isdigit(c = getchar()))
      t[nt++] = c - '0';
    reverse(t, t + nt);
    if (s[ns - 1] == 0 || t[nt - 1] == 0) {
      puts("0");
      continue;
    }
    int nmax = max(ns, nt);
    FFT::mult(s, t, ans, nmax);
    int nb_digits = ns + nt - 1;
    for (int i = 0; i < nb_digits; ++i) {
      ans[i + 1] += ans[i] / 10;
      ans[i] %= 10;
    }
    if (ans[ns + nt - 1] > 0) ++nb_digits;
    for (int i = nb_digits - 1; i >= 0; --i)
      printf("%lld", ans[i]);
    puts("");
  }
  return 0;
}
