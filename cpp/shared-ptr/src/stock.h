
template <typename T>
class SharedPtr {
public:
  SharedPtr() noexcept;

  SharedPtr(std::nullptr_t) noexcept;

  template <typename Y>
  explicit SharedPtr(Y* ptr);

  template <typename Y, typename Deleter>
  SharedPtr(Y* ptr, Deleter deleter);

  template <typename Y>
  SharedPtr(const SharedPtr<Y>& other, T* ptr) noexcept;

  template <typename Y>
  SharedPtr(SharedPtr<Y>&& other, T* ptr) noexcept;

  SharedPtr(const SharedPtr& other) noexcept;

  template <typename Y>
  SharedPtr(const SharedPtr<Y>& other) noexcept;

  SharedPtr(SharedPtr&& other) noexcept;

  template <typename Y>
  SharedPtr(SharedPtr<Y>&& other) noexcept;

  SharedPtr& operator=(const SharedPtr& other) noexcept;

  template <typename Y>
  SharedPtr& operator=(const SharedPtr<Y>& other) noexcept;

  SharedPtr& operator=(SharedPtr&& other) noexcept;

  template <typename Y>
  SharedPtr& operator=(SharedPtr<Y>&& other) noexcept;

  T* get() const noexcept;

  explicit operator bool() const noexcept;

  T& operator*() const noexcept;

  T* operator->() const noexcept;

  std::size_t use_count() const noexcept;

  void reset() noexcept;

  template <typename Y>
  void reset(Y* new_ptr);

  template <typename Y, typename Deleter>
  void reset(Y* new_ptr, Deleter deleter);

  friend bool operator==(const SharedPtr& lhs, const SharedPtr& rhs) noexcept;

  friend bool operator!=(const SharedPtr& lhs, const SharedPtr& rhs) noexcept;

private:
};

template <typename T>
class WeakPtr {
public:
  WeakPtr() noexcept;

  template <typename Y>
  WeakPtr(const SharedPtr<Y>& other) noexcept;

  WeakPtr(const WeakPtr& other) noexcept;

  template <typename Y>
  WeakPtr(const WeakPtr<Y>& other) noexcept;

  WeakPtr(WeakPtr&& other) noexcept;

  template <typename Y>
  WeakPtr(WeakPtr<Y>&& other) noexcept;

  template <typename Y>
  WeakPtr& operator=(const SharedPtr<Y>& other) noexcept;

  WeakPtr& operator=(const WeakPtr& other) noexcept;

  template <typename Y>
  WeakPtr& operator=(const WeakPtr<Y>& other) noexcept;

  WeakPtr& operator=(WeakPtr&& other) noexcept;

  template <typename Y>
  WeakPtr& operator=(WeakPtr<Y>&& other) noexcept;

  SharedPtr<T> lock() const noexcept;

  void reset() noexcept;

private:
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args);
