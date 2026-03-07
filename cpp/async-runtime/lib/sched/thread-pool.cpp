#include "thread-pool.h"

#include "resumable.h"
#include "scheduler.h"

#include <cstddef>
#include <mutex>
#include <stop_token>
#include <thread>

namespace ct::sched {

ThreadPool::ThreadPool(std::size_t threads) {
  _threads.resize(threads);
}

ThreadPool::~ThreadPool() {
  // case 1: spawn started, but didn't add task in list
  {
    std::unique_lock<std::mutex> lock(_m);
    _cvSpawn.wait(lock, [&]() -> bool {
      return _cntSpawn.load() == 0;
    });
  }

  // case 2: exists sad :( task which didn't manage fit in list
  {
    std::unique_lock<std::mutex> lock(_m);
    _cvTaskInList.wait(lock, [&]() -> bool {
      return empty();
    });
  }

  for (auto& th : _threads) {
    th.request_stop();
  }
  _cvRegularTasks.notify_all();
}

void ThreadPool::run() {
  for (auto& th : _threads) {
    th = std::jthread([&](std::stop_token stoken) {
      runLoop(stoken);
    });
  }
}

bool ThreadPool::runLoop(std::stop_token stoken) {
  while (!stoken.stop_requested()) {
    intrusive::utils::ElementImpl* tmp = nullptr;
    {
      std::unique_lock<std::mutex> lock(_m);
      _cvRegularTasks.wait(lock, [&]() -> bool {
        return !empty() || stoken.stop_requested();
      });
      if (empty()) {
        continue;
      }

      tmp = _next;
      _next->unlink();
      _cvTaskInList.notify_one();       
    }

    auto* resumable = static_cast<Resumable<IntrusiveListScheduler>*>(tmp);
    resumable->resume(*this);
  }
  return true;
}

void ThreadPool::spawn(Resumable<IntrusiveListScheduler>& task) {
  _cntSpawn.fetch_add(1, std::memory_order_acq_rel);
  {
    std::lock_guard<std::mutex> lock(_m);
    push_back(task);
    _cvRegularTasks.notify_one();
    if (_cntSpawn.fetch_sub(1, std::memory_order_acq_rel) == 1) {
      _cvSpawn.notify_one();
    }
  }
}

} // namespace ct::sched
