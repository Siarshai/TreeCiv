#ifndef TREE_CIVILIZATION_TREENODE_H
#define TREE_CIVILIZATION_TREENODE_H

#include <QList>
#include <QVariant>
#include <QVector>
#include "ResourceType.h"


enum class DataRoles  {
    DisplayRole = Qt::DisplayRole,
    DisplayAuxiliaryRole = Qt::UserRole + 1,
    DelegateTypeRole = Qt::UserRole + 2,
    LevelRole = Qt::UserRole + 3,
    ResourceTypeRole = Qt::UserRole + 4,
    ResourceAmountRole = Qt::UserRole + 5,
    NodeIdRole = Qt::UserRole + 6
};


class TreeNode {
public:
    explicit TreeNode(TreeNode* parent);
    virtual ~TreeNode();
    [[nodiscard]] TreeNode* child(int number);
    [[nodiscard]] const TreeNode* child(int number) const;
    [[nodiscard]] int childCount() const;
    [[nodiscard]] TreeNode* parent() const;
    [[nodiscard]] int childNumber() const;
    void removeChild(int number);

    void addChild(TreeNode* child);
    [[nodiscard]] TreeNode* recursive_search_for_node(const std::string& uid) const;

    [[nodiscard]] virtual QVariant data(DataRoles role) const = 0;

protected:
    QList<TreeNode*> childItems_;
    TreeNode* parentItem_;
    const std::string uid_;
};


#endif //TREE_CIVILIZATION_TREENODE_H
