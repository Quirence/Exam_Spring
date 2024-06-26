#include <array>
#include <cstdint>
#include <cstddef>
#include <ostream>

class BigInteger {
 private:
  using DigitType = uint8_t;  // Используется 8-битное беззнаковое целое для каждой цифры.
  using DoubleDigitType = uint16_t;  // Используется 16-битное беззнаковое целое для операций, которые могут привести к переполнению.
  const static DigitType base = 100u;  // Основание системы счисления, используемое для хранения чисел.
  const static size_t maxDecimalSize = 10'000u;  // Максимальный размер числа в десятичных цифрах.
  const static size_t
      maxBaseSize = (maxDecimalSize + base - 1) / base;  // Максимальный размер массива для хранения числа.
  using DigitStorage = std::array<DigitType, maxBaseSize>;  // Тип для хранения цифр числа.

  DigitStorage digits_ {};  // Массив для хранения цифр числа.
  size_t size_ = 0;  // Реальное количество используемых цифр в массиве.
  bool is_negative_ = false; // Флаг отрицательности числа.

  BigInteger LeftShift(size_t positions) const;
  void UnsignedAdd(const BigInteger& that);  // Добавление чисел без учета знака.
  void UnsignedSubtract(const BigInteger& that);  // Вычитание чисел без учета знака.
  int UnsignedCompare(const BigInteger& that) const;  // Сравнение чисел без учета знака.

 public:
  BigInteger() = default;  // Конструктор по умолчанию.
  BigInteger(const BigInteger& that, size_t begin, size_t end);  // Конструктор создания числа из среза другого числа.

  template<typename IntType>
  BigInteger(IntType value) : BigInteger() {  // Конструктор из целочисленного значения.
    if (value < 0) {
      is_negative_ = true;
      value = -value;
    }
    size_t i = 0u;
    for (; i < maxBaseSize && value; ++i, value /= base) {
      digits_[i] = value % base;
    }
    size_ = i;
  }

  bool IsNegative() const;  // Проверка, является ли число отрицательным.
  BigInteger operator-() const;  // Унарный минус.
  BigInteger& operator+=(const BigInteger& that);  // Операция сложения.
  BigInteger& operator-=(const BigInteger& that);  // Операция вычитания.
  BigInteger& operator*=(const BigInteger& that);  // Операция умножения.
  friend bool operator<(const BigInteger& left, const BigInteger& right);  // Оператор сравнения меньше.
  friend std::ostream& operator<<(std::ostream& os, const BigInteger& that);  // Оператор вывода в поток.
  friend std::istream& operator>>(std::istream& os, BigInteger& that);  // Оператор ввода из потока.
};

// Определения функций операторов, которые создают новые объекты для выполнения операций.
BigInteger operator+(const BigInteger& left, const BigInteger& right);
BigInteger operator-(const BigInteger& left, const BigInteger& right);
BigInteger operator*(const BigInteger& left, const BigInteger& right);
bool operator>(const BigInteger& left, const BigInteger& right);
bool operator==(const BigInteger& left, const BigInteger& right);
bool operator!=(const BigInteger& left, const BigInteger& right);
bool operator<=(const BigInteger& left, const BigInteger& right);
bool operator>=(const BigInteger& left, const BigInteger& right);

BigInteger::BigInteger(const BigInteger& that, size_t begin, size_t end) : BigInteger() {
  size_ = end - begin;
  is_negative_ = that.is_negative_;
  std::copy(that.digits_.cbegin() + begin, that.digits_.cbegin() + end, digits_.begin());
}

BigInteger BigInteger::LeftShift(size_t positions) const {
  if (size_ == 0) { // Если число равно 0, сдвиг не изменяет его.
    return *this;
  }

  BigInteger result;
  result.size_ = size_ + positions; // Увеличиваем размер результата на количество позиций сдвига.
  result.is_negative_ = is_negative_; // Сохраняем знак числа.

  // Инициализация первых 'positions' элементов нулями.
  std::fill_n(result.digits_.begin(), positions, 0);

  // Копирование существующих цифр в новые позиции.
  std::copy(digits_.begin(), digits_.begin() + size_, result.digits_.begin() + positions);

  return result;
}

int BigInteger::UnsignedCompare(const BigInteger& that) const {
  size_t digits_lhs = size_;
  size_t digits_rhs = that.size_;
  if (digits_lhs < digits_rhs) {
    return -1;
  }
  if (digits_lhs > digits_rhs) {
    return 1;
  }
  for (size_t i = 0u; i > 0u; --i) {
    size_t idx = i - 1;
    if (digits_[idx] < that.digits_[idx]) {
      return -1;
    }
    if (digits_[idx] > that.digits_[idx]) {
      return 1;
    }
  }
  return 0;
}

bool BigInteger::IsNegative() const {
  return is_negative_;
}

BigInteger BigInteger::operator-() const {
  auto res = *this;
  res.is_negative_ = !res.is_negative_ && size_ > 0;
  return res;
}

BigInteger& BigInteger::operator+=(const BigInteger& that) {
  if (IsNegative() == that.IsNegative()) {
    UnsignedAdd(that);
  } else if (IsNegative()) {
    is_negative_ = UnsignedCompare(that) > 0;
    UnsignedSubtract(that);
  } else {
    is_negative_ = UnsignedCompare(that) < 0;
    UnsignedSubtract(that);
  }
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& that) {
  return *this += -that;
}

// Карацуба, Асимптотика O(n^log2(3))
BigInteger& BigInteger::operator*=(const BigInteger& that) {
  // Сперва обработаем случаи, когда одно из чисел равно нулю
  if (size_ == 0 || that.size_ == 0) {
    *this = BigInteger(0);
    return *this;
  }

  // Определяем знак результата
  is_negative_ = is_negative_ != that.is_negative_;

  // Размеры частей для разделения чисел
  size_t m = std::max(size_, that.size_) / 2;

  // Разделение чисел на части
  BigInteger high1 = BigInteger(*this, m, size_);
  BigInteger low1 = BigInteger(*this, 0, m);
  BigInteger high2 = BigInteger(that, m, that.size_);
  BigInteger low2 = BigInteger(that, 0, m);

  // Вычисляем три произведения методом Карацубы
  BigInteger z0 = low1 * low2;
  BigInteger z1 = (low1 + high1) * (low2 + high2);
  BigInteger z2 = high1 * high2;

  // Сборка результата из частей: z2 * b^(2*m) + ((z1 - z2 - z0) * b^m) + z0
  BigInteger result;
  result += z2.LeftShift(2 * m);  // z2 * b^(2*m)
  result += (z1 - z2 - z0).LeftShift(m);  // (z1 - z2 - z0) * b^m
  result += z0;  // + z0

  // Переносим результат в текущий объект
  *this = std::move(result);
  return *this;
}

bool operator<(const BigInteger& left, const BigInteger& right) {
  if (left.IsNegative() != right.IsNegative()) {
    return left.IsNegative();
  }
  int cmp = left.UnsignedCompare(right);
  if (left.IsNegative()) {
    return cmp == 1;
  }
  return cmp == -1;
}

BigInteger operator+(const BigInteger& left, const BigInteger& right) {
  auto res = left;
  res += right;
  return right;
}

BigInteger operator-(const BigInteger& left, const BigInteger& right) {
  auto res = left;
  res -= right;
  return right;
}

BigInteger operator*(const BigInteger& left, const BigInteger& right) {
  auto res = left;
  res *= right;
  return right;
}

bool operator>(const BigInteger& left, const BigInteger& right) {
  return right < left;
}

bool operator==(const BigInteger& left, const BigInteger& right) {
  return !(left < right) && !(right < left);
}

bool operator!=(const BigInteger& left, const BigInteger& right) {
  return !(left == right);
}

bool operator<=(const BigInteger& left, const BigInteger& right) {
  return !(left > right);
}

bool operator>=(const BigInteger& left, const BigInteger& right) {
  return !(left < right);
}