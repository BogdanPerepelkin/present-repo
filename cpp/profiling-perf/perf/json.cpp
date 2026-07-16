#include <benchmark/benchmark.h>

#include <sstream>
#include <string>
#include <string_view>

// DO NOT change this struct
struct User {
  int64_t id;
  std::string username;
};

// DO NOT change this struct
struct Message {
  int64_t id;
  std::string subject;
  std::string body;

  User from;
  User to;
};

void user_format(std::string& s, const User& user) {
  s.append("{\"id\":").append(std::to_string(user.id)).append(",\"username\":\"").append(user.username).append("\"}");
}

// DO NOT change this function's signature
std::string to_json(const Message& msg) {
  constexpr size_t ADDITIONAL_SIZE = 64;
  constexpr size_t NUMBERS_SIZE = 128;

  std::string s;
  s.reserve(
      ADDITIONAL_SIZE + NUMBERS_SIZE + msg.subject.size() + msg.body.size() + msg.from.username.size() +
      msg.to.username.size()
  );

  s.append("{\"id\":")
      .append(std::to_string(msg.id))
      .append(",\"subject\":\"")
      .append(msg.subject)
      .append("\",\"body\":\"")
      .append(msg.body)
      .append("\",\"from\":");
  user_format(s, msg.from);
  s.append(",\"to\":");
  user_format(s, msg.to);
  s.append("}");

  return s;
}

namespace {

void bm_message_to_json(benchmark::State& state) {
  Message msg{
      .id = 1000,
      .subject = "About modules",
      .body = "So, when is that 'modules' proposal coming?",
      .from = User{.id = 12345, .username = "Herb Sutter"},
      .to = User{.id = 1, .username = "Bjarne Stroustrup"}
  };

  for (auto _ : state) {
    benchmark::DoNotOptimize(to_json(msg));
  }

  auto json = to_json(msg);
  std::string_view expected =
      R"({"id":1000,"subject":"About modules","body":"So, when is that 'modules' proposal coming?","from":{"id":12345,"username":"Herb Sutter"},"to":{"id":1,"username":"Bjarne Stroustrup"}})";
  if (json != expected) {
    state.SkipWithError("Wrong output");
  }
}

} // namespace

BENCHMARK(bm_message_to_json);

BENCHMARK_MAIN();
