//Крошинский Евгений 7 группа РЕДАКТОР ГРАФОВ
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QVector>
#include <QGraphicsLineItem>
#include <QPair>
#include <iterator>
#include <dialog.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Node;
class Edge;

class Widget : public QWidget //класс окна
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
private:
    Ui::Widget *ui;
    Dialog dialog;
    QGraphicsScene *scene;

    friend class Edge;
    friend class Node;

    QVector<Node*> allNodes; //вектор всех вершин
    QVector<Edge*> allEdges; //вектор всех рёбер
    int bufX, bufY, bufId = -1;
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

class Node : public QGraphicsEllipseItem //класс вершины
{
public:
    Node(int mx, int my, Widget *ptr);
    ~Node(){}
    Widget *wPtr;
protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    friend class Widget;
};

class Edge : public QGraphicsLineItem //класс ребра
{
public:
    Edge(int mx1, int my1, int mx2, int my2, Widget *ptr, Node *node1, Node *node2);
    // QGraphicsItem interface
protected:
    friend class Node;
    Widget *wPtr;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    Node *node1;
    Node *node2;
};

#endif // WIDGET_H
