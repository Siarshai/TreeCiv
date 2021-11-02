#include "GatheredResourcesModel.h"

GatheredResourcesModel::GatheredResourcesModel(QObject* parent)
        : QAbstractListModel(parent) {
    resources_[ResourceType::LEAF] = 3;
    resources_[ResourceType::ACORN] = 4;
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

ConsumeResult GatheredResourcesModel::try_consume_resources(ConsumeOperation op) {
    // Strategy pattern would come in handy here, but we have only one operation so whatever
    switch (op) {
        case ConsumeOperation::NEST_IMPROVEMENT: {
            static constexpr int leaf_cost = 3;
            static constexpr int acorn_cost = 3;
            auto leaf_it = resources_.find(ResourceType::LEAF);
            auto acorn_it = resources_.find(ResourceType::ACORN);
            if (leaf_it != resources_.end() && leaf_it->second >= leaf_cost
                    && acorn_it != resources_.end() && acorn_it->second >= acorn_cost) {
                std::vector<ResourceType> delete_keys;
                std::vector<ResourceType> update_keys;
                auto consume_resource = [&delete_keys, &update_keys] (auto it, int cost) {
                    if (it->second > cost) {
                        it->second -= cost;
                        update_keys.push_back(it->first);
                    } else {
                        delete_keys.push_back(it->first);
                    }
                };
                consume_resource(leaf_it, leaf_cost);
                consume_resource(acorn_it, acorn_cost);

                deleteResourcesAndRows(delete_keys);
                for (const auto& rt : update_keys) {
                    const int row = get_row(rt);
                    dataChanged(index(row, 0), index(row, 0), { Qt::DisplayRole });
                }

                return ConsumeResult::OK;
            } else {
                return ConsumeResult::INSUFFICIENT_FUNDS;
            }
        }
        default:
            throw std::logic_error("GatheredResourcesModel::try_consume_resources: Unknown operation: "
                                   + std::to_string(static_cast<int>(op)));
    }
}

void GatheredResourcesModel::addResource(int resource_type, int amount) {
    auto real_resource_type = static_cast<ResourceType>(resource_type);
    int row = get_row(real_resource_type);
    if (auto it = resources_.find(real_resource_type); it == resources_.end()) {
        beginInsertRows(QModelIndex(), row, row);
        resources_[real_resource_type] = amount;
        endInsertRows();
    } else {
        it->second += amount;
        dataChanged(index(row, 0), index(row, 0), { Qt::DisplayRole });
    }
}

int GatheredResourcesModel::get_row(const ResourceType& real_resource_type) const {
    auto rt_it = resources_.find(real_resource_type);
    return std::distance(resources_.begin(), rt_it);
}

void GatheredResourcesModel::deleteResourcesAndRows(const std::vector<ResourceType>& delete_keys) {
    for (const auto& rt : delete_keys) {
        auto it = resources_.find(rt);
        const int row = std::distance(resources_.begin(), it);
        emit beginRemoveRows(QModelIndex(), row, row);
        resources_.erase(it);
        emit endRemoveRows();
    }
}