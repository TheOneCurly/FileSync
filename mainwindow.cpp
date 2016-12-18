#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Set up window
    ui->setupUi(this);

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

void MainWindow::publicIPFound(const QString& address){
    ui->publicIPLabel->setText(address);
}

void MainWindow::appendToConsole(const QString& out){
    ui->consoleText->append("> " + out);
}

void MainWindow::on_connectButton_clicked(){
    emit connectToHost(ui->hostConnectionAddress->text());
}
