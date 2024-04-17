#ifndef IMAGE_PREVIEW_H
#define IMAGE_PREVIEW_H

#include <QFrame>
#include <QtGui>
#include <QtCore>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


namespace Ui {
class image_preview;
}

class image_preview : public QGraphicsView
{
    Q_OBJECT
    
public:
    explicit image_preview(QWidget *parent = 0);
    ~image_preview();

    void set_image_by_path(QString image_path);
    void set_image_by_pixmap(QPixmap pix_map);

    void set_image_for_detach_window(QString image_path);
    
private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::image_preview *ui;

    QGraphicsScene *scene;
   // QString current_image_path;

    QGraphicsPixmapItem *pixmap_item;
    QPixmap m_pixmap;

};

#endif // IMAGE_PREVIEW_H
