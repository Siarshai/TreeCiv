#include <set>
#include "TreeGrowthStrategy.h"

#include "ConcreteNodes/BranchNode.h"
#include "ConcreteNodes/ResourceNode.h"


template <typename T>
typename T::iterator select_random_element(T& container) {
    auto n = rand() % container.size();
    auto it = std::begin(container);
    std::advance(it, n);
    return it;
}

template <typename T>
typename T::const_iterator select_random_element(const T& container) {
    auto n = rand() % container.size();
    auto it = std::begin(container);
    std::advance(it, n);
    return it;
}


static constexpr std::array iterable_resource_types{ResourceType::LEAF, ResourceType::ACORN};
static_assert(iterable_resource_types.size() == static_cast<int>(ResourceType::COUNT));


std::set<ResourceNode*> get_resource_nodes(BranchNode* branch) {
    std::set<ResourceNode*> resource_nodes;
    int count = branch->childCount();
    for (int i = 0; i < count; ++i)
        if (auto resource_node = dynamic_cast<ResourceNode*>(branch->child(i)))
            resource_nodes.insert(resource_node);
    return resource_nodes;
}

int max_resource_amount_for_node(const BranchNode* branch, const std::set<ResourceNode*>& resource_nodes,
                                 int max_resource_per_tick) {
    // Need to take into account both capacity expenditure sources because
    // we can possibly mix two growing strategies
    int resource_over_children = std::accumulate(
            resource_nodes.cbegin(), resource_nodes.cend(), 0,
            [](int acc, ResourceNode* rn) { return acc + rn->data(1).toInt(); });
    int capacity_left = branch->get_capacity() - resource_over_children;
    // Possibly can be zero because you can graft leafs beyond max capacity
    return max_resource_per_tick > capacity_left ? std::max(capacity_left, 0) : max_resource_per_tick;
}


ResourceBreedsResourceGrowthStrategy::ResourceBreedsResourceGrowthStrategy(int resource_per_tick)
        : resource_per_tick_(resource_per_tick) {}

void ResourceBreedsResourceGrowthStrategy::grow_resources(TreeNode* node) const {
    for (const auto& rt : iterable_resource_types) {
        std::set<BranchNode*> branches;
        gather_eligible_branches(branches, node, rt);
        if (!branches.empty()) {
            BranchNode* branch = *select_random_element(branches);
            auto resource_nodes = get_resource_nodes(branch);
            int resource_amount = max_resource_amount_for_node(branch, resource_nodes, resource_per_tick_);
            if (resource_amount > 0) {
                // At least one element exists, otherwise that branch wouldn't be eligible
                auto resource_it = select_random_element(resource_nodes);
                (*resource_it)->add_resource(resource_amount);
            }
        }
    }
}

void ResourceBreedsResourceGrowthStrategy::gather_eligible_branches(
        std::set<BranchNode*>& branches, TreeNode* node, ResourceType rt) {
    if (auto resource_node = dynamic_cast<ResourceNode*>(node)) {
        if (resource_node->get_resource_type() == rt) {
            if (auto parent = node->parent()) {
                auto branch_node = dynamic_cast<BranchNode*>(parent);
                if (!branch_node)
                    throw std::logic_error("Tree invariant violated: Resource is not a child of a Branch");
                branches.insert(branch_node);
            }
        }
    }
    const int count = node->childCount();
    for (int i = 0; i < count; ++i)
        gather_eligible_branches(branches, node->child(i), rt);
}


RandomPoppingResourcesGrowthStrategy::RandomPoppingResourcesGrowthStrategy(int resource_per_tick)
        : resource_per_tick_(resource_per_tick) {}

void RandomPoppingResourcesGrowthStrategy::grow_resources(TreeNode* node) const {
    std::set<BranchNode*> branches;
    gather_branches(branches, node);
    if (!branches.empty()) {
        BranchNode* branch = *select_random_element(branches);
        ResourceType rt = *select_random_element(iterable_resource_types);
        auto resource_nodes = get_resource_nodes(branch);
        int resource_amount = max_resource_amount_for_node(branch, resource_nodes, resource_per_tick_);
        if (resource_amount > 0)
            branch->addChild(new ResourceNode(branch, rt, resource_amount));
    }
}

void RandomPoppingResourcesGrowthStrategy::gather_branches(std::set<BranchNode*>& branches, TreeNode* node) {
    if (auto branch_node = dynamic_cast<BranchNode*>(node))
        branches.insert(branch_node);
    const int count = node->childCount();
    for (int i = 0; i < count; ++i)
        gather_branches(branches, node->child(i));
}