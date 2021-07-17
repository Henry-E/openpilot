#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QWidget>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QDebug>

#include "selfdrive/hardware/hw.h"
#include "selfdrive/ui/qt/util.h"
#include "selfdrive/ui/qt/qt_window.h"
#include "selfdrive/ui/qt/widgets/scrollview.h"
#include "selfdrive/ui/qt/kb_test.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
  QHBoxLayout *main_layout = new QHBoxLayout(this);
//  installEventFilter(this);

  QPushButton *btn1 = new QPushButton("Btn 1");
  btn1->setAttribute(Qt::WA_AcceptTouchEvents);
//  btn1->installEventFilter(this);
  main_layout->addWidget(btn1);

  QPushButton *btn2 = new QPushButton("Btn 2");
  btn2->setAttribute(Qt::WA_AcceptTouchEvents);
//  btn2->installEventFilter(this);
  main_layout->addWidget(btn2);

  QPushButton *btn3 = new QPushButton("Btn 3");
  btn3->setAttribute(Qt::WA_AcceptTouchEvents);
//  btn3->installEventFilter(this);
  main_layout->addWidget(btn3);

  QPushButton *btn4 = new QPushButton("Btn 4");
  btn4->setAttribute(Qt::WA_AcceptTouchEvents);
//  btn4->installEventFilter(this);
  main_layout->addWidget(btn4);

  setStyleSheet(R"(
    * {
      outline: none;
      color: white;
      background-color: black;
    }
    QPushButton {
      height: 135px;
      font-size: 75px;
      margin: 0px;
      padding: 0px;
      border-radius: 10px;
      color: #dddddd;
      background-color: #444444;
    }
    QPushButton:pressed {
      background-color: #333333;
    }
  )");
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  qDebug() << "EVENT:" << event;
//  qDebug() << "EVENT type:" << event->type();
//  if (event->type() == QEvent::TouchBegin) {

  if (event->type() == QEvent::TouchBegin || event->type() == QEvent::TouchUpdate || event->type() == QEvent::TouchEnd || event->type() == QEvent::TouchCancel) {
    QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
    for (const QTouchEvent::TouchPoint &tp : touchEvent->touchPoints()) {
      Qt::MouseButton button = Qt::LeftButton;
      Qt::MouseButtons buttons = Qt::LeftButton;
      Qt::KeyboardModifiers modifiers;
      QPointF localPos;
      QPointF screenPos;
      localPos.setX(tp.startPos().x());
      localPos.setY(tp.startPos().y());
      screenPos.setX(tp.screenPos().x());
      screenPos.setY(tp.screenPos().y());
      if (tp.state() == Qt::TouchPointPressed) {
        QMouseEvent eventNew(QEvent::MouseButtonPress, localPos, screenPos, button, buttons, modifiers);  // TODO fix local pos if needed
        QApplication::sendEvent(obj, &eventNew);
        qDebug() << "Sent mouse press with pos:" << localPos;
      } else if (tp.state() == Qt::TouchPointReleased) {
        QMouseEvent eventNew(QEvent::MouseButtonRelease, localPos, screenPos, button, buttons, modifiers);
        QApplication::sendEvent(obj, &eventNew);
        qDebug() << "Sent mouse release with pos:" << localPos;
      }
    }
    event->accept();
    return true;
  } else {
    return QWidget::eventFilter(obj, event);
  }
}


int main(int argc, char *argv[]) {
  initApp();
  QApplication a(argc, argv);
  MainWindow window;
  setMainWindow(&window);

  a.installEventFilter(&window);
  a.setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents, false);
  window.setAttribute(Qt::WA_AcceptTouchEvents);

  return a.exec();
}