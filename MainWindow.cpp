#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    ui->chartView->setRenderHint(QPainter::Antialiasing);

    ui->pushButton->setText("柱状图");
    ui->pushButton_2->setText("雷达图");
    ui->pushButton_3->setText("饼状图");
    ui->pushButton_4->setText("折线图");

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::createABarChart);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::createRadarChart);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::createPieChart);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::createLineChart);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createABarChart() {
    QStringList categories = {"小明", "小红", "小刚", "小美"};

    setMath = new QBarSet("数学");
    setChinese = new QBarSet("语文");
    setEnglish = new QBarSet("英语");

    *setMath << 85 << 90 << 75 << 80;
    *setChinese << 78 << 82 << 88 << 91;
    *setEnglish << 92 << 85 << 80 << 87;

    barSets.clear();
    barSets.append(setMath);
    barSets.append(setChinese);
    barSets.append(setEnglish);

    series = new QBarSeries();
    series->append(setMath);
    series->append(setChinese);
    series->append(setEnglish);

    // 设置柱子标签显示
    series->setLabelsVisible(true);
    series->setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd); // 显示在柱子外部顶部
    series->setLabelsFormat("@value"); // 显示原始数值

    // 创建图表
    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("学生成绩对比图");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 设置X轴
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // 设置Y轴
    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("分数");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->setTheme(QChart::ChartThemeQt);
    chart->legend()->setVisible(true);

    ui->chartView->setChart(chart);

    // ============ 添加点击响应 =============
    connect(setMath, &QBarSet::clicked, [=](int index) {
        QString name = setMath->label(); // "数学"
        QString student = categories[index];
        double score = setMath->at(index);
        qDebug() << QString("科目：%1，学生：%2，分数：%3").arg(name).arg(student).arg(score);
    });

    connect(setChinese, &QBarSet::clicked, [=](int index) {
        QString name = setChinese->label();
        QString student = categories[index];
        double score = setChinese->at(index);
        qDebug() << QString("科目：%1，学生：%2，分数：%3").arg(name).arg(student).arg(score);
    });

    connect(setEnglish, &QBarSet::clicked, [=](int index) {
        QString name = setEnglish->label();
        QString student = categories[index];
        double score = setEnglish->at(index);
        qDebug() << QString("科目：%1\n学生：%2\n分数：%3").arg(name).arg(student).arg(score);
    });
}

void MainWindow::createRadarChart() {
    // 雷达图的数据维度标签
    QStringList categories = {"技术", "沟通", "管理", "创新", "执行力"};

    // 创建极坐标图
    QPolarChart *chart = new QPolarChart();
    chart->setTitle("个人能力雷达图");

    // 创建角度轴（即类目轴）
    QCategoryAxis *angularAxis = new QCategoryAxis();
    int count = categories.size();
    angleStep = 360.0 / count;
    for (int i = 0; i < count; ++i)
        angularAxis->append(categories[i], i * angleStep);

    angularAxis->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    angularAxis->setRange(0, 360);
    chart->addAxis(angularAxis, QPolarChart::PolarOrientationAngular);

    // 创建半径轴（即数值轴）
    QValueAxis *radialAxis = new QValueAxis();
    radialAxis->setRange(0, 100);
    radialAxis->setTickCount(6);
    radialAxis->setLabelFormat("%d");
    chart->addAxis(radialAxis, QPolarChart::PolarOrientationRadial);

    // 示例数据（每个维度的分数）
    data = {85, 60, 75, 90, 80};

    // 构建数据线条
    radarSeries = new QLineSeries();
    for (int i = 0; i < data.size(); ++i) {
        qreal angle = i * angleStep;
        radarSeries->append(angle, data[i]);
    }
    // 闭合雷达图
    radarSeries->append(360, data[0]);

    // 美化
    QPen pen(Qt::blue);
    pen.setWidth(2);
    radarSeries->setPen(pen);
    radarSeries->setBrush(QColor(135, 206, 250, 100)); // 半透明填充

    chart->addSeries(radarSeries);
    radarSeries->attachAxis(angularAxis);
    radarSeries->attachAxis(radialAxis);

    ui->chartView->setChart(chart);

    // 添加数据标签
    for (int i = 0; i < data.size(); ++i) {
        auto *label = new QGraphicsTextItem(QString::number(data[i]));
        label->setDefaultTextColor(Qt::black);
        label->setFont(QFont("Arial", 10, QFont::Bold));
        ui->chartView->scene()->addItem(label);
        dataLabels.append(label);
    }

    for (int i = 0; i < data.size(); ++i) {
        QPointF polarPoint(i * angleStep, data[i]);
        QPointF pos = chart->mapToPosition(polarPoint, radarSeries);
        if (i < dataLabels.size())
            dataLabels[i]->setPos(pos.x() - 10, pos.y() - 20);
    }

    // for (int i = 0; i < data.size(); ++i) {
    //     qreal angle = i * angleStep;
    //     QPointF point(angle, data[i]);
    //     QPointF pos = chart->mapToPosition(point, series); // 极坐标转视图坐标
    //     qDebug()<<pos;
    //
    //     QGraphicsTextItem *label = new QGraphicsTextItem(QString::number(data[i]));
    //     label->setDefaultTextColor(Qt::black);
    //     label->setFont(QFont("Arial", 10, QFont::Bold));
    //     label->setPos(pos.x() - 10, pos.y() - 20); // 微调位置以避免遮挡数据点
    //     ui->chartView->scene()->addItem(label);
    // }
}

void MainWindow::createPieChart() {
    QPieSeries *series = new QPieSeries();
    series->append("技术", 30);
    series->append("沟通", 20);
    series->append("管理", 25);
    series->append("创新", 15);
    series->append("执行力", 10);

    for (QPieSlice *slice: series->slices()) {
        slice->setLabelVisible(true);
        slice->setExplodeDistanceFactor(0.04); // 设置最大外扩距离

        connect(slice, &QPieSlice::hovered, [slice](bool state) {
            slice->setExploded(state); // 鼠标悬停时外扩，移出时恢复
        });
    }

    // 设置所有扇区显示标签
    for (QPieSlice *s: series->slices()) {
        s->setLabel(QString("%1: %2%").arg(s->label()).arg(s->percentage() * 100, 0, 'f', 1));
        s->setLabelVisible(true);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("鼠标悬停时突出显示的饼状图");
    chart->legend()->setAlignment(Qt::AlignRight);

    ui->chartView->setChart(chart);
}

void MainWindow::createLineChart() {
    // 创建折线图系列
    QLineSeries *lineSeries = new QLineSeries();
    lineSeries->setName("折线"); // 设置折线名称
    lineSeries->setPen(QPen(Qt::blue, 2)); // 设置折线颜色和宽度

    // 创建散点图系列（带圆点和标签）
    QScatterSeries *scatterSeries = new QScatterSeries();
    scatterSeries->setName("数据点"); // 设置散点名称
    scatterSeries->setMarkerShape(QScatterSeries::MarkerShapeCircle); // 设置为圆点
    scatterSeries->setMarkerSize(4.0); // 设置圆点大小
    scatterSeries->setBrush(QBrush(Qt::blue)); // 设置圆点填充颜色
    scatterSeries->setPen(QPen(Qt::transparent)); // 去掉散点边框

    // 添加数据点 (x, y) 到两个系列
    QList<QPointF> points = {
        QPointF(0, 6),
        QPointF(1, 4),
        QPointF(2, 8),
        QPointF(3, 5),
        QPointF(4, 7),
        QPointF(5, 3)
    };
    lineSeries->append(points);    // 添加到折线系列
    scatterSeries->append(points); // 添加到散点系列

    // 设置散点标签
    QFont labelFont;
    labelFont.setPointSize(10); // 设置标签字体大小
    scatterSeries->setPointLabelsFont(labelFont);
    scatterSeries->setPointLabelsFormat("@yPoint"); // 显示 Y 值
    scatterSeries->setPointLabelsColor(Qt::black); // 设置标签颜色
    scatterSeries->setPointLabelsClipping(false); // 标签不被裁剪

    // 创建图表对象
    QChart *chart = new QChart();
    chart->addSeries(lineSeries);    // 添加折线系列
    chart->addSeries(scatterSeries); // 添加散点系列
    chart->setTitle("折线图与数据点"); // 设置图表标题
    chart->createDefaultAxes();      // 创建默认坐标轴
    chart->legend()->setVisible(true); // 显示图例
    chart->legend()->setAlignment(Qt::AlignBottom); // 图例位置设置为底部

    // 设置坐标轴范围
    chart->axisX()->setRange(-0.5, 5.5); // X 轴范围（稍作扩展）
    chart->axisY()->setRange(0, 10);     // Y 轴范围

    ui->chartView->setChart(chart);
}

void MainWindow::updateLabels() {

}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    for (int i = 0; i < data.size(); ++i) {
        QPointF polarPoint(i * angleStep, data[i]);
        QPointF pos = chart->mapToPosition(polarPoint, radarSeries);
        if (i < dataLabels.size())
            dataLabels[i]->setPos(pos.x() - 10, pos.y() - 20);
    }
}

