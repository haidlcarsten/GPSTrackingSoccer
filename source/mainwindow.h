#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chartwidget.h"
#include "mannschaft.h"
#include "ui_mainwindow.h"
#include <QTimer>


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 * Mainwindow. Manages all actions and display all data
 */
class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  /**
   * @brief MainWindow
   * default-constructor
   *
   * @param parent
   * which QWidget should be the parent
   */
  explicit MainWindow(QWidget *parent = 0);

  /**
   * @brief ~MainWindow
   * default-destructor
   */
  ~MainWindow();

public slots:

  /**
   * @brief reDrawSpielerList
   * both mannschaft-objects will we drawn new
   */
  void reDrawSpielerList();

private slots:
  /**
   * @brief showFileOpenDialogMannschaftHeim
   * creates a file-open-dialog to add player to the heim-mannschaft
   */
  void showFileOpenDialogMannschaftHeim();

  /**
   * @brief showFileOpenDialogMannschaftGegner
   * creates a file-open-dialog to add player to the gegner-mannschaft
   */
  void showFileOpenDialogMannschaftGegner();

  /**
   * @brief showFileOpenDialogAddPlayer
   * creates an file-open-dialog to add a player to an specific mannschaft
   */
  void showFileOpenDialogAddPlayer();

  /**
   * @brief showHelpMenuDialog
   * creates the help-dialog
   */
  void showHelpMenuDialog();

  /**
   * @brief showInformationDialog
   * creates the about-dialog
   */
  void showInformationDialog();

  /**
   * @brief showSettingsDialog
   * creates the settings-dialog
   */
  void showSettingsDialog();

  /**
   * @brief incrementSlider
   * will increment the slider by one
   */
  void incrementSlider();

  /**
   * @brief incrementSlider
   * will decrement the slider by one
   */
  void decrementSlider();

  /**
   * @brief goToMinute
   *
   */
  void goToMinute();

  /**
   * @brief startPlayback
   * starts to increment the slider by one with an timer
   */
  void startPlayback();

  /**
   * @brief stopPlayback
   * stops the playback
   */
  void stopPlayback();

protected:
    void timerEvent(QTimerEvent *event);

private:
    Ui::MainWindow *ui;

//---------- member-variable ----------
  Mannschaft mMannschaftHeim;
  Mannschaft mMannschaftGegner;
  QBasicTimer mTimer;

//---------- private auxiliary-functions ----------

  /**
   * @brief createMenusAndActions
   * creates all menus and actions for the mainwindow
   */
  void createMenusAndActions();

  /**
   * @brief showFileOpenDialog
   * creates an file-open-dialog
   *
   * @return a list of selected files
   */
  QStringList showFileOpenDialog();

  /**
   * @brief loadSettings
   * settings will be loaded
   */
  void loadSettings();

  /**
   * @brief determineSliderRange
   * get the absolute range for the slider from all players
   */
  void determineSliderRange();

  /**
   * @brief generateMap
   * generates an widget, to check the transformition
   */
  void generateMap();
};

#endif // MAINWINDOW_H
