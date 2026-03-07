#pragma once

class DefaultTag;

namespace ct::intrusive {

template <typename T, typename Tag>
class List;

} // namespace ct::intrusive

namespace ct::intrusive::utils {

class ElementImpl {
public:
  ElementImpl() noexcept;

  ElementImpl(ElementImpl&& other) noexcept;

  ElementImpl(const ElementImpl& other) noexcept;

  ~ElementImpl();

  ElementImpl& operator=(ElementImpl&& other) noexcept;

  ElementImpl& operator=(const ElementImpl& other) noexcept;

  void linkBefore(ElementImpl& that) noexcept;

  void unlink() noexcept;

  ElementImpl* _prev;
  ElementImpl* _next;

private:
  void move(ElementImpl&& other) noexcept;

  template <typename T, typename Tag>
  friend class ct::intrusive::List;
};

} // namespace ct::intrusive::utils
