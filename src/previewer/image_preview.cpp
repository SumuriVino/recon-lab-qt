#include "image_preview.h"
#include "ui_image_preview.h"

image_preview::image_preview(QWidget *parent) :
    QGraphicsView(parent),
    ui(new Ui::image_preview)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Window);


    scene = new QGraphicsScene(this);
    pixmap_item = new QGraphicsPixmapItem();
}

image_preview::~image_preview()
{
    delete ui;
}

void image_preview::set_image_by_path(QString image_path)
{
    //  current_image_path = image_path;


    if(scene != NULL)
    {
        delete pixmap_item;
        delete scene;
    }


    scene = new QGraphicsScene(this);
    pixmap_item = new QGraphicsPixmapItem();

    QPixmap mpix(image_path);
    m_pixmap = mpix;

    int hh = mpix.size().height();
    int ww = mpix.size().width();

    int position = ui->horizontalSlider->sliderPosition();

    pixmap_item->setPixmap(QPixmap(image_path));

    float ratio = (float)ww / hh;

    QTransform matrix;
    matrix.scale((position*10*ratio)/pixmap_item->boundingRect().width(), (position*10)/pixmap_item->boundingRect().height());

    pixmap_item->setTransform(matrix);
    scene->addItem(pixmap_item);

    setScene(scene);

}

void image_preview::set_image_by_pixmap(QPixmap pix_map)
{
    m_pixmap = pix_map;

    if(scene != NULL)
    {
        delete pixmap_item;
        delete scene;
    }

    scene = new QGraphicsScene(this);
    pixmap_item = new QGraphicsPixmapItem();


    int hh = pix_map.size().height();
    int ww = pix_map.size().width();

    int position = ui->horizontalSlider->sliderPosition();

    pixmap_item->setPixmap(pix_map);

    float ratio = (float)ww / hh;

    QTransform matrix;
    matrix.scale((position*10*ratio)/pixmap_item->boundingRect().width(), (position*10)/pixmap_item->boundingRect().height());


    pixmap_item->setTransform(matrix);
    scene->addItem(pixmap_item);

    setScene(scene);

}

void image_preview::set_image_for_detach_window(QString image_path)
{
    // current_image_path = image_path;

//    this->scene->clear();


    if(scene != NULL)
    {
        delete pixmap_item;
        delete scene;
    }

    scene = new QGraphicsScene(this);
    pixmap_item = new QGraphicsPixmapItem();


    if(image_path != "Image Nahi Mili")
        pixmap_item->setPixmap(QPixmap(image_path));

    QPixmap mpix(image_path);
    m_pixmap = mpix;


    int hh = mpix.size().height();
    int ww = mpix.size().width();

    int position = ui->horizontalSlider->sliderPosition();

    float ratio = (float)ww / hh;

    QTransform matrix;
    matrix.scale((position*10*ratio)/pixmap_item->boundingRect().width(), (position*10)/pixmap_item->boundingRect().height());


    pixmap_item->setTransform(matrix);
    scene->addItem(pixmap_item);

    setScene(scene);

}

void image_preview::on_horizontalSlider_sliderMoved(int position)
{
    //delete this->scene;


    if(scene != NULL)
    {
        delete pixmap_item;
        delete scene;
    }

    scene = new QGraphicsScene(this);
    pixmap_item = new QGraphicsPixmapItem();


    // pixmap_item->setPixmap(QPixmap(current_image_path));
    pixmap_item->setPixmap(m_pixmap);

    //    QPixmap mpix(current_image_path);

    //    int hh = mpix.size().height();
    //    int ww = mpix.size().width();

    int hh = m_pixmap.size().height();
    int ww = m_pixmap.size().width();

    float ratio = (float)ww / hh;

    QTransform matrix;
    matrix.scale((position*10*ratio)/pixmap_item->boundingRect().width(), (position*10)/pixmap_item->boundingRect().height());


    pixmap_item->setTransform(matrix);
    scene->addItem(pixmap_item);

    setScene(scene);

}

void image_preview::on_horizontalSlider_valueChanged(int value)
{
    on_horizontalSlider_sliderMoved(value);
}
