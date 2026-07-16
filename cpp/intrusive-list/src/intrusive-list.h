#pragma once

#include "element-impl.h"

#include <algorithm>
#include <cstddef>
#include <type_traits>

namespace ct::intrusive {

template <typename Tag = DefaultTag>
class ListElement : private utils::ElementImpl {
  template <typename T, typename Tag2>
  friend class List;
};

template <typename T, typename Tag = DefaultTag>
class List {
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
      nodePtr_ = static_cast<Node*>(nodePtr_->next_);
      return *this;
    }

    MyIterator operator++(int) noexcept {
      MyIterator tmp = *this;
      ++(*this);
      return tmp;
    }

    MyIterator& operator--() noexcept {
      nodePtr_ = static_cast<Node*>(nodePtr_->prev_);
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

  List(const List&) = delete;
  List& operator=(const List&) = delete;

  List(List&& other) noexcept {
    if (other.empty()) {
      _sent.prev_ = _sent.next_ = std::addressof(this->_sent);
    } else {
      _sent = std::move(other._sent);
    }
  }

  List& operator=(List&& other) noexcept {
    if (other.empty()) {
      _sent.prev_ = _sent.next_ = std::addressof(_sent);
    } else {
      _sent = std::move(other._sent);
    }
    return *this;
  }

  bool empty() const noexcept {
    return _sent.next_ == std::addressof(_sent) && _sent.prev_ == std::addressof(_sent);
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
    erase(std::prev(end()));
  }

  void clear() noexcept {
    _sent.unlink();
  }

  Iterator begin() noexcept {
    return Iterator(_sent.next_);
  }

  ConstIterator begin() const noexcept {
    return ConstIterator(_sent.next_);
  }

  Iterator end() noexcept {
    return Iterator(std::addressof(_sent));
  }

  ConstIterator end() const noexcept {
    return ConstIterator(std::addressof(_sent));
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
    Iterator following = Iterator(node->next_);
    node->unlink();
    return following;
  }

  void splice(ConstIterator pos, List& /*unused*/, ConstIterator first, ConstIterator last) noexcept {
    if (first == last) {
      return;
    }
    Node* start = first.nodePtr_;
    Node* end = (--last).nodePtr_;
    Node* posAfter = pos.nodePtr_;

    start->prev_->next_ = end->next_;
    end->next_->prev_ = start->prev_;

    start->prev_ = posAfter->prev_;
    end->next_ = posAfter;

    posAfter->prev_->next_ = start;
    posAfter->prev_ = end;
  }

private:
  ListElement<Tag> _sent;

  static Node& asNode(T& element) noexcept {
    return static_cast<Node&>(element);
  }
};

} // namespace ct::intrusive
