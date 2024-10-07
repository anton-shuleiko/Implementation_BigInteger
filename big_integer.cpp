#include "big_integer.hpp"

const int kTen = 10;
///////////////Constructers////////////////////////
BigInt::BigInt(std::string num) {
  if (num.empty()) {
    return;
  }
  if (num[0] == '-') {
    this->is_minus = true;
    num = num.substr(1, num.size() - 1);
  }
  std::vector<int64_t> d;
  for (size_t i = 0; i < num.size(); i++) {
    d.push_back(num[i] - '0');
  }
  while (!d.empty()) {
    this->digits.push_back(d[d.size() - 1]);
    d.pop_back();
  }
  if (digits.size() == 1 && digits[0] == 0) {
    this->is_minus = false;
    this->is_null = true;
  }
}

BigInt::BigInt(int64_t num) {
  std::string s;
  s = std::to_string(num);
  *this = BigInt(s);
}

/////////////////Operators//////////////////////////

///////////////////////////////////////////////////////

//////////Comparison/////////////

bool operator==(const BigInt& a, const BigInt& b) {
  return (a.is_minus == b.is_minus && a.is_null == b.is_null &&
          a.digits == b.digits);
}

bool operator!=(const BigInt& a, const BigInt& b) { return !(a == b); }

bool operator>(const BigInt& a, const BigInt& b) {
  if (a.is_minus != b.is_minus) {
    return !a.is_minus;
  }
  if (a.digits.size() != b.digits.size()) {
    return a.digits.size() > b.digits.size() && !a.is_minus;
  }
  int64_t n = a.digits.size() - 1;
  while (n >= 0) {
    if (a.digits[n] != b.digits[n]) {
      return a.digits[n] > b.digits[n] && !a.is_minus;
    }
    n--;
  }
  return a.digits[0] > b.digits[0] && !a.is_minus;
}

bool operator>=(const BigInt& a, const BigInt& b) { return (a > b || a == b); }

bool operator<(const BigInt& a, const BigInt& b) { return !(a >= b); }

bool operator<=(const BigInt& a, const BigInt& b) { return !(a > b); }

////////////////Arithmetics////////////////

BigInt& BigInt::operator+=(const BigInt& num) {
  this->SameSignPlus(num);
  this->DiffSignPlus(num);
  this->DeleteZeros();
  return *this;
}

BigInt operator+(const BigInt& a, const BigInt& b) {
  BigInt res = a;
  res += b;
  return res;
}

BigInt& BigInt::operator-=(const BigInt& num) {
  BigInt n = num;
  n.is_minus ^= 1;
  *this += n;
  return *this;
}

BigInt operator-(const BigInt& a, const BigInt& b) {
  BigInt res = a;
  res -= b;
  return res;
}

BigInt& BigInt::operator*=(const BigInt& num) {
  if (this->is_null || num.is_null) {
    *this = BigInt(0);
    return *this;
  }
  this->is_minus = num.is_minus != this->is_minus;
  std::vector<int64_t> res(this->digits.size() + num.digits.size(), 0);
  for (size_t i = 0; i < this->digits.size(); i++) {
    for (size_t j = 0; j < num.digits.size(); j++) {
      res[i + j] += this->digits[i] * num.digits[j];
    }
  }
  int64_t x = 0;
  for (size_t i = 0; i < res.size(); i++) {
    res[i] += x;
    x = res[i] / (kTen);
    res[i] -= x * kTen;
  }
  while (x > 0) {
    res.push_back(x % kTen);
    x /= kTen;
  }
  this->digits = res;
  while (this->digits[this->digits.size() - 1] == 0) {
    this->digits.pop_back();
  }
  return *this;
}

BigInt operator*(const BigInt& a, const BigInt& b) {
  BigInt res = a;
  res *= b;
  return res;
}

BigInt& BigInt::operator/=(const BigInt& num) {
  if (num.is_null) {
    throw("Arithmetic Error: Division By 0");
  }
  BigInt secondmodule = num;
  secondmodule.is_minus = false;
  BigInt firstmodule = *this;
  firstmodule.is_minus = false;
  if (secondmodule > firstmodule) {
    *this = BigInt(0);
    return *this;
  }
  if (secondmodule == firstmodule) {
    *this = BigInt(1);
    this->is_minus = num.is_minus != this->is_minus;
    return *this;
  }
  return this->Div(num);
}

BigInt operator/(const BigInt& a, const BigInt& b) {
  BigInt res = a;
  res /= b;
  return res;
}

BigInt& BigInt::operator%=(const BigInt& num) {
  BigInt res = *this / num;
  *this -= res * num;
  return *this;
}

BigInt operator%(const BigInt& a, const BigInt& b) {
  BigInt res = a;
  res %= b;
  return res;
}

////////////////Increment and Decrement////////////////
BigInt& BigInt::operator++() {
  *this += 1;
  return *this;
}
BigInt BigInt::operator++(int) {
  BigInt res = *this;
  ++(*this);
  return res;
}
BigInt& BigInt::operator--() {
  *this -= 1;
  return *this;
}
BigInt BigInt::operator--(int) {
  BigInt res = *this;
  --(*this);
  return res;
}
//////////////Унарный Минус//////////////////
BigInt BigInt::operator-() const { return BigInt(0) - *this; }

////////////////////IOSTREAM////////////////////////
std::istream& operator>>(std::istream& is, BigInt& num) {
  std::string s;
  is >> s;
  num = BigInt(s);
  return is;
}
std::ostream& operator<<(std::ostream& os, const BigInt& num) {
  // num.DeleteZeros();
  if (num.is_minus) {
    os << "-";
  }
  for (size_t i = num.digits.size() - 1; i > 0; i--) {
    os << num.digits[i];
  }
  os << num.digits[0];
  return os;
}

///////////////////////METHODS/////////////////////////////
BigInt& BigInt::DeleteZeros() {
  if (!this->digits.empty()) {
    while (this->digits.back() == 0 && this->digits.size() > 1) {
      this->digits.pop_back();
    }
    if (this->digits.size() == 1 && this->digits.back() == 0) {
      this->is_null = true;
      this->is_minus = false;
    } else {
      this->is_null = false;
    }
  }
  return *this;
}

BigInt& BigInt::SameSignPlus(const BigInt& num) {
  if (this->is_minus == num.is_minus) {
    size_t n = this->digits.size();
    size_t m = num.digits.size();
    int64_t x = 0;
    if (m > n) {
      for (size_t i = n; i < m; i++) {
        this->digits.push_back(0);
      }
    }
    for (size_t i = 0; i < this->digits.size(); i++) {
      if (i < m) {
        this->digits[i] += num.digits[i] + x;
      } else {
        this->digits[i] += x;
      }
      x = this->digits[i] / kTen;
      this->digits[i] %= kTen;
    }
    if (x > 0) {
      this->digits.push_back(x);
    }
  }
  return *this;
}

BigInt& BigInt::DiffSignPlus(const BigInt& num) {
  if (this->is_minus != num.is_minus) {
    BigInt a = *this;
    BigInt b = num;
    a.is_minus = false;
    b.is_minus = false;
    if (a < b) {
      std::swap(a.digits, b.digits);
      std::swap(a.is_null, b.is_null);
      this->is_minus = num.is_minus;
    }
    size_t n = a.digits.size();
    size_t m = b.digits.size();
    int64_t x = 0;
    digits.resize(m, 0);
    for (size_t i = 0; i < a.digits.size(); i++) {
      if (i < m) {
        a.digits[i] -= b.digits[i] + x;
      } else {
        a.digits[i] -= x;
      }
      x = 0;
      if (a.digits[i] < 0) {
        x = 1;
        a.digits[i] += kTen;
      }
    }
    this->digits = a.digits;
  }
  return *this;
}

BigInt& BigInt::Div(const BigInt& num) {
  this->is_minus = num.is_minus != this->is_minus;
  BigInt ans = BigInt(0);
  BigInt del = BigInt(0);
  int64_t x = this->digits.size() - 1;
  BigInt a = num;
  a.is_minus = false;
  while (x >= 0) {
    del *= kTen;
    del += BigInt(this->digits[x--]);
    int64_t i = 0;
    while (del >= (a * i)) {
      i++;
    }
    i--;
    ans *= kTen;
    ans += i;
    del -= (a * i);
  }
  this->digits = ans.digits;
  this->DeleteZeros();
  return *this;
}
