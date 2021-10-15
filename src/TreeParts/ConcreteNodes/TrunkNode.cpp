#include "TrunkNode.h"


TrunkNode::TrunkNode(TreeNode* parent, const QString& name)
    : TreeNode(parent), name_(name) {}

int TrunkNode::columnCount() const {
    return 1;
}

QVariant TrunkNode::data(int column) const {
    if (column == 0)
        return "Trunk - " + name_;
    return QVariant();
}
