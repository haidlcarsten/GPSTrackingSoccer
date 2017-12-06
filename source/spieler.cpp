#include "spieler.h"
#include "consts.h"
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
  this->mFilename = info.fileName();
  this->mSpielerName = this->mFilename;

  this->parseData();

  QString timestamp;
  for (int i = this->mFilename.length()-1; i > 0; --i)
  {
    if(mFilename.at(i) != '.')
        continue;
    for (int j = 1;  j <= 6; j++)
    {
       timestamp.append(mFilename.at(i-j));
    }
    break ;
  }

  std::reverse(timestamp.begin(),timestamp.end());
  mStartTime = QTime::fromString(timestamp,"hhmmss");
}

QString Spieler::getFileName() const
{
  return this->mFilename;
}

QString Spieler::getPlayerName() const
{
  return this->mSpielerName;
}

double Spieler::getSpeed()
{
  return this->mAvgSpeed;
}

double Spieler::calcAverageSpeed()
{
    double speed_sum = 0;
    foreach (int tstamp, mAllPlayerData.keys())
    {
      speed_sum = speed_sum + mAllPlayerData.value(tstamp).mSpeed;
    }
    mAvgSpeed = speed_sum / mAllPlayerData.lastKey();
    return mAvgSpeed;
}

float Spieler::calcAverageHeartRate()
{
    float heartrate_sum = 0;
    foreach (int tstamp, mAllPlayerData.keys())
    {
      heartrate_sum = heartrate_sum + mAllPlayerData.value(tstamp).mHeartRate;
    }

    mAvgHeartRate = heartrate_sum/ mAllPlayerData.lastKey();
    return mAvgHeartRate;
}

float Spieler::getHeartRate()
{
  return this->mAvgHeartRate;
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
    QVBoxLayout* basicLayout = new QVBoxLayout;

    QGridLayout* layout = new QGridLayout();

    QLabel* lblName = new QLabel(PLAYER_NAME);
    QLabel* lblSpielerName = new QLabel(this->getFileName());
    layout->addWidget(lblName, 0, 0);
    layout->addWidget(lblSpielerName, 0, 1);

    QLabel* lblSpeedAvg = new QLabel(PLAYER_AVERAGE_SPEED);
    QLabel* lblSpielerSpeedAvg = new QLabel(QString::number(this->getSpeed()));
    layout->addWidget(lblSpeedAvg, 1, 0);
    layout->addWidget(lblSpielerSpeedAvg, 1, 1);

    QLabel* lblHeartRateAvg = new QLabel(PLAYER_AVERAGE_HEARTRATE);
    QLabel* lblSpielerHeartRateAvg = new QLabel(QString::number(this->getHeartRate()));
    layout->addWidget(lblHeartRateAvg, 2, 0);
    layout->addWidget(lblSpielerHeartRateAvg, 2, 1);

    basicLayout->addLayout(layout);
    basicLayout->setAlignment(layout, Qt::AlignTop);

    widgetToDisplay->setLayout(basicLayout);

    this->setSliderValues(this->mAllPlayerData.firstKey(), this->mAllPlayerData.lastKey(), 0);
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

QTime Spieler::getSynchTime() const
{
    return synchTime;
}

void Spieler::calcSynchTime(QTime aSynchTime)
{
    int hour = aSynchTime.hour() - mStartTime.hour();
    int min = aSynchTime.minute() - mStartTime.minute();
    int sec = aSynchTime.second() - mStartTime.second();

    mTimeDiffStartSynchAsInt = sec + (min*60) + (hour*3600);
    mTimeDiffStartSynch.setHMS(hour,min,sec);

}

QTime Spieler::getStartTime() const
{
    return mStartTime;
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
     data.mActivityType = elements[1].toInt();

     // check if we reach the end
     if(data.mActivityType == -1)
         break;

     data.mLapNumber = elements[2].toInt();
     data.mDistance = elements[3].toDouble();
     data.mSpeed = elements[4].toDouble();
     data.mCalories = elements[5].toDouble();
     data.mXvalue = elements[6].toDouble();
     data.mYvalue = elements[7].toDouble();
     data.mElevation = elements[8].toDouble();
     data.mHeartRate = elements[9].toInt();
     data.mCycles = elements[10].toInt();


     mAllPlayerData.insert(timestamp,data);

     if (data.mXvalue > xValueMax )
       xValueMax = data.mXvalue;

     if (data.mXvalue < xValueMin )
       xValueMin = data.mXvalue;

     if (data.mYvalue > yValueMax )
       yValueMax = data.mYvalue;

     if (data.mYvalue < yValueMin )
       yValueMin = data.mYvalue;
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
