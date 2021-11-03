#include <utility>

#include "gtest/gtest.h"
#include "BranchNode.h"
#include "NestNode.h"
#include "ResourceNode.h"
#include "TrunkNode.h"


TEST(TreeParserTest, UpgradeNestTest) {
    auto nest = std::make_unique<NestNode>(nullptr, "nest_name", 1);
    ASSERT_EQ(nest->data(DataRoles::LevelRole).toInt(), 1);
    nest->upgrade_nest();
    ASSERT_EQ(nest->data(DataRoles::LevelRole).toInt(), 2);
}


TEST(TreeParserTest, RemoveChildTest) {
    auto root = std::make_unique<TrunkNode>(nullptr, "root");
    root->addChild(new BranchNode(root.get(), "irrelevant_branch", 10));

    auto branch = new BranchNode(root.get(), "branch", 10);
    root->addChild(branch);

    branch->addChild(new BranchNode(branch, "other_irrelevant_branch", 10));
    branch->addChild(new ResourceNode(branch, ResourceType::LEAF, 1));
    branch->addChild(new ResourceNode(branch, ResourceType::ACORN, 2));
    branch->addChild(new NestNode(branch, "nest_name", 1));

    ASSERT_EQ(branch->childCount(), 4);
    ASSERT_EQ(branch->child(2)->data(DataRoles::DelegateTypeRole).toString().toStdString(), "gatherable");
    branch->removeChild(2);
    ASSERT_EQ(branch->childCount(), 3);
    ASSERT_EQ(branch->child(2)->data(DataRoles::DelegateTypeRole).toString().toStdString(), "nest");
}


TEST(TreeParserTest, SearchForUidTest) {
    auto root = std::make_unique<TrunkNode>(nullptr, "root");
    root->addChild(new BranchNode(root.get(), "irrelevant_branch", 10));

    auto branch = new BranchNode(root.get(), "branch", 10);
    root->addChild(branch);

    branch->addChild(new BranchNode(branch, "other_irrelevant_branch", 10));
    branch->addChild(new ResourceNode(branch, ResourceType::LEAF, 1));
    auto rn1_uid = branch->child(branch->childCount()-1)->data(DataRoles::NodeIdRole).toString().toStdString();
    branch->addChild(new ResourceNode(branch, ResourceType::ACORN, 2));
    auto rn2_uid = branch->child(branch->childCount()-1)->data(DataRoles::NodeIdRole).toString().toStdString();
    branch->addChild(new NestNode(branch, "nest_name", 1));
    auto rn3_uid = branch->child(branch->childCount()-1)->data(DataRoles::NodeIdRole).toString().toStdString();

    TreeNode* found_node = root->recursive_search_for_node(rn1_uid);
    ASSERT_NE(found_node, nullptr);
    ASSERT_EQ(found_node->data(DataRoles::ResourceTypeRole), static_cast<int>(ResourceType::LEAF));
    ASSERT_EQ(found_node->data(DataRoles::ResourceAmountRole), 1);

    found_node = root->recursive_search_for_node(rn2_uid);
    ASSERT_NE(found_node, nullptr);
    ASSERT_EQ(found_node->data(DataRoles::ResourceTypeRole), static_cast<int>(ResourceType::ACORN));
    ASSERT_EQ(found_node->data(DataRoles::ResourceAmountRole), 2);

    found_node = root->recursive_search_for_node(rn3_uid);
    ASSERT_NE(found_node, nullptr);
    ASSERT_EQ(found_node->data(DataRoles::DisplayRole).toString().toStdString(), "nest_name");
    ASSERT_EQ(found_node->data(DataRoles::LevelRole), 1);
}
