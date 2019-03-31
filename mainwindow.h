#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QColor>
#include <QGraphicsScene>

namespace Ui {
class MainWindow;
}
struct DiffResult;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_diffButton_clicked();
private:
    enum TextPos
    {
        Top = 1,
        Bottom = 1 << 1,
        Left = 1 << 2,
        Right = 1 << 3,
        TopLeft = Top | Left,
        TopRight = Top | Right,
        BottomLeft = Bottom | Left,
        BottomRight = Bottom | Right
    };
private:
    inline QPoint mapCoord2Real(int x, int y)
    {
        return QPoint(x * GRID_UNIT, y * GRID_UNIT);
    }
    void drawGrid(const QString &from, const QString &to);
    void drawDiagonal(const QString &from, const QString &to);
    void drawVisited(const DiffResult &result);
    void drawDLine(const DiffResult &result);
    void drawAnswer(const DiffResult &result);
    void addLine(const QPoint &start, const QPoint &end, const QColor &color = Qt::white, int width = 1, bool hasArrow = false);
    void addText(const QPoint &pos, const QString &str, TextPos textPos, const QColor &color = Qt::white, int size = TEXT_SIZE);
private:
    static const int GRID_UNIT = 80;
    static const int TEXT_SIZE = 20;
    static const QString FONT_FAMILY;
    int m_charWidth;
    int m_charHeight;
    Ui::MainWindow *ui;
    QGraphicsScene *m_scene;
};

#endif // MAINWINDOW_H
