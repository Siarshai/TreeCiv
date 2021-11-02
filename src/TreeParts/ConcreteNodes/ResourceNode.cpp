#include "ResourceNode.h"


ResourceNode::ResourceNode(TreeNode* parent, ResourceType resourceType, int amount)
        : TreeNode(parent), resourceType_(resourceType), amount_(amount) {}

QVariant ResourceNode::data(DataRoles role) const {
    switch (role) {
        case DataRoles::DelegateTypeRole:
            return "gatherable";
        case DataRoles::DisplayRole:
            return QString(get_strrepr(resourceType_));  // TODO: To variable
        case DataRoles::ResourceTypeRole:
            return static_cast<int>(resourceType_);
        case DataRoles::ResourceAmountRole:
            return amount_;
        default:
            throw std::logic_error("Requesting from ResourceNode incompatible data: "
                                   + std::to_string(static_cast<int>(role)));
    }
}

ResourceType ResourceNode::get_resource_type() const {
    return resourceType_;
}

void ResourceNode::add_resource(int increment) {
    amount_ += increment;
}
