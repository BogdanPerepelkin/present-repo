// #include "shared-ptr.h"
//
// #include <memory>
//
// class Wrap {
// public:
//   Wrap(int num)
//       : num_(num) {}
//
//   std::string show() {
//     return std::to_string(num_);
//   }
//
// private:
//   int num_;
// };
//
// int main() {
//   ct::SharedPtr<Wrap> p = ct::make_shared<Wrap>(42);
//   std::cout << (*p).show() << std::endl;
// }
