#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<QFileDialog>
#include<QString>

#include <stdio.h>
#include <fstream>
#include "exif.h"

#include <gmapclass.h>
#include <weatherclass.h>
#include <daylightclass.h>

#include <QWebEngineView>

//#include <QDebug>

double Glat=0, Glong=0;
string DateTime; //, HTime;                         // HTime is for the heading
//QString HLocation, HWeather, HPreposition;      // these are for the heading

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setStyleSheet("background-color: #547cff;");
    if(Glat==0 || Glong ==0){
        ui->locateButton->setEnabled(false);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_browseButton_clicked()
{

    QFileDialog dialog(this);                                   // Browsing files
    dialog.setNameFilter(tr("Images(*.jpg *.png *.xpm)"));
    dialog.setViewMode(QFileDialog::Detail);
    QString filename = QFileDialog::getOpenFileName(this,
                       tr("Open Images"), "",
                       tr("Image Files (*jpg *png *bmp)"));
    if(!filename.isEmpty())                                 // Image showing
        {
        QPixmap pix(filename);

        int w = ui->imageLabel->width ();
        int h = ui->imageLabel->height ();
        ui->imageLabel->setPixmap (pix.scaled (w,h,Qt::KeepAspectRatio));

        QByteArray ba = filename.toLatin1();                //Exif Parsing Begins
        const char *temp = ba.data();

        FILE *fp = fopen(temp,"rb");// binPix; ;

        //FILE *fp = fp1;

        fseek(fp, 0, SEEK_END);
        unsigned long fsize = ftell(fp);
        rewind(fp);
        unsigned char *buf = new unsigned char[fsize];

        if (fread(buf, 1, fsize, fp) != fsize) {
            //ui->exifError->setText("Cannot read file.");
            delete[] buf;
            //return 0;
          }

        fclose(fp);

        easyexif::EXIFInfo result;
        int code = result.parseFrom(buf, fsize);
        delete[] buf;
        if (code) {
            //ui->exifError->setText("Error parsing Exif data.");
          }
        Glat = result.GeoLocation.Latitude;
        Glong = result.GeoLocation.Longitude;
        DateTime = result.DateTimeOriginal.c_str();
        //
        //qDebug() << QString::fromStdString(DateTime);
        //ui->headerLabel->setText(DateTime);
        //ui->gLat->setNum(Glat);
        //ui->gLong->setNum(Glong);

        //qDebug (Glat.c_str());

        if (Glat!=0 || Glong!=0){
            ui->locateButton->setEnabled(true);

            GmapClass mapObj;
            ui->locationLabel->setText("Location: " + mapObj.getAddress(Glat, Glong));
            WeatherClass weatherObj;
            ui->weatherLabel->setText("Weather: " + weatherObj.getWeather(Glat, Glong, DateTime));

            //HLocation = mapObj.getAddress(Glat, Glong);
            //HWeather = weatherObj.getWeather(Glat, Glong, DateTime);
            //HPreposition = " in ";
            DayLightClass dlObj;
            ui->DLlabel->setText("Daylight Status: " + dlObj.getDayLight(Glat, Glong, DateTime));

        }
        else{
            //ui->tab_2->setTabEnabled(false);
            ui->locateButton->setEnabled(false);
            ui->locationLabel->setText("");
            ui->weatherLabel->setText("");

            //HLocation = "";
            //HWeather = " Memorable";
            //HPreposition = "";
        }

        //HTime = DateTime.substr(11, 2);
        //ui->headerLabel->setText("A " + HWeather + " " + QString::fromStdString(HTime) + HPreposition +HLocation);

        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        //QWebEngineView *mView_2 = new QWebEngineView();
        ui->mView_2->setAttribute( Qt::WA_DeleteOnClose );      // delete object on closing or exiting
        ui->mView_2->load(QUrl("https://www.google.com/maps/place/"
                           + QString::number(Glat) + "," + QString::number(Glong)));
        ui->mView_2->setZoomFactor(0.85);
        ui->mView_2->resize(680, 490);
        ui->mView_2->setWindowTitle("Image Location on Map");
        ui->mView_2->show();
    }
}

/*void MainWindow::on_locateButton_clicked()
{
    if(Glat!=0 || Glong!=0){                            // Locating on map on new window ** needs object optimization

    }
}
*/
