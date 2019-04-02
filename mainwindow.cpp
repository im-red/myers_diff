#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "myersdiff.h"

#include <QGraphicsTextItem>
#include <QDebug>

const QString MainWindow::FONT_FAMILY = "Arial";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(QBrush(Qt::black));

    ui->fromEdit->setText("ABCABBA");
    ui->toEdit->setText("CBABAC");

    ui->graphicsView->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_diffButton_clicked()
{
    QString from = ui->fromEdit->text();
    QString to = ui->toEdit->text();

    m_scene->clear();
    m_scene->update();
    drawGrid(from, to);
    drawDiagonal(from, to);
    drawKLine(from, to);
    DiffResult result = myersDiff(from, to);

    drawVisited(result);
    drawDLine(result);
    drawAnswer(result);
}

void MainWindow::drawGrid(const QString &from, const QString &to)
{
    for (int i = 0; i < from.size() + 1; i++)
    {
        QPointF start(i, 0);
        QPointF end(i, to.size());
        addLine(start, end);
        if (i != 0)
        {
            addText(start, QString(from[i - 1]), Top);
        }
    }
    for (int i = 0; i < to.size() + 1; i++)
    {
        QPointF start(0, i);
        QPointF end(from.size(), i);
        addLine(start, end);
        if (i != 0)
        {
            addText(start, QString(to[i - 1]), Left);
        }
    }
}

void MainWindow::drawDiagonal(const QString &from, const QString &to)
{
    const int fromLen = from.size();
    const int toLen = to.size();

    for (int i = 0; i < fromLen; i++)
    {
        for (int j = 0; j < toLen; j++)
        {
            if (from[i] == to[j])
            {
                QPointF start(i, j);
                QPointF end(i + 1, j + 1);
                addLine(start, end);
            }
        }
    }
}

void MainWindow::drawVisited(const DiffResult &result)
{
    for (auto &snakeList : result.visited)
    {
        for (auto &snake : snakeList)
        {
            bool midArrow = (snake.start != snake.mid) && (snake.mid == snake.end);
            bool endArrow = snake.mid != snake.end;
            addLine(QPointF(snake.start.x, snake.start.y), QPointF(snake.mid.x, snake.mid.y), QColor(Qt::blue), 1, midArrow);
            addLine(QPointF(snake.mid.x, snake.mid.y), QPointF(snake.end.x, snake.end.y), QColor(Qt::blue), 1, endArrow);
        }
    }
}

void MainWindow::drawDLine(const DiffResult &result)
{
    for (int d = 0; d < result.visited.size(); d++)
    {
        std::vector<Point> points;
        for (int k = -d; k <= d; k += 2)
        {
            points.push_back(result.visited[d][index(k)].end);
        }
        const int pointNum = static_cast<int>(points.size());

        Point firstEnd(0, 0);

        for (int i = 0; i < pointNum - 1; i++)
        {
            if (points[i + 1] == Point(0, 0))
            {
                break;
            }
            firstEnd = points[i + 1];
            QPointF start(points[i].x, points[i].y);
            QPointF end(points[i + 1].x, points[i + 1].y);
            addLine(start, end, QColor(Qt::yellow));
        }

        addText(QPointF(firstEnd.x, firstEnd.y), QString("d = %1").arg(d), BottomRight, QColor(Qt::yellow), TEXT_SIZE / 2);
    }
}

void MainWindow::drawKLine(const QString &from, const QString &to)
{
    const int fromLen = from.size();
    const int toLen = to.size();

    QPointF delta((fromLen + toLen) * 0.5 + 0.5, (fromLen + toLen) * 0.5 + 0.5);

    for (int k = -toLen; k <= fromLen; k++)
    {
        QPointF start(k * 0.5, -k * 0.5);
        QPointF end = start + delta;

        addLine(start, end, QColor(Qt::darkGreen));
        addText(end, QString("k=%1").arg(k), BottomRight, QColor(Qt::darkGreen), TEXT_SIZE / 2);
    }
}

void MainWindow::drawAnswer(const DiffResult &result)
{
    for (auto &snake : result.answer)
    {
        bool midArrow = (snake.start != snake.mid) && (snake.mid == snake.end);
        bool endArrow = snake.mid != snake.end;
        addLine(QPointF(snake.start.x, snake.start.y), QPointF(snake.mid.x, snake.mid.y), QColor(Qt::red), 2, midArrow);
        addLine(QPointF(snake.mid.x, snake.mid.y), QPointF(snake.end.x, snake.end.y), QColor(Qt::red), 2, endArrow);
    }
}

void MainWindow::addLine(const QPointF &start, const QPointF &end, const QColor &color, int width, bool hasArrow)
{
    const QPointF realStart = mapCoord2Real(start.x(), start.y());
    const QPointF realEnd = mapCoord2Real(end.x(), end.y());

    QPen pen(color);
    pen.setWidth(width);
    m_scene->addLine(realStart.x(), realStart.y(), realEnd.x(), realEnd.y(), pen);

    const static int arrowWidth = 20;
    const static int arrowHeight = 10;

    if (hasArrow)
    {
        QBrush brush;
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);

        QPolygonF polygon;
        polygon.push_back(QPointF(-arrowWidth, arrowHeight * 0.5));
        polygon.push_back(QPointF(-arrowWidth, -arrowHeight * 0.5));
        polygon.push_back(QPointF(0, 0));

        auto item = m_scene->addPolygon(polygon, pen, brush);

        qreal angle = QLineF(realEnd.x(), realEnd.y(), realStart.x(), realStart.y())
                .angleTo(QLineF(realEnd.x(), realEnd.y(), realEnd.x() - 1, realEnd.y()));

        item->setRotation(angle);
        item->setPos(realEnd.x(), realEnd.y());
    }
}

void MainWindow::addText(const QPointF &pos, const QString &str, MainWindow::TextPos textPos, const QColor &color, int size)
{
    const QPointF realPos = mapCoord2Real(pos.x(), pos.y());

    QFont font;
    font.setFamily(FONT_FAMILY);
    font.setPointSize(size);

    QFontMetrics fm(font);

    const int textWidth = fm.width(str);
    const int textHeight = fm.height();

    QPointF delta(0, 0);

    if (textPos & Top)
    {
        delta.setY(-textHeight);
    }
    else if (textPos & Bottom)
    {
        delta.setY(0);
    }
    else
    {
        delta.setY(-textHeight * 0.5);
    }

    if (textPos & Left)
    {
        delta.setX(-textWidth);
    }
    else if (textPos & Right)
    {
        delta.setX(0);
    }
    else
    {
        delta.setX(-textWidth * 0.5);
    }

    auto item = m_scene->addText(str, font);
    item->setPos(realPos.x() + delta.x(), realPos.y() + delta.y());
    item->setDefaultTextColor(color);
}
