#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QColor>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QLayout>
#include "imagewidget.h"
#include "histogram.h"
#include <QDebug>
#include <QSlider>
#include <QLabel>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QImage *image;
    Histogram* histogram;
    ImageWidget *img_widget;
    QHBoxLayout *qlayout;
    QVBoxLayout *vlayout;
    int up_bound = 255;
    int down_bound = 0;
    int mat_pos = 128;
    int dispers = (128 + mat_pos) / 2;
    void draw_setup();
    void draw_about();
    QSlider* white_slider, *black_slider, *mat_slider, *dis_slider;
    ~MainWindow();

private slots:
    void slotAuthor();
    void slotAbout();
    void slotLoadImage();
    void slotBaseImage();
    void slotInversImage();
    void slotWhiteImage();
    void slotWhiteSliderMove();
    void slotBlackSliderMove();
    void slotMatSliderMove();
    void slotDispersMove();
    void slotLogarifm();
    void slotDegree();
    void slotLine();
    void slotEqual();
    void slotGausse();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
