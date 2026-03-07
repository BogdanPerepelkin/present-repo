#include "test-classes.h"

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

namespace ct_test {

std::unordered_set<const AddressCheckingObject*> AddressCheckingObject::addresses;

void AddressCheckingObject::add_instance() const {
  auto [it, was_inserted] = addresses.insert(this);
  if (!was_inserted) {
    // clang-format off
    FAIL(
        "New object is created at the address " << static_cast<const void*>(this)
        << " while the previous object at this address was not destroyed"
    );
    // clang-format on
  }
}

void AddressCheckingObject::remove_instance(bool nothrow) const {
  size_t erased_count = addresses.erase(this);
  if (!nothrow && erased_count != 1) {
    FAIL("Destroying non-existing object at the address " << static_cast<const void*>(this));
  }
}

void AddressCheckingObject::assert_exists() const {
  if (!addresses.contains(this)) {
    FAIL("Accessing an non-existing object at address " << static_cast<const void*>(this));
  }
}

void AddressCheckingObject::expect_no_instances() {
  if (!addresses.empty()) {
    addresses.clear();
    FAIL("Not all instances are destroyed");
  }
}

size_t AddressCheckingObject::copy_throw_countdown = 0;

void AddressCheckingObject::process_copying() {
  if (copy_throw_countdown != 0) {
    if (--copy_throw_countdown == 0) {
      throw std::runtime_error("address_checking_object copying failed");
    }
  }
}

void AddressCheckingObject::set_copy_throw_countdown(size_t new_countdown) {
  copy_throw_countdown = new_countdown;
}

AddressCheckingObject::operator int() const {
  assert_exists();
  return value;
}

AddressCheckingObject::AddressCheckingObject() {
  add_instance();
}

AddressCheckingObject::AddressCheckingObject(int value)
    : value(value) {
  add_instance();
}

AddressCheckingObject::AddressCheckingObject(const AddressCheckingObject& other)
    : value(other.value) {
  process_copying();
  add_instance();
}

AddressCheckingObject& AddressCheckingObject::operator=(const AddressCheckingObject& other) {
  assert_exists();
  other.assert_exists();
  process_copying();
  value = other.value;
  return *this;
}

AddressCheckingObject::~AddressCheckingObject() noexcept(false) {
  remove_instance(std::uncaught_exceptions() > 0);
}

} // namespace ct_test
