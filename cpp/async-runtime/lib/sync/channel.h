#pragma once

#include "coro/coroutine.h"
#include "utils.h"

#include <queue>

#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

// Buffered MPMC Channel
// https://tour.golang.org/concurrency/3

namespace ct::sync {

struct SendTag;
struct RecvTag;

template <typename T>
class BufferedChannel {
private:
  struct SendAwaitable;

  struct RecvAwaitable;

  struct OneAllocQueue {
    OneAllocQueue(std::size_t capacity)
        : _capacity(capacity) {
      if (capacity == 0) {
        return;
      }
      _data = std::allocator<T>{}.allocate(capacity);
    }

    ~OneAllocQueue() {
      while (_size != 0) {
        pop();
      }
      if (_data) {
        std::allocator<T>{}.deallocate(_data, _capacity);
      }
    }

    void push(T&& value) {
      std::construct_at(_data + ((_front + _size) % _capacity), std::move(value));
      _size++;
    }

    T& front() {
      return _data[_front];
    }

    void pop() {
      std::destroy_at(_data + _front);
      _front = (_front + 1) % _capacity;
      _size--;
    }

    std::size_t size() {
      return _size;
    }

    bool empty() {
      return _size == 0;
    }

    T* _data = nullptr;
    std::size_t _front = 0;
    std::size_t _size = 0;
    std::size_t _capacity;
  };

  struct ChannelStorage
      : public intrusive::List<SendAwaitable, SendTag>
      , public intrusive::List<RecvAwaitable, RecvTag> {
    using SendList = intrusive::List<SendAwaitable, SendTag>;
    using RecvList = intrusive::List<RecvAwaitable, RecvTag>;

    ChannelStorage(std::size_t capacity)
        : _capacity(capacity)
        , _queue(capacity) {}

    SendList& asSendList() {
      return static_cast<SendList&>(*const_cast<ChannelStorage*>(this));
    }

    RecvList& asRecvList() {
      return static_cast<RecvList&>(*const_cast<ChannelStorage*>(this));
    }

    std::size_t _capacity;
    OneAllocQueue _queue;
    std::mutex _mutex;
  };

  struct AwaitableStorage {
    using Handle = std::coroutine_handle<coro::Coroutine::promise_type>;

    AwaitableStorage(ChannelStorage* storage, const T& value)
        : _storage(storage)
        , _value(value) {}

    AwaitableStorage(ChannelStorage* storage, T&& value)
        : _storage(storage)
        , _value(std::move(value)) {}

    AwaitableStorage(ChannelStorage* storage)
        : _storage(storage) {}

  protected:
    ChannelStorage* _storage;
    Handle _handle;
    std::optional<T> _value = std::nullopt;
  };

  struct SendAwaitable
      : intrusive::ListElement<SendTag>
      , AwaitableStorage {
    using AwaitableStorage::_handle;
    using AwaitableStorage::_storage;
    using AwaitableStorage::_value;
    using AwaitableStorage::AwaitableStorage;
    using typename AwaitableStorage::Handle;

    bool await_ready() const noexcept {
      return false;
    }

    bool await_suspend(Handle handle) noexcept {
      _handle = handle;
      bool result;
      Handle h2resume{};
      {
        std::lock_guard<std::mutex> lg(_storage->_mutex);

        // straight to receiver - fast path
        if (!_storage->asRecvList().empty()) {
          RecvAwaitable& firstReceiver = _storage->asRecvList().back();
          h2resume = firstReceiver._handle;
          firstReceiver._value.emplace(std::move(*_value));
          firstReceiver.unlink();
          result = false;
        }
        // buffered path
        else if (_storage->_queue.size() < _storage->_capacity) {
          _storage->_queue.push(std::move(*_value));
          result = false;
        }
        // suspend
        else {
          _storage->asSendList().push_front(*this);
          result = true;
        }
      }

      if (h2resume) {
        utils::helperSpawn(h2resume);
      }

      return result;
    }

    void await_resume() noexcept {}
  };

  struct RecvAwaitable
      : intrusive::ListElement<RecvTag>
      , AwaitableStorage {
    using AwaitableStorage::_handle;
    using AwaitableStorage::_storage;
    using AwaitableStorage::_value;
    using AwaitableStorage::AwaitableStorage;
    using typename AwaitableStorage::Handle;

    bool await_ready() const noexcept {
      return false;
    }

    bool await_suspend(Handle handle) noexcept {
      _handle = handle;
      std::lock_guard<std::mutex> lg(_storage->_mutex);

      // buffered
      if (!_storage->_queue.empty()) {
        _value.emplace(std::move(_storage->_queue.front()));
        _storage->_queue.pop();

        // resume brother
        if (!_storage->asSendList().empty()) {
          SendAwaitable& firstSender = _storage->asSendList().back();
          _storage->_queue.push(std::move(*firstSender._value));
          firstSender.unlink();
          utils::helperSpawn(firstSender._handle);
        }
        return false;
      }

      // rendezvouz
      if (_storage->_capacity == 0 && !_storage->asSendList().empty()) {
        SendAwaitable& firstSender = _storage->asSendList().back();
        _value.emplace(std::move(*firstSender._value));
        firstSender.unlink();
        utils::helperSpawn(firstSender._handle);
        return false;
      }

      // suspend
      _storage->asRecvList().push_front(*this);
      return true;
    }

    T await_resume() noexcept {
      return std::move(*_value);
    }
  };

public:
  explicit BufferedChannel(std::size_t capacity)
      : _storage(std::make_shared<ChannelStorage>(capacity)) {}

  SendAwaitable send(T&& value) {
    return SendAwaitable(_storage.get(), std::move(value));
  }

  SendAwaitable send(const T& value) {
    return SendAwaitable(_storage.get(), value);
  }

  RecvAwaitable recv() {
    return RecvAwaitable(_storage.get());
  }

private:
  std::shared_ptr<ChannelStorage> _storage;

  friend struct SendAwaitable;

  friend struct RecvAwaitable;
};

} // namespace ct::sync
