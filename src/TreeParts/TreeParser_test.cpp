#include <utility>

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "TreeParser.h"
#include "src/TreeParts/Interfaces/TreeNode.h"

using namespace testing;


TEST(TreeParserTest, EmptyReturnNullptr) {
    ASSERT_EQ(TreeParser().finish(), nullptr);
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
    std::unique_ptr<TreeNode> root(TreeParser().parse_tree(tree_text_repr).finish());
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

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr).finish());
    ASSERT_EQ(node->childCount(), 3);
    for (int i = 0; i < node->childCount(); ++i) {
        TreeNode* subnode = node->child(i);
        QVariant data = subnode->data(0);
        ASSERT_EQ(data.toString().toStdString(), children_data[i]);
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

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr).finish());
    ASSERT_EQ(node->childCount(), 1);
    ASSERT_EQ(node->columnCount(), 1);
    TreeNode* node2 = node->child(0);
    ASSERT_EQ(node2->childCount(), 3);
    ASSERT_EQ(node2->columnCount(), 1);
    for (int i = 0; i < node2->childCount(); ++i) {
        TreeNode* subnode = node2->child(i);
        ASSERT_EQ(subnode->columnCount(), 1);
        QVariant data = subnode->data(0);
        ASSERT_EQ(data.toString().toStdString(), children_data[i]);
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

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr).finish());
    ASSERT_EQ(node->childCount(), 4);
    ASSERT_EQ(node->columnCount(), 1);
    for (int i = 0; i < node->childCount(); ++i) {
        TreeNode* subnode = node->child(i);
        ASSERT_EQ(subnode->columnCount(), 1);
        QVariant data = subnode->data(0);
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

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr).finish());
    ASSERT_EQ(node->childCount(), 2);
    ASSERT_EQ(node->columnCount(), 1);
    for (int i = 0; i < node->childCount(); ++i) {
        TreeNode* subnode = node->child(i);
        ASSERT_EQ(subnode->columnCount(), 1);
        ASSERT_EQ(subnode->data(0).toString().toStdString(), children_data[i].first);
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

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr).finish());
    ASSERT_EQ(node->childCount(), 2);
}


TEST(TreeParserTest, BranchRootIsAllowed) {
    std::vector<std::string> tree_text_repr = {
            "branch t1 1",
            ">",
            "branch t11 2",
            "branch t12 3",
    };

    std::unique_ptr<TreeNode> node(TreeParser().parse_tree(tree_text_repr).finish());
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