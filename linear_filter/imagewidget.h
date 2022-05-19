#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDebug>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    QPainter* painter;
    QImage* img_ptr;

    explicit ImageWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void set_image(QImage& img);
    QImage invers_image(QImage img);
    QImage grey(QImage img, int up = 255, int down = 0);
    QImage log_brightness_filter(QImage img, int up = 255, int down = 0);
    QImage degree_brightness_filter(QImage img, int up = 255, int down = 0);
    QImage line_brightness_filter(QImage img, int up = 255, int down = 0);
    QImage equalization_filter(QImage img, int up = 255, int down = 0);
    QImage gausse_filter(QImage img, int mat = 100, int dis = 114, int up = 255, int down = 0);
signals:

};

#endif // IMAGEWIDGET_H
