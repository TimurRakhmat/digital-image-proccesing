#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedHeight(800);
    this->setFixedWidth(1200);

    QAction *ab_btn = new QAction("&About", this);
    menuBar()->addAction(ab_btn);
    connect(ab_btn, &QAction::triggered, this, &MainWindow::slotAbout);

    QAction *au_btn = new QAction("&Author", this);
    menuBar()->addAction(au_btn);
    connect(au_btn, &QAction::triggered, this, &MainWindow::slotAuthor);

    QAction *limgbtn = new QAction("&Load", this);
    menuBar()->addAction(limgbtn);
    connect(limgbtn, &QAction::triggered, this, &MainWindow::slotLoadImage);



    image = new QImage;
    //image->convertToFormat(QImage::Format_RGBA8888_Premultiplied);
    char path[] = "C:\\Users\\rahma\\Documents\\histogram\\image\\cat.jpg";
    if (!image->load(path))
    {
        qDebug() << "Saving image error!\n";
        exit(-1);
    }
    *image = image->scaled(512, 512); //, Qt::KeepAspectRatio);


    qlayout = new QHBoxLayout();
    vlayout = new QVBoxLayout();

    QAction *firstImg = new QAction("&BaseImage", this);
    connect(firstImg, &QAction::triggered, this, &MainWindow::slotBaseImage);
    menuBar()->addAction(firstImg);

    img_widget = new ImageWidget;
    img_widget->set_image(*image);
    qlayout->addWidget(img_widget);


    QAction *blurbtn = new QAction("&BlurFilter", this);
    connect(blurbtn, &QAction::triggered, this, &MainWindow::slotBlur);

    QAction *mediatbtn = new QAction("&MedianFilter", this);
    connect(mediatbtn, &QAction::triggered, this, &MainWindow::slotMedian);

    QAction *gaussebtn = new QAction("&GausseFilter", this);
    connect(gaussebtn, &QAction::triggered, this, &MainWindow::slotGausse);

    QAction *sharpbtn = new QAction("&SharpnessFilter", this);
    connect(sharpbtn, &QAction::triggered, this, &MainWindow::slotSharp);

    QAction *laplasbtn = new QAction("&LaplasFilter", this);
    connect(laplasbtn, &QAction::triggered, this, &MainWindow::slotLaplas);

    QAction *sobelbtn = new QAction("&SobelFilter", this);
    connect(sobelbtn, &QAction::triggered, this, &MainWindow::slotSobel);

    QAction *dftbtn = new QAction("&DiscretFourierTransform", this);
    connect(dftbtn, &QAction::triggered, this, &MainWindow::slotFourier);

    QAction *fftbtn = new QAction("&FastFourierTransform", this);
    connect(fftbtn, &QAction::triggered, this, &MainWindow::slotFastFourier);

    QMenu* filter = menuBar()->addMenu("&WindowFilterList");
    filter->addAction(blurbtn);
    filter->addAction(mediatbtn);
    filter->addAction(gaussebtn);
    filter->addAction(sharpbtn);
    filter->addAction(laplasbtn);
    filter->addAction(sobelbtn);
    filter->addAction(dftbtn);
    filter->addAction(fftbtn);


    white_slider = new QSlider;
    white_slider->setMinimum(0);
    white_slider->setMaximum(255);
    white_slider->setValue(up_bound);
    connect(white_slider, &QSlider::sliderMoved, this, &MainWindow::slotWhiteSliderMove);

    black_slider = new QSlider;
    black_slider->setMinimum(0);
    black_slider->setMaximum(down_bound);
    connect(black_slider, &QSlider::sliderMoved, this, &MainWindow::slotBlackSliderMove);

    mat_slider = new QSlider;
    mat_slider->setMinimum(0);
    mat_slider->setMaximum(255);
    mat_slider->setValue(mat_pos);
    connect(mat_slider, &QSlider::sliderMoved, this, &MainWindow::slotMatSliderMove);

    dis_slider = new QSlider;
    dis_slider->setMinimum(0);
    dis_slider->setMaximum(255);
    dis_slider->setValue(dispers);
    connect(dis_slider, &QSlider::sliderMoved, this, &MainWindow::slotDispersMove);

    QHBoxLayout* qqlayout = new QHBoxLayout();
    QVBoxLayout* vv1layout = new QVBoxLayout();
    QVBoxLayout* vv2layout = new QVBoxLayout();
    QVBoxLayout* vv3layout = new QVBoxLayout();
    QVBoxLayout* vv4layout = new QVBoxLayout();
    vv1layout->addWidget(new QLabel("whiteBound_255"));
    vv1layout->addWidget(white_slider);
    vv1layout->addWidget(new QLabel("whiteBound_0"));

    vv2layout->addWidget(new QLabel("blackBound_255"));
    vv2layout->addWidget(black_slider);
    vv2layout->addWidget(new QLabel("blackBound_0"));

    vv3layout->addWidget(new QLabel("expectedValue_255"));
    vv3layout->addWidget(mat_slider);
    vv3layout->addWidget(new QLabel("expectedValue_0"));

    vv4layout->addWidget(new QLabel("dispers_255"));
    vv4layout->addWidget(dis_slider);
    vv4layout->addWidget(new QLabel("dispers_0"));

    qqlayout->addLayout(vv1layout);
    qqlayout->addLayout(vv2layout);
    qqlayout->addLayout(vv3layout);
    qqlayout->addLayout(vv4layout);
//    vlayout->addLayout(qqlayout);

    qlayout->addLayout(vlayout);
    draw_setup();

    QAction *exit_btn = new QAction("&Quit", this);
    menuBar()->addAction(exit_btn);
    connect(exit_btn, &QAction::triggered, qApp, QApplication::quit);

}

void MainWindow::draw_setup()
{
    this->centralWidget()->setLayout(qlayout);
}

void MainWindow::draw_about()
{
    QMessageBox msgBox;
    msgBox.setStyleSheet("QDialog {background-color: orange;"
                                  "font-size: 45px;}"
                         "QPushButton{border: 1px solid transparent;text-align: center;"
                                    "font-size: 25px;"
                                    "color:rgba(0,0,255,255);"
                                    "border-radius: 8px;"
                                    "border-width: 3px;"
                                    "border-image: 9,2,5,2; "
                                    "background-position: top left;"
                                    "background-origin: content;"
                                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(200, 110, 100, 200), stop:1 rgba(130, 148, 130, 200));}"
                                    "QPushButton::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(255,200,0,255), stop: 1 rgba(255,0,0,255));}"
                                    "QPushButton{border-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 200, 200, 200), stop:1 rgba(255, 200, 200, 200));}"
                                    );
    msgBox.setText("Данное приложение может применять пространственные фильтры. \
В приложениее присутствуют фильтры: box-filter, median, gauss-blur, sharpness, laplas-operator, sobel-operator, DFT(FFT)");
    msgBox.exec();
}

void MainWindow::slotAuthor()
{
    QMessageBox msgBox;
    msgBox.setStyleSheet("QDialog {background-color: orange;"
                                  "font-size: 45px;}"
                         "QPushButton{border: 1px solid transparent;text-align: center;"
                                    "font-size: 25px;"
                                    "color:rgba(0,0,255,255);"
                                    "border-radius: 8px;"
                                    "border-width: 3px;"
                                    "border-image: 9,2,5,2; "
                                    "background-position: top left;"
                                    "background-origin: content;"
                                    "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(200, 110, 100, 200), stop:1 rgba(130, 148, 130, 200));}"
                                    "QPushButton::chunk {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgba(255,200,0,255), stop: 1 rgba(255,0,0,255));}"
                                    "QPushButton{border-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(255, 200, 200, 200), stop:1 rgba(255, 200, 200, 200));}"
                                    );
    msgBox.setText("Студент группы 210, Рахматуллин Тимур, github::TimurRakhmat");
    msgBox.exec();
}

void MainWindow::slotAbout()
{
    draw_about();
}

void MainWindow::slotLoadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName == "")
    {
        return;
    }
    if (!image->load(fileName))
    {
        qDebug() << "Saving image error!\n";
        exit(-1);
    }
    *image = image->scaled(512, 512); //, Qt::KeepAspectRatio);

    img_widget->set_image(*image);
}

void MainWindow::slotBaseImage()
{
    img_widget->set_image(*image);
    repaint();
}

void MainWindow::slotWhiteSliderMove()
{
    up_bound = white_slider->value();
}

void MainWindow::slotBlackSliderMove()
{
    down_bound = black_slider->value();
}

void MainWindow::slotMatSliderMove()
{
    mat_pos = mat_slider->value();
}

void MainWindow::slotDispersMove()
{
    dispers = dis_slider->value();
}


void MainWindow::slotBlur()
{
    img_widget->box_filter(*image);
    repaint();
}

void MainWindow::slotMedian()
{
    img_widget->median_filter(*image);
    repaint();
}

void MainWindow::slotLaplas()
{
   img_widget->laplas_filter(*image);
    repaint();
}

void MainWindow::slotSharp()
{
    img_widget->sharpness_filter(*image);
    repaint();
}

void MainWindow::slotGausse()
{
    img_widget->gausse_filter(*image);
    repaint();
}

void MainWindow::slotSobel()
{
    img_widget->sobel_filter(*image);
    repaint();
}

void MainWindow::slotFourier()
{
    img_widget->fourier_transform(*image);
    repaint();
}

void MainWindow::slotFastFourier()
{
    img_widget->fourier_transform(*image, true);
    repaint();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete image;
}

