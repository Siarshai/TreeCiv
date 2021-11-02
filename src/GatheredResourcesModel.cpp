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
