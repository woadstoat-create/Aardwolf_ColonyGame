#include <aardwolf/core/BuildInfo.hpp>>

namespace aardwolf::core
{
std::string_view projectName() noexcept
{
    return AARDWOLF_PROJECT_NAME;
}

Version version() noexcept
{
    return Version{static_cast<std::uint32_t>(AARDWOLF_VERSION_MAJOR),
                   static_cast<std::uint32_t>(AARDWOLF_VERSION_MINOR),
                   static_cast<std::uint32_t>(AARDWOLF_VERSION_PATCH)};
}
} // namespace aardwolf::core