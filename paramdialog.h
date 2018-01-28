#ifndef PARAMDIALOG_H
#define PARAMDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE

class ParamDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ParamDialog(QWidget *parent = Q_NULLPTR,
                         Qt::WindowFlags f = Qt::WindowFlags());
    ~ParamDialog();

    QLineEdit *mNoiseEdit;
    QLineEdit *mPeekRadiusEdit;
    QString lastNoise;
    QString lastPeekRadius;

private:
    QPushButton *mConfirmButton;
    QPushButton *mCancelButton;
};

#endif // PARAMDIALOG_H
