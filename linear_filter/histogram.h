#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QPainter>
#include <QPicture>
#include <QImage>
#include <QDebug>

class Histogram : public QWidget
{
    Q_OBJECT
    long long pixelsQuo;
    QPainter painter;
    QPicture picture;
public:
    Histogram();
    void do_histogram(QImage& img, bool is_mono=false);
    void paintEvent(QPaintEvent* event);
    unsigned long long histogram_data[256];
    unsigned long long red_histogram[256];
    unsigned long long green_histogram[256];
    unsigned long long blue_histogram[256];
    int minX, maxX, xmini;
};

#endif // HISTOGRAM_H
