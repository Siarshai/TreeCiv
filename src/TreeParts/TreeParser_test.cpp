#include <utility>

#include "gtest/gtest.h"
#include "TreeParser.h"
#include "src/TreeParts/Interfaces/TreeNode.h"

using namespace testing;


TEST(TreeParserTest, EmptyReturnNullptr) {
    ASSERT_EQ(TreeParser().parse_tree({}), nullptr);
}


TEST(TreeParserTest, GoingDeeperOnEmptyTreeThrows) {
    std::vector<std::string> tree_text_repr = { ">" };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, GoingUpOnEmptyTreeThrows) {
    std::vector<std::string> tree_text_repr = { "<" };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, GoingUpOnRootThrows) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            "<"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, NoGoingDownOnRootFails) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            "trunk t2"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, GoingDeeperTwiceThrows) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            ">"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, SimpleRootCreate) {
    std::vector<std::string> tree_text_repr = { "trunk t1" };
    std::unique_ptr<TreeNode> root(TreeParser().parse_tree(tree_text_repr));
    ASSERT_NE(root, nullptr);
    ASSERT_EQ(root->parent(), nullptr);
    ASSERT_EQ(root->childCount(), 0);
    ASSERT_EQ(root->childNumber(), 0);
}


TEST(TreeParserTest, Depth2Test) {
    std::vector<std::string> children_data = {"t11", "t12", "t13"};
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">"
    };
    for (const auto& name : children_data)
        tree_text_repr.push_back("trunk " + name);

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(node->childCount(), 3);
    for (int i = 0; i < node->childCount(); ++i) {
        TreeNode* subnode = node->child(i);
        QVariant data = subnode->data(DataRoles::DisplayRole);
        ASSERT_EQ(data.toString().toStdString(), children_data[i]);
        QVariant auxiliary_data = subnode->data(DataRoles::DisplayAuxiliaryRole);
        ASSERT_EQ(auxiliary_data.toString().toStdString(), "Trunk");
    }
}


TEST(TreeParserTest, Depth3Test) {
    std::vector<std::string> children_data = {"t111", "t112", "t113"};
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "trunk t11",
            ">",
    };
    for (const auto& name : children_data)
        tree_text_repr.push_back("trunk " + name);

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(node->childCount(), 1);
    TreeNode* node2 = node->child(0);
    ASSERT_EQ(node2->childCount(), 3);
    for (int i = 0; i < node2->childCount(); ++i) {
        TreeNode* subnode = node2->child(i);
        QVariant data = subnode->data(DataRoles::DisplayRole);
        ASSERT_EQ(data.toString().toStdString(), children_data[i]);
        QVariant auxiliary_data = subnode->data(DataRoles::DisplayAuxiliaryRole);
        ASSERT_EQ(auxiliary_data.toString().toStdString(), "Trunk");
    }
}


TEST(TreeParserTest, AscendTest) {
    std::vector<std::string> children_data = {"t12", "t13", "t14"};
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "trunk t11",
            ">",
            "trunk t111",
            "<",
    };
    for (const auto& name : children_data)
        tree_text_repr.push_back("trunk " + name);

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(node->childCount(), 4);
    for (int i = 0; i < node->childCount(); ++i) {
        TreeNode* subnode = node->child(i);
        QVariant data = subnode->data(DataRoles::DisplayRole);
        if (i == 0)
            ASSERT_EQ(data.toString().toStdString(), "t11");
        else
            ASSERT_EQ(data.toString().toStdString(), children_data[i - 1]);
    }
}


TEST(TreeParserTest, BranchNodeTest) {
    std::vector<std::pair<std::string, int>> children_data = {{"t11", 5}, {"t12", 10}};
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">"
    };
    for (const auto& p : children_data)
        tree_text_repr.push_back("branch " + p.first + " " + std::to_string(p.second));

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(node->childCount(), 2);
    for (int i = 0; i < node->childCount(); ++i) {
        TreeNode* subnode = node->child(i);
        ASSERT_EQ(subnode->data(DataRoles::DisplayRole).toString().toStdString(), children_data[i].first);
        QVariant auxiliary_data = subnode->data(DataRoles::DisplayAuxiliaryRole);
        ASSERT_EQ(auxiliary_data.toString().toStdString(), "Branch");
    }
}

TEST(TreeParserTest, ParseInvalidStuffThrows1) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "trunk $%^*(&",
            "trunk t12"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}

TEST(TreeParserTest, ParseInvalidStuffThrows2) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">>>",
            "trunk t11",
            "trunk t12"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}

TEST(TreeParserTest, ParseInvalidStuffThrows3) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "trunk t11",
            "branch t12 DEADBEEF"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}

TEST(TreeParserTest, ParseInvalidStuffThrows4) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "trunk t11",
            "branch t12"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, HeteroTreesAreAllowed) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "trunk t11",
            "branch t12 5",
    };

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(node->childCount(), 2);
}


TEST(TreeParserTest, BranchRootIsAllowed) {
    std::vector<std::string> tree_text_repr = {
            "branch t1 1",
            ">",
            "branch t11 2",
            "branch t12 3",
    };

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(node->childCount(), 2);
}


TEST(TreeParserTest, GrowingTrunkFromBranchesThrows) {
    std::vector<std::string> tree_text_repr = {
            "branch t1 1",
            ">",
            "branch t1 2",
            "trunk t11"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, GoingDownOnNestThrows) {
    std::vector<std::string> tree_text_repr = {
            "nest n1 1",
            ">",
            "nest n2 1"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, GoingDownOnResourceThrows) {
    std::vector<std::string> tree_text_repr = {
            "resource leaf 1",
            ">",
            "resource acorn 1"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, PlacingNestOnTrunkThrows) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "nest n2 1"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, PlacingResourceOnTrunkThrows) {
    std::vector<std::string> tree_text_repr = {
            "trunk t1",
            ">",
            "resource acorn 1"
    };
    ASSERT_THROW(TreeParser().parse_tree(tree_text_repr), std::runtime_error);
}


TEST(TreeParserTest, ChainTest1) {
    const std::vector<std::string> tree_text_repr = {
            "trunk MainTrunk",
            ">",
            "trunk SouthwardSubtrunk",
            ">",
            "branch GrayleafBranch 12",
            ">",
            "resource leaf 1",
            "nest NestName 2",
            "resource acorn 3",
            "<",
            "branch GrayleafBranch 12",
            ">",
            "resource leaf 15",
    };
    std::unique_ptr<TreeNode> root_trunk_node(TreeParser().parse_tree(tree_text_repr));
    ASSERT_EQ(root_trunk_node->childCount(), 1);
    TreeNode* subtrunk_node = root_trunk_node->child(0);
    ASSERT_EQ(subtrunk_node->childCount(), 2);
    TreeNode* branch_node = subtrunk_node->child(0);
    ASSERT_EQ(branch_node->childCount(), 3);

    ASSERT_EQ(branch_node->child(0)->data(DataRoles::DisplayRole).toString().toStdString(), "Leaf");
    ASSERT_EQ(branch_node->child(1)->data(DataRoles::DisplayRole).toString().toStdString(), "NestName");
    ASSERT_EQ(branch_node->child(2)->data(DataRoles::DisplayRole).toString().toStdString(), "Acorn");
    ASSERT_EQ(branch_node->child(0)->data(DataRoles::ResourceAmountRole).toInt(), 1);
    ASSERT_EQ(branch_node->child(0)->data(DataRoles::ResourceTypeRole).toInt(),
            static_cast<int>(ResourceType::LEAF));
    ASSERT_EQ(branch_node->child(1)->data(DataRoles::LevelRole).toInt(), 2);
    ASSERT_EQ(branch_node->child(2)->data(DataRoles::ResourceAmountRole).toInt(), 3);
    ASSERT_EQ(branch_node->child(2)->data(DataRoles::ResourceTypeRole).toInt(),
              static_cast<int>(ResourceType::ACORN));
}