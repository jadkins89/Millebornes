#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Hazard Card Numbers
#define ACCIDENT_CARDS 3
#define OUT_OF_GAS_CARDS 3
#define FLAT_TIRE_CARDS 3
#define STOP_CARDS 5
#define SPEED_LIMIT_CARDS 4

// Remedy Card Numbers
#define REPAIRS_CARDS 6
#define GASOLINE_CARDS 6
#define SPARE_TIRE_CARDS 6
#define GO_CARDS 14
#define END_OF_LIMIT_CARDS 6

// Distance Card Numbers
#define DIST_1_CARDS 10
#define DIST_2_CARDS 10
#define DIST_3_CARDS 10
#define DIST_4_CARDS 12
#define DIST_5_CARDS 4

// Distances
#define DIST_1 25
#define DIST_2 50
#define DIST_3 75
#define DIST_4 100
#define DIST_5 200

// Player Definitions
#define MAX_HAND_SIZE 7
#define GOAL 1000

// Card Sizes
#define SAFETY_WIDTH 35
#define SAFETY_HEIGHT 49
#define HAND_WIDTH 60
#define HAND_HEIGHT 84
#define PLAYED_WIDTH 60
#define PLAYED_HEIGHT 84

// Starting Positions
#define SAFETY_X 725
#define SAFETY_Y 25
#define HAND_X 880
#define HAND_Y 40
#define ACTION_X 10
#define ACTION_Y 40
#define LIMIT_X 640

// Safety Offsets
#define H_OFFSET 54
#define V_OFFSET 55

// Player Zone Offset
#define V_ZONE_OFFSET 150
#define H_ZONE_OFFSET 90

// End Game Points
#define SAFE_TRIP 300
#define TRIP_COMPLETED 400
#define DELAYED_ACTION 300
#define COUPE_FOURRE 300
#define SAFETY_PLAYED 100
#define ALL_FOUR 300

// AI
#define ERROR 7

#endif // DEFINITIONS_H
