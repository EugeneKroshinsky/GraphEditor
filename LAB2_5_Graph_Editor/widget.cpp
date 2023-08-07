//Крошинский Евгений 7 группа РЕДАКТОР ГРАФОВ
#include "widget.h"
#include "ui_widget.h"
#define FIGURE_SIZE 30

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) //главный конструктор окна
{
    ui->setupUi(this);

    dialog.setModal(true);
    dialog.show();

    scene = new QGraphicsScene(geometry(),this);
    ui->graphicsView->setScene(scene);
    scene->addRect(scene->sceneRect());
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event) //добавление вершины
{
    Node *buf = new Node(event->x(), event->y()+45, this);
    allNodes.push_back(buf);
    scene->addItem(buf);
    buf = nullptr;
    ui->label_3->setNum(allNodes.size());
}

Node::Node(int mx, int my, Widget *ptr) //конструктор вершины
{
    wPtr = ptr;
    setPos(mx, my),
    setRect(0,0,FIGURE_SIZE,FIGURE_SIZE);
    QColor color = QColor(Qt::red);
    setBrush(color);
    QPen pen(color);
    setPen(pen);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event) //добавление ребра
{
    QColor color = QColor(Qt::darkRed);
    setBrush(color);
    if(wPtr->bufId<0)
    {
        for(int i =0; i<wPtr->allNodes.size(); i++)
            if(wPtr->allNodes[i]==this)
                wPtr->bufId = i;
        wPtr->bufX = this->scenePos().rx()+FIGURE_SIZE/2;
        wPtr->bufY = this->scenePos().ry()+FIGURE_SIZE/2;
    }
    else
    {
        Edge *buf = new Edge(wPtr->bufX, wPtr->bufY, this->scenePos().rx()+FIGURE_SIZE/2, this->scenePos().ry()+FIGURE_SIZE/2,
                             wPtr, this,  wPtr->allNodes[wPtr->bufId]);
        wPtr->allEdges.push_back(buf);

        wPtr->ui->label_4->setNum(wPtr->allEdges.size());
        wPtr->scene->addItem(buf);
        color = QColor(Qt::red);
        wPtr->allNodes[wPtr->bufId]->setBrush(color);
        setBrush(color);
        wPtr->bufId = -1;
    }
}

void Node::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) //удаление вершины
{
    //удаление смежных рёбер
    int n = wPtr->allEdges.size();
    for(int i=0; i<n; i++)
    {
        if(wPtr->allEdges[i]->node1 == this)
        {
            delete wPtr->allEdges[i];
            wPtr->allEdges.erase(wPtr->allEdges.begin()+i);
            i--;
            n--;
            continue;
        }
        if(wPtr->allEdges[i]->node2==this)
        {
            delete wPtr->allEdges[i];
            wPtr->allEdges.erase(wPtr->allEdges.begin()+i);
            i--;
            n--;
        }
    }
    //удаление вершины
    auto iter = wPtr->allNodes.begin();
    for(int i =0; i<wPtr->allNodes.size(); i++)
    {
        if(wPtr->bufId==i)
            wPtr->bufId = -1;

        if(this==wPtr->allNodes[i])
        {
            wPtr->allNodes.erase(iter);
            break;
        }
        else iter++;
    }

    wPtr->ui->label_3->setNum(wPtr->allNodes.size());
    wPtr->ui->label_4->setNum(wPtr->allEdges.size());
    delete this;
}

Edge::Edge(int mx1, int my1, int mx2, int my2, Widget* ptr, Node *node1, Node *node2) : QGraphicsLineItem(mx1, my1, mx2, my2, this) //конструктор ребра
{
    wPtr = ptr;
    QColor color = QColor(Qt::red);
    QPen pen(color);
    pen.setWidth(10);
    setPen(pen);
    this->node1 = node1;
    this->node2 = node2;
}

void Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) //удаляем ребро
{
    auto iter = wPtr->allEdges.begin();
    for(int i =0; i<wPtr->allEdges.size(); i++)
    {
        if(this==wPtr->allEdges[i])
        {
            wPtr->allEdges.erase(iter);
            break;
        }
        else iter++;
    }
    node1 = nullptr;
    node2 = nullptr;
    delete this;
    wPtr->ui->label_4->setNum(wPtr->allEdges.size());
}

void Widget::on_pushButton_clicked() //очищаем всё через кнопку
{
    scene->clear();
    allEdges.clear();
    allNodes.clear();
    ui->label_4->setNum(allEdges.size());
    ui->label_3->setNum(allNodes.size());
}

void Widget::on_pushButton_2_clicked() //вызываем диалоговое модальное окно
{
    dialog.show();
}
