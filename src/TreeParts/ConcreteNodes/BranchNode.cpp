#include "BranchNode.h"
#include <cmath>


BranchNode::BranchNode(TreeNode* parent, const QString& name, int capacity)
    : TreeNode(parent), name_(name), capacity_(capacity) {}

QVariant BranchNode::data(DataRoles role) const {
    switch (role) {
        case DataRoles::DelegateTypeRole:
            return "carcass";
        case DataRoles::DisplayRole:
            return "Branch - " + name_;
        default:
            throw std::logic_error("Requesting from BranchNode incompatible data: "
                                   + std::to_string(static_cast<int>(role)));
    }
}

int BranchNode::get_capacity() const {
    return capacity_;
}
