#ifndef TYPEDEF_H
#define TYPEDEF_H
#include <cfloat>


/**
 * @brief The parsedData struct
 * the struct will collect one data-line of an csv-player-data
 */
struct parsedData
{
    int mActivityType = -1;
    int mLapNumber    = 0;
    double mDistance  = 0.0;
    double mSpeed     = 0.0;
    double  mCalories = 0.0;
    double mLatitude  = 0.0;
    double mLongitude = 0.0;
    double mElevation = 0.0;
    int mHeartRate    = 0;
    int mCycles       = 0;

    double cRightBottomLong = 0.0;
    double cRightBottomLat  = 0.0;
    double cRightTopLong    = 0.0;
    double cRightTopLat     = 0.0;
    double cLeftBottomLong  = 0.0;
    double cLeftBottomLat   = 0.0;
    double cLeftTopLong     = 0.0;
    double cLeftTopLat      = 0.0;
};


// consts for settings
#define APPLICATION_PATH "." + QApplication::applicationDirPath().left(1)
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

#define SETTINGS_SKIP_PLAYERDATA "settings_skip_playerdata"
#define SETTINGS_MARKERSIZE_PLAYERDATA "settings_markersize_playerdata"
#define SETTINGS_TOLERANCE_FIELDDATA "settings_tolerance_fielddata"

#endif // TYPEDEF_H
