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
#include <QtMath>

#include <QtCharts>

Spieler::Spieler(QString aPfad)
{ 
#ifdef Q_OS_LINUX
  this->mSettings = new QSettings("." + QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

#ifdef Q_OS_WIN
  this->mSettings = new QSettings(QApplication::applicationDirPath().left(1) + SETTINGS_FILE_PATH, QSettings::IniFormat);
#endif

  this->mPfad = aPfad;

  // getting the filename
  QFileInfo info(aPfad);
  this->mFilename = info.fileName();

  // at the beginnen playername is equal  to filename
  this->mSpielerName = this->mFilename;

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

QColor Spieler::getColor()
{
  return this->mPlayerColor;
}

double Spieler::getSpeed()
{
  return this->mAvgSpeed;
}

double Spieler::calcAverageSpeed()
{
    double speed_sum = 0.0f;
    int devideCount = 0;
    foreach (int tstamp, this->mTransformedPlayerData.keys())
    {
      parsedData data = this->mTransformedPlayerData.value(tstamp);
      if(data.mActivityType != -1)
      {
        speed_sum = speed_sum + data.mSpeed;
        devideCount++;
      }
    }
    mAvgSpeed = speed_sum / (float)devideCount;
    return mAvgSpeed;
}

float Spieler::calcAverageHeartRate()
{
  this->mAvgHeartRate = 0.0f;
  int devideCount = 0;

  foreach (int tstamp, this->mTransformedPlayerData.keys())
  {
    parsedData data = this->mTransformedPlayerData.value(tstamp);
    if(data.mActivityType != -1)
    {
      this->mAvgHeartRate += data.mHeartRate;
      devideCount++;
    }
  }

  this->mAvgHeartRate /= (float)devideCount;

  return this->mAvgHeartRate;
}

float Spieler::getHeartRate()
{
  return this->mAvgHeartRate;
}


void Spieler::setSlider(QSlider *aSlider)
{
  this->mSlider = aSlider;
  connect(this->mSlider, &QSlider::valueChanged, this, &Spieler::displayData);
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

    int skip = this->mSettings->value(SETTINGS_SKIP_PLAYERDATA).toInt();
    double markersize = this->mSettings->value(SETTINGS_MARKERSIZE_PLAYERDATA).toDouble();

    this->transfromPlayerData();

    QScatterSeries *seriesdata = new QScatterSeries();
    seriesdata->setName(this->getPlayerName());
    seriesdata->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    seriesdata->setMarkerSize(markersize);

    for(int i = 0; i < this->mSlider->value(); i+= skip+1)
    {
      parsedData data = this->mTransformedPlayerData.value(i);
      if(data.mActivityType != -1)
        seriesdata->append(data.mLongitude,data.mLatitude);
    }

    QChart *chart = new QChart();
    chart->addSeries(seriesdata);

    chart->setTitle(this->getPlayerName() + "Route");
    chart->createDefaultAxes();
    chart->setDropShadowEnabled(false);

    parsedData data = this->mTransformedPlayerData.value(0);
    QValueAxis* axisX = new QValueAxis;
    axisX->setRange(data.cLeftBottomLong, data.cRightBottomLong);
    axisX->setTickCount(1);
    axisX->setLabelFormat("%.6f");

    QValueAxis* axisY = new QValueAxis;
    axisY->setRange(data.cLeftBottomLat, data.cLeftTopLat);
    axisY->setTickCount(1);
    axisY->setLabelFormat("%.6f");

    chart->setAxisX(axisX, seriesdata);
    chart->setAxisY(axisY, seriesdata);

    QChartView *chartView = new QChartView(chart);
    //Add background image for plot Area

    QPixmap field("C:/Users/kotar/Desktop/git_ch/GPSTrackingSoccer/fussballplatz-fertig2.jpg");

    QSizeF  dimension = chart->size();
    qreal width = dimension.width();
    qreal height = dimension.height();

   field.scaled(QSize(width,height));

    if(&field != NULL)
    {
        chart->setPlotAreaBackgroundBrush(field);
        chart->setPlotAreaBackgroundVisible(true);
    }
//    QPainter painter(&field);
//    QPointF TopLeft = chart->plotArea().topLeft();
//    painter.drawImage(TopLeft,&field);

    lyForm->addWidget(datawidget, 0, 0);
    lyForm->addWidget(chartView, 1, 0);

    widgetToDisplay = new QWidget;
    widgetToDisplay->setLayout(lyForm);
  }
  else
    widgetToDisplay = this->mChartWidget->getDefaultWidget();

  this->mChartWidget->setWidget(widgetToDisplay);
}

void Spieler::synchPlayerData()
{
  // parse all data
  this->parseData();

  // if timediff is zero, we dont have to synch
  if(this->mTimeDiffStartSynchAsInt == 0)
  {
    // same data, because no time-shift
    this->mSynchPlayerData = this->mAllPlayerData;
  }
  else
  {
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
        data.mActivityType = -1; // mark as invalid

      this->mSynchPlayerData.insert(i, data);
    }
  }

  this->transfromPlayerData();

  this->calcAverageSpeed();

  this->calcAverageHeartRate();

  this->mAllPlayerData.clear();
  this->mSynchPlayerData.clear();
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
  QLabel* lblSpielerSpeedAvg = new QLabel(QString::number( this->getSpeed()) + " m/s");
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

int Spieler::getPlayerNumber()
{
  return this->mSpielerNummer;
}

parsedData Spieler::getTransformedPlayerData(int aTime)
{
  if(this->mDataIsTransformed)
    return this->mTransformedPlayerData.value(aTime);

  this->transfromPlayerData();
  return this->mTransformedPlayerData.value(aTime);
}

int Spieler::getMaximumTimestamp()
{
    return this->mTransformedPlayerData.lastKey();
}

void Spieler::recalculate()
{
  this->mDataIsTransformed = false;
  this->synchPlayerData();
}

void Spieler::transfromPlayerData()
{
  if(this->mDataIsTransformed)
    return;

  double bottomLeftLong = this->mSettings->value(SETTINGS_COORDINATES_BOTTOM_LEFT_LONGITUDE).toDouble();
  double bottomLeftLat = this->mSettings->value(SETTINGS_COORDINATES_BOTTOM_LEFT_LATITUDE).toDouble();

  double bottomrightLong = this->mSettings->value(SETTINGS_COORDINATES_BOTTOM_RIGHT_LONGITUDE).toDouble();
  double bottomrightLat = this->mSettings->value(SETTINGS_COORDINATES_BOTTEM_RIGHT_LATITUDE).toDouble();

  double toprightLong = this->mSettings->value(SETTINGS_COORDINATES_TOP_RIGHT_LONGITUDE).toDouble();
  double toprightLat = this->mSettings->value(SETTINGS_COORDINATES_TOP_RIGHT_LATITUDE).toDouble();

  double topleftLong = this->mSettings->value(SETTINGS_COORDINATES_TOP_LEFT_LONGITUDE).toDouble();
  double topleftLat = this->mSettings->value(SETTINGS_COORDINATES_TOP_LEFT_LATITUDE).toDouble();

//  double rad = qAtan2((bottomrightLat - bottomLeftLat), (bottomrightLong - bottomLeftLong));
//  double ratio = rad * (180.0/M_PI);
//  double degree = 0.0;

  //projection to mercator map
  double cornerLeftBottomLat = qLn(qTan(bottomLeftLat*(M_PI/180.0)) + (1/qCos(bottomLeftLat*(M_PI/180.0))));
  double cornerLeftBottomLong = bottomLeftLong*(M_PI/180.0);
  double cornerLeftTopLat = qLn(qTan(topleftLat*(M_PI/180.0)) + (1/qCos(topleftLat*(M_PI/180.0))));
  double cornerLeftTopLong = topleftLong*(M_PI/180.0);
  double cornerRightTopLat = qLn(qTan(toprightLat*(M_PI/180.0)) + (1/qCos(toprightLat*(M_PI/180.0))));
  double cornerRightTopLong = toprightLong*(M_PI/180.0);
  double cornerRightBottomLat = qLn(qTan(bottomrightLat*(M_PI/180.0)) + (1/qCos(bottomrightLat*(M_PI/180.0))));
  double cornerRightBottomLong = bottomrightLong*(M_PI/180.0);


  //vector to corner point
  double cltopLat = cornerLeftTopLat - cornerLeftBottomLat;
  double cltopLong = cornerLeftTopLong - cornerLeftBottomLong;
  double crtopLat = cornerRightTopLat - cornerLeftBottomLat;
  double crtopLong = cornerRightTopLong -cornerLeftBottomLong ;
  double crbotLat = cornerRightBottomLat - cornerLeftBottomLat;
  double crbotLong = cornerRightBottomLong - cornerLeftBottomLong;

  //calculate angle
  double rad = qAtan2(crbotLat, crbotLong);
  qreal ratio = rad * (180.0/M_PI);
  qreal degree = 0.0;

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

    //rotate in positive direction
    if (ratio < 0)
    {
        degree = qAbs(rad) ;

        //calculation in radiant-mod

            dataTransform.cLeftBottomLat     = 0.00 ; // reference point y_value
            dataTransform.cLeftBottomLong    = 0.00 ; // reference point x_value
            dataTransform.cLeftTopLat        = (cltopLat * qCos(degree) + cltopLong * qSin(degree))  ;
            dataTransform.cLeftTopLong       = (cltopLong * qCos(degree) - cltopLat * qSin(degree))  ;
            dataTransform.cRightBottomLat    = (crbotLat * qCos(degree) + crbotLong * qSin(degree))   ;
            dataTransform.cRightBottomLong   = (crbotLong * qCos(degree) - crbotLat * qSin(degree))  ;
            dataTransform.cRightTopLat       = (crtopLat * qCos(degree) + crtopLong * qSin(degree))  ;
            dataTransform.cRightTopLong      = (crtopLong * qCos(degree) - crtopLat * qSin(degree)) ;


//            reserve = dataTransform;
//           //linear Transformation
//           // T(ax+by,cx+dy)
//            dataTransform.cLeftBottomLat     = 0.00 ; // reference point y_value
//            dataTransform.cLeftBottomLong    = 0.00 ; // reference point x_value
//            dataTransform.cLeftTopLat        = (2/(reserve.cRightBottomLong*reserve.cLeftTopLat))*0.5*reserve.cRightBottomLong*reserve.cLeftTopLat ;
//            dataTransform.cLeftTopLong       = (2/(reserve.cRightBottomLong*reserve.cLeftTopLat))*(reserve.cLeftTopLat * reserve.cLeftTopLong) ;
//            dataTransform.cRightBottomLat    = (2/(reserve.cRightBottomLong*reserve.cLeftTopLat))*0.5*reserve.cRightBottomLong*reserve.cRightBottomLat ;
//            dataTransform.cRightBottomLong   = (2/(reserve.cRightBottomLong*reserve.cLeftTopLat))*(reserve.cLeftTopLat * reserve.cRightBottomLong) ;
//            dataTransform.cRightTopLat       = (2/(reserve.cRightBottomLong*reserve.cLeftTopLat))*0.5*reserve.cRightBottomLong*reserve.cRightTopLat ;
//            dataTransform.cRightTopLong      = (2/(reserve.cRightBottomLong*reserve.cLeftTopLat))*(reserve.cLeftTopLat * reserve.cRightTopLong) ;


         double coordLatitude = qLn(qTan(data.mLatitude *(M_PI/180.0)) + (1/qCos(data.mLatitude *(M_PI/180.0))));
         double coordLongitude = data.mLongitude *(M_PI/180.0);

         double pdlong = coordLongitude - cornerLeftBottomLong;
         double pdlat = coordLatitude - cornerLeftBottomLat;

         dataTransform.mLatitude     = (pdlat * qCos(degree) + pdlong * qSin(degree))  ;
         dataTransform.mLongitude    = (pdlong* qCos(degree) - pdlat * qSin(degree)) ;

    }
    else    //rotate in negative direction
    {
        degree = rad ;
        dataTransform.cLeftBottomLat     = 0.00; // reference point y_value
        dataTransform.cLeftBottomLong    = 0.00; // reference point x_value
        dataTransform.cLeftTopLat        = (cltopLat * qCos(degree) - cltopLong * qSin(degree))  ;
        dataTransform.cLeftTopLong       = (cltopLong * qCos(degree) + cltopLat * qSin(degree))  ;
        dataTransform.cRightBottomLat    = (crbotLat * qCos(degree) - crbotLong * qSin(degree))  ;
        dataTransform.cRightBottomLong   = (crbotLong * qCos(degree) + crbotLat * qSin(degree))  ;
        dataTransform.cRightTopLat       = (crtopLat * qCos(degree) - crtopLong * qSin(degree))  ;
        dataTransform.cRightTopLong      = (crtopLong * qCos(degree) + crtopLat * qSin(degree))  ;


        double coordLatitude = qLn(qTan(data.mLatitude *(M_PI/180.0)) + (1/qCos(data.mLatitude *(M_PI/180.0))));
        double coordLongitude = data.mLongitude *(M_PI/180.0);

        double pdlong = coordLongitude - cornerLeftBottomLong;
        double pdlat = coordLatitude - cornerLeftBottomLat;

        dataTransform.mLatitude     = (pdlat * qCos(degree) - pdlong * qSin(degree))  ;
        dataTransform.mLongitude    = (pdlong* qCos(degree) + pdlat * qSin(degree)) ;
     }


    //detect player outside of the field
    //  (dataTransform.mLatitude < dataTransform.cLeftBottomLat-0.000001 && dataTransform.mLongitude > dataTransform.cLeftBottomLong )
    //              || (dataTransform.mLongitude < dataTransform.cLeftBottomLong - 0.000001 && dataTransform.mLatitude > dataTransform.cLeftBottomLat )
    //              || (dataTransform.mLatitude < dataTransform.cLeftBottomLat - 0.000001 && dataTransform.mLongitude < dataTransform.cLeftBottomLong - 0.000001 )
    //              || (dataTransform.mLongitude > dataTransform.cRightBottomLong + 0.000001 && dataTransform.mLatitude < dataTransform.cRightBottomLat - 0.000001 )
    //              || (dataTransform.mLongitude > dataTransform.cRightBottomLong + 0.000001 && dataTransform.mLatitude < dataTransform.cRightTopLat )
    //              || (dataTransform.mLongitude > dataTransform.cRightTopLong + 0.000001 && dataTransform.mLatitude > dataTransform.cRightTopLat + 0.000001)
    //              || (dataTransform.mLongitude > dataTransform.cLeftTopLong && dataTransform.mLatitude > dataTransform.cRightTopLat + 0.000001)
    //              || (dataTransform.mLongitude < dataTransform.cLeftTopLong - 0.000001 && dataTransform.mLatitude > dataTransform.cLeftTopLat + 0.000001))

      bool bool1 = dataTransform.mLatitude < dataTransform.cLeftBottomLat-0.000001;
      bool bool2 = dataTransform.mLongitude > dataTransform.cLeftBottomLong;

      bool bool3 = dataTransform.mLatitude > dataTransform.cLeftBottomLat;
      bool bool4 = dataTransform.mLongitude < dataTransform.cLeftBottomLong - 0.000001;

      bool bool5 = dataTransform.mLatitude < dataTransform.cLeftBottomLat - 0.000001;
      bool bool6 = dataTransform.mLongitude < dataTransform.cLeftBottomLong - 0.000001;

      bool bool7 = dataTransform.mLatitude < dataTransform.cRightBottomLat - 0.000001;
      bool bool8 = dataTransform.mLongitude > dataTransform.cRightBottomLong + 0.000001;

      bool bool9 = dataTransform.mLatitude < dataTransform.cRightTopLat;
      bool bool10 = dataTransform.mLongitude > dataTransform.cRightBottomLong + 0.000001;

      bool bool11 = dataTransform.mLatitude > dataTransform.cRightTopLat + 0.000001;
      bool bool12 = dataTransform.mLongitude > dataTransform.cRightTopLong + 0.000001;

      bool bool13 = dataTransform.mLatitude > dataTransform.cRightTopLat + 0.000001;
      bool bool14 = dataTransform.mLongitude > dataTransform.cLeftTopLong;

      bool bool15 = dataTransform.mLatitude > dataTransform.cLeftTopLat + 0.000001;
      bool bool16 = dataTransform.mLongitude < dataTransform.cLeftTopLong - 0.000001 ;


    if ((bool1 && bool2) || (bool3 && bool4) || (bool5 && bool6)|| (bool7 && bool8)|| (bool9 && bool10)
            || (bool11 && bool12)|| (bool13 && bool14)|| (bool15 && bool16))
    {
        dataTransform.mActivityType = -1;
    }


    this->mTransformedPlayerData.insert(time, dataTransform);
  }

  this->mDataIsTransformed = true;
}

void Spieler::displayFrameData(int aTime)
{
  Q_UNUSED(aTime)
  this->displayData(true);
}

void Spieler::setPlayerName(QString aText)
{
  this->mSpielerName = aText;
}

void Spieler::setPlayerNumber(QString aText)
{
  this->mSpielerNummer = aText.toInt();
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
     data.mLatitude   = elements[6].toDouble();
     data.mLongitude  = elements[7].toDouble();
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
