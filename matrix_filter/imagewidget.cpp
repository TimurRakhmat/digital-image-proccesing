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


void ImageWidget::box_filter(QImage img)
{
    uchar *px = img.bits();
    uchar *spx = img.bits();

    QImage* nimg = new QImage(img);
    uchar *pnx = nimg->bits();

    int h = img.height();
    int w = img.width();

    long long** integralPixTable = new long long*[h];
    for(auto ptr = integralPixTable; ptr < integralPixTable + h; ptr++)
    {
        *ptr = new long long[w];
    }

    int r, g, b;
    double y, u, v;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {

            r = *px;
            g = *++px;
            b = *++px;
            px+=2;
            y = 0.2126*r + 0.7152*g + 0.0722*b;
            long long temp = y;
            if (i != 0)
                temp += integralPixTable[i-1][j];
            if (j != 0)
                temp += integralPixTable[i][j-1];
            if (i != 0 && j != 0)
                temp -= integralPixTable[i-1][j-1];
            integralPixTable[i][j] = temp;
        }
    }
    px = spx;

    const int N = 5;
    int low_bound = N / 2;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = *px;
            g = *++px;
            b = *++px;

            u = -0.0999*r - 0.3360*g + 0.4360*b;
            v = 0.6150*r - 0.5586*g - 0.0563*b;

            int ai = low_bound, bi = low_bound, ci = low_bound, di = low_bound;
            int aj = low_bound, bj = low_bound, cj = low_bound, dj = low_bound;
            if (i + low_bound >= h)
            {
                ai = h - i - 1;
                ci = ai;

            }
            if (i - low_bound < 0)
            {
                di = i;
                bi = i;
            }
            if (j + low_bound >= w)
            {
                aj = w - j - 1;
                dj = aj;

            }
            if (j - low_bound < 0)
            {
                bj = j;
                cj = j;
            }
            int k = (ai + di) * (aj + cj);

            y = (integralPixTable[i+ai][j+aj] + integralPixTable[i-bi][j-bj] \
                    - integralPixTable[i+ci][j-cj] - integralPixTable[i-di][j+dj]);

            y /= k;

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

            px++;
            *pnx = r;
            *++pnx = g;
            *++pnx = b;
            *++pnx = *px;
            pnx++;
            px++;
        }
    }

    img_ptr = nimg;
    for(auto ptr = integralPixTable; ptr < integralPixTable + h; ptr++)
    {
        delete[] *ptr;
    }
    delete[] integralPixTable;
}


int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

void ImageWidget::median_filter(QImage img)
{
    uchar *px = img.bits();
    uchar *spx = img.bits();

    QImage* nimg = new QImage(img);
    uchar *pnx = nimg->bits();

    int h = img.height();
    int w = img.width();

    int r, g, b;
    double y, u, v;

    const int N = 3;
    int low_bound = N / 2;

    int *values = new int[N*N];

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = *px;
            g = *++px;
            b = *++px;

            u = -0.0999*r - 0.3360*g + 0.4360*b;
            v = 0.6150*r - 0.5586*g - 0.0563*b;

            int count = 0;
            int local_w, local_h;


            for (int k = 0;  k < N; k++)
            {
                for (int l = 0;  l < N; l++)
                {
                    local_h = (px - spx) / w / 4 - low_bound + k;
                    local_w = (px - spx) / 4 % w - low_bound + l;
                    if (!(local_w > -1 && local_w < w && local_h > -1 && local_h < h))
                        continue;
                    auto cpx = spx + local_h * w * 4 + local_w * 4;
                    r = *cpx;
                    g = *++cpx;
                    b = *++cpx;
                    y = 0.2126*r + 0.7152*g + 0.0722*b;
                    values[count++] = y;
                }
            }

            qsort (values, count, sizeof(int), compare);
            y = values[count / 2];

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

            px++;
            *pnx = r;
            *++pnx = g;
            *++pnx = b;
            *++pnx = *px;
            pnx++;
            px++;
        }
    }

    img_ptr = nimg;
}

void ImageWidget::gausse_filter(QImage img)
{
    const double PI = 4.0 * atan (1.0); // назначение Pi
    int radius = 4; // must be even: radius + 1 + radius - diametr
    int size = radius * 2 + 1;
    double **gaus=new double *[size];

    for (auto ptr = gaus; ptr < gaus + size; ptr++)
    {
        *ptr = new double[size];
    }


    int center = radius;
    double part = 1/(2*PI*radius*radius);
    double sum = 0;
    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            double degree = -(double)((i-center)*(i-center)+(j-center)*(j-center))/(2*radius*radius);
            gaus[i][j]=part*exp(degree);
            sum += gaus[i][j];
        }
    }
    double norm = 1 / sum;

    for(int i=0;i<size;++i){
        for(int j=0;j<size;++j){
            gaus[i][j] *= norm;
        }
    }

    uchar *px = img.bits();
    uchar *spx = img.bits();

    QImage* nimg = new QImage(img);
    uchar *pnx = nimg->bits();

    int h = img.height();
    int w = img.width();

    int r, g, b;
    double y, u, v;


    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = *px;
            g = *++px;
            b = *++px;

            u = -0.0999*r - 0.3360*g + 0.4360*b;
            v = 0.6150*r - 0.5586*g - 0.0563*b;

            double sum = 0;
            int local_w, local_h;
            for (int k = 0;  k < size; k++)
            {
                for (int l = 0;  l < size; l++)
                {
                    local_h = (px - spx) / w / 4 - radius + k;
                    local_w = (px - spx) / 4 % w - radius + l;
                    if (!(local_w > -1 && local_w < w && local_h > -1 && local_h < h))
                        continue;
                    auto cpx = spx + local_h * w * 4 + local_w * 4;
                    r = *cpx;
                    g = *++cpx;
                    b = *++cpx;
                    y = 0.2126*r + 0.7152*g + 0.0722*b;
                    sum += gaus[k][l] * y;
                }
            }


            y = sum;

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

            px++;
            *pnx = r;
            *++pnx = g;
            *++pnx = b;
            *++pnx = *px;
            pnx++;
            px++;
        }
    }

    for (auto ptr = gaus; ptr < gaus + size; ptr++)
    {
        delete[] *ptr;
    }
    delete[] gaus;

    img_ptr = nimg;
}

void ImageWidget::sharpness_filter(QImage img)
{
    uchar *px = img.bits();
    uchar *spx = img.bits();

    QImage* nimg = new QImage(img);
    uchar *pnx = nimg->bits();

    int h = img.height();
    int w = img.width();

    int r, g, b;
    double y, u, v;

    const int N = 3;
    int low_bound = N / 2;

    int transform_matrix[N][N] = { {-1, -2, -1}, {-2, 22, -2}, {-1, -2, -1}};
    double delimetr = (double)1/10;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = *px;
            g = *++px;
            b = *++px;

            u = -0.0999*r - 0.3360*g + 0.4360*b;
            v = 0.6150*r - 0.5586*g - 0.0563*b;

            double sum = 0;
            int local_w, local_h;
            for (int k = 0;  k < N; k++)
            {
                for (int l = 0;  l < N; l++)
                {
                    local_h = (px - spx) / w / 4 - low_bound + k;
                    local_w = (px - spx) / 4 % w - low_bound + l;
                    if (!(local_w > -1 && local_w < w && local_h > -1 && local_h < h))
                        continue;
                    auto cpx = spx + local_h * w * 4 + local_w * 4;
                    r = *cpx;
                    g = *++cpx;
                    b = *++cpx;
                    y = 0.2126*r + 0.7152*g + 0.0722*b;
                    sum += transform_matrix[k][l] * y;
                }
            }


            y = sum * delimetr;

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

            px++;
            *pnx = r;
            *++pnx = g;
            *++pnx = b;
            *++pnx = *px;
            pnx++;
            px++;
        }
    }

    img_ptr = nimg;
}

void ImageWidget::laplas_filter(QImage img)
{
    uchar *px = img.bits();
    uchar *spx = img.bits();

    QImage* nimg = new QImage(img);
    uchar *pnx = nimg->bits();

    int h = img.height();
    int w = img.width();

    int r, g, b;
    double y, u, v;

    const int N = 3;
    int low_bound = N / 2;

    int transform_matrix[N][N] = { {-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = *px;
            g = *++px;
            b = *++px;

            u = -0.0999*r - 0.3360*g + 0.4360*b;
            v = 0.6150*r - 0.5586*g - 0.0563*b;

            double sum = 0;
            int local_w, local_h;
            for (int k = 0;  k < N; k++)
            {
                for (int l = 0;  l < N; l++)
                {
                    local_h = (px - spx) / w / 4 - low_bound + k;
                    local_w = (px - spx) / 4 % w - low_bound + l;
                    if (!(local_w > -1 && local_w < w && local_h > -1 && local_h < h))
                        continue;
                    auto cpx = spx + local_h * w * 4 + local_w * 4;
                    r = *cpx;
                    g = *++cpx;
                    b = *++cpx;
                    y = 0.2126*r + 0.7152*g + 0.0722*b;
                    sum += transform_matrix[k][l] * y;
                }
            }


            y = y + sum;

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

            px++;
            *pnx = r;
            *++pnx = g;
            *++pnx = b;
            *++pnx = *px;
            pnx++;
            px++;
        }
    }

    img_ptr = nimg;
}

void ImageWidget::sobel_filter(QImage img)
{
    uchar *px = img.bits();
    uchar *spx = img.bits();

    QImage* nimg = new QImage(img);
    uchar *pnx = nimg->bits();

    int h = img.height();
    int w = img.width();

    int r, g, b;
    double y, u, v;

    const int N = 3;
    int low_bound = N / 2;

    int transform_matrix[N][N] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int transform_matrix_x[N][N] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            r = *px;
            g = *++px;
            b = *++px;

            u = -0.0999*r - 0.3360*g + 0.4360*b;
            v = 0.6150*r - 0.5586*g - 0.0563*b;

            double sum = 0;
            int local_w, local_h;
            for (int k = 0;  k < N; k++)
            {
                for (int l = 0;  l < N; l++)
                {
                    local_h = (px - spx) / w / 4 - low_bound + k;
                    local_w = (px - spx) / 4 % w - low_bound + l;
                    if (!(local_w > -1 && local_w < w && local_h > -1 && local_h < h))
                        continue;
                    auto cpx = spx + local_h * w * 4 + local_w * 4;
                    r = *cpx;
                    g = *++cpx;
                    b = *++cpx;
                    y = 0.2126*r + 0.7152*g + 0.0722*b;
                    sum += transform_matrix[k][l] * y + y * transform_matrix_x[k][l];
                }
            }


            y = sum;

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

            px++;
            *pnx = r;
            *++pnx = g;
            *++pnx = b;
            *++pnx = *px;
            pnx++;
            px++;
        }
    }

    img_ptr = nimg;
}


vcd fft(const vcd& as) {
    int n = as.size();
    int k = 0; // Длина n в битах
    while ((1 << k) < n) k++;
    vector<int> rev(n);
    rev[0] = 0;
    int high1 = -1;
    for (int i = 1; i < n; i++) {
        if ((i & (i - 1)) == 0) // Проверка на степень двойки. Если i ей является, то i-1 будет состоять из кучи единиц.
            high1++;
        rev[i] = rev[i ^ (1 << high1)]; // Переворачиваем остаток
        rev[i] |= (1 << (k - high1 - 1)); // Добавляем старший бит
    }

    vcd roots(n);
    for (int i = 0; i < n; i++) {
        double alpha = 2 * M_PI * i / n;
        roots[i] = cd(cos(alpha), sin(alpha));
    }

    vcd cur(n);
    for (int i = 0; i < n; i++)
        cur[i] = as[rev[i]];

    for (int len = 1; len < n; len <<= 1) {
        vcd ncur(n);
        int rstep = roots.size() / (len * 2);
        for (int pdest = 0; pdest < n;) {
            int p1 = pdest;
            for (int i = 0; i < len; i++) {
                cd val = roots[i * rstep] * cur[p1 + len];
                ncur[pdest] = cur[p1] + val;
                ncur[pdest + len] = cur[p1] - val;
                pdest++, p1++;
            }
            pdest += len;
        }
        cur.swap(ncur);
    }
    return cur;
}

void ImageWidget::fourier_transform(QImage img, bool fast_flag)
{
    int h = img.height();
    int w = img.width();

    int ** fxy = new int*[h];
    for (auto ffxy = fxy; ffxy < fxy + h; ffxy++)
    {
        *ffxy = new int[w];
    }

    uchar* px = img.bits();
    double y;
    for (int i = 0; i < h; i++)
    {
        for (int j =0; j < w; j++)
        {
            //y = 0.2126*r + 0.7152*g + 0.0722*b;
            y = 0;
            y += 0.2126*(*px);
            y += 0.7152*(*++px);
            y += 0.0722*(*++px);
            px+=2;
            if ((i + j) % 2) // to center
                y *= -1;
            fxy[i][j] = y;
        }
    }

    int ** Fxy = new int*[h];
    for (auto fFxy = Fxy; fFxy < Fxy + h; fFxy++)
    {
        *fFxy = new int[w];
    }



    if (fast_flag)
    {
        int k = 0;
        while ((1 << k) < (w)) k++;
        vcd a(1 << (k));

        int k2 = 0;
        while ((1 << k2) < (h)) k2++;
        vcd b(1 << (k2));

        auto ptr = fxy;
        vector<vcd> box((1<<k2));
        for (int i = 0; i < h; i++)
        {

            auto pptr = *ptr;
            auto vecptr = a.begin();
            for (int y = 0; y < w; y++)
            {
                *vecptr = *pptr;
                pptr++;
                vecptr++;
            }
            ptr++;
            for (auto vecpptr = vecptr; vecpptr != a.end(); vecpptr++)
                *vecpptr = 0;
            auto res1d = fft(a);
            box[i] = res1d;
        }

        for (int i = 0; i < h; i++)
        {
            auto vecptr = b.begin();
            for (int x = 0; x < h; x++)
            {
                *vecptr = box[x][i];
                vecptr++;
            }
            for (auto vecpptr = vecptr; vecpptr != b.end(); vecpptr++)
                *vecpptr = 0;
            auto res1d = fft(b);
            for (int j = 0; j < w; j++)
            {
                auto realpart = res1d[j].real();
                auto imgpart = res1d[j].imag();
                Fxy[j][i] = sqrt(realpart*realpart + imgpart*imgpart);
            }
        }
    }
    else
    {
        const double PI = 4.0 * atan (1.0); // назначение Pi

        for (int i = 0; i < h; i++)
        {
            for (int j =0; j < w; j++)
            {
                double temp = 0, tempy = 0;
                auto ptr = fxy;
                for (int x = 0; x < h; x++)
                {
                    auto pptr = *ptr;

                    for (int y = 0; y < w; y++)
                    {

                        double alpha = -2*PI*((double)i*x/h + (double)j*y/w);
                        temp += *pptr*cos(alpha);
                        tempy += *pptr*sin(alpha);
                        pptr++;
                    }
                    ptr++;
                }
                Fxy[i][j] = sqrt(temp*temp + tempy*tempy);
            }

        }
    }

    QImage* nimg = new QImage(img);
    px = nimg->bits();
    for (int i = 0; i < h; i++)
    {
        for (int j =0; j < w; j++)
        {
            y = 10*log(1 + Fxy[i][j]);
            if (y > 255)
            {
                y = 255;
            }
            if (y < 0)
                y = 0;

            *px = y;
            *++px = y;
            *++px = y;
            px += 2;
        }
    }

    for (auto fFxy = Fxy; fFxy < Fxy + h; fFxy++)
    {
        delete[] *fFxy;
    }
    delete[] Fxy;
    for (auto ffxy = fxy; ffxy < fxy + h; ffxy++)
    {
        delete[] *ffxy;
    }
    delete[] fxy;
    img_ptr = nimg;
}
