#include "mainwindow.h"
#include <QtGlobal>
#include <QApplication>
#include <QDebug>
#include <QThread>

#include "host.h"
#include "client.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QThread clientThread;
    Client *m_client = new Client();
    QObject::connect(&clientThread, SIGNAL(started()), m_client, SLOT(start()));
    QObject::connect(&clientThread, SIGNAL(finished()), m_client, SLOT(deleteLater()));
    m_client->moveToThread(&clientThread);
    clientThread.start();

    QThread hostThread;
    Host *m_host = new Host();
    QObject::connect(&hostThread, SIGNAL(started()), m_host, SLOT(start()));
    QObject::connect(&hostThread, SIGNAL(finished()), m_host, SLOT(deleteLater()));
    m_host->moveToThread(&hostThread);
    hostThread.start();

    qDebug() << "Starting...";

    return a.exec();
}
