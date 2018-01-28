#include "paramdialog.h"
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

ParamDialog::ParamDialog(QWidget *parent, Qt::WindowFlags f) :
    QDialog(parent, f)
{
    mNoiseEdit = new QLineEdit;
    mPeekRadiusEdit = new QLineEdit;

    QFormLayout *paramLayout = new QFormLayout;
    paramLayout->addRow(tr(" 噪声："), mNoiseEdit);
    paramLayout->addRow(tr(" 峰域："), mPeekRadiusEdit);

    mConfirmButton = new QPushButton(tr("确定"));
    mCancelButton = new QPushButton(tr("取消"));
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(mConfirmButton);
    bottomLayout->addWidget(mCancelButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(paramLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    setMaximumSize(200, 150);
    setMinimumHeight(150);
    setWindowFlags(Qt::WindowCloseButtonHint);

    connect(mConfirmButton, SIGNAL(clicked(bool)), this, SLOT(accept()));
    connect(mCancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

ParamDialog::~ParamDialog()
{

}
