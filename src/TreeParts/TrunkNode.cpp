#include "TrunkNode.h"

TrunkNode::TrunkNode(TreeNode* parent, QVariant name)
    : TreeNode(parent), name_(std::move(name)) {}

int TrunkNode::columnCount() const {
    return 1;
}

QVariant TrunkNode::data(int column) const {
    if (column == 0)
        return name_;
    return "";
}
