#include "optional.h"

#include <gtest/gtest.h>

#include <array>
#include <cstddef>
#include <ostream>
#include <string_view>
#include <type_traits>
#include <utility>

namespace ct_test {

namespace {

enum class SpecialMemberVariant {
  TRIVIAL,
  DELETED,
  USER_DEFINED,
  USER_DEFINED_NOEXCEPT,
};

constexpr bool is_special_member_noexcept(SpecialMemberVariant v) noexcept {
  return v == SpecialMemberVariant::USER_DEFINED_NOEXCEPT || v == SpecialMemberVariant::TRIVIAL;
}

constexpr std::string_view to_string(SpecialMemberVariant v) noexcept {
  if (v == SpecialMemberVariant::TRIVIAL) {
    return "trivial";
  } else if (v == SpecialMemberVariant::DELETED) {
    return "deleted";
  } else if (v == SpecialMemberVariant::USER_DEFINED) {
    return "user-defined";
  } else if (v == SpecialMemberVariant::USER_DEFINED_NOEXCEPT) {
    return "user-defined-noexcept";
  }
  std::unreachable();
}

struct ClassConfiguration {
  SpecialMemberVariant dtor;
  SpecialMemberVariant copy_ctor;
  SpecialMemberVariant move_ctor;
  SpecialMemberVariant copy_assign;
  SpecialMemberVariant move_assign;
};

std::ostream& operator<<(std::ostream& out, ClassConfiguration cfg) {
  out << "dtor = " << to_string(cfg.dtor) << ", ";
  out << "copy_ctor = " << to_string(cfg.copy_ctor) << ", ";
  out << "move_ctor = " << to_string(cfg.move_ctor) << ", ";
  out << "copy_assign = " << to_string(cfg.copy_assign) << ", ";
  out << "move_assign = " << to_string(cfg.move_assign);
  return out;
}

template <size_t N>
struct SpecialMemberSpec {
  SpecialMemberVariant ClassConfiguration::* member;
  std::array<SpecialMemberVariant, N> variants;
};

template <SpecialMemberSpec...>
struct SpecialMemberSpecList {};

constexpr auto run_all = [] {
  constexpr std::array all_variants = {
      SpecialMemberVariant::TRIVIAL,
      SpecialMemberVariant::DELETED,
      SpecialMemberVariant::USER_DEFINED,
      SpecialMemberVariant::USER_DEFINED_NOEXCEPT,
  };
  constexpr std::array dtor_variants = {
      SpecialMemberVariant::TRIVIAL,
      SpecialMemberVariant::USER_DEFINED_NOEXCEPT,
  };

  using Params = SpecialMemberSpecList<
      SpecialMemberSpec(&ClassConfiguration::dtor, dtor_variants),
      SpecialMemberSpec(&ClassConfiguration::copy_ctor, all_variants),
      SpecialMemberSpec(&ClassConfiguration::move_ctor, all_variants),
      SpecialMemberSpec(&ClassConfiguration::copy_assign, all_variants),
      SpecialMemberSpec(&ClassConfiguration::move_assign, all_variants)>;

  static constexpr auto ALL_CONFIGS = []<SpecialMemberSpec... SPECS>(SpecialMemberSpecList<SPECS...>) {
    std::array<ClassConfiguration, ((SPECS.variants.size()) * ...)> result{};

    size_t repeats = result.size();

    auto f = [&]<SpecialMemberSpec SPEC> {
      const auto& member = SPEC.member;
      const auto& variants = SPEC.variants;

      repeats /= variants.size();

      for (size_t i = 0; i < result.size(); ++i) {
        result[i].*member = variants[(i / repeats) % variants.size()];
      }
    };

    (f.template operator()<SPECS>(), ...);

    return result;
  }(Params{});

  return [](auto f) {
    [&]<size_t... INDICES>(std::index_sequence<INDICES...>) {
      (f.template operator()<ALL_CONFIGS[INDICES]>(), ...);
    }(std::make_index_sequence<ALL_CONFIGS.size()>{});
  };
}();

template <ClassConfiguration CFG>
struct TestObject {
  using enum SpecialMemberVariant;

  ~TestObject()
    requires (CFG.dtor == TRIVIAL)
  = default;

  ~TestObject()
    requires (CFG.dtor == USER_DEFINED_NOEXCEPT)
  {}

  TestObject(const TestObject&)
    requires (CFG.copy_ctor == TRIVIAL)
  = default;

  TestObject(const TestObject&)
    requires (CFG.copy_ctor == DELETED)
  = delete;

  TestObject(const TestObject&)
    requires (CFG.copy_ctor == USER_DEFINED)
  {}

  TestObject(const TestObject&) noexcept
    requires (CFG.copy_ctor == USER_DEFINED_NOEXCEPT)
  {}

  TestObject(TestObject&&)
    requires (CFG.move_ctor == TRIVIAL)
  = default;

  TestObject(TestObject&&)
    requires (CFG.move_ctor == DELETED)
  = delete;

  TestObject(TestObject&&)
    requires (CFG.move_ctor == USER_DEFINED)
  {}

  TestObject(TestObject&&) noexcept
    requires (CFG.move_ctor == USER_DEFINED_NOEXCEPT)
  {}

  TestObject& operator=(const TestObject&)
    requires (CFG.copy_assign == TRIVIAL)
  = default;

  TestObject& operator=(const TestObject&)
    requires (CFG.copy_assign == DELETED)
  = delete;

  TestObject& operator=(const TestObject&)
    requires (CFG.copy_assign == USER_DEFINED)
  {
    return *this;
  }

  TestObject& operator=(const TestObject&) noexcept
    requires (CFG.copy_assign == USER_DEFINED_NOEXCEPT)
  {
    return *this;
  }

  TestObject& operator=(TestObject&&)
    requires (CFG.move_assign == TRIVIAL)
  = default;

  TestObject& operator=(TestObject&&)
    requires (CFG.move_assign == DELETED)
  = delete;

  TestObject& operator=(TestObject&&)
    requires (CFG.move_assign == USER_DEFINED)
  {
    return *this;
  }

  TestObject& operator=(TestObject&&) noexcept
    requires (CFG.move_assign == USER_DEFINED_NOEXCEPT)
  {
    return *this;
  }
};

// inherit so that deleted moves still allow for calling copy ctor/assignment
template <ClassConfiguration CFG>
struct DerivedTestObject : TestObject<CFG> {
  using TestObject<CFG>::TestObject;
};

} // namespace

TEST(AllVariantsTest, Presence) {
  run_all([]<ClassConfiguration CFG>() {
    using enum SpecialMemberVariant;

    using Object = TestObject<CFG>;
    using Opt = ct::Optional<Object>;

    if constexpr (CFG.copy_ctor != DELETED) {
      EXPECT_TRUE(std::is_copy_constructible_v<Opt>) << CFG;
    }
    if constexpr (CFG.move_ctor != DELETED) {
      EXPECT_TRUE(std::is_move_constructible_v<Opt>) << CFG;
    }
    if constexpr (CFG.copy_assign != DELETED && CFG.copy_ctor != DELETED) {
      EXPECT_TRUE(std::is_copy_assignable_v<Opt>) << CFG;
    }
    if constexpr (CFG.move_assign != DELETED && CFG.move_ctor != DELETED) {
      EXPECT_TRUE(std::is_move_assignable_v<Opt>) << CFG;
    }
  });
}

TEST(AllVariantsTest, Destructor) {
  run_all([]<ClassConfiguration CFG>() {
    using Object = DerivedTestObject<CFG>;
    using Opt = ct::Optional<Object>;

    EXPECT_EQ(std::is_destructible_v<Object>, std::is_destructible_v<Opt>) << CFG;
    EXPECT_EQ(std::is_nothrow_destructible_v<Object>, std::is_nothrow_destructible_v<Opt>) << CFG;
    EXPECT_EQ(std::is_trivially_destructible_v<Object>, std::is_trivially_destructible_v<Opt>) << CFG;
  });
}

TEST(AllVariantsTest, CopyConstructor) {
  run_all([]<ClassConfiguration CFG>() {
    using Object = DerivedTestObject<CFG>;
    using Opt = ct::Optional<Object>;

    EXPECT_EQ(std::is_copy_constructible_v<Object>, std::is_copy_constructible_v<Opt>) << CFG;
    EXPECT_EQ(std::is_nothrow_copy_constructible_v<Object>, std::is_nothrow_copy_constructible_v<Opt>) << CFG;
    EXPECT_EQ(std::is_trivially_copy_constructible_v<Object>, std::is_trivially_copy_constructible_v<Opt>) << CFG;
  });
}

TEST(AllVariantsTest, MoveConstructor) {
  run_all([]<ClassConfiguration CFG>() {
    using Object = DerivedTestObject<CFG>;
    using Opt = ct::Optional<Object>;

    EXPECT_EQ(std::is_move_constructible_v<Object>, std::is_move_constructible_v<Opt>) << CFG;
    EXPECT_EQ(std::is_nothrow_move_constructible_v<Object>, std::is_nothrow_move_constructible_v<Opt>) << CFG;
    EXPECT_EQ(std::is_trivially_move_constructible_v<Object>, std::is_trivially_move_constructible_v<Opt>) << CFG;
  });
}

TEST(AllVariantsTest, CopyAssignment) {
  run_all([]<ClassConfiguration CFG>() {
    using enum SpecialMemberVariant;

    using Object = DerivedTestObject<CFG>;
    using Opt = ct::Optional<Object>;

    if constexpr (CFG.copy_ctor == DELETED) {
      EXPECT_FALSE(std::is_copy_assignable_v<Opt>) << CFG;
    } else {
      EXPECT_EQ(std::is_copy_assignable_v<Object>, std::is_copy_assignable_v<Opt>) << CFG;
    }

    if constexpr (is_special_member_noexcept(CFG.copy_ctor)) {
      EXPECT_EQ(std::is_nothrow_copy_assignable_v<Object>, std::is_nothrow_copy_assignable_v<Opt>) << CFG;
    } else {
      EXPECT_FALSE(std::is_nothrow_copy_assignable_v<Opt>) << CFG;
    }

    if constexpr (CFG.copy_ctor == TRIVIAL && CFG.dtor == TRIVIAL) {
      EXPECT_EQ(std::is_trivially_copy_assignable_v<Object>, std::is_trivially_copy_assignable_v<Opt>) << CFG;
    } else {
      EXPECT_FALSE(std::is_trivially_copy_assignable_v<Opt>) << CFG;
    }
  });
}

TEST(AllVariantsTest, MoveAssignment) {
  run_all([]<ClassConfiguration CFG>() {
    using enum SpecialMemberVariant;

    using Object = DerivedTestObject<CFG>;
    using Opt = ct::Optional<Object>;

    if constexpr (CFG.move_ctor == DELETED && CFG.copy_ctor == DELETED) {
      EXPECT_FALSE(std::is_move_assignable_v<Opt>) << CFG;
    } else {
      EXPECT_EQ(std::is_move_assignable_v<Object>, std::is_move_assignable_v<Opt>) << CFG;
    }

    if constexpr (is_special_member_noexcept(CFG.move_ctor) ||
                  (CFG.move_ctor == DELETED && is_special_member_noexcept(CFG.copy_ctor))) {
      EXPECT_EQ(std::is_nothrow_move_assignable_v<Object>, std::is_nothrow_move_assignable_v<Opt>) << CFG;
    } else {
      EXPECT_FALSE(std::is_nothrow_move_assignable_v<Opt>) << CFG;
    }

    if constexpr ((CFG.move_ctor == TRIVIAL || (CFG.move_ctor == DELETED && CFG.copy_ctor == TRIVIAL)) &&
                  CFG.dtor == TRIVIAL) {
      EXPECT_EQ(std::is_trivially_move_assignable_v<Object>, std::is_trivially_move_assignable_v<Opt>) << CFG;
    } else {
      EXPECT_FALSE(std::is_trivially_move_assignable_v<Opt>) << CFG;
    }
  });
}

} // namespace ct_test
