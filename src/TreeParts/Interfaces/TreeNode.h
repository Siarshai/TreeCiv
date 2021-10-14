#ifndef TREE_CIVILIZATION_TREENODE_H
#define TREE_CIVILIZATION_TREENODE_H

#include <QList>
#include <QVariant>
#include <QVector>


enum class ResourceType {
    LEAF,
    ACORN,
    COUNT
};
const char* get_strrepr(ResourceType rt);
ResourceType get_typerepr(const char* strrepr);

class TreeNode {
public:
    explicit TreeNode(TreeNode* parent);
    virtual ~TreeNode();
    [[nodiscard]] TreeNode* child(int number);
    [[nodiscard]] int childCount() const;
    [[nodiscard]] TreeNode* parent() const;
    [[nodiscard]] int childNumber() const;

    void addChild(TreeNode* child);

    [[nodiscard]] virtual bool setData(int column, const QVariant &value);
    [[nodiscard]] virtual int columnCount() const = 0;
    [[nodiscard]] virtual QVariant data(int column) const = 0;

protected:
    QList<TreeNode*> childItems_;
    TreeNode* parentItem_;
};


#endif //TREE_CIVILIZATION_TREENODE_H
