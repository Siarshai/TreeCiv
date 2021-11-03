#include "TreeNode.h"


std::string random_uid() {
    static const std::string charset("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    static const int length = 16;

    std::string result;
    result.resize(length);
    for (int i = 0; i < length; i++)
        result[i] = charset[rand() % charset.length()];

    return result;
}


TreeNode::TreeNode(TreeNode* parent)
    : parentItem_(parent)
    , uid_(random_uid()) {}

TreeNode::~TreeNode() {
    qDeleteAll(childItems_);
}

TreeNode* TreeNode::child(int number) {
    return childItems_.at(number);
}

const TreeNode* TreeNode::child(int number) const {
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

void TreeNode::removeChild(int number) {
    delete childItems_[number];
    childItems_.removeAt(number);
}

void TreeNode::addChild(TreeNode* child) {
    childItems_.append(child);
}

TreeNode* TreeNode::recursive_search_for_node(const std::string& uid) const {
    for (const auto& child : childItems_) {
        if (child->uid_ == uid)
            return child;
    }
    TreeNode* result = nullptr;
    for (const auto& child : childItems_) {
        result = child->recursive_search_for_node(uid);
        if (result)
            break;
    }
    return result;
}
