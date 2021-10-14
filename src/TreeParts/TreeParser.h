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
    [[nodiscard]] bool match_and_add_trunk_node(const std::string& node_string);
    [[nodiscard]] bool match_and_add_branch_node(const std::string& node_string);
    std::string format_exception(const char* message, const std::string& node_string) const;

    std::deque<TreeNode*> nodes_;
    bool expect_descent_ = false;
    size_t line_no = 0;
};


#endif //TREE_CIVILIZATION_TREEPARSER_H
