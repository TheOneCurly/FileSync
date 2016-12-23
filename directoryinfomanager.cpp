#include "directoryinfomanager.h"

DirectoryInfoManager::DirectoryInfoManager(){

}

DirectoryInfoManager::DirectoryInfoManager(QString& pathString){
    QDir path(pathString);
    if(path.exists()){
        QFileInfoList dirList = path.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        for(int i = 0; i < dirList.length(); i++){
            DirectoryInfo newDir;
            qDebug() << "Directory manager finds: " << dirList[i].filePath();
            newDir.setName(dirList[i].filePath());
            directoryList.append(newDir);
        }
    }
}

DirectoryInfoManager::DirectoryInfoManager(const DirectoryInfoManager &other) : QObject(other.parent()){
    directoryList = other.directoryList;
}

DirectoryInfo::DirectoryInfo(){

}

DirectoryInfo::DirectoryInfo(const DirectoryInfo &other) : QObject(other.parent()){
    name = other.getName();
}

QDataStream &operator<<(QDataStream &ds, const DirectoryInfoManager &obj){
    qDebug() << "Manager data stream start";
    for(int i = 0; i < obj.metaObject()->propertyCount(); i++){
        qDebug() << "Manager data stream " << i;
        if(obj.metaObject()->property(i).isStored(&obj)){
            qDebug() << "Manager data stream " << obj.metaObject()->property(i).read(&obj);
            ds << obj.metaObject()->property(i).read(&obj);
        }
    }
    return ds;
}

QDataStream &operator>>(QDataStream &ds, DirectoryInfoManager &obj){
    QVariant var;
    for(int i = 0; i < obj.metaObject()->propertyCount(); i++){
        if(obj.metaObject()->property(i).isStored(&obj)){
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }
    return ds;
}

QDataStream &operator<<(QDataStream &ds, const DirectoryInfo &obj){
    qDebug() << "Directory info stream start";
    for(int i = 0; i < obj.metaObject()->propertyCount(); i++){
        qDebug() << "Directory info stream " << i;
        if(obj.metaObject()->property(i).isStored(&obj)){
            qDebug() << "Directory info stream " << obj.metaObject()->property(i).read(&obj);
            ds << obj.metaObject()->property(i).read(&obj);
        }
    }
    return ds;
}

QDataStream &operator>>(QDataStream &ds, DirectoryInfo &obj){
    QVariant var;
    for(int i = 0; i < obj.metaObject()->propertyCount(); i++){
        if(obj.metaObject()->property(i).isStored(&obj)){
            ds >> var;
            obj.metaObject()->property(i).write(&obj, var);
        }
    }
    return ds;
}
