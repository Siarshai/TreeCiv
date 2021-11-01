#include <iostream>

#include <QApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>

#include "MapTreeModel.h"
#include "TreeParts/TreeParser.h"
#include "TreeParts/TreeGrowthStrategy.h"


const std::vector<std::string> tree_text_repr = {
        "trunk root",
        ">",
        "trunk MainOldTrusty",
        ">",
        "trunk Southward",
        ">",
        "branch Grayleaf 8",
        ">",
        "nest Capital 1",
        "resource leaf 1",
        "resource leaf 2",
        "<",
        "branch Curvy 5",
        "branch Spikybark 8",
        "<",
        "trunk Northward",
        ">",
        "branch Redbark 10",
        "branch Nutsy 60",
        ">",
        "resource acorn 1",
        "<",
        "branch PointyLeaf 80",
        ">",
        "resource leaf 2",
        "resource leaf 2",
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Stub");
    QCoreApplication::setOrganizationDomain("stub.com");
    QCoreApplication::setApplicationName("TreeCivilization");

    QQmlApplicationEngine engine;
    MapTreeModel map_model(
            TreeParser().parse_tree(tree_text_repr).finish(),
            std::make_unique<RBRGStrategy>(1),
            std::make_unique<RGStrategy>(1));
    engine.rootContext()->setContextProperty("map_model", &map_model);
    engine.load(QUrl("qrc:/resources/main.qml"));

    return app.exec();
}