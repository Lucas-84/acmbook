// File monotone (min/max sur une fenetre glissante)
// Par defaut : file MIN
template<class T>
struct MonotonicQueue {
  deque<pair<T, int>> q;

  // ajoute elt a la file au "temps" t
  // t doit etre croissant au fur et a mesure des appels a add
  void add(T elt, int t) {
    while (!q.empty() && q.back().first > elt)
      q.pop_back();
    q.push_back({elt, t});
  }

  // supprime l'element issu du temps t
  // retourne vrai ssi. il y a effectivement un tel element
  // les appels successifs a remove doivent etre les memes qu'a add
  // (et dans le meme ordre) 
  bool remove(int t) {
    if (!q.empty() && q.front().second == t) {
      q.pop_front();
      return true;
    }
    return false;
  }

  T get() {
    return q.front().first;
  }
};


