#include "spieler.h"
#include <QLabel>
#include <QFormLayout>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QMap>
#include <QFileInfo>
#include <QDebug>


Spieler::Spieler(QString aPfad)
{ 
    this->mPfad = aPfad;
    QFileInfo info(aPfad);
    this->mSpielername = info.fileName();
    this->parseData();


}

QString Spieler::getName()
{
  return this->mSpielername;
}

void Spieler::setSlider(QSlider *aSlider)
{
  this->mSlider = aSlider;
  connect(this->mSlider, &QSlider::valueChanged, this, &Spieler::displayFrameData);
}

void Spieler::displayData(bool aDisplay)
{
  qDebug() << "void Spieler::displayData()";
  qDebug() << this->mSpielername;

  QWidget* test;
  if(aDisplay)
  {
    test = new QWidget;
    QLabel* lbl = new QLabel(this->mSpielername);
    lbl->setWordWrap(true);

    QVBoxLayout* layout = new QVBoxLayout(test);
    layout->addWidget(lbl);

    this->setSliderValues(0, 200, 100);

  }
  else
  {
    test = this->mChartWidget->getDefaultWidget();
    this->setSliderValues(0, 0, 0);
  }

  this->mChartWidget->setChartWidget(test);
}

void Spieler::setSliderValues(int aMin, int aMax, int aValue)
{
  if(aValue )
  this->mSlider->setMinimum(aMin);
  this->mSlider->setMaximum(aMax);
  this->blockSignals(true);
  this->mSlider->setValue(aValue);
  this->blockSignals(false);

}

void Spieler::displayFrameData(int aTime)
{
  qDebug() << aTime;
}
void Spieler::parseData()
{
//    for(QString::iterator iter = tData.begin(); iter != tData.end(); ++iter) //Zeilenumbrüche durch einen Komma ersetzen
//    {
//        if(*iter == '\n')
//               *iter = ',';
//    }
    QFile inputfile(this->mPfad);
    if(!inputfile.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QTextStream stream(&inputfile);
    QString line;      //erstellt ein string mit allen daten
    bool firstline = true;
    while(!stream.atEnd())
    {
//     line = line + stream.readLine() + ",";
     if(firstline)
     {
         firstline = false;
         continue;
     }

     line = stream.readLine();
     QStringList elements = line.trimmed().remove(' ').split(',');

     parsedData data;
     int timestamp = elements[0].toInt();
     data.activityType = elements[1].toInt();

     if(data.activityType == -1)
         break;

     data.lapNumber = elements[2].toInt();
     data.distance = elements[3].toDouble();
     data.speed = elements[4].toDouble();
     data.calories = elements[5].toDouble();
     data.x_value = elements[6].toDouble();
     data.y_value = elements[7].toDouble();
     data.elevation = elements[8].toDouble();
     data.heartRate = elements[9].toInt();
     data.cycles = elements[10].toInt();


     playerData.insert(timestamp,data);
    }
    foreach (int timestamp, playerData.keys()) {
        parsedData data =  playerData.value(timestamp);

         qDebug() << data.lapNumber ;
        qDebug() << data.distance ;
         qDebug() <<data.speed ;
         qDebug() <<data.calories ;
         qDebug() <<data.x_value ;
         qDebug() <<data.y_value;
        qDebug() << data.elevation ;
        qDebug() << data.heartRate ;
        qDebug() << data.cycles ;
    }

}
