#ifndef HOST_H
#define HOST_H

#include <QObject>
#include <QTcpServer>

class Host : public QObject
{
    Q_OBJECT
public:
    explicit Host(QObject *parent = 0);

public slots:
    void start();

private:
    QTcpServer server;
signals:

};

#endif // HOST_H
