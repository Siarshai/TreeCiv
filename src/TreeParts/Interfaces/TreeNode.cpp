#include "TreeNode.h"

#include <boost/algorithm/string.hpp>


static constexpr std::array resource_strrepr{"Leaf", "Acorn"};
static_assert(resource_strrepr.size() == static_cast<int>(ResourceType::COUNT));

const char* get_strrepr(ResourceType rt) {
    return resource_strrepr.at(static_cast<int>(rt));
}

ResourceType get_typerepr(const char* strrepr) {
    auto it = std::find_if(resource_strrepr.cbegin(), resource_strrepr.cend(),
                        [strrepr] (const char* s) { return boost::iequals(strrepr, s); });
    if (it == resource_strrepr.cend())
        throw std::logic_error("Invalid resource type string representation");
    int idx = std::distance(resource_strrepr.begin(), it);
    return static_cast<ResourceType>(idx);
}


TreeNode::TreeNode(TreeNode* parent) : parentItem_(parent) {
}

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

void TreeNode::addChild(TreeNode* child) {
    childItems_.append(child);
}

bool TreeNode::setData(int column, const QVariant& value) {
    return false;
}
