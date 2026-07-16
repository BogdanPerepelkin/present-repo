#include "checked-object.h"

#include "fault-injection.h"

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
  EXPECT_TRUE(instances.contains(&other));

  auto p = instances.insert(this);
  EXPECT_TRUE(p.second);

  data = transcode(transcode(other.data, &other), this);
}

CheckedObject::~CheckedObject() {
  size_t n = instances.erase(this);
  EXPECT_EQ(n, 1U);
  if (n != 1) {
    ADD_FAILURE() << "Destroying non-existing object at " << static_cast<const void*>(this);
  }
}

CheckedObject& CheckedObject::operator=(const CheckedObject& c) {
  EXPECT_TRUE(instances.contains(this));

  data = transcode(transcode(c.data, &c), this);
  return *this;
}

CheckedObject::operator int() const {
  EXPECT_TRUE(instances.contains(this));

  return transcode(data, this);
}

std::set<const CheckedObject*> CheckedObject::instances;

CheckedObject::NoNewInstancesGuard::NoNewInstancesGuard()
    : old_instances(instances) {}

CheckedObject::NoNewInstancesGuard::~NoNewInstancesGuard() {
  EXPECT_EQ(instances, old_instances);
}

void CheckedObject::NoNewInstancesGuard::expect_no_instances() const {
  EXPECT_EQ(instances, old_instances);
}

} // namespace ct_test
