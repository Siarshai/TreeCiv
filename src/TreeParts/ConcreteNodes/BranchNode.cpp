#include "BranchNode.h"


BranchNode::BranchNode(TreeNode* parent, const QString& name, int capacity)
    : TreeNode(parent), name_(std::move(name)), capacity_(capacity) {}

int BranchNode::columnCount() const {
    return 1;
}

QVariant BranchNode::data(int column) const {
    if (column == 0)
        return name_;
    return QVariant();
}
