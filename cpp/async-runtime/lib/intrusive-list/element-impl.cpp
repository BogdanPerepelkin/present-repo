#include "element-impl.h"

#include <utility>

namespace ct::intrusive::utils {

ElementImpl::ElementImpl() noexcept {
  _prev = _next = this;
}

ElementImpl::ElementImpl(ElementImpl&& other) noexcept
    : ElementImpl() {
  move(std::move(other));
}

ElementImpl::ElementImpl(const ElementImpl& other) noexcept = default;

ElementImpl::~ElementImpl() {
  unlink();
}

ElementImpl& ElementImpl::operator=(ElementImpl&& other) noexcept {
  if (this != &other) {
    unlink();
    move(std::move(other));
  }
  return *this;
}

ElementImpl& ElementImpl::operator=(const ElementImpl& other) noexcept {
  if (this != &other) {
    unlink();
  }
  return *this;
}

void ElementImpl::linkBefore(ElementImpl& that) noexcept {
  unlink();
  _prev = that._prev;
  _next = &that;
  that._prev->_next = this;
  that._prev = this;
}

void ElementImpl::unlink() noexcept {
  _prev->_next = _next;
  _next->_prev = _prev;
  _prev = _next = this;
}

void ElementImpl::move(ElementImpl&& other) noexcept {
  _prev = other._prev;
  _next = other._next;
  if (other._prev != &other || other._next != &other) {
    other._prev->_next = this;
    other._next->_prev = this;
  }
  other._prev = other._next = &other;
}

} // namespace ct::intrusive::utils
