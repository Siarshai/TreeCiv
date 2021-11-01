#ifndef TREE_CIVILIZATION_NESTNODE_H
#define TREE_CIVILIZATION_NESTNODE_H


#include <QtCore/QVariant>
#include <src/TreeParts/Interfaces/TreeNode.h>


class NestNode : public TreeNode {
public:
    NestNode(TreeNode* parent, const QString& name, int level);

    [[nodiscard]] QVariant data(DataRoles role) const override;

private:
    QString name_;
    int level_;
};


#endif //TREE_CIVILIZATION_NESTNODE_H
