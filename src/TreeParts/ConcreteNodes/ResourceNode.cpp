#include "ResourceNode.h"


ResourceNode::ResourceNode(TreeNode* parent, ResourceType resourceType, unsigned int quantity)
        : TreeNode(parent), resourceType_(resourceType), quantity_(quantity) {}

int ResourceNode::columnCount() const {
    return 3;
}

QVariant ResourceNode::data(int column) const {
    if (column == 0)
        return get_strrepr(resourceType_);
    else if (column == 2)
        return quantity_;
    return QVariant();
}
