#ifndef TREE_CIVILIZATION_MAPTREEMODEL_H
#define TREE_CIVILIZATION_MAPTREEMODEL_H


#include <QTreeView>
#include <QAbstractItemModel>
#include <src/TreeParts/Interfaces/ITreeGrowthStrategy.h>
#include "Interfaces/IResourceSource.h"


class TreeNode;


class MapTreeModel : public QAbstractItemModel {
Q_OBJECT
public:
    MapTreeModel(
            TreeNode* tree_root,
            std::unique_ptr<IAmountModifyingGrowthStrategy> amount_modifying_strategy,
            std::unique_ptr<ITreeModifyingGrowthStrategy> tree_modifying_strategy,
            std::reference_wrapper<IResourceSource> resource_source,
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

    [[nodiscard]] QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void delete_node_by_uid(const QString& uid);
    Q_INVOKABLE void upgrade_nest_node_request(const QString& uid);

public slots:
    void update_on_growth_timer();

signals:
    void update_growth_progress_bar(float progress_amount);
    void show_insufficient_funds_message();

private:
    [[nodiscard]] TreeNode *getItem(const QModelIndex &index) const;

    TreeNode *rootItem;
    const std::unique_ptr<IAmountModifyingGrowthStrategy> amount_modifying_strategy_;
    const std::unique_ptr<ITreeModifyingGrowthStrategy> tree_modifying_strategy_;
    std::reference_wrapper<IResourceSource> resource_source_;
    QTimer* growth_timer_;
    const int ticks_divider_;
    int current_ticks_;
};


#endif //TREE_CIVILIZATION_MAPTREEMODEL_H
