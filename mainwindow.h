#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QStringListModel>

#include "host.h"
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStringListModel *addressModel;

    Host* host;
    Client* client;

private slots:
    void publicIPFound(const QString&);
    void appendToConsole(const QString&);
    void on_connectButton_clicked();

signals:
    void connectToHost(const QString&);
};

#endif // MAINWINDOW_H
