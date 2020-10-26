#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//For Task 2-------------------------------
#include<QWidget>
#include<QSlider>
#include<QScrollBar>
#include<QSpinBox>
#include<QProgressBar>
#include<QVBoxLayout>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include<QtNetwork/QUdpSocket>
#include<QtNetwork/QNetworkDatagram>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_but_original_clicked();

    void on_but_overlay_clicked();

    void on_btn_load_clicked();

    void on_but_save_clicked();

    void on_sldr_brightness_valueChanged(int value);

    void on_sldr_contrast_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap t2Image;
    QImage *t2imageLoad;
    QString *brcontrastfilename;
    cv::Mat brconImage;
    int contrast_value;
    int brightness_value;
    QImage brconOut;
    QUdpSocket *socket;
};
#endif // MAINWINDOW_H
