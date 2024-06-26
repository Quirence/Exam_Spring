#include <iostream>
#include <cstdint>
#include <cmath>
#include <cstring>
#include <vector>

// Полиномиальное хеширование (б/д)
struct PolynomialHash {
  int64_t operator()(const std::string &str) {
    const static int32_t p = 1000000007;
    const static int16_t x = 263;
    int64_t sum = 0;
    int64_t x_pow = 1;
    for (size_t i = 0; i < str.length(); i++) {
      sum = (sum + (str[i] * x_pow % p)) % p;
      x_pow = (x_pow * x) % p;
    }
    return sum;
  }
};

// Префикс-функция
std::vector<int> PrefixFunction(std::string s) {
  std::vector<int> p(s.size(), 0);
  int i = 1; // итератор, указываюший на элемент
  int j = 0; // длина совпдающего префикса и суффикса
  while (i != static_cast<int>(s.size())) {
    if (s[i] == s[j]) {
      p[i] = j + 1;
      i++;
      j++;
    } else if (j == 0) {
      p[i] = 0;
      i++;
    } else {
      j = p[j - 1];
    }
  }
  return p;
}
// Асимптотика O(N)

// КМП (Кнут-Моррис-Пратт)
void KnuthMorrisPratt(std::string text, std::string word) {
  auto p = PrefixFunction(word);
  int iterator_word = 0;
  int iterator_text = 0;
  while (iterator_text != static_cast<int>(text.size())) {
    if (text[iterator_text] == word[iterator_word]) {
      ++iterator_text;
      ++iterator_word;
      if (iterator_word == static_cast<int>(word.size())) {
        std::cout << iterator_text - static_cast<int>(word.size()) << '\n';
      }
    } else {
      if (iterator_word == 0) {
        ++iterator_text;
      } else {
        iterator_word = p[iterator_word - 1];
      }
    }
  }
}
// Асимптотика O(n + m), длина текста и образа соотвественно