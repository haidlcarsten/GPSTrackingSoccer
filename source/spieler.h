#ifndef SPIELER_H
#define SPIELER_H

#include <QObject>
#include "chartwidget.h"
#include <qdebug.h>
#include"typedef.h"
#include <QObject>
#include <QMap>
#include <QPair>

class Spieler : public QObject
{
  Q_OBJECT
public:
  Spieler(QString aPfad);

  QString getName();
  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}
  void setSlider(QSlider* aSlider);
  void setSliderValues(int aMin, int aMax, int aValue);
  void parseData();

  double getSpeed();
  double average_speed();  //calculate average_speed of the player

  float getHeartRate();
  float average_heartRate();  //calculate average_heartrate of the player

  QPair<double, double> getCornerBottomLeft() const;

  QPair<double, double> getCornerBottomRight() const;

  QPair<double, double> getCornerTopLeft() const;

  QPair<double, double> getCornerTopRight() const;

signals:

public slots:
  void displayData(bool aDisplay);
  void displayFrameData(int aTime);

private:
   QString mSpielername;
   ChartWidget* mChartWidget;
   QSlider* mSlider;
   QString mPfad;
   QMap<int,parsedData> playerData;
   double a_speed; //average speed
   float a_hrate; //average heartrate

   QPair<double,double> mCornerBottomLeft;
   QPair<double,double> mCornerBottomRight;
   QPair<double,double> mCornerTopLeft;
   QPair<double,double> mCornerTopRight;



};

#endif // SPIELER_H
