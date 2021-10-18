#include "ResourceNode.h"
#include <cmath>

ResourceNode::ResourceNode(TreeNode* parent, ResourceType resourceType, int amount)
        : TreeNode(parent), resourceType_(resourceType), amount_(amount) {}

int ResourceNode::columnCount() const {
    return 2;
}

QVariant ResourceNode::data(int column) const {
    if (column == 0)
        return "Resource - " + QString(get_strrepr(resourceType_));  // TODO: To variable
    else if (column == 1)
        return std::floor(amount_);  // TODO: Should i use float number raw?
    return QVariant();
}

ResourceType ResourceNode::get_resource_type() const {
    return resourceType_;
}

void ResourceNode::add_resource(int increment) {
    amount_ += increment;
}
