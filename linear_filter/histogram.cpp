#include "histogram.h"

Histogram::Histogram()
{
    this->setFixedWidth(516);
    this->setFixedHeight(516);
    pixelsQuo = 1;
    memset(histogram_data, 0, sizeof(unsigned long long) * 256);
    memset(red_histogram, 0, sizeof(unsigned long long) * 256);
    memset(green_histogram, 0, sizeof(unsigned long long) * 256);
    memset(blue_histogram, 0, sizeof(unsigned long long) * 256);

}

void Histogram::do_histogram(QImage &img, bool is_mono)
{
    memset(histogram_data, 0, sizeof(unsigned long long) * 256);
    memset(red_histogram, 0, sizeof(unsigned long long) * 256);
    memset(green_histogram, 0, sizeof(unsigned long long) * 256);
    memset(blue_histogram, 0, sizeof(unsigned long long) * 256);

    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();

    pixelsQuo = h * w;

    int r, g, b;
    int lightness = 0;
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;
        px++;
        if (is_mono)
            lightness = r;
        else
            lightness = 0.2126*r + 0.7152*g + 0.0722*b;
        red_histogram[r]++;
        blue_histogram[b]++;
        green_histogram[g]++;
        histogram_data[lightness]++;
    }

}

void Histogram::paintEvent(QPaintEvent *event)
{
    painter.begin(&picture);
    painter.setPen(Qt::black);
    int margin = 4;
    int hh = (height()-margin) / 2;
    int ww = (width()-margin) / 2;
    int maxWidth = 256;

    painter.setPen(Qt::magenta);
    painter.drawLine(0, height(), width(), height());
    painter.drawLine(0, height()/2, width(), height()/2);
    painter.drawLine(0, 0, 0, height());
    painter.drawLine(width()/2, 0, width()/2, height());


    //ligtness
    painter.setPen(Qt::black);
    int maxH = 1;
    for (auto p = histogram_data; p < histogram_data + maxWidth; ++p)
    {
        if (maxH < *p)
        {
            maxH = *p;
        }
    }
    for (auto p = histogram_data; p < histogram_data + maxWidth; ++p)
    {
        unsigned long long h = ((double)*p/maxH * hh);
        painter.drawLine(p - histogram_data, hh, p - histogram_data, hh - h);
    }

    //red
    painter.setPen(Qt::red);
    maxH = 1;
    for (auto p = red_histogram; p < red_histogram + maxWidth; ++p)
    {
        if (maxH < *p)
        {
            maxH = *p;
        }
    }

    for (auto p = red_histogram; p < red_histogram + maxWidth; ++p)
    {
        unsigned long long h = ((double)*p*6/maxH * hh);
        if (h > hh)
            h = hh;
        painter.drawLine(p - red_histogram + ww, hh, p - red_histogram + ww, hh - h);
    }

    //green
    painter.setPen(Qt::green);
    maxH = 1;
    for (auto p = green_histogram; p < green_histogram + maxWidth; ++p)
    {
        if (maxH < *p)
        {
            maxH = *p;
        }
    }
    for (auto p = green_histogram; p < green_histogram + maxWidth; ++p)
    {
        unsigned long long h = ((double)*p*6/maxH * hh);
        if (h > hh)
            h = hh;
        painter.drawLine(p - green_histogram, 2*hh, p - green_histogram, 2*hh - h);
    }

    //blue
    painter.setPen(Qt::blue);
    maxH = 1;
    for (auto p = blue_histogram; p < blue_histogram + maxWidth; ++p)
    {
        if (maxH < *p)
        {
            maxH = *p;
        }
    }
    for (auto p = blue_histogram; p < blue_histogram + maxWidth; ++p)
    {
        unsigned long long h = ((double)*p*6/maxH * hh);
        if (h > hh)
            h = hh;
        painter.drawLine(p - blue_histogram + ww, 2*hh, p - blue_histogram + ww, 2*hh - h);
    }


    painter.end();
    painter.begin(this);
    painter.drawPicture(0, 0, picture);
    painter.end();
}
