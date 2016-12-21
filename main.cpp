#include "mainwindow.h"
#include <QtGlobal>
#include <QApplication>
#include <QStandardPaths>
#include <QSettings>
#include <QDebug>
#include <QThread>

#include "host.h"
#include "client.h"

int main(int argc, char *argv[]){

    // These must be first for settings to load
    QCoreApplication::setOrganizationDomain("TheOneCurly");
    QCoreApplication::setApplicationName("FileSync");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Check for user settings, default unset settings
//    QSettings settings;
//    settings.beginGroup("UserSettings");
//    if(!settings.contains("BaseDirectory")){
//        settings.setValue("BaseDirectory", QStandardPaths.writableLocation(QStandardPaths::AppDataLocation));
//    }
//    settings.endGroup();

    QThread clientThread;
    Client* m_client = Client::getInstance();
    QObject::connect(&clientThread, SIGNAL(started()), m_client, SLOT(start()));
    QObject::connect(&clientThread, SIGNAL(finished()), m_client, SLOT(deleteLater()));
    m_client->moveToThread(&clientThread);
    clientThread.start();

    QThread hostThread;
    Host* m_host = Host::getInstance();
    QObject::connect(&hostThread, SIGNAL(started()), m_host, SLOT(start()));
    QObject::connect(&hostThread, SIGNAL(finished()), m_host, SLOT(deleteLater()));
    m_host->moveToThread(&hostThread);
    hostThread.start();

    qDebug() << "Starting...";

    return a.exec();
}
