#include "TreeParser.h"

#include <iostream>

#include "src/TreeParts/ConcreteNodes/TrunkNode.h"
#include "src/TreeParts/ConcreteNodes/BranchNode.h"
#include "src/TreeParts/ConcreteNodes/ResourceNode.h"
#include "src/TreeParts/ConcreteNodes/NestNode.h"


TreeParser::~TreeParser() {
    if (!nodes_.empty())
        delete nodes_.front();
}

TreeNode* TreeParser::parse_tree(const std::vector<std::string>& tree_text_repr) {
    for (const auto& s : tree_text_repr)
        feed_string(s);

    if (nodes_.empty())
        return nullptr;
    TreeNode* tmp = nodes_.front();
    nodes_.clear();
    return tmp;
}

void TreeParser::feed_string(const std::string& node_string) {
    ++line_no;
    if (node_string.empty())
        return;  // skip empty lines
    if (expect_descent_) {
        expect_descent_ = false;
        if (node_string == ">")
            return;  // do not actually do anything, node was already pushed to deque
        throw std::runtime_error(format_exception("Expected \">\" after first node insert", node_string));
    }
    if (nodes_.empty()) {
        if (node_string == ">")
            throw std::runtime_error(format_exception(
                    "Can not go deeper on empty tree, you need to create node first", node_string));
        if (node_string == "<")
            throw std::runtime_error(format_exception(
                    "Can not go up on empty tree, you need to create node first", node_string));
    } else if (nodes_.size() == 1) {
        if (node_string == "<")
            throw std::runtime_error(format_exception(
                    "Can not go up on root node; did you mean finish()?", node_string));
    }
    TreeNode* parent = nodes_.empty() ? nullptr : nodes_.back();

    static const auto node_creator_fn = {
            &TreeParser::maybe_create_trunk_node,
            &TreeParser::maybe_create_branch_node,
            &TreeParser::maybe_create_resource_node,
            &TreeParser::maybe_create_nest_node
    };
    for (const auto& fn : node_creator_fn) {
        if (auto child = std::invoke(fn, this, parent, node_string)) {
            if (parent) {
                parent->addChild(*child);
            } else {
                nodes_.push_back(*child);
                expect_descent_ = true;
            }
            return;
        }
    }

    if (node_string == ">") {
        if (!nodes_.back()->childCount())
            throw std::runtime_error(format_exception(
                    "Current node has no children to go down to", node_string));
        nodes_.push_back(nodes_.back()->child(nodes_.back()->childCount() - 1));
        return;
    }
    if (node_string == "<") {
        nodes_.pop_back();
        return;
    }
    throw std::runtime_error(format_exception("Can not parse", node_string));
}

std::optional<TreeNode*> TreeParser::maybe_create_trunk_node(TreeNode* parent, const std::string& node_string) const {
    static const std::regex trunk_regex_("trunk ([a-zA-Z0-9]+)");
    std::smatch matches;
    if (std::regex_match(node_string, matches, trunk_regex_)) {
        if (dynamic_cast<const BranchNode*>(parent))
            throw std::runtime_error(format_exception("Can not grow trunk on a branch", node_string));
        std::string name = matches[1].str();
        return new TrunkNode(parent, QString::fromStdString(name));
    }
    return std::nullopt;
}

std::optional<TreeNode*> TreeParser::maybe_create_branch_node(TreeNode* parent, const std::string& node_string) const {
    static const std::regex branch_regex_("branch ([a-zA-Z0-9]+) ([0-9]+)");
    std::smatch matches;
    if (std::regex_match(node_string, matches, branch_regex_)) {
        std::string name = matches[1].str();
        int capacity = atoi(matches[2].str().c_str());
        return new BranchNode(parent, QString::fromStdString(name), capacity);
    }
    return std::nullopt;
}

std::optional<TreeNode*> TreeParser::maybe_create_resource_node(TreeNode* parent, const std::string& node_string) const {
    static const std::regex branch_regex_("resource (leaf|acorn) ([0-9]+)");
    std::smatch matches;
    if (std::regex_match(node_string, matches, branch_regex_)) {
        if (parent != nullptr && !dynamic_cast<const BranchNode*>(parent))
            throw std::runtime_error(format_exception("Can only grow resource on a branch", node_string));
        std::string resource_strrepr = matches[1].str();
        auto rt = get_typerepr(resource_strrepr.c_str());
        unsigned int amount = atoi(matches[2].str().c_str());
        return new ResourceNode(parent, rt, amount);
    }
    return std::nullopt;
}

std::optional<TreeNode*> TreeParser::maybe_create_nest_node(TreeNode* parent, const std::string& node_string) const {
    static const std::regex branch_regex_("nest ([a-zA-Z0-9]+) ([0-9]+)");
    std::smatch matches;
    if (std::regex_match(node_string, matches, branch_regex_)) {
        if (parent != nullptr && !dynamic_cast<const BranchNode*>(parent))
            throw std::runtime_error(format_exception("Can only place a nest on a branch", node_string));
        std::string name = matches[1].str();
        int level = atoi(matches[2].str().c_str());
        return new NestNode(parent, QString::fromStdString(name), level);
    }
    return std::nullopt;
}

std::string TreeParser::format_exception(const char* message, const std::string& node_string) const {
    std::stringstream ss;
    ss << message << ": line " << line_no << " - " << node_string;
    return ss.str();
}
