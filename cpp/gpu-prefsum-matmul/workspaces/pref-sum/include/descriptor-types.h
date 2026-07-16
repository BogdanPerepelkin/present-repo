#pragma once

enum Condition : int {
  X = 0,
  A = 1,
  P = 2,
};

struct Descriptor {
  Descriptor() {
    condition = X;
  }

  int condition;
};
