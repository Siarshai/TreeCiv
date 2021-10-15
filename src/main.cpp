#include <iostream>

#include <QApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <src/TreeParts/TreeParser.h>
#include "MapTreeModel.h"

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
        "resource leaf 3",
        "resource leaf 3",
        "<",
        "branch Curvy 5",
        "branch Spikybark 8",
        "<",
        "trunk Northward",
        ">",
        "branch Redbark 10",
        "branch Nutsy 6",
        ">",
        "resource acorn 4",
        "<",
        "branch PointyLeaf 8",
        ">",
        "resource leaf 3",
        "resource leaf 3",
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Stub");
    QCoreApplication::setOrganizationDomain("stub.com");
    QCoreApplication::setApplicationName("TreeCivilization");

    QQmlApplicationEngine engine;
    MapTreeModel map_model(TreeParser().parse_tree(tree_text_repr).finish());
    engine.rootContext()->setContextProperty("map_model", &map_model);
    engine.load(QUrl("qrc:/resources/main.qml"));
    return app.exec();
}