#ifndef TREE_CIVILIZATION_BRANCHNODE_H
#define TREE_CIVILIZATION_BRANCHNODE_H

#include "TreeNode.h"

class BranchNode : public TreeNode {
public:
    BranchNode(TreeNode* parent, QVariant name, int capacity);

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QVariant data(int column) const override;

private:
    QVariant name_;
    const int capacity_;
};


#endif //TREE_CIVILIZATION_BRANCHNODE_H
