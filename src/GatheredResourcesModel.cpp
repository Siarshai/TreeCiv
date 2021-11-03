#include "GatheredResourcesModel.h"

GatheredResourcesModel::GatheredResourcesModel(
        const std::map<ResourceType, unsigned int>& initial_resources,
        QObject* parent)
        : QAbstractListModel(parent) {
    for (const auto& [rt, amount] : initial_resources)
        addResource(static_cast<int>(rt), amount);
}

int GatheredResourcesModel::rowCount(const QModelIndex& index) const {
    return resources_.size();
}

QVariant GatheredResourcesModel::data(const QModelIndex& index, int role) const {
    if (static_cast<size_t>(index.row()) < resources_.size()) {
        auto it = resources_.cbegin() + index.row();
        return QString::fromStdString(get_strrepr(*it));
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
            std::vector<VRTrit> delete_its;
            for (auto [l, a, it] = std::tuple(0, 0, resources_.rbegin()); it != resources_.rend(); ++it) {
                if (l < leaf_cost && *it == ResourceType::LEAF) {
                    ++l;
                    delete_its.push_back(it);
                }
                if (a < acorn_cost && *it == ResourceType::ACORN) {
                    ++a;
                    delete_its.push_back(it);
                }
                if (l == leaf_cost && a == acorn_cost)
                    break;
            }
            if (delete_its.size() == leaf_cost + acorn_cost) {
                delete_resources_and_rows(delete_its);
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

void GatheredResourcesModel::addResource(int resource_type, unsigned int amount) {
    if (amount > std::numeric_limits<int>::max())
        throw std::logic_error("Negative value passed to GatheredResourcesModel::addResource");
    if (amount == 0)
        return;  // suspicious though
    unsigned int row = resources_.size();
    beginInsertRows(QModelIndex(), row, row + static_cast<unsigned int>(amount - 1));
    for (unsigned int i = 0; i < amount; ++i)
        resources_.push_back(static_cast<ResourceType>(resource_type));
    endInsertRows();
}

void GatheredResourcesModel::delete_resources_and_rows(const std::vector<VRTrit>& iterators_sorted_backwards) {
    for (const auto& rit: iterators_sorted_backwards) {
        auto fit = rit.base() - 1;
        int row = std::distance(resources_.begin(), fit);
        emit beginRemoveRows(QModelIndex(), row, row);
        resources_.erase(fit);
        emit endRemoveRows();
    }
}
