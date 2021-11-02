#ifndef TREE_CIVILIZATION_MAPTREEMODEL_H
#define TREE_CIVILIZATION_MAPTREEMODEL_H


#include <QTreeView>
#include <QAbstractItemModel>
#include <src/TreeParts/Interfaces/ITreeGrowthStrategy.h>


class TreeNode;


class MapTreeModel : public QAbstractItemModel {
Q_OBJECT
public:
    MapTreeModel(
            TreeNode* tree_root,
            std::unique_ptr<IAmountModifyingGrowthStrategy> amount_modifying_strategy,
            std::unique_ptr<ITreeModifyingGrowthStrategy> tree_modifying_strategy,
            QObject* parent = nullptr);
    ~MapTreeModel() override;

    [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;
    [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    [[nodiscard]] QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] QModelIndex parent(const QModelIndex &index) const override;

    [[nodiscard]] int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    [[nodiscard]] int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]] Qt::DropActions supportedDragActions() const override;
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

public slots:
    void update_on_growth_timer();
    void transfer_node(const QVariant& uid);

signals:
    void update_growth_progress_bar(float progress_amount);

private:
    [[nodiscard]] TreeNode *getItem(const QModelIndex &index) const;

    TreeNode *rootItem;
    const std::unique_ptr<IAmountModifyingGrowthStrategy> amount_modifying_strategy_;
    const std::unique_ptr<ITreeModifyingGrowthStrategy> tree_modifying_strategy_;
    QTimer* growth_timer_;
    const int ticks_divider_;
    int current_ticks_;
};


#endif //TREE_CIVILIZATION_MAPTREEMODEL_H
