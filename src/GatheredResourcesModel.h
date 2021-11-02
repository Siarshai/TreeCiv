#ifndef TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H
#define TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H


#include <QtCore/QAbstractListModel>
// TODO: Extract ResourceType to separate file
#include "TreeParts/Interfaces/TreeNode.h"

class GatheredResourcesModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit GatheredResourcesModel(QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& index = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addResource(int resource_type, int amount);
private:
    std::map<ResourceType, int> resources_;
};


#endif //TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H
