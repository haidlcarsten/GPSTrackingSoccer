#ifndef SPIELER_H
#define SPIELER_H

#include <QObject>
#include "chartwidget.h"
#include <qdebug.h>

class Spieler : public QObject
{
  Q_OBJECT
public:
  Spieler(QString aPfad);

  QString getName();
  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}
  void setSlider(QSlider* aSlider) {this->mSlider = aSlider;}

signals:

public slots:
  void displayData();

private:
   QString mSpielername;
   ChartWidget* mChartWidget;
   QSlider* mSlider;
};

#endif // SPIELER_H
