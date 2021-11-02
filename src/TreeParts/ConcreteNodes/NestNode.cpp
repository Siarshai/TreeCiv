#include "NestNode.h"


NestNode::NestNode(TreeNode* parent, const QString& name, int level)
    : TreeNode(parent), name_(name), level_(level) {}

QVariant NestNode::data(DataRoles role) const {
    switch (role) {
        case DataRoles::DelegateTypeRole:
            return "nest";
        case DataRoles::DisplayRole:
            return name_;
        case DataRoles::DisplayAuxiliaryRole:
            return "Squirrels";  // fixed description right now
        case DataRoles::LevelRole:
            return level_;
        case DataRoles::NodeIdRole:
            return QString::fromStdString(uid_);
        default:
            throw std::logic_error("Requesting from NestNode incompatible data: "
                                   + std::to_string(static_cast<int>(role)));
    }
}

void NestNode::upgrade_nest() {
    ++level_;
}
