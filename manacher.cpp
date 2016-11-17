// Tire de https://github.com/stjepang/snippets/blob/master/manacher.cpp 
// Finds all palindromes in a string
//
// Given a string s of length N, finds all palindromes as its substrings.
//
// After calling manacher(s, N, rad), rad[x] will be the radius of the largest
// palindrome centered at index x / 2.
// Example:
//   s   = b a n a n a a
//   rad = 0000102010010
//
// Note: Array rad must be of length at least twice the length of the string.
// Also, "invalid" characters are denoted by -1, therefore the string must not
// contain such characters.
//
// Time complexity: O(N)
//
// Constants to configure:
// - MAX is the maximum length of the string
const int MAX = 1000;
void manacher(char *s, int N, int *rad) {
  static char t[2 * MAX];
  int m = 2 * N - 1;
  fill(t, t + m, -1);
  for (int i = 0; i < N; ++i) t[2 * i] = s[i];

  int x = 0;
  for (int i = 1; i < m; ++i) {
    int& r = rad[i] = 0;
    if (i <= x + rad[x]) 
      r = min(rad[x + x - i], x + rad[x] - i);
    while (i - r - 1 >= 0 && i + r + 1 < m && t[i - r - 1] == t[i + r + 1])
      ++r;
    if (i + r >= x + rad[x])
      x = i;
  }

  for (int i = 0; i < m; ++i)
    if (i - rad[i] == 0 || i + rad[i] == m - 1)
      ++rad[i];

  for (int i = 0; i < m; ++i)
    rad[i] /= 2;
}


