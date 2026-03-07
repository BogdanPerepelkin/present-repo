// #pragma once
//
// #include <pstl/execution_defs.h>
//
// #include <queue>
//
// #include <concepts>
// #include <cstddef>
// #include <functional>
// #include <ios>
// #include <iostream>
// #include <ostream>
// #include <stdexcept>
//
// #include "intrusiveTree.h"
//
// // все, все переписываем
//
// namespace ct {
//
// template <
//     typename TLeft,
//     typename TRight,
//     typename CompareLeft = std::less<TLeft>,
//     typename CompareRight = std::less<TRight>>
// class Bimap {
// public:
//   using Left = TLeft;
//   using Right = TRight;
//
//   template <typename Image>
//   class AbsIterator;
//
//   using LeftIterator = AbsIterator<Left>;
//   using RightIterator = AbsIterator<Right>;
//
//   class FakeNode {
//   public:
//     ~FakeNode() = default;
//
//     FakeNode()
//         : parent_(this) // seg fault
//         , left_(nullptr)
//         , right_(nullptr) {}
//
//     FakeNode(const FakeNode&) = delete;
//     FakeNode& operator=(const FakeNode&) = delete;
//
//     FakeNode(FakeNode&& other) = delete;
//
//     FakeNode& operator=(FakeNode&&) = delete;
//
//   protected:
//     FakeNode* parent_;
//     FakeNode* left_;
//     FakeNode* right_;
//
//     template <typename U>
//     friend class Bimap::AbsIterator; // странный синтаксис
//
//     friend class Bimap;
//   };
//
//   template <typename Val>
//   class Node : public FakeNode {
//   public:
//     using FrontT = std::conditional_t<std::same_as<Val, Left>, Right, Left>;
//
//     ~Node() = default;
//
//     Node(const Node&) = delete;
//     Node& operator=(const Node&) = delete;
//     Node(Node&& other) = delete;
//     Node& operator=(Node&& other) = delete;
//
//     template <typename ForwardVal>
//     Node(ForwardVal&& val)
//         : value_(std::forward<ForwardVal>(val)) {}
//
//     void unlink() {
//       FakeNode* fake = this;
//       fake->parent_ = nullptr;
//       fake->left_ = nullptr;
//       fake->right_ = nullptr;
//     }
//
//   private:
//     Val value_;
//     Node<FrontT>* front_;
//
//     template <typename U>
//     friend class Bimap::AbsIterator;
//
//     template <typename U>
//     friend class Bimap::Node;
//
//     friend class Bimap;
//   };
//
//   template <typename Val>
//   class AbsIterator {
//   public:
//     using difference_type = std::ptrdiff_t;
//     using value_type = std::remove_cvref_t<Val>;
//     using pointer = Val*;
//     using reference = Val&;
//     using iterator_category = std::bidirectional_iterator_tag;
//     using ConstIterator = AbsIterator<const Val>;
//     using Front = std::conditional_t<std::same_as<Val, Left>, Right, Left>;
//
//     AbsIterator() = default;
//
//     ~AbsIterator() = default;
//
//     const Val& operator*() const {
//       return static_cast<Node<Val>*>(nodeP_)->value_;
//     }
//
//     const Val* operator->() const {
//       return &static_cast<Node<Val>*>(nodeP_)->value_;
//     }
//
//     AbsIterator& operator++() {
//       if (nodeP_->right_) {
//         nodeP_ = nodeP_->right_;
//         while (nodeP_->left_) {
//           nodeP_ = nodeP_->left_;
//         }
//       } else {
//         while (nodeP_->parent_ && nodeP_->parent_->right_ == nodeP_) {
//           nodeP_ = nodeP_->parent_;
//         }
//         nodeP_ = nodeP_->parent_;
//       }
//       return *this;
//     }
//
//     AbsIterator operator++(int) {
//       AbsIterator tmp = *this;
//       ++*this;
//       return tmp;
//     }
//
//     AbsIterator& operator--() {
//       if (nodeP_->left_) {
//         nodeP_ = nodeP_->left_;
//         while (nodeP_->right_) {
//           nodeP_ = nodeP_->right_;
//         }
//       } else {
//         while (nodeP_->parent_ && nodeP_->parent_->left_ == nodeP_) {
//           nodeP_ = nodeP_->parent_;
//         }
//         nodeP_ = nodeP_->parent_;
//       }
//       return *this;
//     }
//
//     AbsIterator operator--(int) {
//       AbsIterator tmp = *this;
//       --*this;
//       return tmp;
//     }
//
//     // Front flip() const {
//     // return static_cast<Node<Val>*>(nodeP_)->front_->value_;
//     // }
//
//     // return_type
//     std::conditional_t<std::same_as<Val, Left>, RightIterator, LeftIterator> flip() const {
//       return static_cast<Node<Val>*>(nodeP_)->front_;
//     }
//
//     friend bool operator==(const AbsIterator& lhs, const AbsIterator& rhs) = default;
//
//   private:
//     AbsIterator(const FakeNode* node)
//         : nodeP_(const_cast<FakeNode*>(node)) {}
//
//     FakeNode* nodeP_;
//
//     friend class Bimap;
//   };
//
// public:
//   Bimap() {
//     std::cout << "con empty" << std::endl;
//   }
//
//   Bimap(CompareLeft compareLeft)
//     requires (std::copyable<CompareLeft>)
//       : compareLeft_(compareLeft) {
//     std::cout << "con single" << std::endl;
//   }
//
//   template <typename ComL, typename ComR>
//     requires (std::constructible_from<CompareLeft, ComL &&> && std::constructible_from<CompareRight, ComR &&>)
//   Bimap(ComL&& compareLeft, ComR&& compareRight)
//       : compareLeft_(std::forward<ComL>(compareLeft))
//       , compareRight_(std::forward<ComR>(compareRight)) {
//     std::cout << "con forward" << std::endl;
//   }
//
//   void copyTree(FakeNode* othLeftTree) {
//     std::queue<FakeNode*> queue;
//     queue.push(othLeftTree);
//     while (!queue.empty()) {
//       FakeNode* cur = queue.front();
//       queue.pop();
//       auto* typeNode = static_cast<Node<Left>*>(cur);
//       insert(typeNode->value_, typeNode->front_->value_);
//       if (cur->left_) {
//         queue.push(cur->left_);
//       }
//       if (cur->right_) {
//         queue.push(cur->right_);
//       }
//     }
//   }
//
//   Bimap(const Bimap& other)
//       : compareLeft_(other.compareLeft_)
//       , compareRight_(other.compareRight_) {
//     if (other.leftTree() == nullptr) {
//       return;
//     }
//     copyTree(other.leftTree());
//   }
//
//   Bimap(Bimap&& other) noexcept
//       : compareLeft_(std::move(other.compareLeft_))
//       , compareRight_(std::move(other.compareRight_)) {
//     swap(other);
//   }
//
//   Bimap& operator=(const Bimap& other) {
//     if (this != &other) {
//       Bimap tmp(other);
//       tmp.swap(*this);
//     }
//     return *this;
//   }
//
//   Bimap& operator=(Bimap&& other) noexcept {
//     if (this != &other) {
//       swap(other);
//     }
//     return *this;
//   }
//
//   template <typename Val>
//   void dtor(FakeNode* root) {
//     if (root == nullptr) {
//       return;
//     }
//     std::queue<FakeNode*> queue;
//     queue.push(root);
//     while (!queue.empty()) {
//       FakeNode* cur = queue.front();
//       queue.pop();
//       if (cur->left_) {
//         queue.push(cur->left_);
//       }
//       if (cur->right_) {
//         queue.push(cur->right_);
//       }
//       delete static_cast<Node<Val>*>(cur);
//     }
//   }
//
//   // done
//   ~Bimap() {
//     dtor<Left>(leftTree());
//     dtor<Right>(rightTree());
//   }
//
//   void swap(Bimap& other) noexcept {
//     std::swap(amount, other.amount);
//     std::swap(compareLeft_, other.compareLeft_);
//     std::swap(compareRight_, other.compareRight_);
//
//     std::swap(fakeLeft_.left_, other.fakeLeft_.left_);
//     std::swap(fakeRight_.left_, other.fakeRight_.left_);
//
//     if (fakeLeft_.left_) {
//       fakeLeft_.left_->parent_ = &other.fakeLeft_;
//     }
//     if (fakeRight_.left_) {
//       fakeRight_.left_->parent_ = &other.fakeRight_;
//     }
//     if (other.fakeLeft_.left_) {
//       other.fakeLeft_.left_->parent_ = &fakeLeft_;
//     }
//     if (other.fakeRight_.left_) {
//       other.fakeRight_.left_->parent_ = &fakeRight_;
//     }
//   }
//
//   friend void swap(Bimap& lhs, Bimap& rhs) noexcept {
//     lhs.swap(rhs);
//   }
//
//   template <typename Val, typename Comparator>
//   void insertImpl(Node<Val>* node, FakeNode* fakeRoot, Comparator& comp) {
//     Node<Val>* cur = static_cast<Node<Val>*>(fakeRoot);
//     Node<Val>* par = cur;
//     while (cur != nullptr) {
//       par = cur;
//       if (comp(node->value_, cur->value_)) {
//         cur = static_cast<Node<Val>*>(cur->left_);
//       } else {
//         cur = static_cast<Node<Val>*>(cur->right_);
//       }
//     }
//     node->parent_ = par;
//     if (comp(node->value_, par->value_)) {
//       par->left_ = node;
//     } else {
//       par->right_ = node;
//     }
//   }
//
//   template <typename L, typename R>
//   LeftIterator insert(L&& left, R&& right) {
//     if (findNode<Left>(leftTree(), left, compareLeft_) != nullptr ||
//         findNode<Right>(rightTree(), right, compareRight_) != nullptr) {
//       return end_left();
//     }
//
//     amount++;
//
//     Node<Left>* newLeft = new Node<Left>(std::forward<L>(left));
//     Node<Right>* newRight = new Node<Right>(std::forward<R>(right));
//     newLeft->front_ = newRight;
//     newRight->front_ = newLeft;
//
//     if (leftTree() == nullptr) { // ones
//       fakeLeft_.left_ = newLeft;
//       newLeft->parent_ = &fakeLeft_;
//       fakeRight_.left_ = newRight;
//       newRight->parent_ = &fakeRight_;
//       return LeftIterator(newLeft);
//     }
//
//     insertImpl<Left>(newLeft, leftTree(), compareLeft_);
//     insertImpl<Right>(newRight, rightTree(), compareRight_);
//
//     return LeftIterator(newLeft);
//   }
//
//   LeftIterator insert(Left&& left, Right&& right) {
//     std::cout << "in proxy insert" << std::endl;
//     return insert<Left, Right>(std::move(left), std::move(right));
//   }
//
//   // stock
//   // LeftIterator insert(const Left& left, const Right& right) {}
//   // LeftIterator insert(const Left& left, Right&& right);
//   // LeftIterator insert(Left&& left, const Right& right);
//   // LeftIterator insert(Left&& left, Right&& right);
//
//   template <typename Val>
//   void eraseImpl() {
//     // --amount;
//
//     // TODO...
//   }
//
//   LeftIterator erase_left(LeftIterator it) {}
//
//   RightIterator erase_right(RightIterator it);
//
//   bool erase_left(const Left& left);
//   bool erase_right(const Right& right);
//
//   LeftIterator erase_left(LeftIterator first, LeftIterator last) {}
//
//   RightIterator erase_right(RightIterator first, RightIterator last) {}
//
//   // done copypaste
//   LeftIterator find_left(const Left& left) const {
//     Node<Left>* found = findNode<Left>(leftTree(), left, compareLeft_);
//     if (found == nullptr) {
//       return end_left();
//     }
//     return LeftIterator(found);
//   }
//
//   // done copypaste
//   RightIterator find_right(const Right& right) const {
//     Node<Right>* found = findNode<Right>(rightTree(), right, compareRight_);
//     if (found == nullptr) {
//       return end_right();
//     }
//     return RightIterator(found);
//   }
//
//   // done
//   const Right& at_left(const Left& key) const {
//     return atSide<Left, Right>(leftTree(), key, compareLeft_);
//   }
//
//   // done
//   const Left& at_right(const Right& key) const {
//     return atSide<Right, Left>(rightTree(), key, compareRight_);
//   }
//
//   // last
//   const Right& at_left_or_default(const Left& key) {}
//
//   // last
//   const Left& at_right_or_default(const Right& key) {}
//
//   LeftIterator lower_bound_left(const Left& left) const {
//     // auto& comparator = getComp<Left>();
//     // Node<Left>* cur = leftTree();
//     // while (cur) {
//     //   if (comparator(cur->keyNode_->value_, left)) {
//     //     cur = cur->right_;
//     //   } else {
//     //     break;
//     //   }
//     // }
//     // return LeftIterator(cur);
//   }
//
//   LeftIterator upper_bound_left(const Left& left) const {
//     // auto& comparator = getComp<Left>();
//     // Node<Left>* cur = leftTree();
//     // while (cur) {
//     //   if (comparator(left, cur->keyNode_->value_)) {
//     //     break;
//     //   }
//     //   cur = cur->left_;
//     // }
//     // return LeftIterator(cur);
//   }
//
//   RightIterator lower_bound_right(const Right& right) const;
//   RightIterator upper_bound_right(const Right& right) const;
//
//   LeftIterator begin_left() const {
//     return beginSide<Left, LeftIterator>(leftTree());
//   }
//
//   // copypaste
//   RightIterator begin_right() const {
//     return beginSide<Right, RightIterator>(rightTree());
//   }
//
//   LeftIterator end_left() const {
//     return LeftIterator(&fakeLeft_); // mb unique
//   }
//
//   RightIterator end_right() const {
//     return RightIterator(&fakeRight_); // mb unique
//   }
//
//   // по счетчику
//   bool empty() const {
//     return leftTree() == nullptr;
//   }
//
//   // счетчик
//   std::size_t size() const {
//     return amount;
//   }
//
//   friend bool operator==(const Bimap& lhs, const Bimap& rhs) {
//     // TODO проверка на size
//     for (LeftIterator it = lhs.begin_left(); it != lhs.end_left(); ++it) {
//       if (rhs.find_left(*it) == rhs.end_left() || rhs.find_right(*it.flip()) == rhs.end_right()) {
//         return false;
//       }
//       // из-за return_type find
//       // if (rhs.find_left(*it) == rhs.end_left() || rhs.find_right(it.flip()) == rhs.end_right()) {
//       // return false;
//       // }
//     }
//     return true;
//   }
//
//   friend bool operator!=(const Bimap& lhs, const Bimap& rhs) {
//     return !(lhs == rhs);
//   }
//
// private:
//   FakeNode fakeLeft_;
//   FakeNode fakeRight_;
//   CompareLeft compareLeft_;
//   CompareRight compareRight_;
//   size_t amount{0};
//
//   FakeNode* leftTree() const {
//     return fakeLeft_.left_;
//   }
//
//   FakeNode* rightTree() const {
//     return fakeRight_.left_;
//   }
//
//   template <typename Val>
//   Node<Val>* asNode(FakeNode* fake) const {
//     return static_cast<Node<Val>*>(fake);
//   }
//
//   template <typename ResT, typename Val, typename Comparator>
//   Node<ResT>* findNode(FakeNode* fakeNode, const Val& key, const Comparator& comp) const {
//     Node<Val>* root = asNode<Val>(fakeNode);
//     while (root) {
//       if (comp(key, root->value_)) {
//         root = static_cast<Node<Val>*>(root->left_);
//       } else if (comp(root->value_, key)) {
//         root = static_cast<Node<Val>*>(root->right_);
//       } else {
//         return root;
//       }
//     }
//     return nullptr;
//   }
//
//   template <typename Val, typename Front, typename Comparator>
//   const Front& atSide(FakeNode* fakeNode, const Val& key, Comparator comp) const {
//     Node<Val>* found = findNode<Val>(fakeNode, key, comp);
//     if (found == nullptr) {
//       throw std::out_of_range("atSide");
//     }
//     return found->front_->value_;
//   }
//
//   template <typename Val, typename It>
//   It beginSide(FakeNode* root) const {
//     Node<Val>* cur = asNode<Val>(root);
//     while (cur->left_ != nullptr) {
//       cur = asNode<Val>(cur->left_);
//     }
//     return It(cur);
//   }
// };
//
// } // namespace ct
