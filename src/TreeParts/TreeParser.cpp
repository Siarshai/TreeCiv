#include "TreeParser.h"

#include <iostream>

#include "TrunkNode.h"
#include "BranchNode.h"


TreeParser::~TreeParser() {
    if (!nodes_.empty())
        delete nodes_.front();
}

TreeParser& TreeParser::parse_tree(const std::vector<std::string>& tree_text_repr) {
    for (const auto& s : tree_text_repr)
        feed_string(s);
    return *this;
}

TreeParser& TreeParser::feed_string(const std::string& node_string) {
    if (node_string.empty())
        return *this;  // skip empty lines
    if (expect_descent_) {
        expect_descent_ = false;
        if (node_string == ">")
            return *this;  // do not actually do anything, node was already pushed to deque
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
    if (match_and_add_trunk_node(node_string))
        return *this;
    if (match_and_add_branch_node(node_string))
        return *this;

    if (node_string == ">") {
        if (!nodes_.back()->childCount())
            throw std::runtime_error(format_exception(
                    "Current node has no children to go down to", node_string));
        nodes_.push_back(nodes_.back()->child(nodes_.back()->childCount() - 1));
        return *this;
    }
    if (node_string == "<") {
        nodes_.pop_back();
        return *this;
    }
    throw std::runtime_error(format_exception("Can not parse", node_string));
}

TreeNode* TreeParser::finish() {
    if (nodes_.empty())
        return nullptr;
    TreeNode* tmp = nodes_.front();
    nodes_.clear();
    return tmp;
}

bool TreeParser::match_and_add_trunk_node(const std::string& node_string) {
    static const std::regex trunk_regex_("trunk ([a-zA-Z0-9]+)");
    std::smatch matches;
    if (std::regex_match(node_string, matches, trunk_regex_)) {
        std::string name = matches[1].str();
        auto* child = new TrunkNode(nodes_.empty() ? nullptr : nodes_.back(),
                                    QString::fromStdString(name));
        if (!nodes_.empty()) {
            // child not leaking - deleted in parent's dtor
            bool last_node_is_branch = dynamic_cast<BranchNode*>(nodes_.back());
            if (last_node_is_branch)
                throw std::runtime_error(format_exception("Can not grow trunk on a branch", node_string));
            nodes_.back()->addChild(child);
        } else {
            nodes_.push_back(child);
            expect_descent_ = true;
        }
        return true;
    }
    return false;
}

bool TreeParser::match_and_add_branch_node(const std::string& node_string) {
    static const std::regex branch_regex_("branch ([a-zA-Z0-9]+) ([0-9]+)");
    std::smatch matches;
    if (std::regex_match(node_string, matches, branch_regex_)) {
        std::string name = matches[1].str();
        int capacity = atoi(matches[2].str().c_str());
        auto* child = new BranchNode(nodes_.empty() ? nullptr : nodes_.back(),
                                     QString::fromStdString(name), capacity);
        if (!nodes_.empty()) {
            nodes_.back()->addChild(child);
        } else {
            nodes_.push_back(child);
            expect_descent_ = true;
        }
        return true;
    }
    return false;
}

std::string TreeParser::format_exception(
        const char* message, const std::string& node_string) const {
    std::stringstream ss;
    ss << message << ": line " << line_no << " - " << node_string;
    return ss.str();
}
