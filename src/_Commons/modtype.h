#pragma once
#ifndef MODTYPE_H
#define MODTYPE_H
#include <cstdint>

struct mod_int{
 public:
  int num;
  mod_int() : num(0) {
  }
  mod_int(const std::string& str_val) : num(std::stoi(str_val)) {
  }
  mod_int& operator=(const std::string& str_val) {
    mod_int data;
    data.num = std::stoi(str_val);
    return data;
  }
};


#endif