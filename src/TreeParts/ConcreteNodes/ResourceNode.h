#ifndef TREE_CIVILIZATION_RESOURCENODE_H
#define TREE_CIVILIZATION_RESOURCENODE_H

#include "../Interfaces/TreeNode.h"


class ResourceNode : public TreeNode {
public:
    ResourceNode(TreeNode* parent, ResourceType resourceType, unsigned int level);

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QVariant data(int column) const override;

private:
    const ResourceType resourceType_;
    const unsigned int level_;
};


#endif //TREE_CIVILIZATION_RESOURCENODE_H
