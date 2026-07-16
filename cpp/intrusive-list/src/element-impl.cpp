#include "element-impl.h"

#include <utility>

namespace ct::intrusive::utils {
ElementImpl::ElementImpl() noexcept {
  prev_ = next_ = this;
}

ElementImpl::ElementImpl(ElementImpl&& other) noexcept
    : ElementImpl() {
  move(std::move(other));
}

ElementImpl::ElementImpl(const ElementImpl& /*unused*/) noexcept
    : ElementImpl() {}

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
  prev_ = that.prev_;
  next_ = &that;
  that.prev_->next_ = this;
  that.prev_ = this;
}

void ElementImpl::unlink() noexcept {
  prev_->next_ = next_;
  next_->prev_ = prev_;
  prev_ = next_ = this;
}

void ElementImpl::move(ElementImpl&& other) noexcept {
  prev_ = other.prev_;
  next_ = other.next_;
  if (other.prev_ != &other || other.next_ != &other) {
    other.prev_->next_ = this;
    other.next_->prev_ = this;
  }
  other.prev_ = other.next_ = &other;
}
} // namespace ct::intrusive::utils
