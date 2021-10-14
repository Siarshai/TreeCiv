#include "NestNode.h"


NestNode::NestNode(TreeNode* parent, const QString& name, int level)
    : TreeNode(parent), name_(name), level_(level) {}

int NestNode::columnCount() const {
    return 2;
}

QVariant NestNode::data(int column) const {
    if (column == 0)
        return name_;
    else if (column == 1)
        return level_;
    return QVariant();
}
