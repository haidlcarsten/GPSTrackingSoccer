#ifndef SPIELER_H
#define SPIELER_H

#include <QObject>

class Spieler : public QObject
{
  Q_OBJECT
public:
  Spieler(QString aPfad);

signals:

public slots:

private:
   QString mSpielername;
};

#endif // SPIELER_H
