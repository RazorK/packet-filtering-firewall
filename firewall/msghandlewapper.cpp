#include "msghandlewapper.h"
#include <QtCore/QMetaType>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>

void static msgHandlerFunction(QtMsgType type, const char *msg)
{
    QMetaObject::invokeMethod(MsgHandlerWapper::instance(), "message"
                        , Q_ARG(QtMsgType, type)
                        , Q_ARG(QString, QString::fromLocal8Bit(msg)));
}

MsgHandlerWapper * MsgHandlerWapper::m_instance = 0;

MsgHandlerWapper * MsgHandlerWapper::instance()
{
    static QMutex mutex;
    if (!m_instance) {
        QMutexLocker locker(&mutex);
        if (!m_instance)
            m_instance = new MsgHandlerWapper;
    }

    return m_instance;
}

MsgHandlerWapper::MsgHandlerWapper()
    :QObject(qApp)
{
    qRegisterMetaType<QtMsgType>("QtMsgType");
    qInstallMsgHandler(msgHandlerFunction);
}

