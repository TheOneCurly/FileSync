#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QStringListModel>
#include <QSettings>
#include <QMessageBox>
#include <QCloseEvent>

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

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
    QStringListModel *addressModel;

    Host* host;
    Client* client;

    void saveSettings();
    void loadSettings();

    bool doesUserWantToQuit();

private slots:
    void publicIPFound(const QString&);
    void appendToConsole(const QString&);
    void on_connectButton_clicked();

signals:
    void connectToHost(const QString&);
};

#endif // MAINWINDOW_H
