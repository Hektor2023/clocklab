#pragma once

#include <Arduino.h>
#include <array>

//===================================================================================
template <uint8_t maxSize> 
class LimitedSizeString {
private:
  std::array<char, maxSize> array;

public:
  LimitedSizeString(void);
  LimitedSizeString(const char *cstr);

  uint8_t getMaxLen(void);
  const char *c_str(void);
  bool operator==(LimitedSizeString<maxSize> &rhs);
  bool operator!=(LimitedSizeString<maxSize> &rhs);

  LimitedSizeString<maxSize> &
  operator=(const LimitedSizeString<maxSize> &other);
};

//===================================================================================
template <uint8_t maxSize> LimitedSizeString<maxSize>::LimitedSizeString(void) {
  strncpy(array.data(), "", maxSize);
}

//===================================================================================
template <uint8_t maxSize>
LimitedSizeString<maxSize>::LimitedSizeString(const char *cstr) {
  assert(cstr != nullptr);
  assert(strlen(cstr) <= (maxSize - 1));

  strncpy(array.data(), cstr, maxSize);
}

//===================================================================================
template <uint8_t maxSize> const char *LimitedSizeString<maxSize>::c_str(void) {
  return array.data();
}

//===================================================================================
template <uint8_t maxSize>
LimitedSizeString<maxSize> &
LimitedSizeString<maxSize>::operator=(const LimitedSizeString<maxSize> &other) {
  // Guard self assignment
  if (this == &other)
    return *this;

  strncpy(array.data(), other.array.data(), maxSize);
  return *this;
}

//===================================================================================
template <uint8_t maxSize> 
uint8_t LimitedSizeString<maxSize>::getMaxLen(void) {
  return maxSize - 1;
}

//===================================================================================
template <uint8_t maxSize>
bool LimitedSizeString<maxSize>::operator==(LimitedSizeString<maxSize> &rhs) {
  return strncmp(c_str(), rhs.c_str(), maxSize) == 0;
}

//===================================================================================
template <uint8_t maxSize>
bool LimitedSizeString<maxSize>::operator!=(LimitedSizeString<maxSize> &rhs) {
  return strncmp(c_str(), rhs.c_str(), maxSize) != 0;
}

//===================================================================================