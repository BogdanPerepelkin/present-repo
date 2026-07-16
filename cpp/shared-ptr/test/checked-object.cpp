#include "checked-object.h"

#include <gtest/gtest.h>

namespace ct_test {

namespace {

int transcode(int data, const void* ptr) {
  return data ^ static_cast<int>(reinterpret_cast<std::ptrdiff_t>(ptr) / sizeof(CheckedObject));
}

} // namespace

CheckedObject::CheckedObject(int data)
    : data(transcode(data, this)) {
  auto p = instances.insert(this);
  EXPECT_TRUE(p.second);
}

CheckedObject::CheckedObject(const CheckedObject& other) {
  {
    EXPECT_TRUE(instances.find(&other) != instances.end());
    auto p = instances.insert(this);
    EXPECT_TRUE(p.second);
  }
  data = transcode(transcode(other.data, &other), this);
}

CheckedObject::~CheckedObject() {
  size_t n = instances.erase(this);
  EXPECT_EQ(1u, n);
}

CheckedObject& CheckedObject::operator=(const CheckedObject& c) {
  EXPECT_TRUE(instances.find(this) != instances.end());
  data = transcode(transcode(c.data, &c), this);
  return *this;
}

CheckedObject::operator int() const {
  EXPECT_TRUE(instances.find(this) != instances.end());

  return transcode(data, this);
}

std::set<const CheckedObject*> CheckedObject::instances;

CheckedObject::NoNewInstancesGuard::NoNewInstancesGuard()
    : old_instances(instances) {}

CheckedObject::NoNewInstancesGuard::~NoNewInstancesGuard() {
  EXPECT_TRUE(old_instances == instances);
}

void CheckedObject::NoNewInstancesGuard::expect_no_instances() const {
  EXPECT_TRUE(old_instances == instances);
}

} // namespace ct_test
