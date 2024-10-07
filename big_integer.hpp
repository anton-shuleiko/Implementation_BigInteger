#pragma once
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <type_traits>
#include <vector>

class BigInt {
  // private:
 public:
  std::vector<int64_t> digits;
  bool is_minus = false;
  bool is_null = false;

  //////Constructors///////
  BigInt() = default;
  BigInt(std::string num);
  BigInt(int64_t num);
  BigInt(const BigInt& num) = default;

  //////Operators//////////

  /////////////////////////////////
  BigInt& operator=(const BigInt& num) = default;

  //////////Comparison/////////////

  friend bool operator==(const BigInt& a, const BigInt& b);
  friend bool operator!=(const BigInt& a, const BigInt& b);
  friend bool operator>(const BigInt& a, const BigInt& b);
  friend bool operator>=(const BigInt& a, const BigInt& b);
  friend bool operator<(const BigInt& a, const BigInt& b);
  friend bool operator<=(const BigInt& a, const BigInt& b);
  ////////////////Arithmetics////////////////
  BigInt& operator+=(const BigInt& num);
  friend BigInt operator+(const BigInt& a, const BigInt& b);
  BigInt& operator-=(const BigInt& num);
  friend BigInt operator-(const BigInt& a, const BigInt& b);
  BigInt& operator*=(const BigInt& num);
  friend BigInt operator*(const BigInt& a, const BigInt& b);
  BigInt& operator/=(const BigInt& num);
  friend BigInt operator/(const BigInt& a, const BigInt& b);
  BigInt& operator%=(const BigInt& num);
  friend BigInt operator%(const BigInt& a, const BigInt& b);
  ////////////////Increment and Decrement////////////////
  BigInt& operator++();
  BigInt operator++(int);
  BigInt& operator--();
  BigInt operator--(int);
  ////////////////IOSTREAM//////////////////////////////
  friend std::istream& operator>>(std::istream& is, BigInt& num);
  friend std::ostream& operator<<(std::ostream& os, const BigInt& num);

  //////////////Унарный Минус//////////////////
  BigInt operator-() const;
  /////////////////////////METHODS///////////////////////////////////
  BigInt& DeleteZeros();
  BigInt& SameSignPlus(const BigInt& num);
  BigInt& DiffSignPlus(const BigInt& num);
  BigInt& Div(const BigInt& num);
};
