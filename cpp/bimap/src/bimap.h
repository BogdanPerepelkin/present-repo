#pragma once

#include "nodes.h"

#include <queue>

#include <memory>
#include <stdexcept>

namespace ct {

class LeftTag;
class RightTag;

template <
    typename TLeft,
    typename TRight,
    typename CompareLeft = std::less<TLeft>,
    typename CompareRight = std::less<TRight>>
class Bimap {
private:
  struct DelegateCtor {};

  using Left = TLeft;
  using Right = TRight;

  using FakeNode = nodes::FakeNode;

  using SentinelNode = nodes::SentinelNode;

  template <typename Val, typename Tag>
  struct ValNode;

  struct BiNode;

  using LeftNode = ValNode<Left, LeftTag>;
  using RightNode = ValNode<Right, RightTag>;

  template <typename Tag, typename Comparator>
  class TreeImpl;

  template <typename Val, typename Tag>
  struct ValNode : FakeNode {
    Val value_;

    ~ValNode() = default;

    template <typename V>
    ValNode(V&& value)
        : value_(std::forward<V>(value)) {}
  };

  struct BiNode
      : ValNode<Left, LeftTag>
      , ValNode<Right, RightTag> {
    template <typename L, typename R>
    BiNode(L&& left, R&& right)
        : ValNode<Left, LeftTag>(std::forward<L>(left))
        , ValNode<Right, RightTag>(std::forward<R>(right)) {}
  };

  template <typename Tag>
  class AbsIterator;

public:
  using LeftIterator = AbsIterator<LeftTag>;
  using RightIterator = AbsIterator<RightTag>;

private:
  template <typename Tag>
  class AbsIterator {
  public:
    using Val = std::conditional_t<std::same_as<Tag, LeftTag>, Left, Right>;
    using Front = std::conditional_t<std::same_as<Tag, LeftTag>, Right, Left>;
    using FrontTag = std::conditional_t<std::same_as<Tag, LeftTag>, RightTag, LeftTag>;
    using Node = ValNode<Val, Tag>;

    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_cvref_t<Val>;
    using pointer = const Val*;
    using reference = const Val&;
    using iterator_category = std::bidirectional_iterator_tag;
    using ConstIterator = AbsIterator<const Val>;

    AbsIterator() = default;

    ~AbsIterator() = default;

    const Val& operator*() const {
      return asNode(this->node_)->value_;
    }

    const Val* operator->() const {
      return &asNode(node_)->value_;
    }

    AbsIterator& operator++() {
      node_ = node_->increment();
      return *this;
    }

    AbsIterator operator++(int) {
      AbsIterator tmp = *this;
      ++*this;
      return tmp;
    }

    AbsIterator& operator--() {
      node_ = node_->decrement();
      return *this;
    }

    AbsIterator operator--(int) {
      AbsIterator tmp = *this;
      --*this;
      return tmp;
    }

    std::conditional_t<std::same_as<Tag, LeftTag>, RightIterator, LeftIterator> flip() const noexcept {
      using ResIterator = std::conditional_t<std::same_as<Tag, LeftTag>, RightIterator, LeftIterator>;
      using ThisComparator = std::conditional_t<std::same_as<Tag, LeftTag>, CompareLeft, CompareRight>;
      using FrontComparator = std::conditional_t<std::same_as<Tag, LeftTag>, CompareRight, CompareLeft>;
      if (node_->parent_ == node_) {
        return ResIterator(
            static_cast<TreeImpl<FrontTag, FrontComparator>*>(
                static_cast<Tree*>(static_cast<TreeImpl<Tag, ThisComparator>*>(static_cast<SentinelNode*>(node_)))
            )
        );
      }
      return ResIterator(static_cast<ValNode<Front, FrontTag>*>(static_cast<BiNode*>(asNode(node_))));
    }

    friend bool operator==(const AbsIterator& lhs, const AbsIterator& rhs) = default;

  private:
    FakeNode* node_;

    explicit AbsIterator(const Node* node) noexcept
        : node_(const_cast<Node*>(node)) {}

    explicit AbsIterator(const SentinelNode* sentinel) noexcept
        : node_(const_cast<SentinelNode*>(sentinel)) {}

    Node* asNode(FakeNode* fake) const {
      return static_cast<Node*>(fake);
    }

    friend class Bimap;

    template <typename Tag2, typename Comparator>
    friend class Bimap::TreeImpl;
  };

  struct InsertBuf {
    bool leftKid_;
    FakeNode* par_;
    bool try_;
  };

  template <typename Tag, typename Comparator>
  class TreeImpl : SentinelNode {
  public:
    using Val = std::conditional_t<std::same_as<Tag, LeftTag>, Left, Right>;
    using Front = std::conditional_t<std::same_as<Tag, LeftTag>, Right, Left>;
    using Iterator = std::conditional_t<std::same_as<Tag, LeftTag>, LeftIterator, RightIterator>;
    using FrontIterator = std::conditional_t<std::same_as<Tag, LeftTag>, RightIterator, LeftIterator>;
    using Node = ValNode<Val, Tag>;
    using FrontTag = std::conditional_t<std::same_as<Tag, LeftTag>, RightTag, LeftTag>;

    ~TreeImpl() = default;

    TreeImpl() noexcept = default;

    template <typename C>
    explicit TreeImpl(C&& comp)
        : comp_(std::forward<C>(comp)) {}

    TreeImpl(const TreeImpl& other) = delete;

    TreeImpl(TreeImpl&& other)
        : comp_(std::move(other.comp_)) {
      exchange(other);
    }

    void swap(TreeImpl& other) noexcept {
      using std::swap;
      swap(comp_, other.comp_);
      exchange(other);
    }

    InsertBuf insertImplFind(const Val& val) {
      bool flag = true;
      if (left_ == nullptr) {
        return InsertBuf{.leftKid_ = true, .par_ = this, .try_ = flag};
      }

      FakeNode* cur = left_;
      FakeNode* par = cur;
      while (cur != nullptr) {
        par = cur;
        if (comp_(val, asNode(cur)->value_)) {
          cur = cur->left_;
        } else if (comp_(asNode(cur)->value_, val)) {
          cur = cur->right_;
        } else {
          cur = cur->right_;
          flag = false;
        }
      }

      bool leftKid = false;
      if (comp_(val, asNode(par)->value_)) {
        leftKid = true;
      }
      return InsertBuf{.leftKid_ = leftKid, .par_ = par, .try_ = flag};
    }

    void insertImplIns(const InsertBuf& buf, Node* node) noexcept {
      if (buf.leftKid_) {
        buf.par_->left_ = node;
      } else {
        buf.par_->right_ = node;
      }
      node->parent_ = buf.par_;
    }

    void setRoot(FakeNode* node) {
      left_ = node;
      if (node) {
        node->parent_ = this;
      }
    }

    void transplant(FakeNode* src, FakeNode* replace) noexcept {
      FakeNode* srcPar = src->parent_;
      if (srcPar == this) {
        setRoot(replace);
      } else if (srcPar->left_ == src) {
        srcPar->left_ = replace;
        if (replace) {
          replace->parent_ = srcPar;
        }
      } else {
        srcPar->right_ = replace;
        if (replace) {
          replace->parent_ = srcPar;
        }
      }
    }

    void unlink(Node* rm) noexcept {
      if (!rm->left_) {
        transplant(rm, rm->right_);
      } else if (!rm->right_) {
        transplant(rm, rm->left_);
      } else {
        FakeNode* lowest = rm->right_;
        while (lowest->left_) {
          lowest = lowest->left_;
        }
        if (lowest->parent_ != rm) {
          transplant(lowest, lowest->right_);
          lowest->right_ = rm->right_;
          lowest->right_->parent_ = lowest;
        }
        transplant(rm, lowest);
        lowest->left_ = rm->left_;
        lowest->left_->parent_ = lowest;
      }
      rm->left_ = nullptr;
      rm->right_ = nullptr;
      rm->parent_ = rm;
    }

    Node* findNode(const Val& key) const {
      Node* root = asNode(left_);
      while (root) {
        if (comp_(key, root->value_)) {
          root = asNode(root->left_);
        } else if (comp_(root->value_, key)) {
          root = asNode(root->right_);
        } else {
          return root;
        }
      }
      return nullptr;
    }

    const Front& atSide(const Val& key) const {
      Node* found = findNode(key);
      if (found == nullptr) {
        throw std::out_of_range("my_atside_exception");
      }
      return frontNode(found)->value_;
    }

    Iterator find(const Val& val) const {
      Node* found = findNode(val);
      if (found == nullptr) {
        return end();
      }
      return Iterator(found);
    }

    Iterator end() const noexcept {
      return Iterator(this);
    }

    Iterator begin() const {
      return beginSide(left_);
    }

    Iterator beginSide(FakeNode* root) const noexcept {
      if (root == nullptr) {
        return Iterator(this);
      }
      FakeNode* cur = root;
      while (cur->left_ != nullptr) {
        cur = cur->left_;
      }
      return Iterator(asNode(cur));
    }

    Iterator lowerBound(const Val& value) const {
      return bound(value, [this](const Val& cur, const Val& tar) { return !comp_(cur, tar); });
    }

    Iterator upperBound(const Val& value) const {
      return bound(value, [this](const Val& cur, const Val& tar) { return comp_(tar, cur); });
    }

  private:
    [[no_unique_address]] Comparator comp_;

    Node* asNode(FakeNode* fake) const noexcept {
      return static_cast<Node*>(fake);
    }

    ValNode<Front, FrontTag>* frontNode(Node* node) const {
      return static_cast<ValNode<Front, FrontTag>*>(static_cast<BiNode*>(node));
    }

    template <typename BoundComp>
    Iterator bound(const Val& value, BoundComp&& localComp) const {
      FakeNode* cur = left_;
      FakeNode* res = nullptr;
      while (cur) {
        if (localComp(asNode(cur)->value_, value)) {
          res = cur;
          cur = cur->left_;
        } else {
          cur = cur->right_;
        }
      }
      if (res) {
        return Iterator(asNode(res));
      }
      return Iterator(this);
    }

    friend class Bimap;
  };

  class Tree
      : public TreeImpl<LeftTag, CompareLeft>
      , public TreeImpl<RightTag, CompareRight> {
  public:
    Tree() = default;

    template <typename ComL>
    Tree(ComL&& compareLeft)
        : TreeImpl<LeftTag, CompareLeft>(std::forward<ComL>(compareLeft)) {}

    template <typename ComL, typename ComR>
    Tree(ComL&& compareLeft, ComR&& compareRight)
        : TreeImpl<LeftTag, CompareLeft>(std::forward<ComL>(compareLeft))
        , TreeImpl<ct::RightTag, CompareRight>(compareRight) {}
  };

public:
  ~Bimap() noexcept {
    if (leftTree().left_) {
      erase_left(begin_left(), end_left());
    }
  }

  Bimap() noexcept {}

  Bimap(CompareLeft compareLeft)
    requires (std::copyable<CompareLeft>)
      : tree_(compareLeft) {}

  template <typename ComL, typename ComR>
    requires (std::constructible_from<CompareLeft, ComL &&> && std::constructible_from<CompareRight, ComR &&>)
  Bimap(ComL&& compareLeft, ComR&& compareRight)
      : tree_(std::forward<ComL>(compareLeft), std::forward<ComR>(compareRight)) {}

  Bimap(const Bimap& other)
      // : Bimap ( DelegateCtor{}, other)
      : tree_(other.leftTree().comp_, other.rightTree().comp_) {
    if (other.leftTree().left_ == nullptr) {
      return;
    }

    std::queue<FakeNode*> queue;
    queue.push(other.leftTree().left_);
    while (!queue.empty()) {
      FakeNode* cur = queue.front();
      queue.pop();
      LeftNode* left = static_cast<LeftNode*>(cur);
      RightNode* right = leftTree().frontNode(left);
      insertImpl(left->value_, right->value_);
      if (cur->left_) {
        queue.push(cur->left_);
      }
      if (cur->right_) {
        queue.push(cur->right_);
      }
    }
  }

  Bimap(Bimap&& other) noexcept
      : tree_(std::move(other.tree_))
      , amount_(std::move(other.amount_)) {
    other.amount_ = 0;
  }

  Bimap& operator=(const Bimap& other) {
    if (this != &other) {
      Bimap tmp(other);
      tmp.exchange(*this);
    }
    return *this;
  }

  Bimap& operator=(Bimap&& other) noexcept {
    if (this != &other) {
      exchange(other);
    }
    return *this;
  }

  friend void swap(Bimap& lhs, Bimap& rhs) noexcept {
    lhs.exchange(rhs);
  }

  LeftIterator insert(Left&& left, Right&& right) {
    return insertImpl<Left, Right>(std::move(left), std::move(right));
  }

  template <typename L, typename R>
    requires (std::constructible_from<Left, L &&> && std::constructible_from<Right, R &&>)
  LeftIterator insert(L&& left, R&& right) {
    return insertImpl(std::forward<L>(left), std::forward<R>(right));
  }

  LeftIterator erase_left(LeftIterator it) noexcept {
    LeftIterator result = it;
    ++result;
    eraseImpl(asLeft(it.node_), asRight(it.flip().node_));
    return result;
  }

  RightIterator erase_right(RightIterator it) noexcept {
    RightIterator result = it;
    ++result;
    eraseImpl(asLeft(it.flip().node_), asRight(it.node_));
    return result;
  }

  bool erase_left(const Left& left) {
    LeftIterator it = find_left(left);
    if (it == end_left()) {
      return false;
    }
    erase_left(it);
    return true;
  }

  bool erase_right(const Right& right) {
    RightIterator it = find_right(right);
    if (it == end_right()) {
      return false;
    }
    erase_right(it);
    return true;
  }

  LeftIterator erase_left(LeftIterator first, LeftIterator last) noexcept {
    while (first != last) {
      erase_left(first++);
    }
    return last;
  }

  RightIterator erase_right(RightIterator first, RightIterator last) noexcept {
    while (first != last) {
      erase_right(first++);
    }
    return last;
  }

  LeftIterator find_left(const Left& left) const {
    return leftTree().find(left);
  }

  RightIterator find_right(const Right& right) const {
    return rightTree().find(right);
  }

  const Right& at_left(const Left& key) const {
    return leftTree().atSide(key);
  }

  const Left& at_right(const Right& key) const {
    return rightTree().atSide(key);
  }

  const Right& at_left_or_default(const Left& key)
    requires std::is_default_constructible_v<Right>
  {
    LeftIterator findLe = find_left(key);
    if (findLe != end_left()) {
      return *findLe.flip();
    }

    Right stockFront{};
    RightIterator findRi = find_right(stockFront);
    if (findRi == end_right()) {
      LeftIterator insRes = insert(key, stockFront);
      return *insRes.flip();
    }

    LeftIterator newIt = rudeInsert(key, stockFront);
    erase_left(findRi.flip());

    return *newIt.flip();
  }

  const Left& at_right_or_default(const Right& key)
    requires std::is_default_constructible_v<Left>
  {
    RightIterator findRi = find_right(key);
    if (findRi != end_right()) {
      return *findRi.flip();
    }

    Left stockFront{};
    LeftIterator findLe = find_left(stockFront);
    if (findLe == end_left()) {
      LeftIterator insRes = insert(stockFront, key);
      return *insRes;
    }

    LeftIterator newIt = rudeInsert(stockFront, key);
    erase_right(findLe.flip());

    return *newIt;
  }

  LeftIterator lower_bound_left(const Left& left) const {
    return leftTree().lowerBound(left);
  }

  LeftIterator upper_bound_left(const Left& left) const {
    return leftTree().upperBound(left);
  }

  RightIterator lower_bound_right(const Right& right) const {
    return rightTree().lowerBound(right);
  }

  RightIterator upper_bound_right(const Right& right) const {
    return rightTree().upperBound(right);
  }

  LeftIterator begin_left() const noexcept {
    return leftTree().begin();
  }

  RightIterator begin_right() const noexcept {
    return rightTree().begin();
  }

  LeftIterator end_left() const noexcept {
    return leftTree().end();
  }

  RightIterator end_right() const noexcept {
    return rightTree().end();
  }

  bool empty() const noexcept {
    return size() == 0;
  }

  std::size_t size() const noexcept {
    return amount_;
  }

  friend bool operator==(const Bimap& lhs, const Bimap& rhs) {
    if (lhs.size() != rhs.size()) {
      return false;
    }

    auto itLeft = lhs.begin_left();
    auto itRight = rhs.begin_left();
    while (itLeft != lhs.end_left() && itRight != rhs.end_left()) {
      if (lhs.leftTree().comp_(*itLeft, *itRight) || lhs.leftTree().comp_(*itRight, *itLeft)) {
        return false;
      }
      const Right& valLhs = *itLeft.flip();
      const Right& valRhs = *itRight.flip();
      if (lhs.rightTree().comp_(valLhs, valRhs) || lhs.rightTree().comp_(valRhs, valLhs)) {
        return false;
      }
      ++itLeft;
      ++itRight;
    }
    return true;
  }

  friend bool operator!=(const Bimap& lhs, const Bimap& rhs) {
    return !(lhs == rhs);
  }

private:
  Tree tree_;

  size_t amount_{0};

  // (было для копирующего)
  // Bimap(DelegateCtor, const Bimap& other)
  //     : tree_(other.leftTree().comp_, other.rightTree().comp_) {}

  inline TreeImpl<LeftTag, CompareLeft>& leftTree() {
    return static_cast<TreeImpl<LeftTag, CompareLeft>&>(tree_);
  }

  inline const TreeImpl<LeftTag, CompareLeft>& leftTree() const {
    return static_cast<const TreeImpl<LeftTag, CompareLeft>&>(tree_);
  }

  inline TreeImpl<RightTag, CompareRight>& rightTree() {
    return static_cast<TreeImpl<RightTag, CompareRight>&>(tree_);
  }

  inline const TreeImpl<RightTag, CompareRight>& rightTree() const {
    return static_cast<const TreeImpl<RightTag, CompareRight>&>(tree_);
  }

  void exchange(Bimap& other) noexcept {
    std::swap(amount_, other.amount_);
    leftTree().swap(other.leftTree());
    rightTree().swap(other.rightTree());
  }

  template <typename L, typename R>
  LeftIterator rudeInsertImpl(L&& left, R&& right, InsertBuf& leftBuf, InsertBuf& rightBuf) {
    std::unique_ptr<BiNode> newNode = std::make_unique<BiNode>(std::forward<L>(left), std::forward<R>(right));

    leftTree().insertImplIns(leftBuf, bi2Left(newNode.get()));
    rightTree().insertImplIns(rightBuf, bi2Right(newNode.get()));

    ++amount_;

    return LeftIterator(bi2Left(newNode.release()));
  }

  template <typename L, typename R>
  LeftIterator rudeInsert(L&& left, R&& right) {
    InsertBuf leftBuf = leftTree().insertImplFind(left);
    InsertBuf rightBuf = rightTree().insertImplFind(right);

    return rudeInsertImpl(std::forward<L>(left), std::forward<R>(right), leftBuf, rightBuf);
  }

  template <typename L, typename R>
  LeftIterator insertImpl(L&& left, R&& right) {
    InsertBuf leftBuf = leftTree().insertImplFind(left);
    InsertBuf rightBuf = rightTree().insertImplFind(right);
    if ((leftBuf.try_ == false) || (rightBuf.try_ == false)) {
      return end_left();
    }

    return rudeInsertImpl(std::forward<L>(left), std::forward<R>(right), leftBuf, rightBuf);
  }

  void eraseImpl(LeftNode* rmLeft, RightNode* rmRight) noexcept {
    amount_--;
    leftTree().unlink(rmLeft);
    rightTree().unlink(rmRight);
    delete static_cast<BiNode*>(rmLeft);
  }

  LeftNode* bi2Left(BiNode* binode) {
    return static_cast<LeftNode*>(binode);
  }

  RightNode* bi2Right(BiNode* binode) {
    return static_cast<RightNode*>(binode);
  }

  LeftNode* asLeft(FakeNode* fake) const noexcept {
    return static_cast<LeftNode*>(fake);
  }

  RightNode* asRight(FakeNode* fake) const noexcept {
    return static_cast<RightNode*>(fake);
  }
};

} // namespace ct
