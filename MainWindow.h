#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void ready();

private:
	Ui::MainWindow *ui;
	QGraphicsScene *scene;
};


class DragCircle: public QGraphicsEllipseItem
{
public:
	explicit DragCircle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};


class DragRect : public QGraphicsRectItem
{
public:
	explicit DragRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent = 0);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
};

class ObservableRect : public QGraphicsRectItem
{
public:
	explicit ObservableRect(const QRectF &rect, QGraphicsItem *parent = 0);

private:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

	void forMMKiselev(int number, const QPointF &pos);

	QPointF previousPosition;
};


class DropScene : public QGraphicsScene
{
	Q_OBJECT
public:
	void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
	void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) override;
	void dropEvent(QGraphicsSceneDragDropEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private:
	void addCircle(const QPointF &pos);
	void addRect(const QPointF &pos);
};

#endif // MAINWINDOW_H
