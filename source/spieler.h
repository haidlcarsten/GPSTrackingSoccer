#ifndef SPIELER_H
#define SPIELER_H

#include <QObject>
#include "chartwidget.h"
#include <qdebug.h>
#include"typedef.h"
#include <QObject>
#include <QMap>
#include <QPair>
#include <QTime>

/**
 * @brief The Spieler class
 * a player has different properties. It holds his own data and calculates some
 */
class Spieler : public QObject
{
  Q_OBJECT
public:

  /**
   * @brief Spieler
   * default-constructor
   *
   * @param aPfad
   * the path to the data-file
   */
  Spieler(QString aPfad);

  /**
   * @brief setChartWidget
   * setter for the chart widget
   *
   * @param aWidget
   * chart widget to set
   */
  void setChartWidget(ChartWidget* aWidget) {this->mChartWidget = aWidget;}

  /**
   * @brief setSlider
   * setter for the slider to display the player-data
   *
   * @param aSlider
   * slider to set
   */
  void setSlider(QSlider* aSlider);

  /**
   * @brief getFileName
   * getter for file-name
   *
   * @return
   * filename
   */
  QString getFileName() const;

  /**
   * @brief getPlayerName
   * getter for player-name
   *
   * @return
   * playername
   */
  QString getPlayerName() const;

  /**
   * @brief getSpeed
   * getter for the average speed of the player
   *
   * @return
   * average speed
   */
  double getSpeed();

  /**
   * @brief getHeartRate
   * getter for the average heartrate
   *
   * @return
   * average heartrate
   */
  float getHeartRate();

  /**
   * @brief getCornerBottomLeft
   * getter for corner bottom left
   *
   * @return
   * 2D-Vector of corner
   */
  QPair<double, double> getCornerBottomLeft() const;

  /**
   * @brief getCornerBottomLeft
   * getter for corner bottom right
   *
   * @return
   * 2D-Vector of corner
   */
  QPair<double, double> getCornerBottomRight() const;

  /**
   * @brief getCornerBottomLeft
   * getter for corner top left
   *
   * @return
   * 2D-Vector of corner
   */
  QPair<double, double> getCornerTopLeft() const;

  /**
   * @brief getCornerBottomLeft
   * getter for corner top right
   *
   * @return
   * 2D-Vector of corner
   */
  QPair<double, double> getCornerTopRight() const;

  /**
   * @brief getStartTime
   * getter of the start-time for the player
   *
   * @return
   * time when the run starts
   */
  QTime getStartTime() const;

  /**
   * @brief getSynchTime
   * getter for synch-time
   *
   * @return
   * synchtime
   */
  QTime getSynchTime() const;

  /**
   * @brief parseData
   * parses all data from the filename
   */
  void parseData();

  /**
   * @brief calcAverageSpeed
   * calculates the average speed of the player
   *
   * @return
   * average speed
   */
  double calcAverageSpeed();  //calculate average_speed of the player

  /**
   * @brief calcAverageHeartRate
   * calculates the average heartrate
   *
   * @return
   * average heartrate
   */
  float calcAverageHeartRate();  //calculate average_heartrate of the player

  /**
   * @brief calcSynchTime
   * calculates the synch-time to a given reference-time
   *
   * @param aSynchTime
   * reference-time
   */
  void calcSynchTime(QTime aSynchTime);

signals:

public slots:
  /**
   * @brief displayData
   * display all data to an player (name, speed,...)
   *
   * @param aDisplay
   * bool if data should be displayed or an default-widget will be shown
   */
  void displayData(bool aDisplay);

  /**
   * @brief displayFrameData
   * display the data from the synchPoint til the given timestamp
   *
   * @param aTime
   * given time
   */
  void displayFrameData(int aTime);

  /**
   * @brief setPlayerName
   * slot for setting the playername
   *
   * @param aText
   * new playername
   */
  void setPlayerName(QString aText);

private:
  //---------- member-variable ----------
   QString mPfad;
   QString mFilename;
   QString mSpielerName;
   ChartWidget* mChartWidget;
   QWidget* mPlayerCoordWidget;
   QSlider* mSlider;
   QMap<int, parsedData> mAllPlayerData;
   QMap<int, parsedData> mSynchPlayerData;
   QMap<int, parsedData> mTransformedPlayerData;
   bool mDataIsTransformed = false;
   double mAvgSpeed; //average speed
   float mAvgHeartRate; //average heartrate

   QPair<double,double> mCornerBottomLeft;
   QPair<double,double> mCornerBottomRight;
   QPair<double,double> mCornerTopLeft;
   QPair<double,double> mCornerTopRight;

   QTime mStartTime;
   QTime mSynchTime;

   int mTimeDiffStartSynchAsInt;
   QTime mTimeDiffStartSynch;

//---------- private auxiliary-functions ----------

   /**
    * @brief setSliderValues
    * set values to our slider
    *
    * @param aMin
    * minimum-value
    *
    * @param aMax
    * maximum-value
    *
    * @param aValue
    * value to set
    */
   void setSliderValues(int aMin, int aMax, int aValue);

   /**
    * @brief synchPlayerData
    * synchronise the playerData with the internal synchtime
    * all Data befor the synch-time will be ignored
    */
   void synchPlayerData();

   /**
    * @brief generatePlayerDataWidget
    * creates a basic widget for the player
    *
    * @return
    * widget with the playerData
    */
   QWidget*  generatePlayerDataWidget();


    void transfromPlayerData();

};

#endif // SPIELER_H
