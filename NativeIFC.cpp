#include <QApplication>

#include "src/Gui/View3DInventorViewer.h"
#include "src/Gui/Document.h"
#include "src/Gui/Application.h"

int main(int argc, char *argv[])
{
    QApplication qapp(argc, argv);
    auto app = &(App::GetApplication());
    Gui::Application gapp(false); // gui not enabled
    std::unique_ptr<Gui::Document> newDoc = std::make_unique<Gui::Document>(app->newDocument("testdoc","janus"), &gapp);
    Gui::View3DInventorViewer v(nullptr, nullptr);
    v.setDocument(newDoc.get());
    v.show();
    return qapp.exec();
}
