#ifndef MANNSCHAFT_H
#define MANNSCHAFT_H

#include <QObject>
#include "spieler.h"

class Mannschaft : public QObject
{
  Q_OBJECT
public:
  explicit Mannschaft(QObject *parent = 0);

  void displaySpieler();

  void neuerSpieler(QString aPfad);
  void neueSpieler(QStringList aPfade);

signals:

public slots:

private:
  QList<Spieler*> mListSpieler;
};

#endif // MANNSCHAFT_H
