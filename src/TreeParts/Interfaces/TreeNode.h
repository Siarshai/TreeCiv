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


// TODO: Enhance typing with base template class?
class TreeNode {
public:
    explicit TreeNode(TreeNode* parent);
    virtual ~TreeNode();
    [[nodiscard]] TreeNode* child(int number);
    [[nodiscard]] const TreeNode* child(int number) const;
    [[nodiscard]] int childCount() const;
    [[nodiscard]] TreeNode* parent() const;
    [[nodiscard]] int childNumber() const;

    void addChild(TreeNode* child);

    [[nodiscard]] virtual bool setData(int column, const QVariant &value);
    [[nodiscard]] virtual int columnCount() const = 0;
    [[nodiscard]] virtual QVariant data(int column) const = 0;

protected:
    // TODO: Use std::reference_wrapper here?
    QList<TreeNode*> childItems_;
    TreeNode* parentItem_;
};


#endif //TREE_CIVILIZATION_TREENODE_H
