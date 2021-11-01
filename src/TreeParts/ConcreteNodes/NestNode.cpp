#include "NestNode.h"


NestNode::NestNode(TreeNode* parent, const QString& name, int level)
    : TreeNode(parent), name_(name), level_(level) {}

QVariant NestNode::data(DataRoles role) const {
    switch (role) {
        case DataRoles::DelegateTypeRole:
            return "nest";
        case DataRoles::DisplayRole:
            return "Nest - " + name_;
        case DataRoles::LevelRole:
            return level_;
        default:
            throw std::logic_error("Requesting from NestNode incompatible data: "
                                   + std::to_string(static_cast<int>(role)));
    }
}
