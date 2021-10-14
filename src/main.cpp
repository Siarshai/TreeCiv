#include <iostream>

#include <QApplication>
#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <src/TreeParts/TreeParser.h>
#include "MapTreeModel.h"

const std::vector<std::string> tree_text_repr = {
        "trunk root",
        ">",
        "trunk MainTrunk",
        ">",
        "trunk SouthwardSubtrunk",
        ">",
        "branch GrayleafBranch 12",
        "branch CurvyBranch 5",
        "branch SpikybarkBranch 8",
        "<",
        "trunk NorthwardSubtrunk",
        ">",
        "branch RedbarkBranch 10",
        "branch NutsyBranch 7",
        "branch PointyLeafBranch 8",
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