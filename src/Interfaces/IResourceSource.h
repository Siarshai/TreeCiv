#ifndef TREE_CIVILIZATION_IRESOURCESOURCE_H
#define TREE_CIVILIZATION_IRESOURCESOURCE_H


enum class ConsumeResult {
    OK,
    INSUFFICIENT_FUNDS
};

enum class ConsumeOperation {
    NEST_IMPROVEMENT
};


class IResourceSource {
public:
    virtual ConsumeResult try_consume_resources(ConsumeOperation op) = 0;
    virtual ~IResourceSource() = default;
};

#endif //TREE_CIVILIZATION_IRESOURCESOURCE_H
