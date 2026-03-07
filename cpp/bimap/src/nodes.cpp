#include "nodes.h"

#include <functional>

namespace ct::nodes {

FakeNode::FakeNode() noexcept
    : parent_(this)
    , left_(nullptr)
    , right_(nullptr) {}

SentinelNode::SentinelNode() noexcept {}

void SentinelNode::exchange(SentinelNode& other) noexcept {
  if (this->left_) {
    this->left_->parent_ = &other;
  }
  if (other.left_) {
    other.left_->parent_ = this;
  }
  std::swap(this->left_, other.left_);
}

FakeNode* FakeNode::increment() {
  FakeNode* cur = this;
  if (cur->right_) {
    cur = cur->right_;
    while (cur->left_) {
      cur = cur->left_;
    }
  } else {
    while (cur->parent_ && cur->parent_->right_ == cur) {
      cur = cur->parent_;
    }
    cur = cur->parent_;
  }
  return cur;
}

FakeNode* FakeNode::decrement() {
  FakeNode* cur = this;
  if (cur->left_) {
    cur = cur->left_;
    while (cur->right_) {
      cur = cur->right_;
    }
  } else {
    while (cur->parent_ && cur->parent_->left_ == cur) {
      cur = cur->parent_;
    }
    cur = cur->parent_;
  }
  return cur;
}

} // namespace ct::nodes
