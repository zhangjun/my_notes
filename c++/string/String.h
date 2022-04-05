#pragma once
#include <cstring>

class String {
public:
  char *data_ = nullptr;

public:
  String(const char *str = nullptr);
  ~String();
  String(const String &other);
  String &operator=(const String &other);
  String(const String &&other);
  String &operator=(const String &&other);
};

String::String(const char *str) {
  if (str == nullptr) {
    data_ = new char[1];
    *data_ = '\0';
  } else {
    int length = strlen(str);
    data_ = new char[length + 1];
    // strcpy(data_, str);
    memcpy(data_, str, length);
  }
}

String::~String() {
  if (data_ != nullptr) {
    delete[] data_;
    data_ = nullptr;
  }
}

String::String(const String &other) {
  int length = strlen(other.data_);
  data_ = new char[length + 1];
  memcpy(data_, other.data_, length);
}

String &String::operator=(const String &other) {
  if (this == &other)
    return *this;
  if (data_ != nullptr)
    delete[] data_;
  int length = strlen(other.data_);
  data_ = new char[length + 1];
  memcpy(data_, other.data_, length);
  return *this;
}

String::String(const String &&other) {
  data_ = other.data_;
  other.data_ = nullptr;
}

String &String::operator=(const String &&other) {
  if (this != &other) {
    if (data_ != nullptr)
      delete[] data_;
  }
  data_ = other.data_;
  other.data_ = nullptr;
  return *this;
}
