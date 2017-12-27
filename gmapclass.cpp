#include "gmapclass.h"

//#include "mainwindow.h"
#include <QApplication>

GmapClass::GmapClass(QObject *parent) : QObject(parent)
{

}

QString GmapClass::getAddress(double map_lat, double map_long)
{
    QString addr_qStr; // this string will hold the address or blank string

    QLoggingCategory::setFilterRules("qt.network.ssl.w arning=false");  // just to ignore the logged ssl warnings

    // create custom temporary event loop on stack
    QEventLoop eventLoop;

    // "quit()" the event-loop, when the network request "finished()"
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req( QUrl( QString("https://maps.googleapis.com/maps/api/geocode/json?latlng="
                                       + QString::number(map_lat) + "," + QString::number(map_long)
                                       +"&key=AIzaSyBysUF3Tsd1ebG5zIHeYJOPiVlD_lUfwSs")));
    //simple testing response of ip: http://ip.jsontest.com/
    QNetworkReply *addr_reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (addr_reply->error() == QNetworkReply::NoError) {

        //success
        //qDebug() << "Success" <<addr_reply->readAll();

        //qDebug() << addr_reply->readAll();  //reads quite well!!         // ** start json parsing
        QByteArray jsonData = addr_reply->readAll();
        //file.close();
        //qDebug() << jsonData;
        QJsonDocument document = QJsonDocument::fromJson(jsonData);
        QJsonObject object = document.object();

        QJsonValue value = object.value("results");
        QJsonArray array = value.toArray();

        qDebug() << array[0].toObject().value("formatted_address").toString();
        addr_qStr = array[0].toObject().value("formatted_address").toString();


    }
    else {
        //failure
        qDebug() << "Failure" <<addr_reply->errorString();
        addr_qStr = "Sorry, no address matched.";
    }
    delete addr_reply;
    return addr_qStr;
}

/*void GmapClass::showMap(double map_lat, double map_long)
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QWebEngineView *mView_2 = new QWebEngineView();
    mView_2->setAttribute( Qt::WA_DeleteOnClose );      // delete object on closing or exiting
    mView_2->load(QUrl("https://www.google.com/maps/place/"
                       + QString::number(map_lat) + "," + QString::number(map_long)));
    mView_2->resize(1024, 750);
    mView_2->setWindowTitle("Image Location on Map");
    mView_2->show();
}
*/
