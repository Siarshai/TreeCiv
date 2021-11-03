#ifndef TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H
#define TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H


#include <QtCore/QAbstractListModel>
#include "TreeParts/Interfaces/ResourceType.h"
#include "Interfaces/IResourceSource.h"


class GatheredResourcesModel : public QAbstractListModel, public IResourceSource {
    Q_OBJECT
public:
    explicit GatheredResourcesModel(const std::map<ResourceType, unsigned int>& initial_resources,
            QObject* parent = nullptr);

    [[nodiscard]] int rowCount(const QModelIndex& index = QModelIndex()) const override;
    [[nodiscard]] QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    ConsumeResult try_consume_resources(ConsumeOperation op) override;

    Q_INVOKABLE void addResource(int resource_type, unsigned int amount);
private:
    std::vector<ResourceType> resources_;
    using VRTrit = std::vector<ResourceType>::reverse_iterator;

    void delete_resources_and_rows(const std::vector<VRTrit>& iterators_sorted_backwards);
};


#endif //TREE_CIVILIZATION_GATHEREDRESOURCESMODEL_H
