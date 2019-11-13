#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <Qdebug>
#include <iostream>
#include <QPixmap>
#include <QtWidgets>
#include <QTimer>

using namespace std;
using namespace cv;

//MainWindow *global_ptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    //FileStorage fs;
    filename = "save.xml";
    //fs.open(filename, FileStorage::READ);


    timer = new QTimer(this);


    QPixmap img ("Logo.JPG");

    //cvSetMouseCallback("Webcam", event, &frame);


    ui->label->setPixmap(img.scaled(200,400,Qt::KeepAspectRatio));

    Mat lambda_local(2,4,CV_32FC1);

    lambda = lambda_local;

    cap.read(camerafeed);

    brilho = ui->box_brilho->text().toInt();
    cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);

    contraste = ui->box_contraste->text().toInt();
    cap.set(CV_CAP_PROP_CONTRAST  , contraste);

    exposicao = ui->box_exposicao->text().toInt();
    cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);

    saturacao = ui->box_saturacao->text().toInt();
    cap.set(CV_CAP_PROP_SATURATION, saturacao);

    focus = ui->box_foco->text().toInt();
    cap.set(CV_CAP_PROP_FOCUS, focus);

    somar = soma(somar);

   /* qDebug() << "Brilho: " << brilho;
    qDebug() << "Exposição: " << exposicao;
    qDebug() << "Saturação: " << saturacao;
*/

    //cv::Mat image = cv::imread("c://1.jpg", 1);
    //cv::namedWindow("Imagem");
    //cv::imshow("Imagem", image);

    //namedWindow("Webcam");
}


int MainWindow::soma(int a) {
    a = brilho + saturacao + contraste + exposicao;
    return a;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    breakLoop=true;
    //disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    this->destroy();
    cap.~VideoCapture();
    event->accept();
    destroyAllWindows();
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete cal;
}

void MainWindow::on_horizontalSlider_B_valueChanged()
{
    brilho = ui->box_brilho->text().toInt();
    cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);
    somar = soma(somar);

}

void MainWindow::on_horizontalSlider_C_valueChanged()
{
    contraste = ui->box_contraste->text().toInt();
    cap.set(CV_CAP_PROP_CONTRAST  , contraste);
    somar = soma(somar);

}

void MainWindow::on_horizontalSlider_E_valueChanged()
{

    exposicao = ui->box_exposicao->text().toInt();
    cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);
    somar = soma(somar);

}

void MainWindow::on_horizontalSlider_S_valueChanged()
{
    saturacao = ui->box_saturacao->text().toInt();
    cap.set(CV_CAP_PROP_SATURATION, saturacao);
    somar = soma(somar);

}

void MainWindow::on_pushButton_open_webcam_clicked()
{
    cap.open(0);
    cap.read(camerafeed);
    cap >> frame;

    outputQuad[0] = Point2i( 0,0 );
    outputQuad[1] = Point2i( frame.cols-1,0);
    outputQuad[2] = Point2i( frame.cols-1,frame.rows-1);
    outputQuad[3] = Point2i( 0,frame.rows-1);

    if(!cap.isOpened())  // Check if we succeeded
    {
        cout << "camera is not open" << endl;
    }
    else
    {
        cout << "camera is open" << endl;
        //cap.set(CV_CAP_PROP_SETTINGS, 0);
//        brilho = ui->box_brilho->text().toInt();
//        cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);

//        contraste = ui->box_contraste->text().toInt();
//        cap.set(CV_CAP_PROP_CONTRAST  , contraste);

//        exposicao = ui->box_exposicao->text().toInt();
//        cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);

//        saturacao = ui->box_saturacao->text().toInt();
//        cap.set(CV_CAP_PROP_SATURATION, saturacao);

//        focus = ui->box_foco->text().toInt();
//        cap.set(CV_CAP_PROP_FOCUS, focus);

        cap.set(CV_CAP_PROP_SETTINGS, 0);
        connect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
        timer->start(20);
    }
}

void MainWindow::on_pushButton_close_webcam_clicked()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(update_window()));
    cap.release();

    Mat image = Mat::zeros(frame.size(),CV_8UC3);

    qt_image = QImage((const unsigned char*) (image.data), image.cols, image.rows, QImage::Format_RGB888);

    ui->label_I->setPixmap(QPixmap::fromImage(qt_image));

    ui->label_I->resize(ui->label_I->pixmap()->size());

    cout << "camera is closed" << endl;
}

void MainWindow::update_window()
{

    cap >> frame;

    if(flag == true){
        imshow("Webcam", frame);        
    }

    if (i == 4){
         lambda = getPerspectiveTransform( inputQuad, outputQuad );
         warpPerspective(frame, frame_output, lambda, frame_output.size());

//         cvtColor(frame_output, frame_output, CV_BGR2RGB);

//         qt_image = QImage((const unsigned char*) (frame_output.data), frame_output.cols, frame_output.rows, QImage::Format_RGB888);

//         ui->label_I->setPixmap(QPixmap::fromImage(qt_image));

//         ui->label_I->resize(ui->label_I->pixmap()->size());

         resize_image(frame_output);

         return;
    }

//    cvtColor(frame, frame, CV_BGR2RGB);


//    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

//    ui->label_I->setPixmap(QPixmap::fromImage(qt_image));

//    ui->label_I->resize(ui->label_I->pixmap()->size());

    resize_image(frame);
}

void MainWindow:: resize_image(Mat frame) {

    cvtColor(frame, frame, CV_BGR2RGB);

    qt_image = QImage((const unsigned char*) (frame.data), frame.cols, frame.rows, QImage::Format_RGB888);

    ui->label_I->setPixmap(QPixmap::fromImage(qt_image));

    ui->label_I->resize(ui->label_I->pixmap()->size());

}

void MainWindow::on_pushButton_save_data_clicked()
{
    FileStorage fs;
    fs.open(filename, FileStorage::WRITE);
    fs << "Dados";
    fs << "{";
    fs << "Brightness"<< brilho;
    fs << "Exposure"<< exposicao;
    fs << "Saturation"<< saturacao;
    fs << "Contrast"<< contraste;
    fs << "Focus"<< focus;
//    fs << "InputQuad0X" << inputQuad[0].x;
//    fs << "InputQuad0Y" << inputQuad[0].y;
//    fs << "InputQuad1X" << inputQuad[1].x;
//    fs << "InputQuad1Y" << inputQuad[1].y;
//    fs << "InputQuad2X" << inputQuad[2].x;
//    fs << "InputQuad2Y" << inputQuad[2].y;
//    fs << "InputQuad3X" << inputQuad[3].x;
//    fs << "InputQuad3Y" << inputQuad[3].y;
    fs << "}";

    fs.release();

    qDebug() << "salvei";
}

void MainWindow::on_horizontalSlider_F_valueChanged()
{
    focus = ui->box_foco->text().toInt();
    cap.set(CV_CAP_PROP_FOCUS, focus);
}

void MainWindow::onMouse(int event, int x, int y) {

    if  ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        inputQuad[i] = Point2f(x,y);
        cout << "Left button of the mouse is clicked - position (" << inputQuad[i].x << ", " << inputQuad[i].y << ")" << endl;
        i++;
        if(i == 4) {
            flag = false;
            //destroyWindow("Webcam");
        }
    }
    if (event == EVENT_RBUTTONDOWN)
            i = 0;
}

void MainWindow::mouseHandleStatic(int event, int x, int y, int , void* userdata)
{
    MainWindow* cal = static_cast<MainWindow*>(userdata);
    cal->onMouse(event, x, y);
}

void MainWindow::on_pushButton_cortar_clicked() {
    i = 0;
    imshow("Webcam", frame);
    setMouseCallback("Webcam", mouseHandleStatic, this);
    //setMouseCallback("Webcam", , NULL);
    flag = true;
}

void MainWindow::GetAndSetSaveConfigurations()
{
    if(fs.isOpened())
    {
        qDebug() << "entrei na parte que nomeia os atributos";

        FileNode data = fs["Dados"];
        cap.set(CV_CAP_PROP_EXPOSURE  , (int)(data["Exposure"]));
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"])+1);
        Sleep(30);
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"]));
        Sleep(30);
        cap.set(CV_CAP_PROP_CONTRAST  , (int)(data["Contrast"]));
        cap.set(CV_CAP_PROP_SATURATION, (int)(data["Saturation"]));
        cap.set(CV_CAP_PROP_EXPOSURE  , (int)(data["Exposure"]));
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"])+1);
        Sleep(30);
        cap.set(CV_CAP_PROP_BRIGHTNESS, (int)(data["Brightness"]));
        Sleep(30);

        //FALTA O FOCO

        brilho = (int)(data["Brightness"]);
        exposicao = (int)(data["Exposure"]);
        saturacao = (int)(data["Saturation"]);
        contraste = (int)(data["Contrast"]);
        focus = (int)(data["Focus"]);

//        inputQuad[0].x = (float)(data["InputQuad0X"]);
//        inputQuad[1].x = (float)(data["InputQuad1X"]);
//        inputQuad[2].x = (float)(data["InputQuad2X"]);
//        inputQuad[3].x = (float)(data["InputQuad3X"]);

//        inputQuad[0].y = (float)(data["InputQuad0Y"]);
//        inputQuad[1].y = (float)(data["InputQuad1Y"]);
//        inputQuad[2].y = (float)(data["InputQuad2Y"]);
//        inputQuad[3].y = (float)(data["InputQuad3Y"]);
//        i = 4;


        ui->horizontalSlider_B->sliderMoved(data["Brightness"]);
        ui->box_brilho->valueChanged(data["Brightness"]);

        ui->horizontalSlider_S->sliderMoved(data["Saturation"]);
        ui->box_saturacao->valueChanged(data["Saturation"]);

        ui->horizontalSlider_C->sliderMoved(data["Contrast"]);
        ui->box_contraste->valueChanged(data["Contrast"]);

        ui->horizontalSlider_E->sliderMoved(data["Exposure"]);
        ui->box_exposicao->valueChanged(data["Exposure"]);

        ui->horizontalSlider_F->sliderMoved(data["Focus"]);
        ui->box_foco->valueChanged(data["Focus"]);

    }
    else
    {
        cap.set(CV_CAP_PROP_CONTRAST  , contraste);
        cap.set(CV_CAP_PROP_SATURATION, saturacao);
        cap.set(CV_CAP_PROP_EXPOSURE  , exposicao);
        cap.set(CV_CAP_PROP_BRIGHTNESS, brilho+1);
        Sleep(30);
        cap.set(CV_CAP_PROP_BRIGHTNESS, brilho);
        Sleep(30);
        qDebug() << "Não salvei nada";
        //FALTA FOCO;
   }
}

void MainWindow::on_actionCarregar_Arquivos_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Abrir arquivo"),"C:", tr("XML (*.xml)"));
       if(!fileName.isNull())
       {
           setfilename(fileName);
           filename = fileName.toStdString();
           qDebug() << "abriu o arquivo ";
       }
}

void MainWindow::setfilename(QString name)
{
    filename = name.toStdString();
    FileStorage fs;
    //fs.release();
    fs.open(filename, FileStorage::READ);
    GetAndSetSaveConfigurations();
    fs.release();

}
