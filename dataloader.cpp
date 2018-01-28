#include "dataloader.h"
#include "mainwindow.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

DataLoader::DataLoader(MainWindow *window, QString fileName) :
    mWindow(window),
    mFileName(fileName)
{

}

void DataLoader::doWork()
{
    qDebug() << "in dowork";
    QFile file(mFileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "文件打开失败！";
        return;
    }

    QTextStream in(&file);

    mWindow->mSeries1->clear();
    mWindow->mSeries2->clear();
    mWindow->mSeries3->clear();

    mWindow->mXminF = 10000.0f;
    mWindow->mXmaxF = 0.0f;
    mWindow->mYminF = 10000.0f;
    mWindow->mYmaxF = 0.0f;

    QString content = in.readAll();
    QStringList contentList = content.split('\n');

    for (int i=1; i<contentList.count()-1; i++)
    {
        QString line = contentList.at(i);
        QStringList strData = line.split(',');
        float x = strData[0].toFloat();
        float y = strData[1].toFloat();
        QPointF p((qreal) x, (qreal) y);
        mWindow->mSeries1->append(p);
        mWindow->mSeries2->append(p);

        if (x < mWindow->mXminF)
        {
            mWindow->mXminF = x;
        }
        if (x > mWindow->mXmaxF)
        {
            mWindow->mXmaxF = x;
        }
        if (y < mWindow->mYminF)
        {
            mWindow->mYminF = y;
        }
        if (y > mWindow->mYmaxF)
        {
            mWindow->mYmaxF = y;
            mWindow->mXToMaxYF = x;
        }
    }
    file.close();

    emit finished();
}
