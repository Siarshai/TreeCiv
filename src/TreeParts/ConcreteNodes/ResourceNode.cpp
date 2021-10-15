#include "ResourceNode.h"


ResourceNode::ResourceNode(TreeNode* parent, ResourceType resourceType, unsigned int level)
        : TreeNode(parent), resourceType_(resourceType), level_(level) {}

int ResourceNode::columnCount() const {
    return 3;
}

QVariant ResourceNode::data(int column) const {
    if (column == 0)
        return "Resource - " + QString(get_strrepr(resourceType_));  // TODO: To variable
    else if (column == 1)
        return level_;
    return QVariant();
}
