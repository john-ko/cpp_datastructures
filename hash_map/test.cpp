
#include <iostream>
#include <functional>

#include "hash_map.hpp"
#include "hash_node.hpp"

void test_hash_node()
{
  HashBSTNode<int, int> node;

}

void test_hash_map()
{
  HashMap<std::string, int> map;

  std::cout << "test hash:" << map._hash("fdsfs")%10 << std::endl;
  std::cout << "map size: " << map._size << std::endl;
  std::cout << "map capacity:" << map._capacity << std::endl;
  std::cout << "load_factor:" << map._load_factor << std::endl;

  // if (map._table[0] == nullptr) {
  //   std::cout<< "NULL PTR!!!" << std::endl;
  // }
  //map["hello"] = 4;
  map["asdf"] = 2;
  map.insert("hello", 7);
  map["fdsfs"] = 10;
  map["a"] = 1;
  map["b"] = 1;
  map["c"] = 1;
  map["d"] = 1;
  map["e"] = 1;
  map["f"] = 1;

  std::cout << map["hello"] << std::endl;
  std::cout << map._table[4].hash_key << std::endl;

  std::cout << std::endl;

  std::cout << map._size/10 << std::endl;

  for (int i=0; i < map._capacity; ++i) {
    std::cout << i << ". " << map._table[i].value << ":" << map._table[i].flag << std::endl;
  }


}

int main()
{
  test_hash_map();

  return 0;
}