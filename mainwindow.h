#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QtWidgets>
#include <QTimer>
#include <string>
#include <QFileDialog>
#include <Qdebug>

#include "opencv2/core/core.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "opencv2/opencv.hpp"
#include "opencv2/core/types.hpp"
#include <iostream>



//using namespace std;
//using namespace  cv;

//void mouseHandleStatic(int event, int x, int y, int, void *userdata);

namespace Ui {
class MainWindow;
class Dialog;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::string filename;
    cv::FileStorage fs;
    std::ofstream outSaveData;

    int contraste = 0;
    int exposicao = 0;
    int saturacao = 0;
    int brilho = 0;
    int focus = 0;
    int somar = 0;
    bool breakLoop= false;
    bool flag = false;
    int soma(int a);
    int i = 0;
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent (QCloseEvent *event);
    //void mousePressEvent(QMouseEvent *event);
    void resize_image(cv::Mat frame);
    void GetAndSetSaveConfigurations();
    void setfilename(QString);

    ~MainWindow();

private slots:

    void on_horizontalSlider_B_valueChanged();

    void on_horizontalSlider_C_valueChanged();

    void on_horizontalSlider_E_valueChanged();

    void on_horizontalSlider_F_valueChanged();

    void on_horizontalSlider_S_valueChanged();

    void on_pushButton_open_webcam_clicked();

    void on_pushButton_close_webcam_clicked();

    void update_window();

    void on_pushButton_save_data_clicked();

    void on_pushButton_cortar_clicked();

    void on_actionCarregar_Arquivos_triggered();

private:
    Ui::MainWindow *ui;
    Ui::Dialog *d;

    QTimer *timer;
    cv::VideoCapture cap;
    cv::Mat camerafeed;

    cv::Mat frame;
    cv::Mat frame_output;
    QImage qt_image;

    // Input Quadilateral or Image plane coordinates
    cv::Point2f inputQuad[4];
    // Output Quadilateral or World plane coordinates
    cv::Point2f outputQuad[4];

    cv::Mat lambda;

    void onMouse(int event, int x, int y);
    static void mouseHandleStatic(int event, int x, int y, int, void *userdata);
};

#endif // MAINWINDOW_H



