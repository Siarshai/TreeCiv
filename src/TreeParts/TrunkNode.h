#ifndef TREE_CIVILIZATION_TRUNKNODE_H
#define TREE_CIVILIZATION_TRUNKNODE_H

#include "TreeNode.h"

class TrunkNode : public TreeNode {
public:
    explicit TrunkNode(TreeNode* parent, QVariant name);

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QVariant data(int column) const override;

private:
    QVariant name_;
};


#endif //TREE_CIVILIZATION_TRUNKNODE_H
