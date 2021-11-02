#include "GatheredResourcesModel.h"

GatheredResourcesModel::GatheredResourcesModel(QObject* parent)
        : QAbstractListModel(parent) {
    resources_[ResourceType::ACORN] = 2;
    resources_[ResourceType::LEAF] = 1;
}

int GatheredResourcesModel::rowCount(const QModelIndex& index) const {
    return resources_.size();
}

QVariant GatheredResourcesModel::data(const QModelIndex& index, int role) const {
    const size_t row = index.row();
    if (row < resources_.size()) {
        auto it = resources_.cbegin();
        std::advance(it, row);
        const auto& [resource, amount] = *it;
        std::string resource_strrepr = get_strrepr(resource);
        return QString::fromStdString(resource_strrepr + " " + std::to_string(amount));
    }
    return QVariant();
}

QHash<int, QByteArray> GatheredResourcesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(Qt::DisplayRole)] = "display";
    return roles;
}

void GatheredResourcesModel::addResource(int resource_type, int amount) {
    auto real_resource_type = static_cast<ResourceType>(resource_type);
    const int& row = resource_type;
    if (auto it = resources_.find(real_resource_type); it == resources_.end()) {
        beginInsertRows(QModelIndex(), row, row);
        resources_[real_resource_type] = amount;
        endInsertRows();
    } else {
        it->second += amount;
        dataChanged(index(row, 0), index(row, 0), { Qt::DisplayRole });
    }
}
