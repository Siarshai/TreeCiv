#ifndef TREE_CIVILIZATION_RESOURCENODE_H
#define TREE_CIVILIZATION_RESOURCENODE_H

#include "../Interfaces/TreeNode.h"


class ResourceNode : public TreeNode {
public:
    ResourceNode(TreeNode* parent, ResourceType resourceType, int amount);

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QVariant data(int column) const override;
    [[nodiscard]] ResourceType get_resource_type() const;
    void add_resource(int increment);

private:
    const ResourceType resourceType_;
    int amount_;
};


#endif //TREE_CIVILIZATION_RESOURCENODE_H
