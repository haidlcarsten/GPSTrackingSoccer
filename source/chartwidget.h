#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H

#include <QPointer>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QWidget>
#include "ui_chartwidget.h"

/**
 * @brief The ChartWidget class
 * the ChartWidget class can display the charts. It uses a QStackedWidget
 */
class ChartWidget : public QWidget
{
    Q_OBJECT

public:
  /**
   * @brief ChartWidget
   * default-construtcor
   *
   * @param parent
   * which QWidget will be the parent widget
   *
   */
  explicit ChartWidget(QWidget *parent = 0);

  /**
    *@brief
    *default-destructor
    */
  ~ChartWidget();

  /**
   * @brief setChartWidget
   * places a Widget and add it to an stack
   *
   * @param aWidget
   * widget which is to place
   */
  void setWidget(QWidget* aWidget);

  /**
   * @brief removeChartWidget
   * removes an widget from the stack
   *
   * @param aWidget
   * widget which will be removed
   */
  void removeWidget(QWidget* aWidget);

  /**
   * @brief getDefaultWidget
   * returns an empty widget as default
   *
   * @return
   * default-widget
   */
  QWidget* getDefaultWidget() {return this->mEmptyWidget;}

  /**
   * @brief setDefaultWidget
   * sets the holder for an default-widget otherwise it is an empty-widget
   *
   * @param aDefaultWidget
   * new default-widget
   */
  void setDefaultWidget(QWidget* aDefaultWidget){this->mEmptyWidget = aDefaultWidget;}
private:
    // knowing the last shown widget
    QWidget *mWidget;

    // topLayout as basic
    QVBoxLayout mTopLayout;
    // changeing the different Widgets
    QStackedWidget mStack;
    // basic default widget
    QWidget* mEmptyWidget;
};

#endif // CHARTWIDGET_H
