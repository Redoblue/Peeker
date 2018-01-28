#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>

QT_BEGIN_NAMESPACE
class QFrame;
class QPushButton;
class QLineEdit;
class Chart;
class ChartView;
class ParamDialog;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupUI();
    void connectSS();
    void adjustView(float xmin, float xmax, float ymin, float ymax);

    QLineSeries *mSeries1;
    QScatterSeries *mSeries2;
    QScatterSeries *mSeries3;
    float mNoiseF;
    float mPeekRadiusF;
    float mXminF;
    float mXmaxF;
    float mYminF;
    float mYmaxF;
    float mXToMaxYF;

protected:
    virtual void keyPressEvent(QKeyEvent *event);

public slots:
    void onOpenClicked();
    void onCalcClicked();
    void onSaveClicked();
    void onParamClicked();
    void onHelpClicked();
    void onMainPeekViewRequired();
    void onAllPeekViewRequired();
    void onFileOpened();
    void onCalcFinished();
    void onDlgFinished();

signals:
    void fileOpened();
    void calcFinished();

private:
    QPushButton *mOpenButton;
    QPushButton *mCalcButton;
    QPushButton *mSaveButton;
    QPushButton *mMainPeekButton;
    QPushButton *mAllPeekButton;
    QPushButton *mParamButton;
    QPushButton *mHelpButton;
    Chart *mChart;
    ChartView *mChartView;
    ParamDialog *mParamDialog;

    QString mLastDir;
};

#endif // MAINWINDOW_H
