#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {

  int *ptr;

  std::mutex mtx;
  std::condition_variable cv;

  std::thread t1 = std::thread([&]() {
    int a = 4;
    ptr = &a;

    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);
  });

  std::thread t2 = std::thread([&]() {
    int b = 7;
    std::cout << *ptr << std::endl;
    std::vector<int> vec;
    for (size_t i = 0; i < 100000000; i++) {
      vec.push_back(i);
    }
    std::unique_lock<std::mutex> lock(mtx);
    cv.notify_one();
  });

  t1.join();
  t2.join();

  return 0;
}