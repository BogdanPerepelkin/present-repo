#pragma once

#include "intrusiveTree.h"

#include <pstl/execution_defs.h>

#include <queue>

#include <concepts>
#include <cstddef>
#include <functional>
#include <ios>
#include <iostream>
#include <ostream>
#include <stdexcept>

// все, все переписываем

namespace ct {

template <
    typename TLeft,
    typename TRight,
    typename CompareLeft = std::less<TLeft>,
    typename CompareRight = std::less<TRight>>
class Bimap {
public:
  using Left = TLeft;
  using Right = TRight;

  class LeftTag;
  class RightTag;

  template <typename Image>
  class AbsIterator;

  using LeftIterator = typename Tree<Left, LeftTag>::Iterator;
  using RightIterator = typename Tree<Right, RightTag>::Iterator;

  class Node
      : public TreeEl<Left>
      , public TreeEl<Right> {
  public:
    ~Node() = default;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&& other) = delete;
    Node& operator=(Node&& other) = delete;

    template <typename LVal, typename RVal>
    Node(LVal&& left, RVal&& right)
        : leftValue_(std::forward<LVal>(left))
        , rightValue_(std::forward<RVal>(right)) {}

    // void unlink() {
    //   FakeNode* fake = this;
    //   fake->parent_ = nullptr;
    //   fake->left_ = nullptr;
    //   fake->right_ = nullptr;
    // }

  private:
    Left leftValue_;
    Right rightValue_;

    template <typename U>
    friend class Bimap::AbsIterator;

    template <typename U>
    friend class Bimap::Node;

    friend class Bimap;
  };

public:
  Bimap()
  {
    std::cout << "con empty" << std::endl;
  }

  Bimap(CompareLeft compareLeft)
    requires (std::copyable<CompareLeft>)
      : fakeLeft_(compareLeft) {
    std::cout << "con single" << std::endl;
  }

  template <typename ComL, typename ComR>
    requires (std::constructible_from<CompareLeft, ComL &&> && std::constructible_from<CompareRight, ComR &&>)
  Bimap(ComL&& compareLeft, ComR&& compareRight)
      : fakeLeft_(std::forward<ComL>(compareLeft))
      , fakeRight_(std::forward<ComR>(compareRight)) {
    std::cout << "con forward" << std::endl;
  }

  // void copyTree(NodeImpl* othLeftTree) {
  //   std::queue<NodeImpl*> queue;
  //   queue.push(othLeftTree);
  //   while (!queue.empty()) {
  //     NodeImpl* cur = queue.front();
  //     queue.pop();
  //     Node* valueNode = static_cast<Node*>(cur);
  //     insert(valueNode->value_, valueNode->front_->value_);
  //     if (cur->left_) {
  //       queue.push(cur->left_);
  //     }
  //     if (cur->right_) {
  //       queue.push(cur->right_);
  //     }
  //   }
  // }

  Bimap(const Bimap& other)
      : compareLeft_(other.compareLeft_)
      , compareRight_(other.compareRight_) {
    if (other.leftTree() == nullptr) {
      return;
    }
    copyTree(other.leftTree());
  }

  Bimap(Bimap&& other) noexcept
      : compareLeft_(std::move(other.compareLeft_))
      , compareRight_(std::move(other.compareRight_)) {
    swap(other);
  }

  Bimap& operator=(const Bimap& other) {
    if (this != &other) {
      Bimap tmp(other);
      tmp.swap(*this);
    }
    return *this;
  }

  Bimap& operator=(Bimap&& other) noexcept {
    if (this != &other) {
      swap(other);
    }
    return *this;
  }

  template <typename Val>
  void dtor(FakeNode* root) {
    if (root == nullptr) {
      return;
    }
    std::queue<FakeNode*> queue;
    queue.push(root);
    while (!queue.empty()) {
      FakeNode* cur = queue.front();
      queue.pop();
      if (cur->left_) {
        queue.push(cur->left_);
      }
      if (cur->right_) {
        queue.push(cur->right_);
      }
      delete static_cast<Node<Val>*>(cur);
    }
  }

  // done
  ~Bimap() {
    dtor<Left>(leftTree());
    dtor<Right>(rightTree());
  }

  void swap(Bimap& other) noexcept {
    std::swap(amount, other.amount);
    std::swap(compareLeft_, other.compareLeft_);
    std::swap(compareRight_, other.compareRight_);

    std::swap(fakeLeft_.left_, other.fakeLeft_.left_);
    std::swap(fakeRight_.left_, other.fakeRight_.left_);

    if (fakeLeft_.left_) {
      fakeLeft_.left_->parent_ = &other.fakeLeft_;
    }
    if (fakeRight_.left_) {
      fakeRight_.left_->parent_ = &other.fakeRight_;
    }
    if (other.fakeLeft_.left_) {
      other.fakeLeft_.left_->parent_ = &fakeLeft_;
    }
    if (other.fakeRight_.left_) {
      other.fakeRight_.left_->parent_ = &fakeRight_;
    }
  }

  friend void swap(Bimap& lhs, Bimap& rhs) noexcept {
    lhs.swap(rhs);
  }

  template <typename Val, typename Comparator>
  void insertImpl(Node<Val>* node, FakeNode* fakeRoot, Comparator& comp) {
    Node<Val>* cur = static_cast<Node<Val>*>(fakeRoot);
    Node<Val>* par = cur;
    while (cur != nullptr) {
      par = cur;
      if (comp(node->value_, cur->value_)) {
        cur = static_cast<Node<Val>*>(cur->left_);
      } else {
        cur = static_cast<Node<Val>*>(cur->right_);
      }
    }
    node->parent_ = par;
    if (comp(node->value_, par->value_)) {
      par->left_ = node;
    } else {
      par->right_ = node;
    }
  }

  template <typename L, typename R>
  LeftIterator insert(L&& left, R&& right) {
    if (findNode<Left>(leftTree(), left, compareLeft_) != nullptr ||
        findNode<Right>(rightTree(), right, compareRight_) != nullptr) {
      return end_left();
    }

    amount++;

    Node* newNode = new Node(std::forward<L>(left), std::forward<R>(right));
    // Node<Left>* newLeft = new Node<Left>(std::forward<L>(left));
    // Node<Right>* newRight = new Node<Right>(std::forward<R>(right));
    // newLeft->front_ = newRight;
    // newRight->front_ = newLeft;

    if (leftTree() == nullptr) {
      fakeLeft_.left_ = newLeft;
      newLeft->parent_ = &fakeLeft_;
      fakeRight_.left_ = newRight;
      newRight->parent_ = &fakeRight_;
      return LeftIterator(newLeft);
    }

    // insertImpl<Left>(newLeft, leftTree(), compareLeft_);
    // insertImpl<Right>(newRight, rightTree(), compareRight_);

    // я в левое дерево передам ноду, которая имплиментит и левое и правое,
    // а приму в левом как левое. А затем то же самое для правого
    fakeLeft_.insert(...)
    fakeRight_.insert(...)

    return LeftIterator(newLeft);
  }

  LeftIterator insert(Left&& left, Right&& right) {
    std::cout << "in proxy insert" << std::endl;
    return insert<Left, Right>(std::move(left), std::move(right));
  }

  // stock
  // LeftIterator insert(const Left& left, const Right& right) {}
  // LeftIterator insert(const Left& left, Right&& right);
  // LeftIterator insert(Left&& left, const Right& right);
  // LeftIterator insert(Left&& left, Right&& right);

  template <typename Val>
  void eraseImpl() {
    // --amount;

    // TODO...
  }

  LeftIterator erase_left(LeftIterator it) {}

  RightIterator erase_right(RightIterator it);

  bool erase_left(const Left& left);
  bool erase_right(const Right& right);

  LeftIterator erase_left(LeftIterator first, LeftIterator last) {}

  RightIterator erase_right(RightIterator first, RightIterator last) {}

  // done copypaste
  LeftIterator find_left(const Left& left) const {
    Node<Left>* found = findNode<Left>(leftTree(), left, compareLeft_);
    if (found == nullptr) {
      return end_left();
    }
    return LeftIterator(found);
  }

  // done copypaste
  RightIterator find_right(const Right& right) const {
    Node<Right>* found = findNode<Right>(rightTree(), right, compareRight_);
    if (found == nullptr) {
      return end_right();
    }
    return RightIterator(found);
  }

  // done
  const Right& at_left(const Left& key) const {
    return atSide<Left, Right>(leftTree(), key, compareLeft_);
  }

  // done
  const Left& at_right(const Right& key) const {
    return atSide<Right, Left>(rightTree(), key, compareRight_);
  }

  // last
  const Right& at_left_or_default(const Left& key) {}

  // last
  const Left& at_right_or_default(const Right& key) {}

  LeftIterator lower_bound_left(const Left& left) const {
    // auto& comparator = getComp<Left>();
    // Node<Left>* cur = leftTree();
    // while (cur) {
    //   if (comparator(cur->keyNode_->value_, left)) {
    //     cur = cur->right_;
    //   } else {
    //     break;
    //   }
    // }
    // return LeftIterator(cur);
  }

  LeftIterator upper_bound_left(const Left& left) const {
    // auto& comparator = getComp<Left>();
    // Node<Left>* cur = leftTree();
    // while (cur) {
    //   if (comparator(left, cur->keyNode_->value_)) {
    //     break;
    //   }
    //   cur = cur->left_;
    // }
    // return LeftIterator(cur);
  }

  RightIterator lower_bound_right(const Right& right) const;
  RightIterator upper_bound_right(const Right& right) const;

  LeftIterator begin_left() const {
    return beginSide<Left, LeftIterator>(leftTree());
  }

  // copypaste
  RightIterator begin_right() const {
    return beginSide<Right, RightIterator>(rightTree());
  }

  LeftIterator end_left() const {
    return LeftIterator(&fakeLeft_); // mb unique
  }

  RightIterator end_right() const {
    return RightIterator(&fakeRight_); // mb unique
  }

  // по счетчику
  bool empty() const {
    return leftTree() == nullptr;
  }

  // счетчик
  std::size_t size() const {
    return amount;
  }

  friend bool operator==(const Bimap& lhs, const Bimap& rhs) {
    // TODO проверка на size
    for (LeftIterator it = lhs.begin_left(); it != lhs.end_left(); ++it) {
      if (rhs.find_left(*it) == rhs.end_left() || rhs.find_right(*it.flip()) == rhs.end_right()) {
        return false;
      }
      // из-за return_type find
      // if (rhs.find_left(*it) == rhs.end_left() || rhs.find_right(it.flip()) == rhs.end_right()) {
      // return false;
      // }
    }
    return true;
  }

  friend bool operator!=(const Bimap& lhs, const Bimap& rhs) {
    return !(lhs == rhs);
  }

private:
  class ValOfLeft {
    const auto& operator()(const Node& node) const noexcept {
      return node.leftValue_;
    }
  };

  class ValOfRight {
    const auto& operator()(const Node& node) const noexcept {
      return node.leftValue_;
    }
  };

  Tree<Node, CompareLeft, ValOfLeft, LeftTag> fakeLeft_;
  Tree<Node, CompareRight, ValOfRight, RightTag> fakeRight_;

  // CompareLeft compareLeft_;
  // CompareRight compareRight_;
  size_t amount{0};


  FakeNode* leftTree() const {
    return fakeLeft_.left_;
  }

  FakeNode* rightTree() const {
    return fakeRight_.left_;
  }

  template <typename Val>
  Node<Val>* asNode(FakeNode* fake) const {
    return static_cast<Node<Val>*>(fake);
  }

  template <typename ResT, typename Val, typename Comparator>
  Node<ResT>* findNode(FakeNode* fakeNode, const Val& key, const Comparator& comp) const {
    Node<Val>* root = asNode<Val>(fakeNode);
    while (root) {
      if (comp(key, root->value_)) {
        root = static_cast<Node<Val>*>(root->left_);
      } else if (comp(root->value_, key)) {
        root = static_cast<Node<Val>*>(root->right_);
      } else {
        return root;
      }
    }
    return nullptr;
  }

  template <typename Val, typename Front, typename Comparator>
  const Front& atSide(FakeNode* fakeNode, const Val& key, Comparator comp) const {
    Node<Val>* found = findNode<Val>(fakeNode, key, comp);
    if (found == nullptr) {
      throw std::out_of_range("atSide");
    }
    return found->front_->value_;
  }

  template <typename Val, typename It>
  It beginSide(FakeNode* root) const {
    // зачем здесь так много asNode???
    // достаточно только в return
    Node<Val>* cur = asNode<Val>(root);
    while (cur->left_ != nullptr) {
      cur = asNode<Val>(cur->left_);
    }
    return It(cur);
  }
};

} // namespace ct
