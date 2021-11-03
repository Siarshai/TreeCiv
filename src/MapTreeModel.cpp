#include <sstream>
#include <src/TreeParts/ConcreteNodes/NestNode.h>
#include "MapTreeModel.h"
#include "src/TreeParts/Interfaces/TreeNode.h"


MapTreeModel::MapTreeModel(
        TreeNode* tree_root,
        std::unique_ptr<IAmountModifyingGrowthStrategy> amount_modifying_strategy,
        std::unique_ptr<ITreeModifyingGrowthStrategy> tree_modifying_strategy,
        std::reference_wrapper<IResourceSource> resource_source,
        QObject* parent)
        : QAbstractItemModel(parent)
        , amount_modifying_strategy_(std::move(amount_modifying_strategy))
        , tree_modifying_strategy_(std::move(tree_modifying_strategy))
        , resource_source_(resource_source)
        , growth_timer_(new QTimer(this))
        , ticks_divider_(20)
        , current_ticks_(0) {
    rootItem = tree_root;
    connect(growth_timer_, &QTimer::timeout, this, &MapTreeModel::update_on_growth_timer);
    growth_timer_->start(1000/ticks_divider_);
}

MapTreeModel::~MapTreeModel() {
    delete rootItem;
}

QVariant MapTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    TreeNode *item = getItem(index);
    assert(item);
    return item->data(static_cast<DataRoles>(role));
}

QVariant MapTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(DataRoles::DisplayRole);
    return QVariant();
}

QModelIndex MapTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    // TODO: Probably unneeded
    if (parent.isValid() && parent.column() != 0)  // TODO: && parent.column() != 0 wtf?
        return QModelIndex();
    TreeNode *parentItem = getItem(parent);
    TreeNode *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex MapTreeModel::parent(const QModelIndex& index) const {
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childItem = getItem(index);
    TreeNode *parentItem = childItem->parent();

    if (parentItem == nullptr || parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int MapTreeModel::rowCount(const QModelIndex& parent) const {
    TreeNode *parentItem = getItem(parent);
    return parentItem->childCount();
}

int MapTreeModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

Qt::DropActions MapTreeModel::supportedDragActions() const {
    return Qt::MoveAction;
}

Qt::ItemFlags MapTreeModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index) | Qt::ItemIsDragEnabled;
}

bool MapTreeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    return false; // return QAbstractItemModel::setData(index, value, role);
}

bool MapTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant& value, int role) {
    return false; // return QAbstractItemModel::setHeaderData(section, orientation, value, role);
}

bool MapTreeModel::insertColumns(int position, int columns, const QModelIndex& parent) {
    return false; // return QAbstractItemModel::insertColumns(position, columns, parent);
}

bool MapTreeModel::removeColumns(int position, int columns, const QModelIndex& parent) {
    return false; // return QAbstractItemModel::removeColumns(position, columns, parent);
}

bool MapTreeModel::insertRows(int position, int rows, const QModelIndex& parent) {
    return false; // return QAbstractItemModel::insertRows(position, rows, parent);
}

bool MapTreeModel::removeRows(int position, int rows, const QModelIndex& parent) {
    return false; // QAbstractItemModel::removeRows(position, rows, parent);
}

void MapTreeModel::update_on_growth_timer() {
    current_ticks_ = (current_ticks_ + 1) % ticks_divider_;
    float progress = static_cast<float>(current_ticks_)/static_cast<float>(ticks_divider_);
    emit update_growth_progress_bar(progress);
    if (current_ticks_ == 0) {
        std::set<TreeNode*> nodes_changed = amount_modifying_strategy_->grow_resources(rootItem);
        for (auto node : nodes_changed) {
            emit dataChanged(
                    createIndex(node->childNumber(), 0, node),
                    createIndex(node->childNumber(), 0, node),
                    {static_cast<int>(DataRoles::ResourceAmountRole)}
            );
        }
        tree_modifying_strategy_->grow_resources(rootItem,
                [this](TreeNode* parent) {
                    auto index = createIndex(parent->childNumber(), 0, parent);
                    emit beginInsertRows(index, parent->childCount(), parent->childCount());
                },
                [this]() { emit endInsertRows(); } );
    }
}

void MapTreeModel::delete_node_by_uid(const QString& uid) {
    TreeNode* node = rootItem->recursive_search_for_node(uid.toStdString());
    if (!node)
        throw std::logic_error("MapTreeModel::delete_node_by_uid node uid not found");
    TreeNode* parent = node->parent();
    int row = node->childNumber();
    auto index = createIndex(parent->childNumber(), 0, parent);
    emit beginRemoveRows(index, row, row);
    parent->removeChild(row);
    emit endRemoveRows();
}

void MapTreeModel::upgrade_nest_node_request(const QString& uid) {
    auto result = resource_source_.get().try_consume_resources(ConsumeOperation::NEST_IMPROVEMENT);
    if (result == ConsumeResult::OK) {
        TreeNode* node = rootItem->recursive_search_for_node(uid.toStdString());
        if (!node)
            throw std::logic_error("MapTreeModel::upgrade_nest_node_request node uid not found");
        NestNode* nest_node = dynamic_cast<NestNode*>(node);
        if (!nest_node)
            throw std::logic_error("MapTreeModel::upgrade_nest_node_request non-nest node found by given uid");
        nest_node->upgrade_nest();
        emit dataChanged(
                createIndex(node->childNumber(), 0, node),
                createIndex(node->childNumber(), 0, node),
                {static_cast<int>(DataRoles::LevelRole)}
        );
    } else {
        emit show_insufficient_funds_message();
    }
}

TreeNode* MapTreeModel::getItem(const QModelIndex& index) const {
    if (index.isValid()) {
        if (auto* item = static_cast<TreeNode*>(index.internalPointer()))
            return item;
    }
    return rootItem;
}

QHash<int, QByteArray> MapTreeModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[static_cast<int>(DataRoles::DisplayRole)] = "display";
    roles[static_cast<int>(DataRoles::DisplayAuxiliaryRole)] = "display_auxiliary";
    roles[static_cast<int>(DataRoles::DelegateTypeRole)] = "delegate_type";
    roles[static_cast<int>(DataRoles::LevelRole)] = "level";
    roles[static_cast<int>(DataRoles::ResourceTypeRole)] = "resource_type";
    roles[static_cast<int>(DataRoles::ResourceAmountRole)] = "resource_amount";
    roles[static_cast<int>(DataRoles::NodeIdRole)] = "node_id";
    return roles;
}
