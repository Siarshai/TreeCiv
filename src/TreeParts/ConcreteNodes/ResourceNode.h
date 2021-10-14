#ifndef TREE_CIVILIZATION_RESOURCENODE_H
#define TREE_CIVILIZATION_RESOURCENODE_H

#include "../Interfaces/TreeNode.h"


class ResourceNode : public TreeNode {
public:
    ResourceNode(TreeNode* parent, ResourceType resourceType, unsigned int quantity);

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QVariant data(int column) const override;

private:
    const ResourceType resourceType_;
    const unsigned int quantity_;
};


#endif //TREE_CIVILIZATION_RESOURCENODE_H
