#include "TrunkNode.h"


TrunkNode::TrunkNode(TreeNode* parent, const QString& name)
    : TreeNode(parent), name_(name) {}

QVariant TrunkNode::data(DataRoles role) const {
    switch (role) {
        case DataRoles::DelegateTypeRole:
            return "carcass";
        case DataRoles::DisplayRole:
            return "Trunk - " + name_;
        default:
            throw std::logic_error("Requesting from TrunkNode incompatible data: "
                + std::to_string(static_cast<int>(role)));
    }
}

