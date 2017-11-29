#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <cfloat>


/**
 * @brief The parsedData struct
 * the struct will collect one data-line of an csv-player-data
 */
struct parsedData {
    int mTime;
    int mActivityType;
    int mLapNumber;
    double mDistance;
    double mSpeed;
    double  mCalories;
    double mXvalue;
    double mYvalue;
    double mElevation;
    int mHeartRate;
    int mCycles;
};


#define SETTINGS_COORDINATES_EXISTS "settings_coordinates_exists"

#endif // TYPEDEF_H
