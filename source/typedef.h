#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <cfloat>


/**
 * @brief The parsedData struct
 * the struct will collect one data-line of an csv-player-data
 */
struct parsedData
{
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


// consts for settings
#define SETTINGS_FILE_PATH "config.ini"

#define SETTINGS_COORDINATES_EXISTS "settings_coordinates_exists"

#define SETTINGS_COORDINATES_BOTTOM_LEFT_LONGITUDE "settings_coordinates_bottom_left_longitude"
#define SETTINGS_COORDINATES_BOTTOM_LEFT_LATITUDE "settings_coordinates_bottom_left_latitude"


#define SETTINGS_COORDINATES_BOTTOM_RIGHT_LONGITUDE "settings_coordinates_bottom_right_longitude"
#define SETTINGS_COORDINATES_BOTTEM_RIGHT_LATITUDE "settings_coordinates_bottom_right_latitude"

#define SETTINGS_COORDINATES_TOP_LEFT_LONGITUDE "settings_coordinates_top_left_longitude"
#define SETTINGS_COORDINATES_TOP_LEFT_LATITUDE "settings_coordinates_top_left_latitude"

#define SETTINGS_COORDINATES_TOP_RIGHT_LONGITUDE "settings_coordinates_top_right_longitude"
#define SETTINGS_COORDINATES_TOP_RIGHT_LATITUDE "settings_coordinates_top_right_latitude"

#endif // TYPEDEF_H
