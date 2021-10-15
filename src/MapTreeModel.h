#ifndef TREE_CIVILIZATION_MAPTREEMODEL_H
#define TREE_CIVILIZATION_MAPTREEMODEL_H


#include <QTreeView>
#include <QAbstractItemModel>


class TreeNode;


class MapTreeModel : public QAbstractItemModel {
Q_OBJECT
public:
    enum TreeRoles  {
        DisplayRole = Qt::DisplayRole,
        LevelRole = Qt::UserRole + 1
    };
    Q_ENUM(TreeRoles)

    explicit MapTreeModel(TreeNode* tree_root, QObject* parent = nullptr);
    ~MapTreeModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    [[nodiscard]] QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

private:
    [[nodiscard]] TreeNode *getItem(const QModelIndex &index) const;

    TreeNode *rootItem;
};


#endif //TREE_CIVILIZATION_MAPTREEMODEL_H
