#include <QtGui/QApplication>
#include <QGraphicsObject>
#include <QDeclarativeView>
#include <QUrl>
#include <QIcon>
#include <QTextCodec>
#include "qmlapplicationviewer.h"
#include "stenographer.h"
#include "recthread.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    #ifdef Q_OS_LINUX
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
    #else //Q_OS_MSDOS
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("IBM866"));
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP-1251"));
        //QTextCodec::setCodecForLocale (QTextCodec::codecForName("Windows-1251"));
        //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));
    #endif
    QTextCodec::setCodecForTr(QTextCodec::codecForCStrings());

    QScopedPointer<QApplication> app(createApplication(argc, argv));

    Stenographer st;

//    QmlApplicationViewer viewer;
//    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
//    viewer.setMainQmlFile(QLatin1String("qml/stenographer/main.qml"));
//    viewer.setMainQmlFile(QUrl("qrc:/main.qml"));
//    viewer.showExpanded();

//    QDeclarativeView view;
//    view.setSource(QUrl("qrc:/main.qml"));
//    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
//    view.engine()->addImageProvider(QLatin1String("MyImage"), new ImageProvider(&st));

    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());
    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setSource(QUrl("qrc:/qml/main.qml"));
//    viewer->setSource(QUrl("./qml/main.qml"));
    viewer->setWindowIcon(QIcon(":/icon80.png"));
    viewer->showExpanded();

    QObject *rootObject = dynamic_cast<QObject*>(viewer->rootObject());

    QObject::connect(rootObject, SIGNAL(beginStopRec()), &st, SLOT(beginStopRec()));
    QObject::connect(rootObject, SIGNAL(playLast(QVariant)), &st, SLOT(playLast(QVariant)));
    QObject::connect(rootObject, SIGNAL(convertAndSent()), &st, SLOT(convertAndSent()));

    QObject::connect(&st, SIGNAL(updateData(QVariant)), rootObject, SLOT(updateData(QVariant)));
    QObject::connect(&st, SIGNAL(updateLog(QVariant)), rootObject, SLOT(updateLog(QVariant)));

    QObject::connect(&st, SIGNAL(setVolumeRange(QVariant)), rootObject, SLOT(setVolumeRange(QVariant)));
    QObject::connect(&st, SIGNAL(setVolumeValue(QVariant)), rootObject, SLOT(setVolumeValue(QVariant)));

    QObject::connect(&st, SIGNAL(testStopped(QVariant)), rootObject, SLOT(stopTest(QVariant)));
    QObject::connect(rootObject, SIGNAL(startTest()), &st, SLOT(startTest()));

    QObject::connect(&st, SIGNAL(needToStop()), rootObject, SLOT(stopRec()));

    QObject::connect(rootObject, SIGNAL(switchAutoMod()), &st, SLOT(switchAutoMod()));
    QObject::connect(rootObject, SIGNAL(setOverK(QVariant)), &st, SLOT(setOverK(QVariant)));
    QObject::connect(rootObject, SIGNAL(setIT1(QVariant)), &st, SLOT(setIT1(QVariant)));
    QObject::connect(rootObject, SIGNAL(setITstep1(QVariant)), &st, SLOT(setITstep1(QVariant)));
    QObject::connect(rootObject, SIGNAL(setIT2(QVariant)), &st, SLOT(setIT2(QVariant)));
    QObject::connect(rootObject, SIGNAL(setITstep2(QVariant)), &st, SLOT(setITstep2(QVariant)));
    QObject::connect(rootObject, SIGNAL(setDelay(QVariant)), &st, SLOT(setDelay(QVariant)));

    QObject::connect(&st, SIGNAL(setAudioDevices(QVariant)), rootObject, SLOT(setAudioDevices(QVariant)));

    QObject::connect(rootObject, SIGNAL(getOptions()), &st, SLOT(getOptions()));
    QObject::connect(rootObject, SIGNAL(saveText(QVariant, QVariant)), &st, SLOT(saveText(QVariant,QVariant)));
    QObject::connect(rootObject, SIGNAL(setSaveTempFiles(QVariant)), &st, SLOT(setSaveTempFiles(QVariant)));

    return app->exec();
}
