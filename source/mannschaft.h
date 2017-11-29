#ifndef MANNSCHAFT_H
#define MANNSCHAFT_H

#include <QObject>
#include <QPair>
#include "spieler.h"
#include "chartwidget.h"

class QLayout;

/**
 * @brief The Mannschaft class
 * a Mannschaft contains a list of players.
 * It can display the players and manage them
 */
class Mannschaft : public QObject
{
  Q_OBJECT
public:
  /**
   * @brief Mannschaft
   * default-constructor
   *
   * @param parent
   * which widget should be parent
   */
  explicit Mannschaft(QObject *parent = 0);

  /**
   * @brief displaySpieler
   * creates an widget to display the players
   *
   * @return
   * widget to display the players
   */
  QWidget *displaySpieler();

  /**
   * @brief neuerSpieler
   * creates a new player
   *
   * @param aPfad
   * path to player-data
   */
  void neuerSpieler(QString aPfad);

  /**
   * @brief neueSpieler
   * create new players
   *
   * @param aPfade
   * a list of player-filpath
   */
  void neueSpieler(QStringList aPfade);

  /**
   * @brief setChartWidget
   * a widget to display the data
   *
   * @param aWidget
   * a widget to display
   */
  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}

  /**
   * @brief setSlider
   * slider to control the timestamp
   *
   * @param aSlider
   * a slider to control
   */
  void setSlider(QSlider* aSlider) {this->mSlider = aSlider;}

  /**
   * @brief getTeamSpeed
   * getter for team-speet
   *
   * @return
   * average of team-speed
   */
  double getTeamAverageSpeed();

  /**
   * @brief getTeamAverageHeartrate
   * getter for the average heartrate
   *
   * @return
   * average team heartrate
   */
  float getTeamAverageHeartrate();

  /**
   * @brief calcAverageSpeed
   * calculates the average speed of the team
   *
   * @return
   * average speed of the team
   */
  double calcAverageSpeed();

  /**
   * @brief calcAverageHeartrate
   * calculates the average speed of the team
   *
   * @return
   * average heartrate of the team
   */
  float calcAverageHeartrate();  //calculate team_average_heartrate of the player

  /**
   * @brief calcMeanCornePoint
   * calculates the mean of the corner points based on all player
   */
  void calcMeanCornePoint();

  /**
   * @brief calcSynchPoint
   * calculates a synch-point for all players
   */
  void calcSynchPoint();

signals:
  /**
   * @brief playerChanged
   * signal emit, after player-data changed
   */
  void playerChanged();

public slots:

private:
//---------- member-variable ----------
  QList<Spieler*> mListSpieler;
  ChartWidget* mChartWidget;
  QSlider* mSlider;

  QTime mSynchPoint;
  double mAvgSpeed;
  float mAvgHeartrate;

  QPair<double,double> mCornerBottomLeft;
  QPair<double,double> mCornerBottomRight;
  QPair<double,double> mCornerTopLeft;
  QPair<double,double> mCornerTopRight;


};

#endif // MANNSCHAFT_H
