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


#include <QtCharts>

Spieler::Spieler(QString aPfad)
{ 
  this->mPfad = aPfad;

  // getting the filename
  QFileInfo info(aPfad);
  this->mFilename = info.fileName();

  // at the beginnen playername is equal  to filename
  this->mSpielerName = this->mFilename;

  // parse all data
  this->parseData();

  // getting the timestamp from filename
  // the filename should have the structure "name-yyyymmddThhmmss.csv"
  QString timestamp;
  for (int i = this->mFilename.length()-1; i > 0; --i)
  {
    if(this->mFilename.at(i) != '.')
        continue;
    for (int j = 1;  j <= 6; j++)
    {
       timestamp.append(this->mFilename.at(i-j));
    }
    break ;
  }

  std::reverse(timestamp.begin(),timestamp.end());
  this->mStartTime = QTime::fromString(timestamp,"hhmmss");

  // at the begin the timediff is zero
  this->mTimeDiffStartSynchAsInt = 0;
  this->mTimeDiffStartSynch = QTime(0, 0, 0);

  this->calcAverageSpeed();
  this->calcAverageHeartRate();
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
  // creates an widget to show the information to an player
  QWidget* widgetToDisplay; // just a holder

  // if we should display the player or not
  if(aDisplay)
  {
    QGridLayout* lyForm = new QGridLayout;

    auto datawidget = this->generatePlayerDataWidget();
    this->displayFrameData(this->mSlider->value());

    lyForm->addWidget(datawidget, 0, 0);

    if(this->mPlayerCoordWidget)
      lyForm->addWidget(this->mPlayerCoordWidget, 1, 0);

    widgetToDisplay = new QWidget;
    widgetToDisplay->setLayout(lyForm);

    this->setSliderValues(this->mSynchPlayerData.firstKey(), this->mSynchPlayerData.lastKey(), 0);
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
  this->mSlider->setMinimum(aMin);
  this->mSlider->setMaximum(aMax);
  this->blockSignals(true);
  this->mSlider->setValue(aValue);
  this->blockSignals(false);
}

void Spieler::synchPlayerData()
{
  // if timediff is zero, we dont have to synch
  if(this->mTimeDiffStartSynchAsInt == 0)
  {
    // same data, because no time-shift
    this->mSynchPlayerData = this->mAllPlayerData;
    return;
  }

  int amountKeys = this->mAllPlayerData.keys().count();

  for(int i = 0; i < amountKeys; i++)
  {
    // in this case, we dont need to check the key, because the key is just a number

    int pos = i + this->mTimeDiffStartSynchAsInt;
    parsedData data;
    if(pos <= amountKeys)
      data = this->mAllPlayerData.value(pos);

    // in case we have no mor data but we need more, we have to insert dummy-data
    if(pos > amountKeys)
    {
      data.mActivityType = -1; // mark as invalid
      data.mLapNumber  = 0;
      data.mDistance   = 0.0;
      data.mSpeed      = 0.0;
      data.mCalories   = 0.0;
      data.mLatitude     = 0.0;
      data.mLongitude     = 0.0;
      data.mElevation  = 0.0;
      data.mHeartRate  = 0;
      data.mCycles     = 0;
   }

    this->mSynchPlayerData.insert(i, data);
  }
}

QWidget *Spieler::generatePlayerDataWidget()
{
  QWidget* result = new QWidget();

  QVBoxLayout* basicLayout = new QVBoxLayout;

  QGridLayout* layout = new QGridLayout();

  QLabel* lblName = new QLabel(PLAYER_NAME);
  QLabel* lblSpielerName = new QLabel(this->getPlayerName());
  layout->addWidget(lblName, 0, 0);
  layout->addWidget(lblSpielerName, 0, 1);

  QLabel* lblSpeedAvg = new QLabel(PLAYER_AVERAGE_SPEED);
  QLabel* lblSpielerSpeedAvg = new QLabel(QString::number(this->getSpeed()) + " m/s");
  layout->addWidget(lblSpeedAvg, 1, 0);
  layout->addWidget(lblSpielerSpeedAvg, 1, 1);

  QLabel* lblHeartRateAvg = new QLabel(PLAYER_AVERAGE_HEARTRATE);
  QLabel* lblSpielerHeartRateAvg = new QLabel(QString::number(this->getHeartRate()) + " bpm");
  layout->addWidget(lblHeartRateAvg, 2, 0);
  layout->addWidget(lblSpielerHeartRateAvg, 2, 1);

  basicLayout->addLayout(layout);
  basicLayout->setAlignment(layout, Qt::AlignTop);

  result->setLayout(basicLayout);

  return result;
}

void Spieler::transfromPlayerData()
{
  if(this->mDataIsTransformed)
    return;

#ifdef Q_OS_LINUX
  QSettings settings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

#ifdef Q_OS_WIN
  QSettings settings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  double settingsLongitude = settings.value(SETTINGS_COORDINATES_BOTTOM_LEFT_LONGITUDE).toDouble();
  double settingsLatitude = settings.value(SETTINGS_COORDINATES_BOTTOM_LEFT_LATITUDE).toDouble();

  // transform the data

  foreach (int time, this->mSynchPlayerData.keys())
  {
    parsedData data = this->mSynchPlayerData.value(time);

    parsedData dataTransform;

    // set same values
    dataTransform.mActivityType = data.mActivityType;
    dataTransform.mCalories     = data.mCalories;
    dataTransform.mCycles       = data.mCycles;
    dataTransform.mDistance     = data.mDistance;
    dataTransform.mHeartRate    = data.mHeartRate;
    dataTransform.mElevation    = data.mElevation;
    dataTransform.mLapNumber    = data.mLapNumber;
    dataTransform.mSpeed        = data.mSpeed;

    // transform the coordinates
    dataTransform.mLatitude     = data.mLatitude - settingsLatitude;
    dataTransform.mLongitude    = data.mLongitude - settingsLongitude;

    this->mTransformedPlayerData.insert(time, dataTransform);
  }

  this->mDataIsTransformed = true;
}

void Spieler::displayFrameData(int aTime)
{
  Q_UNUSED(aTime)

  this->transfromPlayerData();


  QChart *chart = new QChart();
  QChartView *chartView = new QChartView(chart);

  this->mPlayerCoordWidget = chartView;
}

void Spieler::setPlayerName(QString aText)
{
  this->mSpielerName = aText;
}

QTime Spieler::getSynchTime() const
{
    return this->mSynchTime;
}

void Spieler::calcSynchTime(QTime aSynchTime)
{
  // calculate the difference between the given time to our starttime, the given time should alway be later or same
  int hour = aSynchTime.hour() - this->mStartTime.hour();
  int min = aSynchTime.minute() - this->mStartTime.minute();
  int sec = aSynchTime.second() - this->mStartTime.second();

  this->mTimeDiffStartSynchAsInt = sec + (min*60) + (hour*3600);
  this->mTimeDiffStartSynch.setHMS(hour,min,sec);

  // synch the playerdata
  this->synchPlayerData();
}

QTime Spieler::getStartTime() const
{
  return this->mStartTime;
}

QPair<double, double> Spieler::getCornerTopRight() const
{
  return this->mCornerTopRight;
}

QPair<double, double> Spieler::getCornerTopLeft() const
{
  return this->mCornerTopLeft;
}

QPair<double, double> Spieler::getCornerBottomRight() const
{
  return this->mCornerBottomRight;
}

QPair<double, double> Spieler::getCornerBottomLeft() const
{
  return this->mCornerBottomLeft;
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

     // getting all elements by splitting at ','
     QStringList elements = line.trimmed().remove(' ').split(',');

     parsedData data;
     int timestamp = elements[0].toInt();
     data.mActivityType = elements[1].toInt();

     // check if we reach the end
     if(data.mActivityType == -1)
         break;

     // copy our parsed data
     data.mLapNumber  = elements[2].toInt();
     data.mDistance   = elements[3].toDouble();
     data.mSpeed      = elements[4].toDouble();
     data.mCalories   = elements[5].toDouble();
     data.mLatitude     = elements[6].toDouble();
     data.mLongitude     = elements[7].toDouble();
     data.mElevation  = elements[8].toDouble();
     data.mHeartRate  = elements[9].toInt();
     data.mCycles     = elements[10].toInt();

     // insert data
     this->mAllPlayerData.insert(timestamp,data);

     // getting the max and min of all values
     if (data.mLatitude > xValueMax)
       xValueMax = data.mLatitude;

     if (data.mLatitude < xValueMin)
       xValueMin = data.mLatitude;

     if (data.mLongitude > yValueMax)
       yValueMax = data.mLongitude;

     if (data.mLongitude < yValueMin)
       yValueMin = data.mLongitude;
    }

    // set all corners of one player
    this->mCornerBottomLeft.first   = xValueMin;
    this->mCornerBottomLeft.second  = yValueMin;

    this->mCornerTopRight.first     = xValueMax;
    this->mCornerTopRight.second    = yValueMax;

    this->mCornerBottomRight.first  = xValueMax;
    this->mCornerBottomRight.second = yValueMin;

    this->mCornerTopLeft.first      = xValueMin;
    this->mCornerTopLeft.second     = yValueMax;


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
