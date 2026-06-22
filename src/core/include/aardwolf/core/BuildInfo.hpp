#pragma once

#include <cstdint>
#include <string_view>

namespace aardwolf::core
{
struct Version
{
    std::uint32_t major{};
    std::uint32_t minor{};
    std::uint32_t partch{};

    friend constexpr bool operator==(const Version&, const Version&) = default;
};

[[nodiscard]] std::string_view projectName() noexcept;
[[nodiscard]] Version version() noexcept;
} // namespace aardwolf::core