#ifndef TREE_CIVILIZATION_ITREEGROWTHSTRATEGY_H
#define TREE_CIVILIZATION_ITREEGROWTHSTRATEGY_H

#include <set>
#include <QtCore>


class TreeNode;


class IAmountModifyingGrowthStrategy : public QObject {
Q_OBJECT
public:
    virtual ~IAmountModifyingGrowthStrategy() = default;
    virtual std::set<TreeNode*> grow_resources(TreeNode* node) const = 0;
};



using before_insert_cb_t = std::function<void(TreeNode* parent)>;
using after_insert_cb_t = std::function<void()>;

class ITreeModifyingGrowthStrategy : public QObject {
Q_OBJECT
public:
    virtual ~ITreeModifyingGrowthStrategy() = default;
    virtual void grow_resources(TreeNode* node, before_insert_cb_t before_insert_cb,
                                after_insert_cb_t after_insert_cb) const = 0;
};


#endif //TREE_CIVILIZATION_ITREEGROWTHSTRATEGY_H
