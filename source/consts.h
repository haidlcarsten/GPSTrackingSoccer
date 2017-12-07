#ifndef CONSTS
#define CONSTS

//! all const we use, we will define in this file
//! all labels and so on will be placed here


// general:
#define APPLICATION_NAME "GPS Tracking Visualisation"
#define APPLICATION_NAME_HELP "GPS Tracking Visualisation Hilfe"
#define BUTTON_TEXT_YES "Ja"
#define BUTTON_TEXT_NO "Nein"
#define BUTTON_TEXT_Abort "Abbrechen"



// Settingsdialog
#define STD_GRBBOX_COORDINATES "Koordinaten"
#define STD_LBL_LONGITUDE "Längengrad"
#define STD_LBL_LATITUDE "Breitengrad"
#define STD_LBL_CORNER_BOTTOM_LEFT "Ecke unten links:"
#define STD_LBL_CORNER_BOTTOM_RIGHT "Ecke unten rechts:"
#define STD_LBL_CORNER_TOP_LEFT "Ecke oben links:"
#define STD_LBL_CORNER_TOP_RIGHT "Ecke oben rechts:"
#define STD_INFO_CORNER_BOTTOM_LEFT "Tragen Sie eine Koordinate für die Ecke unten links ein."
#define STD_INFO_CORNER_BOTTOM_RIGHT "Tragen Sie eine Koordinate für die Ecke unten rechts ein."
#define STD_INFO_CORNER_TOP_LEFT "Tragen Sie eine Koordinate für die Ecke oben links ein."
#define STD_INFO_CORNER_TOP_RIGHT  "Tragen Sie eine Koordinate für die Ecke oben rechts ein."

#define STD_CORNER_BOTTOM_LEFT_LONG_TOOLTIP ""
#define STD_CORNER_BOTTOM_LEFT_LAT_TOOLTIP ""

#define STD_CORNER_BOTTOM_RIGHT_LONG_TOOLTIP ""
#define STD_CORNER_BOTTOM_RIGHT_LAT_TOOLTIP ""

#define STD_CORNER_TOP_LEFT_LONG_TOOLTIP ""
#define STD_CORNER_TOP_LEFT_LAT_TOOLTIP ""

#define STD_CORNER_TOP_RIGHT_LONG_TOOLTIP ""
#define STD_CORNER_TOP_RIGHT_LAT_TOOLTIP ""

// spieler.h / spieler.cpp
#define PLAYER_AVERAGE_HEARTRATE "Durchschnittliche Herzrate:"
#define PLAYER_AVERAGE_SPEED "Durchschnittliche Geschwindigkeit:"
#define PLAYER_NAME "Name:"


// mannschaft.h / mannschaft.cpp
#define TEAM_SPEED "Geschwindigkeit:"
#define TEAM_HEARTRATE "Herzrate:"
#define TEAM_GRB_DATA "Mannschaftsdaten"
#define TEAM_GRB_TEAM "Mannschaft"
#define TEAM_PLAYER_INDIVIDUAL_TOOLTIP "Die Daten werden angezeigt bezüglich Spieler: " // remeber to look into file mannschaft.cpp to complete the tooltip if necessary


// mainwindow.h / mainwindow.cpp
#define MW_FILE_MENU "Datei"
#define MW_FILE_MENU_LOAD_TEAM_HOME "Heim-Mannschaft laden..."
#define MW_FILE_MENU_LOAD_TEAM_ENEMY "Gegner-Mannschaft laden..."
#define MW_FILE_MENU_LOAD_PLAYER "Spieler laden..."
#define MW_FILE_MENU_LOAD_SETTINGS "Einstellungen"
#define MW_FILE_MENU_EXIT "Beenden"

#define MW_HELP_MENU "Hilfe"
#define MW_HELP_MENU_HELP "Hilfe.."
#define MW_HELP_MENU_INFORMATION "Über"

#define MW_LOAD_PLAYER_QUESTION "Sollen der Heim-Mannschaft Spieler hinzugefügt werden?"
#define MW_LOAD_PLAYER_QUESTION_INFORMATION "Bei Wahl von \"Ja\" werden der Heim-Mannschaft Spieler hinzugefügt, bei der Wahl von \"Nein\" werden der Gegner-Mannschaft Spieler hinzugefügt."

#define MW_HELP_TEXT "Folgt..."
#define MW_ABOUT_TEXT "Programmiert für medIT im Rahmen eines Seminars im Master Elektrotechnik\nPogrammiert von Kotaro Kurokawa und Carsten Haidl."
#define MW_NO_SETTINGS "Es sind keine Koordinaten in den Einstellungen eingetragen.\nBitte tragen Sie welche ein, bevor das System genutzt werden kann."

#define MW_TEAM_HOME_DOCK "Heim-Mannschaft"
#define MW_TEAM_ENEMY_DOCK "Gegner-Mannschaft"
#endif // CONSTS

