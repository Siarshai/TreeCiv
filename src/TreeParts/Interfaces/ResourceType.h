#ifndef TREE_CIVILIZATION_RESOURCETYPE_H
#define TREE_CIVILIZATION_RESOURCETYPE_H


enum class ResourceType {
    LEAF,
    ACORN,
    COUNT
};
const char* get_strrepr(ResourceType rt);
ResourceType get_typerepr(const char* strrepr);


#endif //TREE_CIVILIZATION_RESOURCETYPE_H
