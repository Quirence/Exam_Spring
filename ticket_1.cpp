#include <iostream>
#include <list>
#include <string>

class HashTable {
 private:
  static const int HashGroups = 10;
  std::list<std::pair<int, std::string>> table[HashGroups];

 public:
  bool IsEmpty() const;
  int HashFunction(int key);
  void Insert(int key, const std::string& value);
  void Remove(int key);
  std::string Find(int key);
  void PrintTable();

};

bool HashTable::IsEmpty() const {
  unsigned long long sum = 0;
  for (const auto& bucket : table) {
    sum += bucket.size();
  }
  return sum == 0;
}

int HashTable::HashFunction(int key) {
  return key % HashGroups;
}

void HashTable::Insert(int key, const std::string& value) {
  int hash = HashFunction(key);
  auto& bucket = table[hash];
  for (auto& pair : bucket) {
    if (pair.first == key) {
      pair.second = value;
      return;
    }
  }
  bucket.emplace_back(key, value);
}

void HashTable::Remove(int key) {
  int hash = HashFunction(key);
  auto& bucket = table[hash];
  for (auto it = bucket.begin(); it != bucket.end(); ++it) {
    if (it->first == key) {
      bucket.erase(it);
      return;
    }
  }
}

std::string HashTable::Find(int key) {
  int hash = HashFunction(key);
  auto& bucket = table[hash];
  for (const auto& pair : bucket) {
    if (pair.first == key) {
      return pair.second;
    }
  }
  return "Not Found";
}

void HashTable::PrintTable() {
  for (int i = 0; i < HashGroups; ++i) {
    std::cout << "Group " << i << ": ";
    for (const auto& pair : table[i]) {
      std::cout << "[" << pair.first << ": " << pair.second << "] ";
    }
    std::cout << std::endl;
  }
}

int main() {
  HashTable ht;
  std::string value1 = "value1";
  std::string value2 = "value2";
  std::string value3 = "value3";

  ht.Insert(1, value1);
  ht.Insert(2, value2);
  ht.Insert(3, value3);

  ht.PrintTable();

  ht.Remove(2);
  ht.PrintTable();

  std::cout << "Find key 1: " << ht.Find(1) << std::endl;
  std::cout << "Find key 2: " << ht.Find(2) << std::endl;

  return 0;
}
