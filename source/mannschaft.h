#ifndef MANNSCHAFT_H
#define MANNSCHAFT_H

#include <QObject>
#include "spieler.h"
#include "chartwidget.h"

class QLayout;

class Mannschaft : public QObject
{
  Q_OBJECT
public:
  explicit Mannschaft(QObject *parent = 0);

  QLayout* displaySpieler();

  void neuerSpieler(QString aPfad);
  void neueSpieler(QStringList aPfade);

  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}
  void setSlider(QSlider* aSlider) {this->mSlider = aSlider;}

signals:
  void playerChanged();

public slots:

private:
  QList<Spieler*> mListSpieler;
  ChartWidget* mChartWidget;
  QSlider* mSlider;
};

#endif // MANNSCHAFT_H
