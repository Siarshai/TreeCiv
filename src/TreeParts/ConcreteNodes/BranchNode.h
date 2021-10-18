#ifndef TREE_CIVILIZATION_BRANCHNODE_H
#define TREE_CIVILIZATION_BRANCHNODE_H

#include "src/TreeParts/Interfaces/TreeNode.h"


class BranchNode : public TreeNode {
public:
    BranchNode(TreeNode* parent, const QString& name, int capacity);

    [[nodiscard]] int columnCount() const override;
    [[nodiscard]] QVariant data(int column) const override;
    [[nodiscard]] int get_capacity() const;

private:
    QString name_;
    const int capacity_;
};


#endif //TREE_CIVILIZATION_BRANCHNODE_H
