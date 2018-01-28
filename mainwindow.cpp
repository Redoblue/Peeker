#include "mainwindow.h"
#include "chart.h"
#include "chartview.h"
#include "paramdialog.h"
#include "dataloader.h"

#include <QFrame>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineSeries>
#include <QPointF>
#include <QtMath>
#include <QWidget>
#include <QRandomGenerator>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QFileDialog>
#include <QDebug>
#include <QStack>
#include <QMap>
#include <QMessageBox>
#include <QTime>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mLastDir(QDir::homePath()),
    mNoiseF(0.5),
    mPeekRadiusF(10)
{
    mParamDialog = new ParamDialog;
    mParamDialog->lastNoise = "0.5";
    mParamDialog->lastPeekRadius = "10";

    setupUI();
    connectSS();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setupUI()
{
    mOpenButton = new QPushButton();
    mOpenButton->setToolTip(tr("打开"));
    mOpenButton->setMinimumSize(64, 64);
    mOpenButton->setIcon(QIcon(":/res/icons/op_open.png"));
    mOpenButton->setIconSize(QSize(64, 64));
    mOpenButton->setFlat(true);
    mOpenButton->setCursor(Qt::PointingHandCursor);

    mCalcButton = new QPushButton();
    mCalcButton->setToolTip(tr("寻峰"));
    mCalcButton->setMinimumSize(64, 64);
    mCalcButton->setIcon(QIcon(":/res/icons/op_peek.png"));
    mCalcButton->setIconSize(QSize(64, 64));
    mCalcButton->setFlat(true);
    mCalcButton->setEnabled(false);
    mCalcButton->setCursor(Qt::PointingHandCursor);

    mSaveButton = new QPushButton();
    mSaveButton->setToolTip(tr("保存"));
    mSaveButton->setMinimumSize(64, 64);
    mSaveButton->setIcon(QIcon(":/res/icons/op_save.png"));
    mSaveButton->setIconSize(QSize(64, 64));
    mSaveButton->setFlat(true);
    mSaveButton->setEnabled(false);
    mSaveButton->setCursor(Qt::PointingHandCursor);

    QVBoxLayout *opLayout = new QVBoxLayout;
    opLayout->addWidget(mOpenButton);
    opLayout->addWidget(mCalcButton);
    opLayout->addWidget(mSaveButton);
    QGroupBox *opGroup = new QGroupBox();
    opGroup->setLayout(opLayout);
    opGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mMainPeekButton = new QPushButton();
    mMainPeekButton->setToolTip(tr("主峰"));
    mMainPeekButton->setMinimumSize(64, 64);
    mMainPeekButton->setIcon(QIcon(":/res/icons/view_main.png"));
    mMainPeekButton->setIconSize(QSize(64, 64));
    mMainPeekButton->setFlat(true);
    mMainPeekButton->setEnabled(false);
    mMainPeekButton->setCursor(Qt::PointingHandCursor);

    mAllPeekButton = new QPushButton();
    mAllPeekButton->setToolTip(tr("总览"));
    mAllPeekButton->setMinimumSize(64, 64);
    mAllPeekButton->setIcon(QIcon(":/res/icons/view_all.png"));
    mAllPeekButton->setIconSize(QSize(64, 64));
    mAllPeekButton->setFlat(true);
    mAllPeekButton->setEnabled(false);
    mAllPeekButton->setCursor(Qt::PointingHandCursor);

    QVBoxLayout *viewLayout = new QVBoxLayout;
    viewLayout->addWidget(mMainPeekButton);
    viewLayout->addWidget(mAllPeekButton);
    QGroupBox *viewGroup = new QGroupBox();
    viewGroup->setLayout(viewLayout);
    viewGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    mParamButton = new QPushButton();
    mParamButton->setToolTip(tr("参数"));
    mParamButton->setMinimumSize(64, 64);
    mParamButton->setIcon(QIcon(":/res/icons/param.png"));
    mParamButton->setIconSize(QSize(64, 64));
    mParamButton->setFlat(true);
    mParamButton->setCursor(Qt::PointingHandCursor);

    mHelpButton = new QPushButton();
    mHelpButton->setToolTip(tr("帮助"));
    mHelpButton->setMinimumSize(64, 64);
    mHelpButton->setIcon(QIcon(":/res/icons/help.png"));
    mHelpButton->setIconSize(QSize(64, 64));
    mHelpButton->setFlat(true);
    mHelpButton->setCursor(Qt::PointingHandCursor);

    QVBoxLayout *paramLayout = new QVBoxLayout;
    paramLayout->addWidget(mParamButton);
    paramLayout->addWidget(mHelpButton);
    QGroupBox *paramGroup = new QGroupBox();
    paramGroup->setLayout(paramLayout);
    paramGroup->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    QVBoxLayout *leftLayout = new QVBoxLayout();
    leftLayout->addWidget(opGroup, 0, Qt::AlignHCenter);
    leftLayout->addWidget(viewGroup, 0, Qt::AlignHCenter);
    leftLayout->addStretch();
    leftLayout->addWidget(paramGroup, 0, Qt::AlignHCenter);
    leftLayout->setSpacing(20);

    QFrame *leftFrame = new QFrame;
    leftFrame->setLayout(leftLayout);
    leftFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    leftFrame->setStyleSheet("QGroupBox{background: transparent; border: none}"
                             "QPushButton:pressed {background: #EFEFEF;}"
                             "QPushButton:hover {background: #232323;}");

    // 设置左侧背景
    QPalette leftPal;
    leftPal.setColor(QPalette::Window, QColor(59, 59, 59));
    leftFrame->setPalette(leftPal);
    leftFrame->setAutoFillBackground(true);

    // set series
    mSeries1 = new QLineSeries;
    mSeries1->setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    mSeries2 = new QScatterSeries;
    mSeries2->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mSeries2->setMarkerSize(5);
    mSeries2->setColor(QColor(Qt::blue));
    mSeries2->setBorderColor(Qt::blue);
    mSeries3 = new QScatterSeries;
    mSeries3->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    mSeries3->setMarkerSize(5);
    mSeries3->setColor(QColor(Qt::red));
    mSeries3->setBorderColor(Qt::red);
    mSeries3->setMarkerShape(QScatterSeries::MarkerShapeCircle);

    mChart = new Chart();
    //mChart->setTitle(tr("Name"));
    mChart->addSeries(mSeries1);
    mChart->addSeries(mSeries2);
    mChart->addSeries(mSeries3);
    mChart->setAnimationOptions(QChart::SeriesAnimations);
    mChart->legend()->hide();
    mChart->createDefaultAxes();

    mChartView = new ChartView(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);
    mChartView->setCursor(Qt::CrossCursor);
    mChartView->setEnabled(false);

    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addWidget(leftFrame);
    mainLayout->addWidget(mChartView);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);

    QWidget *mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);
    mainWidget->setMinimumWidth(1200);

    setCentralWidget(mainWidget);
    setWindowTitle(tr("Peeker"));
    resize(1200, 600);

    QDesktopWidget wid;
    this->move(wid.screen()->width()/2-width()/2,
               wid.screen()->height()/2-height()/2);
}

void MainWindow::connectSS()
{
    connect(mOpenButton, SIGNAL(clicked(bool)), this, SLOT(onOpenClicked()));
    connect(mCalcButton, SIGNAL(clicked(bool)), this, SLOT(onCalcClicked()));
    connect(mSaveButton, SIGNAL(clicked(bool)), this, SLOT(onSaveClicked()));
    connect(mAllPeekButton, SIGNAL(clicked(bool)), this, SLOT(onAllPeekViewRequired()));
    connect(mMainPeekButton, SIGNAL(clicked(bool)), this, SLOT(onMainPeekViewRequired()));
    connect(mParamButton, SIGNAL(clicked(bool)), this, SLOT(onParamClicked()));
    connect(mParamDialog, SIGNAL(accepted()), this, SLOT(onDlgFinished()));
    connect(mHelpButton, SIGNAL(clicked(bool)), this, SLOT(onHelpClicked()));

    //connect(this, SIGNAL(fileOpened()), this, SLOT(onAllPeekViewRequired()));
    //connect(this, SIGNAL(fileOpened()), this, SLOT(onFileOpened()));
    connect(this, SIGNAL(calcFinished()), this, SLOT(onCalcFinished()));
}

void MainWindow::onOpenClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("打开CSV文件"),
                                                    mLastDir,
                                                    tr("CSV Files (*.csv)"));
    if (!QFileInfo::exists(fileName))
    {
        qDebug() << "不是一个CSV文件!";
        return;
    }

    mLastDir = fileName;

    qDebug() << "change cursor";
    setCursor(Qt::WaitCursor);

    QThread *loaderThread = new QThread;
    DataLoader *loader = new DataLoader(this, fileName);
    loader->moveToThread(loaderThread);
    connect(loaderThread, SIGNAL(started()), loader, SLOT(doWork()));
    connect(loader, SIGNAL(finished()), loaderThread, SLOT(quit()));
    connect(loader, SIGNAL(finished()), loader, SLOT(deleteLater()));
    connect(loader, SIGNAL(finished()), this, SLOT(onFileOpened()));
    connect(loader, SIGNAL(finished()), this, SLOT(onAllPeekViewRequired()));
    connect(loaderThread, SIGNAL(finished()), loaderThread, SLOT(deleteLater()));
    loaderThread->start();
}

void MainWindow::onCalcClicked()
{
    mSeries3->clear();

    QStack<int> stack;

    for (int i=0; i<mSeries1->count(); i++)
    {
        float tmpY = (float) mSeries1->at(i).y();
        if (tmpY < mNoiseF)
        {
            if (!stack.isEmpty())
            {
                int tmpI = stack.pop();
                mSeries3->append(mSeries1->at(tmpI));
            }
        }
        else
        {
            if (!stack.isEmpty())
            {
                int tmpI = stack.top();
                if (mSeries1->at(tmpI).y() < tmpY)
                {
                    stack.pop();
                    stack.push(i);
                }
            }
            else
            {
                stack.push(i);
            }
        }
    }

    if (mSeries3->count() == 0)
    {
        QMessageBox::information(this, tr("计算失败"), tr("没有小于噪声级别的数据点！"), tr("确定"));
        return;
    }

    emit(calcFinished());
}

void MainWindow::onSaveClicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("保存CSV文件"),
                                                    QDir::homePath() + "/untitled.csv",
                                                    tr("CSV Files (*.csv)"));

    if (fileName.isNull())
    {
        qDebug() << "文件名不符合要求！";
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "文件创建失败！";
        return;
    }

    QTextStream out(&file);

    QString content;
    for (int i=0; i<mSeries3->count(); i++)
    {
        qreal x = mSeries3->at(i).x();
        qreal y = mSeries3->at(i).y();

        //content.append(QString::number(x));
        content.append(QString::number(x));
        content.append(',');
        content.append(QString::number(y));
        content.append('\n');
    }
    out << content;
    file.close();

    QMessageBox::information(this, tr("保存成功"), tr("数据已保存在 %1！").arg(fileName), tr("确定"));
}

void MainWindow::onAllPeekViewRequired()
{
    adjustView(mXminF, mXmaxF, mYminF, mYmaxF);
}

void MainWindow::onMainPeekViewRequired()
{
    adjustView(mXToMaxYF - mPeekRadiusF, mXToMaxYF + mPeekRadiusF,
               mYminF, mYmaxF);
}

void MainWindow::adjustView(float xmin, float xmax, float ymin, float ymax)
{
    float xOffset = (xmax - xmin) * 0.05f;
    float yOffset = (ymax - ymin) * 0.05f;
    mChart->axisX()->setRange(xmin - xOffset, xmax + xOffset);
    mChart->axisY()->setRange(ymin, ymax + yOffset);
}

void MainWindow::onFileOpened()
{
    unsetCursor();
    mSaveButton->setEnabled(false);
    mCalcButton->setEnabled(true);
    mMainPeekButton->setEnabled(true);
    mAllPeekButton->setEnabled(true);
    mChartView->setEnabled(true);
}

void MainWindow::onCalcFinished()
{
    mSaveButton->setEnabled(true);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_F11:
        if (!isFullScreen()) {
            showFullScreen();
        } else {
            showNormal();
        }
        break;
    case Qt::Key_Escape:
        if (isFullScreen()) {
            showNormal();
        }
    default:
        break;
    }
}

void MainWindow::onParamClicked()
{
    //mParamDialog = new ParamDialog;
    mParamDialog->mNoiseEdit->setText(mParamDialog->lastNoise);
    mParamDialog->mPeekRadiusEdit->setText(mParamDialog->lastPeekRadius);
    mParamDialog->exec();
}

void MainWindow::onDlgFinished()
{
    mParamDialog->lastNoise = mParamDialog->mNoiseEdit->text();
    mParamDialog->lastPeekRadius = mParamDialog->mPeekRadiusEdit->text();
    mNoiseF = mParamDialog->mNoiseEdit->text().toFloat();
    mPeekRadiusF = mParamDialog->mPeekRadiusEdit->text().toFloat();
}

void MainWindow::onHelpClicked()
{
    QMessageBox::information(this, tr("帮助"), tr("此软件在张丽君同学的坑蒙拐骗下开发，\n保留权利，概不负责！\n\nby BLQ"), tr("确定"));
}
