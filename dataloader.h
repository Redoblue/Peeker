#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class MainWindow;
class QFile;
QT_END_NAMESPACE

class DataLoader : public QObject
{
    Q_OBJECT

public:
    explicit DataLoader(MainWindow *window, QString fileName);

signals:
    void finished();

public slots:
    void doWork();

private:
    MainWindow *mWindow;
    QString mFileName;
};

#endif // DATALOADER_H
