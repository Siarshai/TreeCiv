#include "ResourceNode.h"


ResourceNode::ResourceNode(TreeNode* parent, ResourceType resourceType, int amount)
        : TreeNode(parent)
        , resourceType_(resourceType)
        , resourceTypeStrrepr_(get_strrepr(resourceType_))
        , amount_(amount) {}

QVariant ResourceNode::data(DataRoles role) const {
    switch (role) {
        case DataRoles::DelegateTypeRole:
            return "gatherable";
        case DataRoles::DisplayRole:
            return resourceTypeStrrepr_;
        case DataRoles::ResourceTypeRole:
            return static_cast<int>(resourceType_);
        case DataRoles::ResourceAmountRole:
            return amount_;
        case DataRoles::NodeIdRole:
            return QString::fromStdString(uid_);
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
