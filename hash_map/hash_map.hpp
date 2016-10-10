#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <functional>

template <typename K, typename V>
struct HashPair {
  unsigned char flag;
  std::size_t hash_key;
  V value;
};

template <typename K, typename V>
class HashMap {


public:
  // put these in private
  std::hash<K> _hash;
  unsigned int _size;
  unsigned int _capacity;
  float _load_factor;
  HashPair<K, V>* _table;



  // private function to be used by inserting by key
  // so it can be used by insert as well as resizing the table
  V& _insert_by_hash(HashPair<K, V>*, std::size_t);

  HashPair<K, V>& _find(HashPair<K, V>*, std::size_t);






  // TODO PUBLIC:
  HashMap();
  ~HashMap();

  // hash_map(const &hash_map);
  
  // hash_map& operator=(const hash_map& other);

  // insert by reference, used by operator[]
  // this functions will check for loadfactor
  V& insert(K);

  // to be used as map.insert(key, value)
  // calls operator[] underneath
  bool insert(K, V);

  // value_t& operator[](std::size_t idx)
  
  const V& operator[](K) const;

  V& operator[](K);

  bool contains(K);

  //bool find_o_of_n_so_dont_use(K);





  // gets current size
  unsigned int size();

  // gets the current capacity
  unsigned int capacity();

  std::size_t hash(K);
  // bool has(K);
  
  // V get(K)

  // bool remove(K);

  void resize();
};





template <typename K, typename V>
HashMap<K, V>::HashMap()
{
  _size = 0;
  _capacity = 10;
  _load_factor = 0.7;

  // create a new table of size CAPACITIY and zero out array
  _table = new HashPair<K,V>[_capacity]();
}


template <typename K, typename V>
HashMap<K, V>::~HashMap()
{
  delete[] _table;
}


template <typename K, typename V>
V& HashMap<K, V>::_insert_by_hash(HashPair<K, V>* table, std::size_t hash)
{
  // index
  std::size_t i = hash % _capacity;

  while(table[i].flag != 0) {
    if (table[i].hash_key == hash) {
      return table[i].value;
    }

    i = (i + 1) % _capacity;

  }

  table[i].hash_key = hash;
  table[i].flag = 1;
  return table[i].value;
}

/**
 * INSERT
 */
template <typename K, typename V>
V& HashMap<K, V>::insert(K key)
{

  // check for load_factor
  std::cout << (_size / _capacity) << std:: endl;
  if ((float(_size) / _capacity) > _load_factor) {
    resize();
  }

  _size++;
  return _insert_by_hash(_table, _hash(key));
}

template <typename K, typename V>
bool HashMap<K, V>::insert(K key, V value)
{
  // TODO make sure
  operator[](key) = value;
  return true;
}

template <typename K, typename V>
V& HashMap<K, V>::operator[](K key) {
  return insert(key);
}


template <typename K, typename V>
const V& HashMap<K, V>::operator[](K index) const{
  std::size_t i = _hash(index) % _capacity;
  std::cout << "-- 2" << std::endl;
  return _table[i].value;
}

template <typename K, typename V>
unsigned int HashMap<K, V>::size()
{
  return _size;
}

template <typename K, typename V>
unsigned int HashMap<K, V>::capacity()
{
  return _capacity;
}

template <typename K, typename V>
std::size_t HashMap<K, V>::hash(K key)
{
  return _hash(key);
}

template <typename K, typename V>
bool HashMap<K, V>::contains(K key)
{
  return _hash(key);
}



template <typename K, typename V>
void HashMap<K, V>::resize()
{
  HashPair<K, V>* new_table = new HashPair<K, V>[_capacity*2];
  unsigned int size = _capacity;
  _capacity *= 2;
  for (int i = 0; i < size; ++i) {
    if (_table[i].flag == 1) {
      _insert_by_hash(new_table, _table[i].hash_key) = _table[i].value;
    }
  }

  delete[] _table;
  
  _table = new_table;
}

#endif