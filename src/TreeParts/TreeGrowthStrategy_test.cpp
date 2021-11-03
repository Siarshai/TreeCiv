#include <src/TreeParts/ConcreteNodes/ResourceNode.h>
#include "gtest/gtest.h"
#include "TreeParser.h"
#include "TreeGrowthStrategy.h"


TEST(TreeGrowthStrategiesTest, RBRSimpleTestWorksAtAll) {
    const std::vector<std::string> tree_text_repr = {
            "trunk t",
            ">",
            "branch b 10",
            ">",
            "resource leaf 1"
    };
    auto strategy = std::make_shared<RBRGStrategy>(1);
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));
    for (int _ = 0; _ < 5; ++_)
        strategy->grow_resources(root_trunk_node.get());
    ASSERT_EQ(root_trunk_node->child(0)->child(0)->data(DataRoles::ResourceAmountRole).toInt(), 6);
}


TEST(TreeGrowthStrategiesTest, RBRNoGrowBeyondCapacity) {
    const std::vector<std::string> tree_text_repr = {
            "trunk t",
            ">",
            "branch b 10",
            ">",
            "resource leaf 1"
    };
    auto strategy = std::make_shared<RBRGStrategy>(1);
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));
    for (int _ = 0; _ < 50; ++_)
        strategy->grow_resources(root_trunk_node.get());
    ASSERT_EQ(root_trunk_node->child(0)->child(0)->data(DataRoles::ResourceAmountRole).toInt(), 10);
}


TEST(TreeGrowthStrategiesTest, RBRGrowsOnlyMatchingResourceWhereItIsPresent) {
    const std::vector<std::string> tree_text_repr = {
            "trunk t",
            ">",
            "branch b1 5",
            ">",
            "resource leaf 1",
            "resource leaf 1",
            "<",
            "branch b2 5",
            ">",
            "resource acorn 1",
            "resource acorn 1",
            "<",
            "branch b3 10",
    };
    auto strategy = std::make_shared<RBRGStrategy>(1);
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));
    for (int _ = 0; _ < 1000; ++_)
        strategy->grow_resources(root_trunk_node.get());

    auto are_only_expected_resources_grow = [] (TreeNode* branch, ResourceType expected_resource_type,
                                                float expected_amount_low, float expected_amount_high) {
        ASSERT_EQ(branch->childCount(), 2);
        auto* leaf_node_1 = dynamic_cast<ResourceNode*>(branch->child(0));
        auto* leaf_node_2 = dynamic_cast<ResourceNode*>(branch->child(1));
        ASSERT_NE(leaf_node_1, nullptr);
        ASSERT_NE(leaf_node_2, nullptr);
        ASSERT_EQ(leaf_node_1->get_resource_type(), expected_resource_type);
        ASSERT_EQ(leaf_node_2->get_resource_type(), expected_resource_type);
        auto r1_amount = leaf_node_1->data(DataRoles::ResourceAmountRole).toInt();
        auto r2_amount = leaf_node_2->data(DataRoles::ResourceAmountRole).toInt();
        ASSERT_GE(r1_amount + r2_amount, expected_amount_low);
        ASSERT_LE(r1_amount + r2_amount, expected_amount_high);
    };

    // Amount can be lower than branch capacity due to unequal resource distribution
    // So if there is two resource nodes amount of resource may be divided as 2.3 2.7
    // between them. Even though capacity of branch is 5, rounded values will be 2 and 2.
    // This is feature, not a bug.
    are_only_expected_resources_grow(root_trunk_node->child(0), ResourceType::LEAF, 4, 5);
    are_only_expected_resources_grow(root_trunk_node->child(1), ResourceType::ACORN, 4, 5);
    ASSERT_EQ(root_trunk_node->child(2)->childCount(), 0);
}


TEST(TreeGrowthStrategiesTest, RGSimpleTestWorksAtAll) {
    const std::vector<std::string> tree_text_repr = {
            "trunk t",
            ">",
            "branch b 10",
            ">",
            "resource leaf 1"
    };
    auto strategy = std::make_shared<RGStrategy>(1);
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));

    int before_insert_calls = 0;
    int after_insert_calls = 0;
    for (int _ = 0; _ < 5; ++_) {
        strategy->grow_resources(root_trunk_node.get(),
                [&before_insert_calls](TreeNode*) { ++before_insert_calls; },
                [&after_insert_calls]() { ++after_insert_calls; });
    }

    const int expected_resource_node_amount = 6;
    ASSERT_EQ(root_trunk_node->child(0)->childCount(), expected_resource_node_amount);
    for (int i = 0; i < expected_resource_node_amount; ++i) {
        auto resource_node = dynamic_cast<ResourceNode*>(root_trunk_node->child(0)->child(i));
        ASSERT_EQ(resource_node->data(DataRoles::ResourceAmountRole).toInt(), 1);
    }
    ASSERT_EQ(before_insert_calls, expected_resource_node_amount - 1);
    ASSERT_EQ(after_insert_calls, expected_resource_node_amount - 1);
}


TEST(TreeGrowthStrategiesTest, RGSimpleTestWorksAtAll2BigIncrements) {
    const std::vector<std::string> tree_text_repr = {
            "trunk t",
            ">",
            "branch b 10"
    };
    auto strategy = std::make_shared<RGStrategy>(2);
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));
    for (int _ = 0; _ < 3; ++_)
        strategy->grow_resources(root_trunk_node.get(), [](TreeNode*){}, [](){});
    const int expected_resource_node_amount = 3;
    ASSERT_EQ(root_trunk_node->child(0)->childCount(), expected_resource_node_amount);
    for (int i = 1; i < expected_resource_node_amount; ++i) {
        auto resource_node = dynamic_cast<ResourceNode*>(root_trunk_node->child(0)->child(i));
        ASSERT_NE(resource_node, nullptr);
        ASSERT_EQ(resource_node->data(DataRoles::ResourceAmountRole).toInt(), 2);
    }
}


TEST(TreeGrowthStrategiesTest, RGNoGrowthBeyondCapacity) {
    const std::vector<std::string> tree_text_repr = {
            "trunk t",
            ">",
            "branch b 10",
    };
    auto strategy = std::make_shared<RGStrategy>(1);
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));
    for (int _ = 0; _ < 100; ++_)
        strategy->grow_resources(root_trunk_node.get(), [](TreeNode*){}, [](){});
    ASSERT_EQ(root_trunk_node->child(0)->childCount(), 10);
}

