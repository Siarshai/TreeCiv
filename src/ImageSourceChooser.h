#ifndef TREE_CIVILIZATION_IMAGESOURCECHOOSER_H
#define TREE_CIVILIZATION_IMAGESOURCECHOOSER_H

#include <QtCore/QObject>
// TODO: Move ResourceType to separate folder
#include "TreeParts/Interfaces/TreeNode.h"


class TreeDelegateImageChooser : public QObject {
    Q_OBJECT
public:
    static constexpr std::array resource_image_path{"images/leaf.png", "images/acorn.png"};
    static_assert(resource_image_path.size() == static_cast<int>(ResourceType::COUNT));

    Q_INVOKABLE static QString getImageForResourceType(int resource_type) {
        if (resource_type < 0 || resource_type >= static_cast<int>(ResourceType::COUNT))
            throw std::logic_error(std::to_string(resource_type) + " is outside of bounds of ResourceType");
        return resource_image_path.at(resource_type);
    }
};

#endif //TREE_CIVILIZATION_IMAGESOURCECHOOSER_H
