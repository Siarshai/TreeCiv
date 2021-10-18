#ifndef TREE_CIVILIZATION_TREEPARSER_H
#define TREE_CIVILIZATION_TREEPARSER_H

#include <memory>
#include <vector>
#include <stack>
#include <regex>

class TreeNode;

class TreeParser {
public:
    ~TreeParser();
    TreeParser& parse_tree(const std::vector<std::string>& tree_text_repr);
    TreeParser& feed_string(const std::string& node_string);
    TreeNode* finish();

private:
    // Could have used here Strategy pattern, but program doesn't seem to benefit from
    // decoupling here: no dynamically registering Strategies (yet).
    [[nodiscard]] TreeNode* maybe_create_trunk_node(TreeNode* parent, const std::string& node_string) const;
    [[nodiscard]] TreeNode* maybe_create_branch_node(TreeNode* parent, const std::string& node_string) const;
    [[nodiscard]] TreeNode* maybe_create_resource_node(TreeNode* parent, const std::string& node_string) const;
    [[nodiscard]] TreeNode* maybe_create_nest_node(TreeNode* parent, const std::string& node_string) const;
    std::string format_exception(const char* message, const std::string& node_string) const;

    std::deque<TreeNode*> nodes_;
    bool expect_descent_ = false;
    size_t line_no = 0;
};


#endif //TREE_CIVILIZATION_TREEPARSER_H
