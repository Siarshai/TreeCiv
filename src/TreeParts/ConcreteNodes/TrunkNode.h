#ifndef TREE_CIVILIZATION_TRUNKNODE_H
#define TREE_CIVILIZATION_TRUNKNODE_H

#include "src/TreeParts/Interfaces/TreeNode.h"

class TrunkNode : public TreeNode {
public:
    TrunkNode(TreeNode* parent, const QString& name);

    [[nodiscard]] QVariant data(DataRoles role) const override;

private:
    QString name_;
};


#endif //TREE_CIVILIZATION_TRUNKNODE_H
