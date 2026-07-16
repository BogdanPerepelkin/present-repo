//#pragma once
//#include <cstddef>
//#include <functional>
//#include <iostream>
//#include <new>
//#include <ostream>
//#include <type_traits>
//
//namespace ct {
//
//inline constexpr std::size_t SMALL_SIZE = sizeof(void*) * 2;
//inline constexpr std::size_t SMALL_ALIGNMENT = alignof(void*);
//
//template <typename F>
//concept small_object =
//    sizeof(F) <= SMALL_SIZE && SMALL_ALIGNMENT % alignof(F) == 0 && (std::is_nothrow_move_constructible_v<F>);
//
//// +wibe
//class BadFunctionCall : public std::exception {
//public:
//  BadFunctionCall(const char* message)
//      : _message(message) {}
//
//  const char* what() const noexcept override {
//    return _message;
//  }
//
//private:
//  char* _message = nullptr;
//};
//
//template <typename F>
//using FunctionPtr = F*;
//
//template <typename F>
//class Function;
//
//template <typename R, typename... Args>
//class Function<R(Args...)> {
//private:
//  using Storage = std::byte[SMALL_SIZE];
//
//  struct AbsOperations {
//    virtual ~AbsOperations() = default;
//    virtual void destroy(Storage& storage) const = 0;
//    virtual void copy(const Storage& from, Storage& to) const = 0;
//    virtual void move(Storage& from, Storage& to) const = 0;
//    virtual void swap(Storage& fst, Storage& snd) const = 0;
//    virtual R invoke(Storage& storage, Args&&... args) const = 0;
//
//    virtual void debug() const = 0;
//  };
//
//  template <typename F, bool Collable = std::is_invocable_r_v<R, F, Args...>, bool IsSmall = small_object<F>>
//  struct Operations : AbsOperations {
//    void destroy(Storage& storage) const override {}
//
//    void copy(const Storage& from, Storage& to) const override {}
//
//    void move(Storage& from, Storage& to) const override {}
//
//    void swap(Storage& fst, Storage& sec) const override {}
//
//    R invoke(Storage& storage, Args&&... args) const override {
//      throw BadFunctionCall("Function: call operator() with empty object");
//    }
//
//    void debug() const override {
//      std::cout << "Operations cst empty" << std::endl;
//    }
//  };
//
//  // small
//  template <typename F>
//  // struct SmallOperations : AbsOperations {
//  struct Operations<F, true, true> : AbsOperations {
//    void destroy(Storage& storage) const override {
//      std::launder(ptrOn<F>(storage))->~F();
//    }
//
//    void copy(const Storage& from, Storage& to) const override {
//      new (&to[0]) F(*std::launder(ptrOn<const F>(from)));
//    }
//
//    void move(Storage& from, Storage& to) const override {
//      auto* funcPtr = std::launder(ptrOn<F>(from));
//      new (&to[0]) F(std::move(*funcPtr));
//      funcPtr->~F();
//    }
//
//    void swap(Storage& fst, Storage& sec) const override {
//      auto* fstPtr = std::launder(ptrOn<F>(fst));
//      auto* secPtr = std::launder(ptrOn<F>(sec));
//      F tmp(std::move(*fstPtr));
//      fstPtr->~F();
//      new (&fst[0]) F(std::move(*secPtr));
//      secPtr->~F();
//      new (&sec[0]) F(std::move(tmp));
//    }
//
//    R invoke(Storage& storage, Args&&... args) const override {
//      return (*std::launder(ptrOn<F>(storage)))(std::forward<Args>(args)...);
//    }
//
//    static F* targetImpl(Storage& s) noexcept {
//      return std::launder(reinterpret_cast<F*>(&s[0]));
//    }
//
//    static const F* targetImpl(const Storage& s) noexcept {
//      return std::launder(reinterpret_cast<const F*>(&s[0]));
//    }
//
//    void debug() const override {
//      std::cout << "Operations cst small" << std::endl;
//    }
//  };
//
//  // large
//  template <typename F>
//  // struct LargeOperations : AbsOperations {
//  struct Operations<F, true, false> : AbsOperations {
//    void destroy(Storage& storage) const override {
//      delete *ptrOn<F*>(storage);
//    }
//
//    void copy(const Storage& from, Storage& to) const override {
//      new (&to[0]) F*(new F(**ptrOn<F* const>(from)));
//    }
//
//    void move(Storage& from, Storage& to) const override {
//      auto* ptr = ptrOn<F*>(from);
//      new (&to[0]) F*(std::move(*ptr));
//      *ptr = nullptr;
//    }
//
//    void swap(Storage& fst, Storage& sec) const override {
//      auto* fstPtr = ptrOn<F*>(fst);
//      auto* secPtr = ptrOn<F*>(sec);
//      std::swap(*fstPtr, *secPtr);
//    }
//
//    R invoke(Storage& storage, Args&&... args) const override {
//      return (**ptrOn<F*>(storage))(std::forward<Args>(args)...);
//    }
//
//    // static F* targetImpl(const Storage& storage) {
//    //   return *ptrOn<F*>(storage);
//    // }
//    static F* targetImpl(Storage& storage) {
//      return *ptrOn<F*>(storage);
//    }
//
//    static const F* targetImpl(const Storage& storage) noexcept {
//      return *ptrOn<F* const>(storage);
//    }
//
//    void debug() const override {
//      std::cout << "Operations cst lagre" << std::endl;
//    }
//  };
//
//  template <typename F> // для оптимизации памяти для каждого типа
//  static constexpr Operations<F> OP{};
//
//  // template <typename F>
//  // static constexpr SmallOperations<F> SMALL_OP{};
//  //
//  // template <typename F>
//  // static constexpr LargeOperations<F> LARGE_OP{};
//
//  // static constexpr EmptyOp EMPTY_OP{};
//
//  alignas(SMALL_ALIGNMENT) Storage _storage;
//
//  const AbsOperations* _ops;
//
//  Storage& rmCv(const Storage& storage) const noexcept {
//    return const_cast<Storage&>(storage);
//  }
//
//  template <typename OnType>
//  static constexpr OnType* ptrOn(Storage& storage) noexcept {
//    return reinterpret_cast<OnType*>(&(storage)[0]);
//  }
//
//  template <typename OnType>
//  static constexpr OnType* ptrOn(const Storage& storage) noexcept {
//    return reinterpret_cast<OnType*>(&(storage)[0]);
//  }
//
//public:
//  Function() noexcept
//      : _ops(&OP<void>) {
//    _ops->debug();
//  }
//
//  template <typename F>
//    requires small_object<F>
//  Function(F func)
//      : _ops(&OP<F>) {
//    _ops->debug();
//    std::cout << "Function cst small" << std::endl;
//    new (&_storage[0]) F(std::move(func));
//  }
//
//  template <typename F>
//  Function(F func)
//      : _ops(&OP<F>) {
//    _ops->debug();
//    static_assert(small_object<F*>);
//    new (&_storage[0]) F*(new F(std::move(func)));
//  }
//
//  Function(const Function& other)
//      : _ops(other._ops) {
//    _ops->copy(other._storage, _storage);
//  }
//
//  Function(Function&& other) noexcept
//      : _ops(other._ops) {
//    _ops->move(other._storage, _storage);
//    other._ops = &OP<void>;
//  }
//
//  Function& operator=(const Function& other) {
//    if (this != &other) {
//      Function tmp(other);
//      _ops->destroy(_storage);
//      _ops = tmp._ops;
//      _ops->move(tmp._storage, _storage);
//    }
//    return *this;
//  }
//
//  Function& operator=(Function&& other) noexcept {
//    if (this != &other) {
//      _ops->destroy(_storage);
//      _ops = other._ops;
//      _ops->move(other._storage, _storage);
//      other._ops = &OP<void>;
//    }
//    return *this;
//  }
//
//  ~Function() {
//    _ops->destroy(_storage);
//  }
//
//  explicit operator bool() const noexcept {
//    return _ops != &OP<void>;
//  }
//
//  R operator()(Args... args) const {
//    return _ops->invoke(rmCv(_storage), std::forward<Args>(args)...);
//  }
//
//  template <typename T>
//  T* target() noexcept {
//    if (_ops == &OP<T>) {
//      return (Operations<T>::targetImpl(_storage));
//    }
//    return nullptr;
//  }
//
//  template <typename T>
//  const T* target() const noexcept {
//    if (_ops == &OP<T>) {
//      return (Operations<T>::targetImpl(_storage));
//    }
//    return nullptr;
//  }
//
//private:
//  // const Operations* _ops;
//
//  // template <typename F>
//  // static constexpr Operations SMALL_OBJECT_OPS{
//  //     .destroy = [](Storage& storage) { std::launder(ptrOn<F>(storage))->~F(); },
//  //     .copy = [](const Storage& from, Storage& to) { new (&to[0]) F(*std::launder(ptrOn<const F>(from))); },
//  //     .move =
//  //         [](Storage& from, Storage& to) {
//  //           auto* funcPtr = std::launder(ptrOn<F>(from));
//  //           new (&to[0]) F(std::move(*funcPtr));
//  //           funcPtr->~F();
//  //         },
//  //     .swap =
//  //         [](Storage& fst, Storage& sec) {
//  //           auto* fstPtr = std::launder(ptrOn<F>(fst));
//  //           auto* secPtr = std::launder(ptrOn<F>(sec));
//  //           F tmp(std::move(*fstPtr));
//  //           fstPtr->~F();
//  //           new (&fst[0]) F(std::move(*secPtr));
//  //           secPtr->~F();
//  //           new (&sec[0]) F(std::move(tmp));
//  //         },
//  //     .invoke = [](Storage& storage, Args... args) -> R {
//  //       return (*std::launder(ptrOn<F>(storage)))(std::forward<Args>(args)...);
//  //     },
//  // };
//
//  // template <typename F>
//  // static constexpr Operations LARGE_OBJECT_OPS{
//  //     .destroy = [](Storage& storage) { delete *ptrOn<F*>(storage); },
//  //     .copy = [](const Storage& from, Storage& to) { new (&to[0]) F*(new F(**ptrOn<F* const>(from))); },
//  //     .move =
//  //         [](Storage& from, Storage& to) {
//  //           auto* ptr = ptrOn<F*>(from);
//  //           new (&to[0]) F*(std::move(*ptr));
//  //           *ptr = nullptr;
//  //         },
//  //     .swap =
//  //         [](Storage& fst, Storage& sec) {
//  //           auto* fstPtr = ptrOn<F*>(fst);
//  //           auto* secPtr = ptrOn<F*>(sec);
//  //           std::swap(*fstPtr, *secPtr);
//  //         },
//  //     .invoke = [](Storage& storage, Args... args) -> R { return (**ptrOn<F*>(storage))(std::forward<Args>(args)...);
//  //     },
//  // };
//
//  // static constexpr Operations EMPTY_OBJECT_OPS{
//  //     .destroy = [](Storage& storage) {},
//  //     .copy = [](const Storage& from, Storage& to) {},
//  //     .move = [](Storage& from, Storage& to) {},
//  //     .swap = [](Storage& fst, Storage& sec) {},
//  //     .invoke = [](Storage&, Args...) -> R { throw std::bad_function_call(); },
//  // };
//};
//
//} // namespace ct
