#ifndef MANNSCHAFT_H
#define MANNSCHAFT_H

#include <QObject>
#include "spieler.h"
#include "chartwidget.h"
#include <QPair>

class QLayout;

class Mannschaft : public QObject
{
  Q_OBJECT
public:
  explicit Mannschaft(QObject *parent = 0);

  QWidget *displaySpieler();

  void neuerSpieler(QString aPfad);
  void neueSpieler(QStringList aPfade);

  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}
  void setSlider(QSlider* aSlider) {this->mSlider = aSlider;}

  double get_Team_Speed();
  double t_average_speed();  //calculate team_average_speed of the player

  float get_Team_HeartRate();
  float t_average_heartRate();  //calculate team_average_heartrate of the player

  void meanCornePoint();

  void synch_point();

signals:
  void playerChanged();

public slots:

private:
  QList<Spieler*> mListSpieler;
  ChartWidget* mChartWidget;
  QSlider* mSlider;

  double t_speed;
  float t_heartrate;

  QPair<double,double> mCornerBottomLeft;
  QPair<double,double> mCornerBottomRight;
  QPair<double,double> mCornerTopLeft;
  QPair<double,double> mCornerTopRight;

  QTime synchPoint;

};

#endif // MANNSCHAFT_H
