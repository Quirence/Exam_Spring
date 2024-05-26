#include <iostream>
#include <string>
#include <cstdint>

class DivHash {
 public:
  long long operator()(long long key, long long size) {
    return key % size;
  }
};

class PolynomialHash {
 public:
  int64_t operator()(const std::string &str) {
    const static int64_t p = 1000000007;
    const static int64_t x = 263;
    int64_t sum = 0;
    int64_t x_pow = 1;
    for (int64_t i = 0; i < str.length(); ++i) {
      sum = (sum + (str[i] * x_pow % p)) % p;
      x_pow = (x_pow * x) % p;
    }
    return sum;
  }
};

class HashTable {
  std::string table[20000];
  void Insert(std::string name);
  bool Remove(std::string name);
  PolynomialHash hash;
  DivHash stepper;
 public:
  void Push(std::string name);
  bool Pop(std::string name);
  bool Search(std::string name);
};

void HashTable::Insert(std::string name) {
  int64_t index = hash(name) % 20000;
  int64_t step = stepper(index, 20000);
  while (table[index] != "") {
    index += step;
    index = index % 20000;
  }
  table[index] = name;
}

bool HashTable::Remove(std::string name) {
  int64_t index = hash(name) % 20000;
  int64_t step = stepper(index, 20000);
  if (table[index] == "") {
    return false;
  }
  if (table[index] == name) {
    table[index] = "";
    return true;
  }
  while (table[index] != name) {
    index += step;
    index = index % 20000;
    if (table[index] == "") {
      return false;
    }
  }
  table[index] = "";
  return true;
}

bool HashTable::Search(std::string name) {
  int64_t index = hash(name) % 20000;
  int64_t step = stepper(index, 20000);
  if (table[index] == "") {
    return false;
  }
  if (table[index] == name) {
    return true;
  }
  while (table[index] != name) {
    index += step;
    index = index % 20000;
    if (table[index] == "") {
      return false;
    }
  }
  return true;
}

void HashTable::Push(std::string name) {
  Insert(name);
}

bool HashTable::Pop(std::string name) {
  return Remove(name);
}

int main() {
  HashTable table;
  int commands;
  std::cin >> commands;
  for (int i = 0; i < commands; ++i) {
    std::string command;
    std::cin >> command;
    if (command == "push") {
      std::string input;
      std::cin >> input;
      table.Push(input);
    } else if (command == "pop") {
      std::string input;
      std::cin >> input;
      std::cout << ((table.Pop(input)) ? "TRUE" : "FALSE") << "\n";
    } else if (command == "search") {
      std::string input;
      std::cin >> input;
      std::cout << ((table.Search(input)) ? "TRUE" : "FALSE") << "\n";
    }
  }
}