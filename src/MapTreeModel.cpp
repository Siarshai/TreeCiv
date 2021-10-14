#include "MapTreeModel.h"
#include "src/TreeParts/ConcreteNodes/TrunkNode.h"
#include "src/TreeParts/ConcreteNodes/BranchNode.h"


MapTreeModel::MapTreeModel(TreeNode* tree_root, QObject* parent)
    : QAbstractItemModel(parent) {
    rootItem = tree_root;
}

MapTreeModel::~MapTreeModel() {
    delete rootItem;
}

QVariant MapTreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeNode *item = getItem(index);
    return item->data(index.column());
}

QVariant MapTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);
    return QVariant();
}

QModelIndex MapTreeModel::index(int row, int column, const QModelIndex& parent) const {
    if (parent.isValid() && parent.column() != 0)
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

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int MapTreeModel::rowCount(const QModelIndex& parent) const {
    TreeNode *parentItem = getItem(parent);
    return parentItem->childCount();
}

int MapTreeModel::columnCount(const QModelIndex& parent) const {
    return rootItem->columnCount();
}

Qt::ItemFlags MapTreeModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
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

TreeNode* MapTreeModel::getItem(const QModelIndex& index) const {
    if (index.isValid()) {
        if (auto* item = static_cast<TreeNode*>(index.internalPointer()))
            return item;
    }
    return rootItem;
}

QHash<int, QByteArray> MapTreeModel::roleNames() const {
    return QAbstractItemModel::roleNames();
}
