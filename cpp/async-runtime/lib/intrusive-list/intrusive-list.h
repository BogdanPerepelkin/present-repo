#pragma once

#include "element-impl.h"

#include <algorithm>
#include <cstddef>
#include <type_traits>

namespace ct::intrusive {

template <typename Tag = DefaultTag>
class ListElement : public utils::ElementImpl {
  template <typename T, typename Tag2>
  friend class List;
};

template <typename T, typename Tag = DefaultTag>
class List : public ListElement<Tag> {
  static_assert(std::is_base_of_v<ListElement<Tag>, T>, "T must derive from ListElement");

public:
  template <typename U>
  class MyIterator {
    static_assert(std::is_base_of_v<ListElement<Tag>, U>, "T must derive from ListElement");

  public:
    using difference_type = std::ptrdiff_t;
    using value_type = std::remove_cvref_t<U>;
    using pointer = U*;
    using reference = U&;
    using iterator_category = std::bidirectional_iterator_tag;
    using ConstIterator = MyIterator<const U>;

    using Node = ListElement<Tag>;

    ~MyIterator() = default;

    MyIterator() = default;

    reference operator*() const noexcept {
      return *static_cast<U*>(nodePtr_);
    }

    pointer operator->() const noexcept {
      return static_cast<pointer>(nodePtr_);
    }

    MyIterator& operator++() noexcept {
      nodePtr_ = static_cast<Node*>(nodePtr_->_next);
      return *this;
    }

    MyIterator operator++(int) noexcept {
      MyIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    MyIterator& operator--() noexcept {
      nodePtr_ = static_cast<Node*>(nodePtr_->_prev);
      return *this;
    }

    MyIterator operator--(int) noexcept {
      MyIterator tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator==(const MyIterator& other) const noexcept = default;

    operator ConstIterator() const noexcept {
      return ConstIterator(nodePtr_);
    }

  private:
    Node* nodePtr_;

    MyIterator(utils::ElementImpl* other) noexcept
        : nodePtr_(static_cast<Node*>(other)) {}

    MyIterator(const utils::ElementImpl* other) noexcept
        : nodePtr_(static_cast<Node*>(const_cast<utils::ElementImpl*>(other))) {}

    template <typename V, typename Tag2>
    friend class ct::intrusive::List;
  };

  using Iterator = MyIterator<T>;
  using ConstIterator = MyIterator<const T>;
  using Node = ListElement<Tag>;

  List() noexcept = default;

  ~List() = default;

  List(const List& other) = default;

  List& operator=(const List&) = default;

  List(List&& other) noexcept {
    if (other.empty()) {
      this->_prev = this->_next = this;
    } else {
      ListElement<Tag>::operator=(std::move(other));
    }
  }

  List& operator=(List&& other) noexcept {
    if (other.empty()) {
      this->_prev = this->_next = this;
    } else {
      ListElement<Tag>::operator=(std::move(other));
    }
    return *this;
  }

  bool empty() const noexcept {
    return this->_next == this && this->_prev == this;
  }

  size_t size() const noexcept {
    return std::distance(begin(), end());
  }

  T& front() noexcept {
    return *begin();
  }

  const T& front() const noexcept {
    return *begin();
  }

  T& back() noexcept {
    return *(--end());
  }

  const T& back() const noexcept {
    return *(--end());
  }

  void push_front(T& value) noexcept {
    insert(begin(), value);
  }

  void push_back(T& value) noexcept {
    insert(end(), value);
  }

  void pop_front() noexcept {
    erase(begin());
  }

  void pop_back() noexcept {
    erase(--end());
  }

  void clear() noexcept {
    this->unlink();
  }

  Iterator begin() noexcept {
    return Iterator(this->_next);
  }

  ConstIterator begin() const noexcept {
    return ConstIterator(this->_next);
  }

  Iterator end() noexcept {
    return Iterator(this);
  }

  ConstIterator end() const noexcept {
    return ConstIterator(this);
  }

  Iterator insert(ConstIterator pos, T& value) noexcept {
    if (&value == pos.nodePtr_) {
      return Iterator();
    }
    asNode(value).linkBefore(*pos.nodePtr_);
    return Iterator(&asNode(value));
  }

  Iterator erase(ConstIterator pos) noexcept {
    Node* node = pos.nodePtr_;
    Iterator following = Iterator(node->_next);
    node->unlink();
    return following;
  }

private:
  static Node& asNode(T& element) noexcept {
    return static_cast<Node&>(element);
  }
};

} // namespace ct::intrusive
