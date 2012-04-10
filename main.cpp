#include <QtGui/QApplication>
#include <QGraphicsObject>
#include <QDeclarativeView>
#include <QUrl>
#include <QIcon>
#include <QTextCodec>
#include "qmlapplicationviewer.h"
#include "stenographer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    #ifdef Q_OS_LINUX
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    #else //Q_OS_MSDOS
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("IBM866"));
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
    #endif
    QTextCodec::setCodecForTr(QTextCodec::codecForCStrings());

    QScopedPointer<QApplication> app(createApplication(argc, argv));

    Stenographer st;

//    QmlApplicationViewer viewer;
//    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
//    viewer.setMainQmlFile(QLatin1String("qml/stenographer/main.qml"));
//    viewer.setMainQmlFile(QUrl("qrc:/main.qml"));
//    viewer.showExpanded();

//    QDeclarativeView viewer;
//    viewer.setSource(QUrl("qrc:/main.qml"));
//    viewer.setSource(QUrl("qml/stenographer/main.qml"));
//    viewer.setResizeMode(QDeclarativeView::SizeRootObjectToView);

    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setSource(QUrl("qrc:/main.qml"));
    viewer->setWindowIcon(QIcon(":/icon80.png"));
    viewer->showExpanded();

    QObject *rootObject = dynamic_cast<QObject*>(viewer->rootObject());

    QObject::connect(rootObject, SIGNAL(beginStopRec()), &st, SLOT(beginStopRec()));
    QObject::connect(rootObject, SIGNAL(playLast()), &st, SLOT(playLast()));
    QObject::connect(rootObject, SIGNAL(convertAndSent()), &st, SLOT(convertAndSent()));

    QObject::connect(&st, SIGNAL(updateData(QVariant)), rootObject, SLOT(updateData(QVariant)));
    QObject::connect(&st, SIGNAL(updateLog(QVariant)), rootObject, SLOT(updateLog(QVariant)));

    return app->exec();
}
