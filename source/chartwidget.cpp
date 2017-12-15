#include "chartwidget.h"

ChartWidget::ChartWidget(QWidget *parent) : QWidget(parent),
  mTopLayout(this)
{
  this->mEmptyWidget = new QWidget(this);
  mTopLayout.setContentsMargins(0, 0, 0, 0);
  mTopLayout.addWidget(&mStack);

  // Create and add the empty widget. This widget is shown when no item is selected.
  mStack.addWidget(mEmptyWidget);
  mStack.setCurrentWidget(mEmptyWidget);

}

ChartWidget::~ChartWidget()
{
}

void ChartWidget::setWidget(QWidget *aWidget)
{
  if (aWidget)
  {
    // Show the widget
    this->mWidget = aWidget;

    if (this->mStack.indexOf(this->mWidget) == -1)
      // The widget was just created and is not in the stack yet.
      this->mStack.addWidget(this->mWidget);

    // Show the chart widget
    this->mStack.setCurrentWidget(this->mWidget);
  }
  else
  {
    // Show the empty widget in case of aWidget was not defined
    mStack.setCurrentWidget(mEmptyWidget);
  }
}

void ChartWidget::removeWidget(QWidget* aWidget)
{
  if (aWidget)
    this->mStack.removeWidget(aWidget);
}
