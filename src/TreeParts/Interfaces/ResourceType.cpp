#include "ResourceType.h"
#include <array>
#include <boost/algorithm/string.hpp>


static constexpr std::array resource_strrepr{"Leaf", "Acorn"};
static_assert(resource_strrepr.size() == static_cast<int>(ResourceType::COUNT));

const char* get_strrepr(ResourceType rt) {
    return resource_strrepr.at(static_cast<int>(rt));
}

ResourceType get_typerepr(const char* strrepr) {
    auto it = std::find_if(resource_strrepr.cbegin(), resource_strrepr.cend(),
                           [strrepr] (const char* s) { return boost::iequals(strrepr, s); });
    if (it == resource_strrepr.cend())
        throw std::logic_error("Invalid resource type string representation");
    int idx = std::distance(resource_strrepr.begin(), it);
    return static_cast<ResourceType>(idx);
}
