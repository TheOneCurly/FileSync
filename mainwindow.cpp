#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Set up window
    ui->setupUi(this);
    loadSettings();

    host = Host::getInstance();
    client = Client::getInstance();
    QObject::connect(host, SIGNAL(appendToConsole(QString)), this, SLOT(appendToConsole(QString)));
    QObject::connect(client, SIGNAL(appendToConsole(QString)), this, SLOT(appendToConsole(QString)));
    QObject::connect(this,SIGNAL(connectToHost(QString)), client, SLOT(connectToHost(QString)));

    QStringList addresses = Host::getLocalAddresses();
    addressModel = new QStringListModel(addresses);
    ui->addressListView->setModel(addressModel);

    // Start public IP Query
    QObject::connect(host, SIGNAL(publicAddressFound(QString)), this, SLOT(publicIPFound(QString)));
    host->lookupPublicAddress();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event){
    if(doesUserWantToQuit()){
        saveSettings();
        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::saveSettings(){
    QSettings settings;
    settings.beginGroup("mainwindow");

    settings.setValue("size", size());
    settings.setValue("pos", pos());

    settings.endGroup();
}

void MainWindow::loadSettings(){
    QSettings settings;
    settings.beginGroup("mainwindow");

    resize(settings.value("size", QSize(500, 300)).toSize());
    move(settings.value("pos", QPoint(200,200)).toPoint());

    settings.endGroup();
}

bool MainWindow::doesUserWantToQuit(){
    const QMessageBox::StandardButton ret =
            QMessageBox::warning(this, tr("FileSync"), tr("Are you sure you want to quit?"),
                                 QMessageBox::Yes | QMessageBox::Cancel);
    switch(ret){
    case QMessageBox::Yes:
        return true;
    case QMessageBox::Cancel:
        return false;
    default:
        return false;
    }
}

void MainWindow::publicIPFound(const QString& address){
    ui->publicIPLabel->setText(address);
}

void MainWindow::appendToConsole(const QString& out){
    ui->consoleText->append("> " + out);
}

void MainWindow::on_connectButton_clicked(){
    emit connectToHost(ui->hostConnectionAddress->text());
}
