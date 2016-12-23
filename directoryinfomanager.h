#ifndef DIRECTORYINFOMANAGER_H
#define DIRECTORYINFOMANAGER_H

#include <QObject>
#include <QDebug>
#include <QList>
#include <QDir>
#include <QFileInfoList>
#include <QDataStream>
#include <QMetaProperty>

class DirectoryInfo;
//Q_DECLARE_METATYPE(QList<DirectoryInfo>)

class DirectoryInfoManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<DirectoryInfo> directoryList READ getDirectoryList WRITE setDirectoryList NOTIFY directoryListChanged)
public:
    DirectoryInfoManager();
    DirectoryInfoManager(QString&);
    DirectoryInfoManager(const DirectoryInfoManager&);

    QList<DirectoryInfo> getDirectoryList() const { return directoryList; }
    void setDirectoryList(QList<DirectoryInfo> newDirectoryList){
        directoryList = newDirectoryList;
    }

private:
    QList<DirectoryInfo> directoryList;

signals:
    void directoryListChanged();
};

class DirectoryInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
public:
    DirectoryInfo();
    DirectoryInfo(const DirectoryInfo&);

    QString getName() const { return name; }
    void setName(QString newName) { name = newName; }

private:
    QString name;

signals:
    void nameChanged();

};

QDataStream &operator<<(QDataStream&, const DirectoryInfoManager&);
QDataStream &operator>>(QDataStream&, DirectoryInfoManager&);

QDataStream &operator<<(QDataStream&, const DirectoryInfo&);
QDataStream &operator>>(QDataStream&, DirectoryInfo&);

#endif // DIRECTORYINFOMANAGER_H
