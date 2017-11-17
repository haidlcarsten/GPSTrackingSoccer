#ifndef SPIELER_H
#define SPIELER_H

#include <QObject>
#include "chartwidget.h"
#include <qdebug.h>
#include"typedef.h"
#include <QObject>
#include <QMap>

class Spieler : public QObject
{
  Q_OBJECT
public:
  Spieler(QString aPfad);

  QString getName();
  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}
  void setSlider(QSlider* aSlider);
  void setSliderValues(int aMin, int aMax, int aValue);
  void parseData ();


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

};

#endif // SPIELER_H
