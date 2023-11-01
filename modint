#include <cassert>
#include <iostream>

template <typename T, T _modulo> class ModInt {
private:
  T _data;

public:
  inline void fix() {
    if (_data < 0) _data += _modulo;
    if (_data >= _modulo) _data -= _modulo;
  }
  ModInt(T _data = 0) : _data(_data % _modulo) {}
  T data() const { return _data; }
  T modulo() const { return _modulo; }
  operator bool() const { return _data != 0; }
  operator T() const { return _data; }
  ModInt operator-() const { return res(fix(_modulo - _data)); }

  template <typename S> friend ModInt operator^(ModInt lhs, S rhs) {
    ModInt res(1);
    for (; rhs; rhs /= 2, lhs *= lhs)
      if (rhs % 2) res *= lhs;
    return res;
  }
  template <typename S> ModInt &operator^=(S exp) {
    ModInt res = *this ^ exp;
    return *this = res, *this;
  }

  ModInt operator~() {
    assert(_data != 0);
    return *this ^ (_modulo - 2);
  }
  ModInt &operator*=(ModInt rhs) { return _data = 1ll * _data * rhs._data % _modulo, *this; }
  ModInt &operator+=(ModInt rhs) { return _data += rhs.data(), fix(), *this; }
  ModInt &operator-=(ModInt rhs) { return _data -= rhs.data(), fix(), *this; }
  ModInt &operator/=(ModInt rhs) { return *this *= ~rhs, *this; }
  friend ModInt operator*(ModInt lhs, ModInt rhs) {
    ModInt res = lhs;
    res *= rhs;
    return res;
  }
  friend ModInt operator+(ModInt lhs, ModInt rhs) {
    ModInt res = lhs;
    res += rhs;
    return res;
  }
  friend ModInt operator-(ModInt lhs, ModInt rhs) {
    ModInt res = lhs;
    res -= rhs;
    return res;
  }
  friend ModInt operator/(ModInt lhs, ModInt rhs) {
    ModInt res = lhs;
    res /= rhs;
    return res;
  }
  template <typename S> ModInt &operator*=(S rhs) { return _data *= ModInt(T(rhs)), *this; }
  template <typename S> ModInt &operator+=(S rhs) { return _data += ModInt(T(rhs)), *this; }
  template <typename S> ModInt &operator-=(S rhs) { return _data -= ModInt(T(rhs)), *this; }
  template <typename S> ModInt &operator/=(S rhs) { return *this /= ModInt(T(rhs)), *this; }
  template <typename S> friend ModInt operator*(ModInt lhs, S rhs) { return lhs * ModInt(T(rhs)); }
  template <typename S> friend ModInt operator+(ModInt lhs, S rhs) { return lhs + ModInt(T(rhs)); }
  template <typename S> friend ModInt operator-(ModInt lhs, S rhs) { return lhs - ModInt(T(rhs)); }
  template <typename S> friend ModInt operator/(ModInt lhs, S rhs) { return lhs / ModInt(T(rhs)); }
  friend std::istream &operator>>(std::istream &is, ModInt &s) { return is >> s._data, s.fix(), is; }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &x) { return os << x.data(); }
  friend bool operator==(ModInt lhs, ModInt rhs) { return lhs.data() == rhs.data(); }
  friend bool operator!=(ModInt lhs, ModInt rhs) { return lhs.data() != rhs.data(); }
};
