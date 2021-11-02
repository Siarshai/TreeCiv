#ifndef TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H
#define TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H


#include <QtCore/QAbstractListModel>
// TODO: Extract ResourceType to separate file
#include "TreeParts/Interfaces/TreeNode.h"
#include "Interfaces/IResourceSource.h"


class GatheredResourcesModel : public QAbstractListModel, public IResourceSource {
    Q_OBJECT
public:
    explicit GatheredResourcesModel(QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& index = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    ConsumeResult try_consume_resources(ConsumeOperation op) override;

    Q_INVOKABLE void addResource(int resource_type, int amount);
private:
    void deleteResourcesAndRows(const std::vector<ResourceType>& delete_keys);
    [[nodiscard]] int get_row(const ResourceType& real_resource_type) const;

    std::map<ResourceType, int> resources_;
};


#endif //TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H
