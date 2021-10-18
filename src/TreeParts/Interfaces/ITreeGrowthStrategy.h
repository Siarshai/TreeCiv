#ifndef TREE_CIVILIZATION_ITREEGROWTHSTRATEGY_H
#define TREE_CIVILIZATION_ITREEGROWTHSTRATEGY_H


#include <QtCore>
#include "TreeNode.h"


class ITreeGrowthStrategy : public QObject {
Q_OBJECT
public:
    virtual ~ITreeGrowthStrategy() = default;
    virtual void grow_resources(TreeNode* node) const = 0;
};


#endif //TREE_CIVILIZATION_ITREEGROWTHSTRATEGY_H
