#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QDrag>
#include <QMimeData>
#include <QGraphicsSceneDragDropEvent>
#include <QKeyEvent>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	setWindowState(Qt::WindowFullScreen);
	ui->setupUi(this);
	connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ready);
	scene = new DropScene();
	ui->graphicsView->setAcceptDrops(true);
	ui->graphicsView->setScene(scene);
	ui->graphicsView->setAttribute(Qt::WA_AcceptTouchEvents);
	ui->graphicsView_2->setAttribute(Qt::WA_AcceptTouchEvents);
	auto scene2 = new QGraphicsScene();
	ui->graphicsView_2->setScene(scene2);
	auto c = new DragCircle(0, 0, 100, 100);
	auto r = new DragRect(0, 150, 100, 100);
	scene2->addItem(c);
	scene2->addItem(r);

	scene->setBackgroundBrush(QBrush(QImage("/home/dvvrd/Desktop/wKHV3zvi2L8.jpg")));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::ready()
{
	ui->pushButton->hide();
	ui->graphicsView_2->hide();
	for (auto item : ui->graphicsView->scene()->items()) {
		item->setFlag(QGraphicsItem::ItemIsMovable, false);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		item->setFlag(QGraphicsItem::ItemIsFocusable, true);
	}
}

DragCircle::DragCircle(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
	: QGraphicsEllipseItem(x, y, w, h, parent)
{
}

void DragCircle::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	QDrag *drag = new QDrag(this->scene());
	QMimeData *data = new QMimeData();
	data->setText("circle");
	drag->setMimeData(data);
	drag->start();
}

DragRect::DragRect(qreal x, qreal y, qreal w, qreal h, QGraphicsItem *parent)
	: QGraphicsRectItem(x, y, w, h, parent)
{
}

void DragRect::mousePressEvent(QGraphicsSceneMouseEvent *)
{
	QDrag *drag = new QDrag(this->scene());
	QMimeData *data = new QMimeData();
	data->setText("rect");
	drag->setMimeData(data);
	drag->start();
}

void DropScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
	} else {
		QGraphicsScene::dragEnterEvent(event);
	}
}

void DropScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
	} else {
		QGraphicsScene::dragMoveEvent(event);
	}
}

void DropScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
	} else {
		QGraphicsScene::dragLeaveEvent(event);
	}
}

void DropScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasText() && (event->mimeData()->text() == "circle" || event->mimeData()->text() == "rect")) {
		event->acceptProposedAction();
		event->accept();
		if (event->mimeData()->text() == "circle") {
			addCircle(event->scenePos());
		} else {
			addRect(event->scenePos());
		}
	} else {
		QGraphicsScene::dropEvent(event);
	}

}

void DropScene::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Delete) {
		QList<QGraphicsItem *> items = selectedItems();
		for (auto item : items) {
			delete item;
		}
	} else {
		QGraphicsScene::keyPressEvent(event);
	}
}

void DropScene::addCircle(const QPointF &pos)
{
	int idx = 1;
	while (true) {
		bool exists = false;
		for (auto i : items()) {
			if (dynamic_cast<QGraphicsEllipseItem*>(i)) {
				if (i->data(Qt::UserRole).toInt() == idx) {
					exists = true;
					break;
				}
			}
		}
		if (exists) ++idx; else break;
	}
	QSizeF size(100, 100);
	auto item = new QGraphicsEllipseItem(QRectF(QPointF(), size));
	item->setData(Qt::UserRole, idx);
	auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
	number->setFlag(QGraphicsItem::ItemIsSelectable, false);
	number->setPos((item->rect().width() - number->boundingRect().width())/2, (item->rect().height() - number->boundingRect().height())/2);
	item->setPos(pos.x() - size.width()/2, pos.y() - size.height()/2);
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	addItem(item);
}

void DropScene::addRect(const QPointF &pos)
{
	int idx = 1;
	while (true) {
		bool exists = false;
		for (auto i : items()) {
			if (dynamic_cast<QGraphicsRectItem*>(i)) {
				if (i->data(Qt::UserRole).toInt() == idx) {
					exists = true;
					break;
				}
			}
		}
		if (exists) ++idx; else break;
	}
	QSizeF size(100, 100);
	auto item = new ObservableRect(QRectF(QPointF(), size));
	item->setData(Qt::UserRole, idx);
	auto number = new QGraphicsTextItem(QString("%0").arg(idx), item);
	number->setFlag(QGraphicsItem::ItemIsSelectable, false);
	number->setPos((item->rect().width() - number->boundingRect().width())/2, (item->rect().height() - number->boundingRect().height())/2);
	item->setPos(pos.x() - size.width()/2, pos.y() - size.height()/2);
	item->setFlag(QGraphicsItem::ItemIsMovable, true);
	item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	addItem(item);
}

ObservableRect::ObservableRect(const QRectF &rect, QGraphicsItem *parent)
	: QGraphicsRectItem(rect, parent)
{
}

void ObservableRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsRectItem::mousePressEvent(event);
	event->accept();
	previousPosition = event->scenePos();
}

void ObservableRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	event->accept();
	if (event->scenePos() == previousPosition && !(flags() & QGraphicsItem::ItemIsMovable)) {
		forMMKiselev(data(Qt::UserRole).toInt(), scenePos());
	}
}

#include <QMessageBox>
void ObservableRect::forMMKiselev(int number, const QPointF &pos)
{
	QMessageBox::information(nullptr, "hello", QString("%0: %1 %2").arg(number).arg(pos.x()).arg(pos.y()));
}
