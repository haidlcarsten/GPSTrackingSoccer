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
#include <limits>


Spieler::Spieler(QString aPfad)
{ 
  this->mPfad = aPfad;
  QFileInfo info(aPfad);
  this->mSpielername = info.fileName();
  this->parseData();
  this->a_speed = 0;
  this->a_hrate = 0;
}

QString Spieler::getName()
{
  return this->mSpielername;
}

double Spieler::getSpeed()
{
  return this->a_speed;
}

double Spieler::average_speed()
{
    double speed_sum = 0;
    foreach (int tstamp, playerData.keys())
    {
      speed_sum = speed_sum + playerData.value(tstamp).speed;
    }
    a_speed = speed_sum / playerData.lastKey();
    return a_speed;
}

float Spieler::average_heartRate()
{
    float heartrate_sum = 0;
    foreach (int tstamp, playerData.keys())
    {
      heartrate_sum = heartrate_sum + playerData.value(tstamp).heartRate;
    }

    a_hrate = heartrate_sum/ playerData.lastKey();
    return a_hrate;
}

float Spieler::getHeartRate()
{
  return this->a_hrate;
}


void Spieler::setSlider(QSlider *aSlider)
{
  this->mSlider = aSlider;
  connect(this->mSlider, &QSlider::valueChanged, this, &Spieler::displayFrameData);
}

void Spieler::displayData(bool aDisplay)
{
  QWidget* widgetToDisplay;
  if(aDisplay)
  {
    widgetToDisplay = new QWidget;
    QGridLayout* layout = new QGridLayout(widgetToDisplay);

    QLabel* lblName = new QLabel("Name:");
    QLabel* lblSpielerName = new QLabel(this->getName());
    layout->addWidget(lblName, 0, 0);
    layout->addWidget(lblSpielerName, 0, 1);

    QLabel* lblSpeedAvg = new QLabel("Durchschnittliche Geschwindigkeit:");
    QLabel* lblSpielerSpeedAvg = new QLabel(QString::number(this->getSpeed()));
    layout->addWidget(lblSpeedAvg, 1, 0);
    layout->addWidget(lblSpielerSpeedAvg, 1, 1);

    QLabel* lblHeartRateAvg = new QLabel("Durchschnittliche Herzrate:");
    QLabel* lblSpielerHeartRateAvg = new QLabel(QString::number(this->getHeartRate()));
    layout->addWidget(lblHeartRateAvg, 2, 0);
    layout->addWidget(lblSpielerHeartRateAvg, 2, 1);

    this->setSliderValues(this->playerData.firstKey(), this->playerData.lastKey(), 0);
  }
  else
  {
    widgetToDisplay = this->mChartWidget->getDefaultWidget();
    this->setSliderValues(0, 0, 0);
  }

  this->mChartWidget->setChartWidget(widgetToDisplay);
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

}

QPair<double, double> Spieler::getCornerTopRight() const
{
    return mCornerTopRight;
}

QPair<double, double> Spieler::getCornerTopLeft() const
{
    return mCornerTopLeft;
}

QPair<double, double> Spieler::getCornerBottomRight() const
{
    return mCornerBottomRight;
}

QPair<double, double> Spieler::getCornerBottomLeft() const
{
    return mCornerBottomLeft;
}


void Spieler::parseData()
{
    QFile inputfile(this->mPfad);
    if(!inputfile.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QTextStream stream(&inputfile);
    QString line;      //erstellt ein string mit allen daten
    bool firstline = true;

    double xValueMax = 0;
    double yValueMax = 0;
    double xValueMin = DBL_MAX;
    double yValueMin = DBL_MAX;

    while(!stream.atEnd())
    {

     line = stream.readLine();
     if(firstline)
     {
         firstline = false;
         continue;
     }


     QStringList elements = line.trimmed().remove(' ').split(',');

     parsedData data;
     int timestamp = elements[0].toInt();
     data.activityType = elements[1].toInt();

     // check if we reach the end
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

     if (data.x_value > xValueMax )
       xValueMax = data.x_value;

     if (data.x_value < xValueMin )
       xValueMin = data.x_value;

     if (data.y_value > yValueMax )
       yValueMax = data.y_value;

     if (data.y_value < yValueMin )
       yValueMin = data.y_value;
    }

    mCornerBottomLeft.first = xValueMin;
    mCornerBottomLeft.second = yValueMin;

    mCornerTopRight.first = xValueMax;
    mCornerTopRight.second = yValueMax;

    mCornerBottomRight.first = xValueMax;
    mCornerBottomRight.second = yValueMin;

    mCornerTopLeft.first = xValueMin;
    mCornerTopLeft.second = yValueMax;


    // a debug output
//    foreach (int timestamp, playerData.keys())
//    {
//      parsedData data =  playerData.value(timestamp);

//      qDebug() << data.lapNumber ;
//      qDebug() << data.distance ;
//      qDebug() <<data.speed ;
//      qDebug() <<data.calories ;
//      qDebug() <<data.x_value ;
//      qDebug() <<data.y_value;
//      qDebug() << data.elevation ;
//      qDebug() << data.heartRate ;
//      qDebug() << data.cycles ;
//    }

}
