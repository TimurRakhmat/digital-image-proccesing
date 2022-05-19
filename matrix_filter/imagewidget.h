#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <stdlib.h>
#include <complex>

const double M_PI = acos(-1);

using namespace std;

typedef complex<double> cd;
typedef vector<cd> vcd;

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    QPainter* painter;
    QImage* img_ptr;

    explicit ImageWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void set_image(QImage& img);


    void box_filter(QImage img);
    void median_filter(QImage img);
    void gausse_filter(QImage img);

    void sharpness_filter(QImage img);
    void laplas_filter(QImage img);
    void sobel_filter(QImage img);

    void fourier_transform(QImage img, bool flag_fast=false);

signals:

};

#endif // IMAGEWIDGET_H
