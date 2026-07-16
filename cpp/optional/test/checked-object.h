#pragma once

#include <set>

namespace ct_test {

struct CheckedObject {
  struct NoNewInstancesGuard;

  CheckedObject() = delete;
  explicit CheckedObject(int data);
  CheckedObject(const CheckedObject& other);
  ~CheckedObject();

  CheckedObject& operator=(const CheckedObject& c);
  operator int() const;

private:
  int data;

  static std::set<const CheckedObject*> instances;
};

struct CheckedObject::NoNewInstancesGuard {
  NoNewInstancesGuard();

  NoNewInstancesGuard(const NoNewInstancesGuard&) = delete;
  NoNewInstancesGuard& operator=(const NoNewInstancesGuard&) = delete;

  ~NoNewInstancesGuard();

  void expect_no_instances() const;

private:
  std::set<const CheckedObject*> old_instances;
};

} // namespace ct_test
