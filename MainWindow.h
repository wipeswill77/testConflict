//
// Created by 31053 on 2025/4/9.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void createABarChart();

    void createRadarChart();

    void createPieChart();
    void createLineChart();

    void updateLabels();

void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;

    QBarSet *setMath=nullptr;
    QBarSet *setChinese=nullptr;
    QBarSet *setEnglish=nullptr;

    QBarSeries *series=nullptr;
    QChart *chart=nullptr;
    QList<QBarSet *> barSets; // 存储多个 BarSet

    QVector<QGraphicsTextItem*> dataLabels;

    QVector<qreal> data;
    QLineSeries *radarSeries=nullptr;
    qreal angleStep;
};


#endif //MAINWINDOW_H
