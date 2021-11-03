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
    TreeNode* parse_tree(const std::vector<std::string>& tree_text_repr);
    void feed_string(const std::string& node_string);

private:
    [[nodiscard]] bool skip_empty_line(const std::string& node_string);
    [[nodiscard]] bool maybe_process_up_and_down_on_semiempty_tree(const std::string& node_string);
    [[nodiscard]] bool maybe_process_node_creation(const std::string& node_string);
    [[nodiscard]] bool maybe_process_up_and_down_general(const std::string& node_string);

    // Could have used here Strategy pattern, but program doesn't seem to benefit from
    // decoupling here: no dynamically registering Strategies (yet).
    [[nodiscard]] std::optional<TreeNode*> maybe_create_trunk_node(TreeNode* parent, const std::string& node_string) const;
    [[nodiscard]] std::optional<TreeNode*> maybe_create_branch_node(TreeNode* parent, const std::string& node_string) const;
    [[nodiscard]] std::optional<TreeNode*> maybe_create_resource_node(TreeNode* parent, const std::string& node_string) const;
    [[nodiscard]] std::optional<TreeNode*> maybe_create_nest_node(TreeNode* parent, const std::string& node_string) const;
    std::string format_exception(const char* message, const std::string& node_string) const;

    std::deque<TreeNode*> nodes_;
    bool expect_descent_ = false;
    size_t line_no = 0;
};


#endif //TREE_CIVILIZATION_TREEPARSER_H
