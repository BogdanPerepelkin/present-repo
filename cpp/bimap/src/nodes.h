#pragma once

namespace ct::nodes {

struct FakeNode {
  FakeNode* parent_;
  FakeNode* left_;
  FakeNode* right_;

  ~FakeNode() = default;

  FakeNode() noexcept;

  FakeNode(const FakeNode& other) = delete;
  FakeNode& operator=(const FakeNode& other) = delete;
  FakeNode(FakeNode&& other) noexcept = delete;
  FakeNode& operator=(FakeNode&& other) = delete;

  FakeNode* increment();

  FakeNode* decrement();
};

struct SentinelNode : FakeNode {
  SentinelNode() noexcept;

  void exchange(SentinelNode& other) noexcept;
};

} // namespace ct::nodes
