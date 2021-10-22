#ifndef TREE_CIVILIZATION_TREEGROWTHSTRATEGY_H
#define TREE_CIVILIZATION_TREEGROWTHSTRATEGY_H

#include <set>
#include "Interfaces/ITreeGrowthStrategy.h"
#include "src/TreeParts/Interfaces/TreeNode.h"


class BranchNode;
class ResourceNode;

class ResourceBreedsResourceGrowthStrategy : public IAmountModifyingGrowthStrategy {
public:
    explicit ResourceBreedsResourceGrowthStrategy(int resource_per_tick);
    std::set<TreeNode*> grow_resources(TreeNode* node) const override;
private:
    static void gather_eligible_branches(std::set<BranchNode*>& branches,
                                         TreeNode* node, ResourceType rt);
    int resource_per_tick_;
};


class RandomPoppingResourcesGrowthStrategy : public ITreeModifyingGrowthStrategy {
public:
    explicit RandomPoppingResourcesGrowthStrategy(int resource_per_tick);
    void grow_resources(TreeNode* node,
            before_insert_cb_t before_insert_cb, after_insert_cb_t after_insert_cb) const override;
private:
    static void gather_branches(std::set<BranchNode*>& branches, TreeNode* node);
    int resource_per_tick_;
};

using RBRGStrategy = ResourceBreedsResourceGrowthStrategy;
using RGStrategy = RandomPoppingResourcesGrowthStrategy;

#endif //TREE_CIVILIZATION_TREEGROWTHSTRATEGY_H
