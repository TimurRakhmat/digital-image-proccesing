#include "imagewidget.h"

ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent)
{
    painter = new QPainter(this);
}

void ImageWidget::paintEvent(QPaintEvent *event)
{
    painter->begin(this);
    if (img_ptr)
    {
        painter->drawImage(0, 0, *img_ptr);
    }
    painter->end();
}

void ImageWidget::set_image(QImage& img)
{
    img_ptr = &img;
}

QImage ImageWidget::invers_image(QImage img)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();

    int pixelsQuo = h * w;
    int r, g, b;
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;
        px -= 2;
        *px = 255 - r;
        *++px = 255 - g;
        *++px = 255 - b;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}

QImage ImageWidget::grey(QImage img, int up, int down)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();

    int pixelsQuo = h * w;
    int r, g, b;
    int lightness = 0;
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;
        lightness = 0.3*r+0.59*g+0.11*b;
        if (lightness > up)
            lightness = 255;
        else if (lightness < down)
            lightness = 0;
        px -= 2;
        *px = lightness;
        *++px = lightness;
        *++px = lightness;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}

QImage ImageWidget::log_brightness_filter(QImage img, int up, int down)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();
    double c = 255/log(256);

    int pixelsQuo = h * w;
    int r, g, b;
    double y, u, v;
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        y =  c*log(1 + y);

        if (y > up)
            y = 255;
        else if (y < down)
            y = 0;

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;
        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        if (r < 0)
            r = 0;
        if (b < 0)
            b = 0;
        if (g < 0)
            g = 0;

        px -= 2;
        *px = r;
        *++px = g;
        *++px = b;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}

QImage ImageWidget::degree_brightness_filter(QImage img, int up, int down)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();
    double c = 255/pow(255, 1.25);

    int pixelsQuo = h * w;
    int r, g, b;
    double y, u, v;
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        y =  c*pow(y, 1.25);
        if (y > up)
            y = 255;
        else if (y < down)
            y = 0;

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;
        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        if (r < 0)
            r = 0;
        if (b < 0)
            b = 0;
        if (g < 0)
            g = 0;

        px -= 2;
        *px = r;
        *++px = g;
        *++px = b;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}

QImage ImageWidget::line_brightness_filter(QImage img, int up, int down)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();
    int c1 = 80, p1 = 60;
    int c2 = 180, p2 = 220;

    double q1 = (double)p1/c1;
    double q2 = (double)(255-p2)/(255-c2);
    double q3 = (double)(p2-p1)/(c2-c1);

    int pixelsQuo = h * w;
    int r, g, b;
    double y, u, v;
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        if (y < c1)
        {
            y *= q1;
        }
        else if (y > c2)
        {
            y = (double)(y - c2)*q2 + p2;
        }
        else
        {
            y = (double)(y - c1)*q3 + p1;
        }

        if (y > up)
            y = 255;
        else if (y < down)
            y = 0;

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;

        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        if (r < 0)
            r = 0;
        if (b < 0)
            b = 0;
        if (g < 0)
            g = 0;

        px -= 2;
        *px = r;
        *++px = g;
        *++px = b;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}

QImage ImageWidget::equalization_filter(QImage img, int up, int down)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();

    int pixelsQuo = h * w;
    unsigned long long histogram_data[256];
    memset(histogram_data, 0, sizeof(unsigned long long) * 256);

    int r, g, b;
    double y, u, v;

    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;

        histogram_data[(int)y]++;
        px += 2;
    }

    unsigned long long histogram_data_transform[256];
    memset(histogram_data_transform, 0, sizeof(unsigned long long) * 256);

    long long nlightness = 0;
    for (auto p = histogram_data, pt = histogram_data_transform; p < histogram_data + 256; ++p, ++pt)
    {
        nlightness = 0;
        for (auto pp = histogram_data; pp <= p; ++pp)
        {
            nlightness += *pp;
        }
        *pt = (double)nlightness/pixelsQuo*255;
    }

    px = img.bits();
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

        y = histogram_data_transform[(int)y];

        if (y > up)
            y = 255;
        else if (y < down)
            y = 0;

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;

        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        if (r < 0)
            r = 0;
        if (b < 0)
            b = 0;
        if (g < 0)
            g = 0;

        px -= 2;
        *px = r;
        *++px = g;
        *++px = b;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}

QImage ImageWidget::gausse_filter(QImage img, int mat, int dis, int up, int down)
{
    uchar *px = img.bits();

    int h = img.height();
    int w = img.width();

    int pixelsQuo = h * w;
    unsigned long long histogram_data[256];
    memset(histogram_data, 0, sizeof(unsigned long long) * 256);

    int r, g, b;
    double y, u, v;

    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;

        histogram_data[(int)y]++;
        px += 2;
    }

    unsigned long long histogram_data_transform[256];
    memset(histogram_data_transform, 0, sizeof(unsigned long long) * 256);

    long long nlightness = 0;
    for (auto p = histogram_data, pt = histogram_data_transform; p < histogram_data + 256; ++p, ++pt)
    {
        nlightness = 0;
        for (auto pp = histogram_data; pp <= p; ++pp)
        {
            nlightness += *pp;
        }
        *pt = (double)nlightness/pixelsQuo*255;
    }

    unsigned long long histogram_data_transform_final[256];
    memset(histogram_data_transform_final, 0, sizeof(unsigned long long) * 256);

    int z0 = mat;
    int m = dis;
    if (2*dis - mat > 255 || 2*dis - mat < 0)
        m = (128 + z0) / 2;

    double g1 = 255 + z0 - 2*m;
    g1 /= 255*z0;

    double g2 = 2*m - z0;
    g2 /= 255*(255 - z0);

    double k = 1;

    for (auto p = histogram_data_transform, pt = histogram_data_transform_final; p < histogram_data_transform + 256; ++p, ++pt)
    {
        k = (double)*p / g1 / 255;

        if (k < z0)
            *pt = k;
        else
        {
            *pt = (double)((double)*p / 255 - z0*(g1-g2)) / g2;
        }
    }

    px = img.bits();
    for (int i = 0; i < pixelsQuo; i++)
    {
        r = *px;
        g = *++px;
        b = *++px;

        y = 0.2126*r + 0.7152*g + 0.0722*b;
        u = -0.0999*r - 0.3360*g + 0.4360*b;
        v = 0.6150*r - 0.5586*g - 0.0563*b;

//        k = (double)histogram_data_transform[(int)y] / g1 / 255;    вот это место было оптимизированно...

//        if (k < z0)
//            y = k;
//        else
//        {
//            y = (double)histogram_data_transform[(int)y] / 255 - z0*(g1-g2);
//            y /= g2;
//        }

        y = histogram_data_transform_final[(int)y];

        if (y > up)
            y = 255;
        else if (y < down)
            y = 0;

        r = y + 1.2803*v;
        g = y - 0.2148*u - 0.3805*v;
        b = y + 2.1279*u;

        if (r > 255)
            r = 255;
        if (g > 255)
            g = 255;
        if (b > 255)
            b = 255;

        if (r < 0)
            r = 0;
        if (b < 0)
            b = 0;
        if (g < 0)
            g = 0;

        px -= 2;
        *px = r;
        *++px = g;
        *++px = b;
        ++px;
        px++;
    }

    img_ptr = new QImage(img);

    return img;
}
