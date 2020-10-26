#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
#include<QMessageBox>
#include<QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<QtNetwork/QUdpSocket>
#include<QtNetwork/QNetworkDatagram>

#include <iostream>

using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_but_original_clicked() //When clicked, load the original image.
{
    QString originalFilename=QFileDialog::getOpenFileName(
                  this,
                  tr("Open Original File"),
                  "",
                  "Bitmap (*.bmp)"

                  );

    QImage original(originalFilename); //loads an image from the filename
    ui->lbl_original->setPixmap(QPixmap::fromImage(original).scaled(250,250,Qt::KeepAspectRatio)); //set the image to the respective label
}



void MainWindow::on_but_overlay_clicked() //When clicked, load the overlay image.
{
    QString overlayFilename= QFileDialog::getOpenFileName(
                  this,
                  tr("Open Overlay File"),
                  "",
                  "Bitmap (*.bmp)"

                  );
    QImage overlay(overlayFilename); //loads an image from the filename
    ui->lbl_overlay->setPixmap(QPixmap::fromImage(overlay).scaled(250,250,Qt::KeepAspectRatio)); //set the image to the respective label
}

void MainWindow::on_btn_load_clicked()
{
    QString brconFilename= QFileDialog::getOpenFileName(
                  this,
                  tr("Open Overlay File"),
                  "",
                  "Bitmap (*.bmp)"
                  );
    t2imageLoad = new QImage();
    brcontrastfilename = new QString();
    brcontrastfilename->replace(0,99, brconFilename); //set the new filename when the image is loaded. Will not work for filenames over 100 characters.

    t2imageLoad->load(brconFilename);

    QImage brcon(brconFilename);
    ui->lbl_t2display->setPixmap(QPixmap::fromImage(brcon).scaled(250,250,Qt::KeepAspectRatio));


}

void MainWindow::on_sldr_brightness_valueChanged(int value)
{
//When the slider is adjusted, the image's X changes by a value.
    //Each time the slider is changed
       //--Change the Brightness
       //--Display the Image

    brightness_value = value;

    QString Data = "000";

    // read an image
    cv::Mat image = cv::imread(brcontrastfilename->toStdString(), 0);
    cv::Mat image_higher_brightness;

    //Change the brightness
    image.convertTo(brconImage, -1, contrast_value, brightness_value);
    QString cv = QString::number(contrast_value);

    QString bv = QString::number(brightness_value);

    Data.replace(1,1,cv); //replace 1st and 2nd seg values with brightness and contrast
    Data.replace(2,1,bv);

    socket->write(Data.toStdString().c_str(), 3); //send to board
    //socket->writeDatagram(packettobesent.toStdString(),host,80);

    brconOut = QImage((uchar*) brconImage.data, brconImage.cols, brconImage.rows, brconImage.step, QImage::Format_Grayscale8);
    ui->lbl_t2display->setPixmap(QPixmap::fromImage(brconOut).scaled(250,250,Qt::KeepAspectRatio));
}

void MainWindow::on_sldr_contrast_valueChanged(int value)
{
//When the slider is adjusted, the image's Y changes by a value.
    QString Data = "000";


    contrast_value = value;
    // read an image
    cv::Mat image = cv::imread(brcontrastfilename->toStdString(), 0);
    cv::Mat image_higher_contrast;

    QString cv = QString::number(contrast_value);

    QString bv = QString::number(brightness_value);

    Data.replace(1,1,cv); //replace 1st and 2nd seg values with brightness and contrast
    Data.replace(2,1,bv);  //send to board

    //Change the brightness
    image.convertTo(brconImage, -1, contrast_value, brightness_value);
    socket->write(Data.toStdString().c_str(), 3);
    //convert to QImage
    brconOut = QImage((uchar*) brconImage.data, brconImage.cols, brconImage.rows, brconImage.step, QImage::Format_Grayscale8);
    //display to label
    ui->lbl_t2display->setPixmap(QPixmap::fromImage(brconOut).scaled(250,250,Qt::KeepAspectRatio));

}


void MainWindow::on_but_save_clicked()
{
        QString imagePath = QFileDialog::getSaveFileName(
                        this,
                        tr("Save File"),
                        "",
                        tr("Bitmap (*.bmp)" )
                        );


        //brconOut.save(imagePath);
        cv::imwrite(imagePath.toStdString(), brconImage );
        //t2imageLoad->save(imagePath);
}

void MainWindow::on_pushButton_clicked()
{
    socket = new QUdpSocket;

    socket->connectToHost("192.168.1.204", 53); //ip address for board and port. (Nicholas). Changes each time the board is turned on

    bool connectSuccess = socket->waitForConnected(1000);

    if(!connectSuccess)
    {
        exit(-1);
    }


   // QString Data = "023";
    //socket->write(Data.toStdString().c_str(), 3);

 }



