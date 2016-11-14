#include <bits/stdc++.h>
using namespace std;
#define mp make_pair
#define pb push_back
#define fst first
#define snd second
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

// Inspire de https://github.com/stjepang/snippets/blob/master/gauss.cpp
// Elimination de Gauss
// Resout un systeme d'equations lineaires
// Complexite : O(nb_lins * nb_cols^2)
// Si le systeme a au moins une solution, value contiendra une solution possible
const int MAX_NB_COLS = 250;
const double eps = 1e-8;

struct Gauss {
  // posnz[i] = -1 si la i-eme composante est libre
  int posnz[MAX_NB_COLS];
  // value[i] = la valeur de X(i) verifiant l'equation ci-dessous 
  int value[MAX_NB_COLS];
  // vrai ssi. le systeme a >= 1 solution
  bool has_solution;

  int inverse(int k) { return k; }

  // mat[0 .. nb_lins-1][0 .. nb_cols-1] * X = mat[0 .. nb_lins-1][nb_cols]
  Gauss(int mat[][MAX_NB_COLS + 1], int nb_lins, int nb_cols) {
    fill(posnz, posnz + nb_cols, -1);

    int posnz_cur = 0; 
    for (int col = 0; col < nb_cols; ++col) {
      int max_lin = posnz_cur;

      for (int lin = max_lin + 1; lin < nb_lins; ++lin)
        if (fabs(mat[lin][col]) > fabs(mat[max_lin][col]))
          max_lin = lin;

      // La colonne est nulle
      // Condition de la forme == 0 si on est dans les entiers
      if (mat[max_lin][col] == 0) continue;
      //printf("Column %d: %d\n", col + 1, max_lin + 1);

      for (int i = 0; i <= nb_cols; ++i)
        swap(mat[max_lin][i], mat[posnz_cur][i]);

      for (int lin = 0; lin < nb_lins; ++lin) {
        if (lin == posnz_cur) continue;
        // Pour Gauss modulaire : remplacer par l'inverse de mat[posnz_cur][col]
        int factor = mat[lin][col] * inverse(mat[posnz_cur][col]);
        for (int i = 0; i <= nb_cols; ++i)
          mat[lin][i] = ((mat[lin][i] - factor * mat[posnz_cur][i]) % 2 + 2) % 2; // Gauss mod : rajouter le modulo
      }

      //for (int i = 0; i < nb_lins; ++i, puts(""))
      //  for (int j = 0; j <= nb_cols; ++j)
      //    printf("%f ", mat[i][j]);
      //puts("----------");
      posnz[col] = posnz_cur++;
    }
    
    // Genere une solution valide
    for (int col = 0; col < nb_cols; ++col) {
      if (posnz[col] != -1)
        value[col] = mat[posnz[col]][nb_cols] * inverse(mat[posnz[col]][col]);
      else 
        value[col] = 0;
    }

    //for (int i = 0; i < nb_cols; ++i)
    //  printf("%d ", posnz[i]);
    //puts("");
    //for (int i = 0; i < nb_cols; ++i)
    //  printf("%f ", value[i]);
    //puts("");

    // Verifie que la solution generee est valide
    has_solution = true;
    for (int lin = 0; lin < nb_lins; ++lin) {
      int sum = 0;
      for (int col = 0; col < nb_cols; ++col)
        sum = (sum + mat[lin][col] * value[col]) % 2;
      if (sum != mat[lin][nb_cols])
        has_solution = false;
    }
  }
};

int mat[MAX_NB_COLS][MAX_NB_COLS + 1];

int main() {
  int N;
  scanf("%d", &N);
  for (int i = 0; i < N; ++i) {
    int v;
    while (scanf("%d", &v) == 1 && v != -1) {
      --v;
      mat[v][i] = 1;
    }
  }
  for (int i = 0; i < N; ++i)
    mat[i][N] = 1;
  Gauss g(mat, N, N);
  if (!g.has_solution)
    puts("No solution");
  else {
    for (int i = 0; i < N; ++i)
      if (g.value[i] > eps)
        printf("%d ", i + 1);
    puts("");
  }
  return 0;
}
