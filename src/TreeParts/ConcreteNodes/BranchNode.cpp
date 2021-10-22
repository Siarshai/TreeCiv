#include "BranchNode.h"
#include <cmath>


BranchNode::BranchNode(TreeNode* parent, const QString& name, int capacity)
    : TreeNode(parent), name_(name), capacity_(capacity) {}

int BranchNode::columnCount() const {
    return 1;
}

QVariant BranchNode::data(int column) const {
    if (column == 0)
        return "Branch - " + name_;
    return QVariant("");
}

int BranchNode::get_capacity() const {
    return capacity_;
}
