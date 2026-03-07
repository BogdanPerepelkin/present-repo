#pragma once

#include "resumable.h"
#include "scheduler.h"

#include <condition_variable>
#include <thread>
#include <vector>

namespace ct::sched {

class ThreadPool final : public IntrusiveListScheduler {
public:
  explicit ThreadPool(std::size_t threads);

  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool&) = delete;
  ThreadPool& operator=(const ThreadPool&) = delete;

  // Non-moveable
  ThreadPool(ThreadPool&&) = delete;
  ThreadPool& operator=(ThreadPool&&) = delete;

  void run();

  void spawn(Resumable<IntrusiveListScheduler>& task) final;

private:
  std::mutex _m;
  std::condition_variable _cvRegularTasks;
  std::atomic<std::size_t> _cntSpawn{0};
  std::condition_variable _cvSpawn;
  std::condition_variable _cvTaskInList;
  std::vector<std::jthread> _threads;

  bool runLoop(std::stop_token stoken);
};

} // namespace ct::sched
