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
  double value[MAX_NB_COLS];
  // vrai ssi. le systeme a >= 1 solution
  bool has_solution;

  // mat[0 .. nb_lins-1][0 .. nb_cols-1] * X = mat[0 .. nb_lins-1][nb_cols]
  Gauss(double mat[][MAX_NB_COLS + 1], int nb_lins, int nb_cols) {
    fill(posnz, posnz + nb_cols, -1);

    int posnz_cur = 0; 
    for (int col = 0; col < nb_cols; ++col) {
      int max_lin = posnz_cur;

      for (int lin = max_lin + 1; lin < nb_lins; ++lin)
        if (fabs(mat[lin][col]) > fabs(mat[max_lin][col]))
          max_lin = lin;

      // La colonne est nulle
      // Condition de la forme == 0 si on est dans les entiers
      if (fabs(mat[max_lin][col]) < eps) continue;

      for (int i = 0; i <= nb_cols; ++i)
        swap(mat[max_lin][i], mat[posnz_cur][i]);

      for (int lin = 0; lin < nb_lins; ++lin) {
        if (lin == posnz_cur) continue;
        // Pour Gauss modulaire : remplacer par l'inverse de mat[posnz_cur][col]
        double factor = mat[lin][col] / mat[posnz_cur][col];
        for (int i = 0; i <= nb_cols; ++i)
          mat[lin][i] -= factor * mat[posnz_cur][i];
          // Gauss mod : rajouter le modulo
      }

      posnz[col] = posnz_cur++;
    }
    
    // Genere une solution valide
    for (int col = 0; col < nb_cols; ++col) {
      if (posnz[col] != -1)
        value[col] = mat[posnz[col]][nb_cols] / mat[posnz[col]][col];
      // Gauss mod
      else 
        value[col] = 0;
    }

    // Verifie que la solution generee est valide
    has_solution = true;
    for (int lin = 0; lin < nb_lins; ++lin) {
      double sum = 0;
      for (int col = 0; col < nb_cols; ++col)
        sum += mat[lin][col] * value[col]; // Gauss mod
      if (fabs(sum - mat[lin][nb_cols]) > eps) // Gauss mod
        has_solution = false;
    }
  }
};

