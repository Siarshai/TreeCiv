#include "TreeNode.h"

TreeNode::TreeNode(TreeNode* parent) : parentItem_(parent) {
}

TreeNode::~TreeNode() {
    qDeleteAll(childItems_);
}

TreeNode* TreeNode::child(int number) {
    return childItems_.at(number);
}

int TreeNode::childCount() const {
    return childItems_.size();
}

TreeNode* TreeNode::parent() const {
    return parentItem_;
}

int TreeNode::childNumber() const {
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<TreeNode*>(this));
    return 0;
}

void TreeNode::addChild(TreeNode* child) {
    childItems_.append(child);
}

bool TreeNode::setData(int column, const QVariant& value) {
    return false;
}
