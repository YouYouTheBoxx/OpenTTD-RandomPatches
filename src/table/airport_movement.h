/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file airport_movement.h Heart of the airports and their finite state machines */

#ifndef AIRPORT_MOVEMENT_H
#define AIRPORT_MOVEMENT_H


/**
 * State machine input struct (from external file, etc.)
 * Finite sTate mAchine --> FTA
 */
struct AirportFTAbuildup {
	byte position; ///< The position that an airplane is at.
	byte heading;  ///< The current orders (eg. TAKEOFF, HANGAR, ENDLANDING, etc.).
	uint64 block;  ///< The block this position is on on the airport (st->airport.flags).
	byte next;     ///< Next position from this position.
};

///////////////////////////////////////////////////////////////////////
/////*********Movement Positions on Airports********************///////

/**
 * Airport movement data creation macro.
 * @param x     X position.
 * @param y     Y position.
 * @param flags Movement flags.
 * @param dir   Direction.
 */
#define AMD(x, y, flags, dir) { x, y, flags, dir }

/** Dummy airport. */
static const AirportMovingData _airport_moving_data_dummy[] = {
	AMD(    0,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
	AMD(    0,   96, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
	AMD(   96,   96, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
	AMD(   96,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN,     DIR_N ),
};

/** Country Airfield (small) 4x3. */
static const AirportMovingData _airport_moving_data_country[22] = {
	AMD(   53,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
	AMD(   53,   27, 0,                               DIR_N ), // 01 Taxi to right outside depot
	AMD(   32,   23, AMED_EXACTPOS,                   DIR_NW), // 02 Terminal 1
	AMD(   10,   23, AMED_EXACTPOS,                   DIR_NW), // 03 Terminal 2
	AMD(   43,   37, 0,                               DIR_N ), // 04 Going towards terminal 2
	AMD(   24,   37, 0,                               DIR_N ), // 05 Going towards terminal 2
	AMD(   53,   37, 0,                               DIR_N ), // 06 Going for takeoff
	AMD(   61,   40, AMED_EXACTPOS,                   DIR_NE), // 07 Taxi to start of runway (takeoff)
	AMD(    3,   40, AMED_NOSPDCLAMP,                 DIR_N ), // 08 Accelerate to end of runway
	AMD(  -79,   40, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 09 Take off
	AMD(  177,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 10 Fly to landing position in air
	AMD(   56,   40, AMED_NOSPDCLAMP | AMED_LAND,     DIR_N ), // 11 Going down for land
	AMD(    3,   40, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 12 Just landed, brake until end of runway
	AMD(    7,   40, 0,                               DIR_N ), // 13 Just landed, turn around and taxi 1 square
	AMD(   53,   40, 0,                               DIR_N ), // 14 Taxi from runway to crossing
	AMD(    1,  193, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 15 Fly around waiting for a landing spot (north-east)
	AMD(    1,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 16 Fly around waiting for a landing spot (north-west)
	AMD(  257,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 17 Fly around waiting for a landing spot (south-west)
	AMD(  273,   47, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 18 Fly around waiting for a landing spot (south)
	AMD(   44,   37, AMED_HELI_RAISE,                 DIR_N ), // 19 Helicopter takeoff
	AMD(   44,   40, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 20 In position above landing spot helicopter
	AMD(   44,   40, AMED_HELI_LOWER,                 DIR_N ), // 21 Helicopter landing
};

/** Commuter Airfield (small) 5x4. */
static const AirportMovingData _airport_moving_data_commuter[38] = {
	AMD(   69,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
	AMD(   72,   22, 0,                               DIR_N ), // 01 Taxi to right outside depot
	AMD(    8,   22, AMED_EXACTPOS,                   DIR_SW), // 02 Taxi to right outside depot
	AMD(   24,   36, AMED_EXACTPOS,                   DIR_SE), // 03 Terminal 1
	AMD(   40,   36, AMED_EXACTPOS,                   DIR_SE), // 04 Terminal 2
	AMD(   56,   36, AMED_EXACTPOS,                   DIR_SE), // 05 Terminal 3
	AMD(   40,    8, AMED_EXACTPOS,                   DIR_NE), // 06 Helipad 1
	AMD(   56,    8, AMED_EXACTPOS,                   DIR_NE), // 07 Helipad 2
	AMD(   24,   22, 0,                               DIR_SW), // 08 Taxiing
	AMD(   40,   22, 0,                               DIR_SW), // 09 Taxiing
	AMD(   56,   22, 0,                               DIR_SW), // 10 Taxiing
	AMD(   72,   40, 0,                               DIR_SE), // 11 Airport OUTWAY
	AMD(   72,   54, AMED_EXACTPOS,                   DIR_NE), // 12 Accelerate to end of runway
	AMD(    7,   54, AMED_NOSPDCLAMP,                 DIR_N ), // 13 Release control of runway, for smoother movement
	AMD(    5,   54, AMED_NOSPDCLAMP,                 DIR_N ), // 14 End of runway
	AMD(  -79,   54, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 15 Take off
	AMD(  145,   54, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 16 Fly to landing position in air
	AMD(   73,   54, AMED_NOSPDCLAMP | AMED_LAND,     DIR_N ), // 17 Going down for land
	AMD(    3,   54, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 18 Just landed, brake until end of runway
	AMD(   12,   54, AMED_SLOWTURN,                   DIR_NW), // 19 Just landed, turn around and taxi
	AMD(    8,   32, 0,                               DIR_NW), // 20 Taxi from runway to crossing
	AMD(    1,  149, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 21 Fly around waiting for a landing spot (north-east)
	AMD(    1,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 22 Fly around waiting for a landing spot (north-west)
	AMD(  193,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 23 Fly around waiting for a landing spot (south-west)
	AMD(  225,   62, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 24 Fly around waiting for a landing spot (south)
	/* Helicopter */
	AMD(   80,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 25 Bufferspace before helipad
	AMD(   80,    0, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 26 Bufferspace before helipad
	AMD(   32,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 27 Get in position for Helipad1
	AMD(   48,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 28 Get in position for Helipad2
	AMD(   32,    8, AMED_HELI_LOWER,                 DIR_N ), // 29 Land at Helipad1
	AMD(   48,    8, AMED_HELI_LOWER,                 DIR_N ), // 30 Land at Helipad2
	AMD(   32,    8, AMED_HELI_RAISE,                 DIR_N ), // 31 Takeoff Helipad1
	AMD(   48,    8, AMED_HELI_RAISE,                 DIR_N ), // 32 Takeoff Helipad2
	AMD(   64,   22, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 33 Go to position for Hangarentrance in air
	AMD(   64,   22, AMED_HELI_LOWER,                 DIR_N ), // 34 Land in front of hangar
	AMD(   40,    8, AMED_EXACTPOS,                   DIR_N ), // 35 pre-helitakeoff helipad 1
	AMD(   56,    8, AMED_EXACTPOS,                   DIR_N ), // 36 pre-helitakeoff helipad 2
	AMD(   64,   25, AMED_HELI_RAISE,                 DIR_N ), // 37 Take off in front of hangar
};

/** City Airport (large) 6x6. */
static const AirportMovingData _airport_moving_data_city[] = {
	AMD(   85,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
	AMD(   85,   22, 0,                               DIR_N ), // 01 Taxi to right outside depot
	AMD(   26,   41, AMED_EXACTPOS,                   DIR_SW), // 02 Terminal 1
	AMD(   56,   22, AMED_EXACTPOS,                   DIR_SE), // 03 Terminal 2
	AMD(   38,    8, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 3
	AMD(   65,    6, 0,                               DIR_N ), // 05 Taxi to right in infront of terminal 2/3
	AMD(   80,   27, 0,                               DIR_N ), // 06 Taxiway terminals 2-3
	AMD(   44,   63, 0,                               DIR_N ), // 07 Taxi to Airport center
	AMD(   58,   71, 0,                               DIR_N ), // 08 Towards takeoff
	AMD(   72,   85, 0,                               DIR_N ), // 09 Taxi to runway (takeoff)
	AMD(   89,   85, AMED_EXACTPOS,                   DIR_NE), // 10 Taxi to start of runway (takeoff)
	AMD(    3,   85, AMED_NOSPDCLAMP,                 DIR_N ), // 11 Accelerate to end of runway
	AMD(  -79,   85, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 12 Take off
	AMD(  177,   87, AMED_HOLD       | AMED_SLOWTURN, DIR_N ), // 13 Fly to landing position in air
	AMD(   89,   87, AMED_HOLD       | AMED_LAND,     DIR_N ), // 14 Going down for land
	AMD(   20,   87, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 15 Just landed, brake until end of runway
	AMD(   20,   87, 0,                               DIR_N ), // 16 Just landed, turn around and taxi 1 square // NOT USED
	AMD(   36,   71, 0,                               DIR_N ), // 17 Taxi from runway to crossing
	AMD(  160,   87, AMED_HOLD       | AMED_SLOWTURN, DIR_N ), // 18 Fly around waiting for a landing spot (north-east)
	AMD(  140,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 19 Final approach fix
	AMD(  257,    1, AMED_HOLD       | AMED_SLOWTURN, DIR_N ), // 20 Fly around waiting for a landing spot (south-west)
	AMD(  273,   49, AMED_HOLD       | AMED_SLOWTURN, DIR_N ), // 21 Fly around waiting for a landing spot (south)
	AMD(   44,   63, AMED_HELI_RAISE,                 DIR_N ), // 22 Helicopter takeoff
	AMD(   28,   74, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 23 In position above landing spot helicopter
	AMD(   28,   74, AMED_HELI_LOWER,                 DIR_N ), // 24 Helicopter landing
	AMD(  145,    1, AMED_HOLD       | AMED_SLOWTURN, DIR_N ), // 25 Fly around waiting for a landing spot (north-west)
	AMD(  -32,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 26 Initial approach fix (north)
	AMD(  300,  -48, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 27 Initial approach fix (south)
	AMD(  140,  -48, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 28 Intermediate Approach fix (south), IAF (west)
	AMD(  -32,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 29 Initial approach fix (east)
};

/** Metropolitan Airport (metropolitan) - 2 runways. */
static const AirportMovingData _airport_moving_data_metropolitan[28] = {
	AMD(   85,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar
	AMD(   85,   22, 0,                               DIR_N ), // 01 Taxi to right outside depot
	AMD(   26,   41, AMED_EXACTPOS,                   DIR_SW), // 02 Terminal 1
	AMD(   56,   22, AMED_EXACTPOS,                   DIR_SE), // 03 Terminal 2
	AMD(   38,    8, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 3
	AMD(   65,    6, 0,                               DIR_N ), // 05 Taxi to right in infront of terminal 2/3
	AMD(   80,   27, 0,                               DIR_N ), // 06 Taxiway terminals 2-3
	AMD(   49,   58, 0,                               DIR_N ), // 07 Taxi to Airport center
	AMD(   72,   58, 0,                               DIR_N ), // 08 Towards takeoff
	AMD(   72,   69, 0,                               DIR_N ), // 09 Taxi to runway (takeoff)
	AMD(   89,   69, AMED_EXACTPOS,                   DIR_NE), // 10 Taxi to start of runway (takeoff)
	AMD(    3,   69, AMED_NOSPDCLAMP,                 DIR_N ), // 11 Accelerate to end of runway
	AMD(  -79,   69, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 12 Take off
	AMD(  177,   85, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 13 Fly to landing position in air
	AMD(   89,   85, AMED_NOSPDCLAMP | AMED_LAND,     DIR_N ), // 14 Going down for land
	AMD(    3,   85, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 15 Just landed, brake until end of runway
	AMD(   21,   85, 0,                               DIR_N ), // 16 Just landed, turn around and taxi 1 square
	AMD(   21,   69, 0,                               DIR_N ), // 17 On Runway-out taxiing to In-Way
	AMD(   21,   58, AMED_EXACTPOS,                   DIR_SW), // 18 Taxi from runway to crossing
	AMD(    1,  193, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 19 Fly around waiting for a landing spot (north-east)
	AMD(    1,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 20 Fly around waiting for a landing spot (north-west)
	AMD(  257,    1, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 21 Fly around waiting for a landing spot (south-west)
	AMD(  273,   49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 22 Fly around waiting for a landing spot (south)
	AMD(   44,   58, 0,                               DIR_N ), // 23 Helicopter takeoff spot on ground (to clear airport sooner)
	AMD(   44,   63, AMED_HELI_RAISE,                 DIR_N ), // 24 Helicopter takeoff
	AMD(   15,   54, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 25 Get in position above landing spot helicopter
	AMD(   15,   54, AMED_HELI_LOWER,                 DIR_N ), // 26 Helicopter landing
	AMD(   21,   58, AMED_EXACTPOS,                   DIR_SW), // 27 Transitions after landing to on-ground movement
};

/** International Airport (international) - 2 runways, 6 terminals, dedicated helipad. */
static const AirportMovingData _airport_moving_data_international[53] = {
	AMD(    7,   55, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
	AMD(  100,   21, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
	AMD(    7,   70, 0,                               DIR_N ), // 02 Taxi to right outside depot (Hangar 1)
	AMD(  100,   36, 0,                               DIR_N ), // 03 Taxi to right outside depot (Hangar 2)
	AMD(   38,   70, AMED_EXACTPOS,                   DIR_SW), // 04 Terminal 1
	AMD(   38,   54, AMED_EXACTPOS,                   DIR_SW), // 05 Terminal 2
	AMD(   38,   38, AMED_EXACTPOS,                   DIR_SW), // 06 Terminal 3
	AMD(   70,   70, AMED_EXACTPOS,                   DIR_NE), // 07 Terminal 4
	AMD(   70,   54, AMED_EXACTPOS,                   DIR_NE), // 08 Terminal 5
	AMD(   70,   38, AMED_EXACTPOS,                   DIR_NE), // 09 Terminal 6
	AMD(  104,   71, AMED_EXACTPOS,                   DIR_NE), // 10 Helipad 1
	AMD(  104,   55, AMED_EXACTPOS,                   DIR_NE), // 11 Helipad 2
	AMD(   22,   87, 0,                               DIR_N ), // 12 Towards Terminals 4/5/6, Helipad 1/2
	AMD(   60,   87, 0,                               DIR_N ), // 13 Towards Terminals 4/5/6, Helipad 1/2
	AMD(   66,   87, 0,                               DIR_N ), // 14 Towards Terminals 4/5/6, Helipad 1/2
	AMD(   86,   87, AMED_EXACTPOS,                   DIR_NW), // 15 Towards Terminals 4/5/6, Helipad 1/2
	AMD(   86,   70, 0,                               DIR_N ), // 16 In Front of Terminal 4 / Helipad 1
	AMD(   86,   54, 0,                               DIR_N ), // 17 In Front of Terminal 5 / Helipad 2
	AMD(   86,   38, 0,                               DIR_N ), // 18 In Front of Terminal 6
	AMD(   86,   22, 0,                               DIR_N ), // 19 Towards Terminals Takeoff (Taxiway)
	AMD(   66,   22, 0,                               DIR_N ), // 20 Towards Terminals Takeoff (Taxiway)
	AMD(   60,   22, 0,                               DIR_N ), // 21 Towards Terminals Takeoff (Taxiway)
	AMD(   38,   22, 0,                               DIR_N ), // 22 Towards Terminals Takeoff (Taxiway)
	AMD(   22,   70, 0,                               DIR_N ), // 23 In Front of Terminal 1
	AMD(   22,   58, 0,                               DIR_N ), // 24 In Front of Terminal 2
	AMD(   22,   38, 0,                               DIR_N ), // 25 In Front of Terminal 3
	AMD(   22,   22, AMED_EXACTPOS,                   DIR_NW), // 26 Going for Takeoff
	AMD(   22,    6, 0,                               DIR_N ), // 27 On Runway-out, prepare for takeoff
	AMD(    3,    6, AMED_EXACTPOS,                   DIR_SW), // 28 Accelerate to end of runway
	AMD(   60,    6, AMED_NOSPDCLAMP,                 DIR_N ), // 29 Release control of runway, for smoother movement
	AMD(  105,    6, AMED_NOSPDCLAMP,                 DIR_N ), // 30 End of runway
	AMD(  190,    6, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 31 Take off
	AMD(  193,  104, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 32 Fly to landing position in air
	AMD(  105,  104, AMED_NOSPDCLAMP | AMED_LAND,     DIR_N ), // 33 Going down for land
	AMD(    3,  104, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 34 Just landed, brake until end of runway
	AMD(   12,  104, AMED_SLOWTURN,                   DIR_N ), // 35 Just landed, turn around and taxi 1 square
	AMD(    7,   84, 0,                               DIR_N ), // 36 Taxi from runway to crossing
	AMD(    1,  209, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 37 Fly around waiting for a landing spot (north-east)
	AMD(    1,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 38 Fly around waiting for a landing spot (north-west)
	AMD(  273,    6, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 39 Fly around waiting for a landing spot (south-west)
	AMD(  305,   81, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 40 Fly around waiting for a landing spot (south)
	/* Helicopter */
	AMD(  128,   80, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 41 Bufferspace before helipad
	AMD(  128,   80, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 42 Bufferspace before helipad
	AMD(   96,   71, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 43 Get in position for Helipad1
	AMD(   96,   55, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 44 Get in position for Helipad2
	AMD(   96,   71, AMED_HELI_LOWER,                 DIR_N ), // 45 Land at Helipad1
	AMD(   96,   55, AMED_HELI_LOWER,                 DIR_N ), // 46 Land at Helipad2
	AMD(  104,   71, AMED_HELI_RAISE,                 DIR_N ), // 47 Takeoff Helipad1
	AMD(  104,   55, AMED_HELI_RAISE,                 DIR_N ), // 48 Takeoff Helipad2
	AMD(  104,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 49 Go to position for Hangarentrance in air
	AMD(  104,   32, AMED_HELI_LOWER,                 DIR_N ), // 50 Land in HANGAR2_AREA to go to hangar
	AMD(    7,   70, AMED_HELI_RAISE,                 DIR_N ), // 51 Takeoff from HANGAR1_AREA
	AMD(  100,   36, AMED_HELI_RAISE,                 DIR_N ), // 52 Takeoff from HANGAR2_AREA
};

/** Intercontinental Airport - 4 runways, 8 terminals, 2 dedicated helipads. */
static const AirportMovingData _airport_moving_data_intercontinental[77] = {
	AMD(    8,   87, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar 1
	AMD(  136,   72, AMED_EXACTPOS,                   DIR_SE), // 01 In Hangar 2
	AMD(    8,  104, 0,                               DIR_N ), // 02 Taxi to right outside depot 1
	AMD(  136,   88, 0,                               DIR_N ), // 03 Taxi to right outside depot 2
	AMD(   56,  120, AMED_EXACTPOS,                   DIR_W ), // 04 Terminal 1
	AMD(   56,  104, AMED_EXACTPOS,                   DIR_SW), // 05 Terminal 2
	AMD(   56,   88, AMED_EXACTPOS,                   DIR_SW), // 06 Terminal 3
	AMD(   56,   72, AMED_EXACTPOS,                   DIR_SW), // 07 Terminal 4
	AMD(   88,  120, AMED_EXACTPOS,                   DIR_N ), // 08 Terminal 5
	AMD(   88,  104, AMED_EXACTPOS,                   DIR_NE), // 09 Terminal 6
	AMD(   88,   88, AMED_EXACTPOS,                   DIR_NE), // 10 Terminal 7
	AMD(   88,   72, AMED_EXACTPOS,                   DIR_NE), // 11 Terminal 8
	AMD(   88,   56, AMED_EXACTPOS,                   DIR_SE), // 12 Helipad 1
	AMD(   72,   56, AMED_EXACTPOS,                   DIR_NE), // 13 Helipad 2
	AMD(   40,  136, 0,                               DIR_N ), // 14 Term group 2 enter 1 a
	AMD(   56,  136, 0,                               DIR_N ), // 15 Term group 2 enter 1 b
	AMD(   88,  136, 0,                               DIR_N ), // 16 Term group 2 enter 2 a
	AMD(  104,  136, 0,                               DIR_N ), // 17 Term group 2 enter 2 b
	AMD(  104,  120, 0,                               DIR_N ), // 18 Term group 2 - opp term 5
	AMD(  104,  104, 0,                               DIR_N ), // 19 Term group 2 - opp term 6 & exit2
	AMD(  104,   88, 0,                               DIR_N ), // 20 Term group 2 - opp term 7 & hangar area 2
	AMD(  104,   72, 0,                               DIR_N ), // 21 Term group 2 - opp term 8
	AMD(  104,   56, 0,                               DIR_N ), // 22 Taxi Term group 2 exit a
	AMD(  104,   40, 0,                               DIR_N ), // 23 Taxi Term group 2 exit b
	AMD(   56,   40, 0,                               DIR_N ), // 24 Term group 2 exit 2a
	AMD(   40,   40, 0,                               DIR_N ), // 25 Term group 2 exit 2b
	AMD(   40,  120, 0,                               DIR_N ), // 26 Term group 1 - opp term 1
	AMD(   40,  104, 0,                               DIR_N ), // 27 Term group 1 - opp term 2 & hangar area 1
	AMD(   40,   88, 0,                               DIR_N ), // 28 Term group 1 - opp term 3
	AMD(   40,   72, 0,                               DIR_N ), // 29 Term group 1 - opp term 4
	AMD(   18,   72, 0,                               DIR_NW), // 30 Outway 1
	AMD(    8,   40, 0,                               DIR_NW), // 31 Airport OUTWAY
	AMD(    8,   24, AMED_EXACTPOS,                   DIR_SW), // 32 Accelerate to end of runway
	AMD(  119,   24, AMED_NOSPDCLAMP,                 DIR_N ), // 33 Release control of runway, for smoother movement
	AMD(  117,   24, AMED_NOSPDCLAMP,                 DIR_N ), // 34 End of runway
	AMD(  197,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 35 Take off
	AMD(  254,   84, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 36 Flying to landing position in air
	AMD(  117,  168, AMED_NOSPDCLAMP | AMED_LAND,     DIR_N ), // 37 Going down for land
	AMD(    8,  168, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 38 Just landed, brake until end of runway
	AMD(    8,  168, 0,                               DIR_N ), // 39 Just landed, turn around and taxi
	AMD(    8,  144, 0,                               DIR_NW), // 40 Taxi from runway
	AMD(    8,  128, 0,                               DIR_NW), // 41 Taxi from runway
	AMD(    8,  120, AMED_EXACTPOS,                   DIR_NW), // 42 Airport entrance
	AMD(   56,  344, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 43 Fly around waiting for a landing spot (north-east)
	AMD( -200,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 44 Fly around waiting for a landing spot (north-west)
	AMD(   56, -168, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 45 Fly around waiting for a landing spot (south-west)
	AMD(  312,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 46 Fly around waiting for a landing spot (south)
	/* Helicopter */
	AMD(   96,   40, AMED_NOSPDCLAMP,                 DIR_N ), // 47 Bufferspace before helipad
	AMD(   96,   40, AMED_NOSPDCLAMP,                 DIR_N ), // 48 Bufferspace before helipad
	AMD(   82,   54, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 49 Get in position for Helipad1
	AMD(   64,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 50 Get in position for Helipad2
	AMD(   81,   55, AMED_HELI_LOWER,                 DIR_N ), // 51 Land at Helipad1
	AMD(   64,   56, AMED_HELI_LOWER,                 DIR_N ), // 52 Land at Helipad2
	AMD(   80,   56, AMED_HELI_RAISE,                 DIR_N ), // 53 Takeoff Helipad1
	AMD(   64,   56, AMED_HELI_RAISE,                 DIR_N ), // 54 Takeoff Helipad2
	AMD(  136,   96, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 55 Go to position for Hangarentrance in air
	AMD(  136,   96, AMED_HELI_LOWER,                 DIR_N ), // 56 Land in front of hangar2
	AMD(  126,  104, 0,                               DIR_SE), // 57 Outway 2
	AMD(  136,  136, 0,                               DIR_NE), // 58 Airport OUTWAY 2
	AMD(  136,  152, AMED_EXACTPOS,                   DIR_NE), // 59 Accelerate to end of runway2
	AMD(   16,  152, AMED_NOSPDCLAMP,                 DIR_N ), // 60 Release control of runway2, for smoother movement
	AMD(   20,  152, AMED_NOSPDCLAMP,                 DIR_N ), // 61 End of runway2
	AMD(  -56,  152, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N ), // 62 Take off2
	AMD(   24,    8, AMED_NOSPDCLAMP | AMED_LAND,     DIR_N ), // 63 Going down for land2
	AMD(  136,    8, AMED_NOSPDCLAMP | AMED_BRAKE,    DIR_N ), // 64 Just landed, brake until end of runway2in
	AMD(  136,    8, 0,                               DIR_N ), // 65 Just landed, turn around and taxi
	AMD(  136,   24, 0,                               DIR_SE), // 66 Taxi from runway 2in
	AMD(  136,   40, 0,                               DIR_SE), // 67 Taxi from runway 2in
	AMD(  136,   56, AMED_EXACTPOS,                   DIR_NE), // 68 Airport entrance2
	AMD(  -56,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 69 Fly to landing position in air2
	AMD(   88,   40, 0,                               DIR_N ), // 70 Taxi Term group 2 exit - opp heli1
	AMD(   72,   40, 0,                               DIR_N ), // 71 Taxi Term group 2 exit - opp heli2
	AMD(   88,   57, AMED_EXACTPOS,                   DIR_SE), // 72 pre-helitakeoff helipad 1
	AMD(   71,   56, AMED_EXACTPOS,                   DIR_NE), // 73 pre-helitakeoff helipad 2
	AMD(    8,  120, AMED_HELI_RAISE,                 DIR_N ), // 74 Helitakeoff outside depot 1
	AMD(  136,  104, AMED_HELI_RAISE,                 DIR_N ), // 75 Helitakeoff outside depot 2
	AMD(  197,  168, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 76 Fly to landing position in air1
};

/** Terminus4 Airport - 8 hangars, 8 runways, 36 terminals, 12 helipad. */
static const AirportMovingData _airport_moving_data_terminus4[239] = {
	/* Hangars and Outside Hangars */
AMD(136,  200, AMED_EXACTPOS,                   DIR_NE), //  00 In Hangar 01
AMD(200,  200, AMED_EXACTPOS,                   DIR_SW), //  01 In Hangar 02
AMD(40,  136, AMED_EXACTPOS,                   DIR_NW), //  02 In Hangar 03
AMD(40,  200, AMED_EXACTPOS,                   DIR_SE), //  03 In Hangar 04
AMD(104,   40, AMED_EXACTPOS,                   DIR_SW), //  04 In Hangar 05
AMD(40,   40, AMED_EXACTPOS,                   DIR_NE), //  05 In Hangar 06
AMD(200,  104, AMED_EXACTPOS,                   DIR_SE), //  06 In Hangar 07
AMD(200,   40, AMED_EXACTPOS,                   DIR_NW), //  07 In Hangar 08
AMD(120,  200, 0,                               DIR_N), //  08 Taxi to right outside Hangar01
AMD(216,  200, 0,                               DIR_N), //  09 Taxi to right outside Hangar02
AMD(40,  120, 0,                               DIR_N), //  10 Taxi to right outside Hangar03
AMD(40,  216, 0,                               DIR_N), //  11 Taxi to right outside Hangar04
AMD(120,   40, 0,                               DIR_N), //  12 Taxi to right outside Hangar05
AMD(24,   40, 0,                               DIR_N), //  13 Taxi to right outside Hangar06
AMD(200,  120, 0,                               DIR_N), //  14 Taxi to right outside Hangar07
AMD(200,   24, 0,                               DIR_N), //  15 Taxi to right outside Hangar08
	/* Airplane Terminals */
AMD(88,  168, AMED_EXACTPOS,                   DIR_NW), //  16 Terminal 01
AMD(104,  168, AMED_EXACTPOS,                   DIR_NW), //  17 Terminal 02
AMD(120,  168, AMED_EXACTPOS,                   DIR_NW), //  18 Terminal 03
AMD(136,  168, AMED_EXACTPOS,                   DIR_NW), //  19 Terminal 04
AMD(152,  168, AMED_EXACTPOS,                   DIR_NW), //  20 Terminal 05
AMD(168,  168, AMED_EXACTPOS,                   DIR_N), //  21 Terminal 06
AMD(200,  168, AMED_EXACTPOS,                   DIR_NW), //  22 Terminal 07
AMD(216,  168, AMED_EXACTPOS,                   DIR_NW), //  23 Terminal 08
AMD(232,  168, AMED_EXACTPOS,                   DIR_NW), //  24 Terminal 09
AMD(72,   88, AMED_EXACTPOS,                   DIR_SW), //  25 Terminal 10
AMD(72,  104, AMED_EXACTPOS,                   DIR_SW), //  26 Terminal 11
AMD(72,  120, AMED_EXACTPOS,                   DIR_SW), //  27 Terminal 12
AMD(72,  136, AMED_EXACTPOS,                   DIR_SW), //  28 Terminal 13
AMD(72,  152, AMED_EXACTPOS,                   DIR_SW), //  29 Terminal 14
AMD(72,  168, AMED_EXACTPOS,                   DIR_W), //  30 Terminal 15
AMD(72,  200, AMED_EXACTPOS,                   DIR_SW), //  31 Terminal 16
AMD(72,  216, AMED_EXACTPOS,                   DIR_SW), //  32 Terminal 17
AMD(72,  232, AMED_EXACTPOS,                   DIR_SW), //  33 Terminal 18
AMD(152,   72, AMED_EXACTPOS,                   DIR_SE), //  34 Terminal 19
AMD(136,   72, AMED_EXACTPOS,                   DIR_SE), //  35 Terminal 20
AMD(120,   72, AMED_EXACTPOS,                   DIR_SE), //  36 Terminal 21
AMD(104,   72, AMED_EXACTPOS,                   DIR_SE), //  37 Terminal 22
AMD(88,   72, AMED_EXACTPOS,                   DIR_SE), //  38 Terminal 23
AMD(72,   72, AMED_EXACTPOS,                   DIR_S), //  39 Terminal 24
AMD(40,   72, AMED_EXACTPOS,                   DIR_SE), //  40 Terminal 25
AMD(24,   72, AMED_EXACTPOS,                   DIR_SE), //  41 Terminal 26
AMD(8,   72, AMED_EXACTPOS,                   DIR_SE), //  42 Terminal 27
AMD(168,  152, AMED_EXACTPOS,                   DIR_NE), //  43 Terminal 28
AMD(168,  136, AMED_EXACTPOS,                   DIR_NE), //  44 Terminal 29
AMD(168,  120, AMED_EXACTPOS,                   DIR_NE), //  45 Terminal 30
AMD(168,  104, AMED_EXACTPOS,                   DIR_NE), //  46 Terminal 31
AMD(168,   88, AMED_EXACTPOS,                   DIR_NE), //  47 Terminal 32
AMD(168,   72, AMED_EXACTPOS,                   DIR_E), //  48 Terminal 33
AMD(168,   40, AMED_EXACTPOS,                   DIR_NE), //  49 Terminal 34
AMD(168,   24, AMED_EXACTPOS,                   DIR_NE), //  50 Terminal 35
AMD(168,    8, AMED_EXACTPOS,                   DIR_NE), //  51 Terminal 36
	/* Cargo Terminals (Uses Helipads) */
AMD(152,  200, AMED_EXACTPOS,                   DIR_SE), //  52 Cargo Terminal 01
AMD(168,  200, AMED_EXACTPOS,                   DIR_SE), //  53 Cargo Terminal 02
AMD(184,  200, AMED_EXACTPOS,                   DIR_SE), //  54 Cargo Terminal 03
AMD(40,  152, AMED_EXACTPOS,                   DIR_NE), //  55 Cargo Terminal 04
AMD(40,  168, AMED_EXACTPOS,                   DIR_NE), //  56 Cargo Terminal 05
AMD(40,  184, AMED_EXACTPOS,                   DIR_NE), //  57 Cargo Terminal 06
AMD(88,   40, AMED_EXACTPOS,                   DIR_NW), //  58 Cargo Terminal 07
AMD(72,   40, AMED_EXACTPOS,                   DIR_NW), //  59 Cargo Terminal 08
AMD(56,   40, AMED_EXACTPOS,                   DIR_NW), //  60 Cargo Terminal 09
AMD(200,   88, AMED_EXACTPOS,                   DIR_SW), //  61 Cargo Terminal 10
AMD(200,   72, AMED_EXACTPOS,                   DIR_SW), //  62 Cargo Terminal 11
AMD(200,   56, AMED_EXACTPOS,                   DIR_SW), //  63 Cargo Terminal 12
	/* Helicopter Terminals */
AMD(152,  200, AMED_EXACTPOS,                   DIR_SE), //  64 Helipad 01
AMD(168,  200, AMED_EXACTPOS,                   DIR_SE), //  65 Helipad 02
AMD(184,  200, AMED_EXACTPOS,                   DIR_SE), //  66 Helipad 03
AMD(40,  152, AMED_EXACTPOS,                   DIR_NE), //  67 Helipad 04
AMD(40,  168, AMED_EXACTPOS,                   DIR_NE), //  68 Helipad 05
AMD(40,  184, AMED_EXACTPOS,                   DIR_NE), //  69 Helipad 06
AMD(88,   40, AMED_EXACTPOS,                   DIR_NW), //  70 Helipad 07
AMD(72,   40, AMED_EXACTPOS,                   DIR_NW), //  71 Helipad 08
AMD(56,   40, AMED_EXACTPOS,                   DIR_NW), //  72 Helipad 09
AMD(200,   88, AMED_EXACTPOS,                   DIR_SW), //  73 Helipad 10
AMD(200,   72, AMED_EXACTPOS,                   DIR_SW), //  74 Helipad 11
AMD(200,   56, AMED_EXACTPOS,                   DIR_SW), //  75 Helipad 12
	/* Airplane - Holding Pattern */
AMD(120,  648, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  76 Fly around waiting for a landing spot (south-east) 56,  504
AMD(-408,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  77 Fly around waiting for a landing spot (north-east) -280,   56
AMD(120, -408, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  78 Fly around waiting for a landing spot (north-west) 152, -280
AMD(648,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  79 Fly around waiting for a landing spot (south-west) 504, 152
	/* Helicopter - Holding Pattern */
AMD(120,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  80 Bufferspace before helipad
AMD(120,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  81 Bufferspace before helipad
	/* Helicopter Hangar 01 */
AMD(120,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), //  82 Go to position for Hangarentrance in air
AMD(120,  200, AMED_HELI_LOWER,                 DIR_N), //  83 Land in HANG01_block to go to hangar
	// Start of Pod 1
	/* Airplane Landing - Runway 01 */
AMD(408,  232, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  84 Fly to landing position in air
AMD(200,  232, AMED_HOLD | AMED_LAND,           DIR_N), //  85 Going down for land
AMD(104,  232, AMED_HOLD | AMED_BRAKE,          DIR_N), //  86 Just landed, brake until end of runway
AMD(104,  232, AMED_SLOWTURN,                   DIR_N), //  87 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 02 */
AMD(424,  216, AMED_HOLD | AMED_SLOWTURN,       DIR_N), //  88 Fly to landing position in air
AMD(216,  216, AMED_HOLD | AMED_LAND,           DIR_N), //  89 Going down for land
AMD(120,  216, AMED_HOLD | AMED_BRAKE,          DIR_N), //  90 Just landed, brake until end of runway
	/* Airplane End Landing */
AMD(104,  216, 0,                               DIR_N), //  91 Taxi02
	/* Airplane Taxi */
AMD(104,  200, 0,                               DIR_N), //  92 Taxi03
AMD(104,  184, 0,                               DIR_N), //  93 Taxi04
AMD(120,  184, 0,                               DIR_N), //  94 Taxi05
AMD(136,  184, 0,                               DIR_N), //  95 Taxi06
AMD(152,  184, 0,                               DIR_N), //  96 Taxi07
AMD(168,  184, 0,                               DIR_N), //  97 Taxi08
AMD(184,  184, 0,                               DIR_N), //  98 Taxi09
AMD(200,  184, 0,                               DIR_N), //  99 Taxi10
AMD(216,  184, 0,                               DIR_N), // 100 Taxi11
AMD(232,  184, 0,                               DIR_N), // 101 Taxi12
AMD(232,  200, 0,                               DIR_N), // 102 Taxi13
	/* Airplane Takeoff */
AMD(232,  216, 0,                               DIR_N), // 103 On Runway 02, prepare for takeoff
AMD(200,  216, AMED_NOSPDCLAMP,                 DIR_N), // 104 Release control of runway, for smoother movement
AMD(152,  216, AMED_NOSPDCLAMP,                 DIR_N), // 105 Airborne point - Takeoff
AMD(88,  216, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 106 End Take off    Airplane leaves Airport Control
	/* Helicopter Takeoff */
AMD(152,  200, AMED_HELI_RAISE,                 DIR_N), // 107 Takeoff Helipad1
AMD(168,  200, AMED_HELI_RAISE,                 DIR_N), // 108 Takeoff Helipad2
AMD(184,  200, AMED_HELI_RAISE,                 DIR_N), // 109 Takeoff Helipad2
	// End of Pod1
	// Start of Pod 2
	/* Airplane Landing - Runway 03 */
AMD(8,  408, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 110 Fly to landing position in air
AMD(8,  200, AMED_HOLD | AMED_LAND,           DIR_N), // 111 Going down for land
AMD(8,  104, AMED_HOLD | AMED_BRAKE,          DIR_N), // 112 Just landed, brake until end of runway
AMD(8,  104, AMED_SLOWTURN,                   DIR_N), // 113 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 04 */
AMD(24,  424, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 114 Fly to landing position in air
AMD(24,  216, AMED_HOLD | AMED_LAND,           DIR_N), // 115 Going down for land
AMD(24,  120, AMED_HOLD | AMED_BRAKE,          DIR_N), // 116 Just landed, brake until end of runway
	/* Airplane End Landing */
AMD(24,  104, 0,                               DIR_N), // 117 Taxi15
	/* Airplane Taxi */
AMD(40,  104, 0,                               DIR_N), // 118 Taxi16
AMD(56,  104, 0,                               DIR_N), // 119 Taxi17
AMD(56,  120, 0,                               DIR_N), // 120 Taxi18
AMD(56,  136, 0,                               DIR_N), // 121 Taxi19
AMD(56,  152, 0,                               DIR_N), // 122 Taxi20
AMD(56,  168, 0,                               DIR_N), // 123 Taxi21
AMD(56,  184, 0,                               DIR_N), // 124 Taxi22
AMD(56,  200, 0,                               DIR_N), // 125 Taxi23
AMD(56,  216, 0,                               DIR_N), // 126 Taxi24
AMD(56,  232, 0,                               DIR_N), // 127 Taxi25
AMD(40,  232, 0,                               DIR_N), // 128 Taxi26
	/* Airplane Takeoff */
AMD(24,  232, 0,                               DIR_N), // 129 On Runway 02, prepare for takeoff
AMD(24,  200, AMED_NOSPDCLAMP,                 DIR_N), // 130 Release control of runway, for smoother movement
AMD(24,  152, AMED_NOSPDCLAMP,                 DIR_N), // 131 Airborne point - Takeoff
AMD(24,   88, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 132 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 03 */
AMD(32,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 133 Go to position for Hangarentrance in air
AMD(40,  120, AMED_HELI_LOWER,                 DIR_N), // 134 Land in HANG03_block to go to hangar
	/* Helicopter Takeoff */
AMD(40,  152, AMED_HELI_RAISE,                 DIR_N), // 135 Takeoff Helipad1
AMD(40,  168, AMED_HELI_RAISE,                 DIR_N), // 136 Takeoff Helipad2
AMD(40,  184, AMED_HELI_RAISE,                 DIR_N), // 137 Takeoff Helipad2
	// End of Pod2
	// Start of Pod 3
	/* Airplane Landing - Runway 05 */
AMD(-168,    8, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 138 Fly to landing position in air
AMD(40,    8, AMED_HOLD | AMED_LAND,           DIR_N), // 139 Going down for land
AMD(136,    8, AMED_HOLD | AMED_BRAKE,          DIR_N), // 140 Just landed, brake until end of runway
AMD(136,    8, AMED_SLOWTURN,                   DIR_N), // 141 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 06 */
AMD(-184,   24, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 142 Fly to landing position in air
AMD(24,   24, AMED_HOLD | AMED_LAND,           DIR_N), // 143 Going down for land
AMD(120,   24, AMED_HOLD | AMED_BRAKE,          DIR_N), // 144 Just landed, brake until end of runway
	/* Airplane Taxi - End Landing */
AMD(136,   24, 0,                               DIR_N), // 145 Taxi28
	/* Airplane Taxi */
AMD(136,   40, 0,                               DIR_N), // 146 Taxi29
AMD(136,   56, 0,                               DIR_N), // 147 Taxi30
AMD(120,   56, 0,                               DIR_N), // 148 Taxi31
AMD(104,   56, 0,                               DIR_N), // 149 Taxi32
AMD(88,   56, 0,                               DIR_N), // 150 Taxi33
AMD(72,   56, 0,                               DIR_N), // 151 Taxi34
AMD(56,   56, 0,                               DIR_N), // 152 Taxi35
AMD(40,   56, 0,                               DIR_N), // 153 Taxi36
AMD(24,   56, 0,                               DIR_N), // 154 Taxi37
AMD(8,   56, 0,                               DIR_N), // 155 Taxi38
AMD(8,   40, 0,                               DIR_N), // 156 Taxi39
	/* Airplane Takeoff */
AMD(8,   24, 0,                               DIR_N), // 157 On Runway 06, prepare for takeoff
AMD(40,   24, AMED_NOSPDCLAMP,                 DIR_N), // 158 Release control of runway, for smoother movement
AMD(88,   24, AMED_NOSPDCLAMP,                 DIR_N), // 159 Airborne point - Takeoff
AMD(152,   24, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 160 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 05 */
AMD(120,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 161 Go to position for Hangarentrance in air
AMD(120,   40, AMED_HELI_LOWER,                 DIR_N), // 162 Land in HANG05_block to go to hangar
	/* Helicopter Takeoff */
AMD(88,   40, AMED_HELI_RAISE,                 DIR_N), // 163 Takeoff Helipad07
AMD(72,   40, AMED_HELI_RAISE,                 DIR_N), // 164 Takeoff Helipad08
AMD(56,   40, AMED_HELI_RAISE,                 DIR_N), // 165 Takeoff Helipad09
	// End of Pod3
	// Start of Pod 4
	/* Airplane Landing - Runway 07 */
AMD(232, -168, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 166 Fly to landing position in air
AMD(232,   40, AMED_HOLD | AMED_LAND,           DIR_N), // 167 Going down for land
AMD(232,  136, AMED_HOLD | AMED_BRAKE,          DIR_N), // 168 Just landed, brake until end of runway
AMD(232,  136, AMED_SLOWTURN,                   DIR_N), // 169 Just landed, taxi 1 square forward, this is to free the runway quicker.
	/* Airplane Landing - Runway 04 */
AMD(216, -184, AMED_HOLD | AMED_SLOWTURN,       DIR_N), // 170 Fly to landing position in air
AMD(216,   24, AMED_HOLD | AMED_LAND,           DIR_N), // 171 Going down for land
AMD(216,  120, AMED_HOLD | AMED_BRAKE,          DIR_N), // 172 Just landed, brake until end of runway
	/* Airplane End Landing */
AMD(216,  136, 0,                               DIR_N), // 173 Taxi15
	/* Airplane Taxi */
AMD(200,  136, 0,                               DIR_N), // 174 Taxi16
AMD(184,  136, 0,                               DIR_N), // 175 Taxi17
AMD(184,  120, 0,                               DIR_N), // 176 Taxi18
AMD(184,  104, 0,                               DIR_N), // 177 Taxi19
AMD(184,   88, 0,                               DIR_N), // 178 Taxi20
AMD(184,   72, 0,                               DIR_N), // 179 Taxi21
AMD(184,   56, 0,                               DIR_N), // 180 Taxi22
AMD(184,   40, 0,                               DIR_N), // 181 Taxi23
AMD(184,   24, 0,                               DIR_N), // 182 Taxi24
AMD(184,    8, 0,                               DIR_N), // 183 Taxi25
AMD(200,    8, 0,                               DIR_N), // 184 Taxi26
	/* Airplane Takeoff */
AMD(216,    8, 0,                               DIR_N), // 185 On Runway 02, prepare for takeoff
AMD(216,   40, AMED_NOSPDCLAMP,                 DIR_N), // 186 Release control of runway, for smoother movement
AMD(216,   88, AMED_NOSPDCLAMP,                 DIR_N), // 187 Airborne point - Takeoff
AMD(216,  152, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 188 End Take off    Airplane leaves Airport Control
	/* Helicopter Hangar 07 */
AMD(208,  120, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 189 Go to position for Hangarentrance in air
AMD(200,  120, AMED_HELI_LOWER,                 DIR_N), // 190 Land in HANG07_block to go to hangar
	/* Helicopter Takeoff */
AMD(200,   88, AMED_HELI_RAISE,                 DIR_N), // 191 Takeoff Helipad1
AMD(200,   72, AMED_HELI_RAISE,                 DIR_N), // 192 Takeoff Helipad2
AMD(200,   56, AMED_HELI_RAISE,                 DIR_N), // 193 Takeoff Helipad2
	// End of Pod4
	/* Crossover Taxi from Pod 1 to Pod 2 */
AMD(88,  184, 0,                               DIR_N), // 194 Crossover 
AMD(72,  184, 0,                               DIR_N), // 195 Crossover
	/* Crossover Taxi from Pod 2 to Pod 3 */
AMD(56,   88, 0,                               DIR_N), // 196 Crossover 
AMD(56,   72, 0,                               DIR_N), // 197 Crossover
	/* Crossover Taxi from Pod 3 to Pod 4 */
AMD(152,   56, 0,                               DIR_N), // 198 Crossover 
AMD(168,   56, 0,                               DIR_N), // 199 Crossover
	/* Crossover Taxi from Pod 4 to Pod 1 */
AMD(184,  152, 0,                               DIR_N), // 200 Crossover 
AMD(184,  168, 0,                               DIR_N), // 201 Crossover
	/*  Added holding spots */
AMD(600,  168, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 202 Fly around waiting for a landing spot SW Check Runway 2
AMD(584,  184, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 203 Fly around waiting for a landing spot SW Check Runway 1
AMD(72,  600, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 204 Fly around waiting for a landing spot SE Check Runway 4
AMD(56,  584, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 205 Fly around waiting for a landing spot SE Check Runway 3
AMD(-360,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 206 Fly around waiting for a landing spot NE Check Runway 6
AMD(-344,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 207 Fly around waiting for a landing spot NE Check Runway 5
AMD(168, -360, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 208 Fly around waiting for a landing spot NW Check Runway 8
AMD(184, -344, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 209 Fly around waiting for a landing spot NW Check Runway 7
	/* Helicopter Entry. Helicopters need a seperate entry or it will clear flags for airplanes causing then to run into each other.*/
AMD(136,  136, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 210 Helicopter Entry
	// Abort Landings
AMD(72,  232, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 211 Abort Landing Runway 01
AMD(8,   72, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 212 Abort Landing Runway 03
AMD(168,    8, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 213 Abort Landing Runway 05
AMD(232,  168, AMED_NOSPDCLAMP | AMED_TAKEOFF,  DIR_N), // 214 Abort Landing Runway 07
	// Helicopter Positioning
AMD(152,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 215 Get in position for Helipad01
AMD(168,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 216 Get in position for Helipad02
AMD(184,  208, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 217 Get in position for Helipad03
AMD(32,  152, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 218 Get in position for Helipad04
AMD(32,  168, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 219 Get in position for Helipad05
AMD(32,  184, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 220 Get in position for Helipad06
AMD(88,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 221 Get in position for Helipad07
AMD(72,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 222 Get in position for Helipad08
AMD(56,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 223 Get in position for Helipad09
AMD(208,   88, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 224 Get in position for Helipad10
AMD(208,   72, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 225 Get in position for Helipad11
AMD(208,   56, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N), // 226 Get in position for Helipad12
	// Helicopter Landing at Terminals
AMD(152,  200, AMED_HELI_LOWER,                 DIR_N), // 227 Land at Helipad01
AMD(168,  200, AMED_HELI_LOWER,                 DIR_N), // 228 Land at Helipad02
AMD(184,  200, AMED_HELI_LOWER,                 DIR_N), // 229 Land at Helipad03
AMD(40,  152, AMED_HELI_LOWER,                 DIR_N), // 230 Land at Helipad04
AMD(40,  168, AMED_HELI_LOWER,                 DIR_N), // 231 Land at Helipad05
AMD(40,  184, AMED_HELI_LOWER,                 DIR_N), // 232 Land at Helipad06
AMD(88,   40, AMED_HELI_LOWER,                 DIR_N), // 233 Land at Helipad07
AMD(72,   40, AMED_HELI_LOWER,                 DIR_N), // 234 Land at Helipad08
AMD(56,   40, AMED_HELI_LOWER,                 DIR_N), // 235 Land at Helipad09
AMD(200,   88, AMED_HELI_LOWER,                 DIR_N), // 236 Land at Helipad10
AMD(200,   72, AMED_HELI_LOWER,                 DIR_N), // 237 Land at Helipad11
AMD(200,   56, AMED_HELI_LOWER,                 DIR_N), // 238 Land at Helipad12
};

/** Heliport (heliport). */
static const AirportMovingData _airport_moving_data_heliport[9] = {
	AMD(    5,    9, AMED_EXACTPOS,                   DIR_NE), // 0 - At heliport terminal
	AMD(    2,    9, AMED_HELI_RAISE,                 DIR_N ), // 1 - Take off (play sound)
	AMD(   -3,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 2 - In position above landing spot helicopter
	AMD(   -3,    9, AMED_HELI_LOWER,                 DIR_N ), // 3 - Land
	AMD(    2,    9, 0,                               DIR_N ), // 4 - Goto terminal on ground
	AMD(  -31,   59, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 5 - Circle #1 (north-east)
	AMD(  -31,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 6 - Circle #2 (north-west)
	AMD(   49,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 7 - Circle #3 (south-west)
	AMD(   70,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 8 - Circle #4 (south)
};

/** HeliDepot 2x2 (heliport). */
static const AirportMovingData _airport_moving_data_helidepot[18] = {
	AMD(   24,    4, AMED_EXACTPOS,                   DIR_NE), // 0 - At depot
	AMD(   24,   28, 0,                               DIR_N ), // 1 Taxi to right outside depot
	AMD(    5,   38, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 2 Flying
	AMD(  -15,  -15, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 3 - Circle #1 (north-east)
	AMD(  -15,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 4 - Circle #2 (north-west)
	AMD(   49,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 5 - Circle #3 (south-west)
	AMD(   49,  -15, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 6 - Circle #4 (south-east)
	AMD(    8,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 7 - PreHelipad
	AMD(    8,   32, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 8 - Helipad
	AMD(    8,   16, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 9 - Land
	AMD(    8,   16, AMED_HELI_LOWER,                 DIR_NW), // 10 - Land
	AMD(    8,   24, AMED_HELI_RAISE,                 DIR_N ), // 11 - Take off (play sound)
	AMD(   32,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NW), // 12 Air to above hangar area
	AMD(   32,   24, AMED_HELI_LOWER,                 DIR_NW), // 13 Taxi to right outside depot
	AMD(    8,   24, AMED_EXACTPOS,                   DIR_NW), // 14 - on helipad1
	AMD(   24,   28, AMED_HELI_RAISE,                 DIR_N ), // 15 Takeoff right outside depot
	AMD(    8,   24, AMED_HELI_RAISE,                 DIR_SW), // 16 - Take off (play sound)
	AMD(    8,   24, AMED_SLOWTURN | AMED_EXACTPOS,   DIR_E ), // 17 - turn on helipad1 for takeoff
};

/** HeliDepot 2x2 (heliport). */
static const AirportMovingData _airport_moving_data_helistation[33] = {
	AMD(    8,    3, AMED_EXACTPOS,                   DIR_SE), // 00 In Hangar2
	AMD(    8,   22, 0,                               DIR_N ), // 01 outside hangar 2
	AMD(  116,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 02 Fly to landing position in air
	AMD(   14,   22, AMED_HELI_RAISE,                 DIR_N ), // 03 Helitakeoff outside hangar1(play sound)
	AMD(   24,   22, 0,                               DIR_N ), // 04 taxiing
	AMD(   40,   22, 0,                               DIR_N ), // 05 taxiing
	AMD(   40,    8, AMED_EXACTPOS,                   DIR_NE), // 06 Helipad 1
	AMD(   56,    8, AMED_EXACTPOS,                   DIR_NE), // 07 Helipad 2
	AMD(   56,   24, AMED_EXACTPOS,                   DIR_NE), // 08 Helipad 3
	AMD(   40,    8, AMED_EXACTPOS,                   DIR_N ), // 09 pre-helitakeoff helipad 1
	AMD(   56,    8, AMED_EXACTPOS,                   DIR_N ), // 10 pre-helitakeoff helipad 2
	AMD(   56,   24, AMED_EXACTPOS,                   DIR_N ), // 11 pre-helitakeoff helipad 3
	AMD(   32,    8, AMED_HELI_RAISE,                 DIR_N ), // 12 Takeoff Helipad1
	AMD(   48,    8, AMED_HELI_RAISE,                 DIR_N ), // 13 Takeoff Helipad2
	AMD(   48,   24, AMED_HELI_RAISE,                 DIR_N ), // 14 Takeoff Helipad3
	AMD(   84,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 15 Bufferspace before helipad
	AMD(   68,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 16 Bufferspace before helipad
	AMD(   32,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 17 Get in position for Helipad1
	AMD(   48,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 18 Get in position for Helipad2
	AMD(   48,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_NE), // 19 Get in position for Helipad3
	AMD(   40,    8, AMED_HELI_LOWER,                 DIR_N ), // 20 Land at Helipad1
	AMD(   48,    8, AMED_HELI_LOWER,                 DIR_N ), // 21 Land at Helipad2
	AMD(   48,   24, AMED_HELI_LOWER,                 DIR_N ), // 22 Land at Helipad3
	AMD(    0,   22, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 23 Go to position for Hangarentrance in air
	AMD(    0,   22, AMED_HELI_LOWER,                 DIR_N ), // 24 Land in front of hangar
	AMD(  148,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 25 Fly around waiting for a landing spot (south-east)
	AMD(  148,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 26 Fly around waiting for a landing spot (south-west)
	AMD(  132,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 27 Fly around waiting for a landing spot (south-west)
	AMD(  100,   24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 28 Fly around waiting for a landing spot (north-east)
	AMD(   84,    8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 29 Fly around waiting for a landing spot (south-east)
	AMD(   84,   -8, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 30 Fly around waiting for a landing spot (south-west)
	AMD(  100,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 31 Fly around waiting for a landing spot (north-west)
	AMD(  132,  -24, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 32 Fly around waiting for a landing spot (north-east)
};

/** Oilrig. */
static const AirportMovingData _airport_moving_data_oilrig[9] = {
	AMD(   31,    9, AMED_EXACTPOS,                   DIR_NE), // 0 - At oilrig terminal
	AMD(   28,    9, AMED_HELI_RAISE,                 DIR_N ), // 1 - Take off (play sound)
	AMD(   23,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 2 - In position above landing spot helicopter
	AMD(   23,    9, AMED_HELI_LOWER,                 DIR_N ), // 3 - Land
	AMD(   28,    9, 0,                               DIR_N ), // 4 - Goto terminal on ground
	AMD(  -31,   69, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 5 - circle #1 (north-east)
	AMD(  -31,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 6 - circle #2 (north-west)
	AMD(   69,  -49, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 7 - circle #3 (south-west)
	AMD(   69,    9, AMED_NOSPDCLAMP | AMED_SLOWTURN, DIR_N ), // 8 - circle #4 (south)
};

#undef AMD

///////////////////////////////////////////////////////////////////////
/////**********Movement Machine on Airports*********************///////
static const byte _airport_entries_dummy[] = {0, 1, 2, 3};
static const AirportFTAbuildup _airport_fta_dummy[] = {
	{ 0, TO_ALL, 0, 3},
	{ 1, TO_ALL, 0, 0},
	{ 2, TO_ALL, 0, 1},
	{ 3, TO_ALL, 0, 2},
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

/* First element of terminals array tells us how many depots there are (to know size of array)
 * this may be changed later when airports are moved to external file  */
static const HangarTileTable _airport_depots_country[] = { {{3, 0}, DIR_SE, 0} };
static const byte _airport_terminal_country[] = {1, 2};
static const byte _airport_entries_country[] = {16, 15, 18, 17};
static const AirportFTAbuildup _airport_fta_country[] = {
	{  0, HANGAR, NOTHING_block, 1 },
	{  1, TERMGROUP, AIRPORT_BUSY_block, 0 }, { 1, HANGAR, 0, 0 }, { 1, TERM1, TERM1_block, 2 }, { 1, TERM2, 0, 4 }, { 1, HELITAKEOFF, 0, 19 }, { 1, TO_ALL, 0, 6 },
	{  2, TERM1, TERM1_block, 1 },
	{  3, TERM2, TERM2_block, 5 },
	{  4, TERMGROUP, AIRPORT_BUSY_block, 0 }, { 4, TERM2, 0, 5 }, { 4, HANGAR, 0, 1 }, { 4, TAKEOFF, 0, 6 }, { 4, HELITAKEOFF, 0, 1 },
	{  5, TERMGROUP, AIRPORT_BUSY_block, 0 }, { 5, TERM2, TERM2_block, 3 }, { 5, TO_ALL, 0, 4 },
	{  6, 0, AIRPORT_BUSY_block, 7 },
	/* takeoff */
	{  7, TAKEOFF, AIRPORT_BUSY_block, 8 },
	{  8, STARTTAKEOFF, NOTHING_block, 9 },
	{  9, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 10, FLYING, NOTHING_block, 15 }, { 10, LANDING, 0, 11 }, { 10, HELILANDING, 0, 20 },
	{ 11, LANDING, AIRPORT_BUSY_block, 12 },
	{ 12, TO_ALL, AIRPORT_BUSY_block, 13 },
	{ 13, ENDLANDING, AIRPORT_BUSY_block, 14 }, { 13, TERM2, 0, 5 }, { 13, TO_ALL, 0, 14 },
	{ 14, TO_ALL, AIRPORT_BUSY_block, 1 },
	/* In air */
	{ 15, TO_ALL, NOTHING_block, 16 },
	{ 16, TO_ALL, NOTHING_block, 17 },
	{ 17, TO_ALL, NOTHING_block, 18 },
	{ 18, TO_ALL, NOTHING_block, 10 },
	{ 19, HELITAKEOFF, NOTHING_block, 0 },
	{ 20, HELILANDING, AIRPORT_BUSY_block, 21 },
	{ 21, HELIENDLANDING, AIRPORT_BUSY_block, 1 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

static const HangarTileTable _airport_depots_commuter[] = { {{4, 0}, DIR_SE, 0} };
static const byte _airport_terminal_commuter[] = { 1, 3 };
static const byte _airport_entries_commuter[] = {22, 21, 24, 23};
static const AirportFTAbuildup _airport_fta_commuter[] = {
	{  0, HANGAR, NOTHING_block, 1 }, { 0, HELITAKEOFF, TAXIWAY_BUSY_block, 1 }, { 0, TO_ALL, 0, 1 },
	{  1, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 1, HANGAR, 0, 0 }, { 1, TAKEOFF, 0, 11 }, { 1, TERM1, TAXIWAY_BUSY_block, 10 }, { 1, TERM2, TAXIWAY_BUSY_block, 10 }, { 1, TERM3, TAXIWAY_BUSY_block, 10 }, { 1, HELIPAD1, TAXIWAY_BUSY_block, 10 }, { 1, HELIPAD2, TAXIWAY_BUSY_block, 10 }, { 1, HELITAKEOFF, TAXIWAY_BUSY_block, 37 }, { 1, TO_ALL, 0, 0 },
	{  2, TERMGROUP, AIRPORT_ENTRANCE_block, 2 }, { 2, HANGAR, 0, 8 }, { 2, TERM1, 0, 8 }, { 2, TERM2, 0, 8 }, { 2, TERM3, 0, 8 }, { 2, HELIPAD1, 0, 8 }, { 2, HELIPAD2, 0, 8 }, { 2, HELITAKEOFF, 0, 8 }, { 2, TO_ALL, 0, 2 },
	{  3, TERM1, TERM1_block, 8 }, { 3, HANGAR, 0, 8 }, { 3, TAKEOFF, 0, 8 }, { 3, TO_ALL, 0, 3 },
	{  4, TERM2, TERM2_block, 9 }, { 4, HANGAR, 0, 9 }, { 4, TAKEOFF, 0, 9 }, { 4, TO_ALL, 0, 4 },
	{  5, TERM3, TERM3_block, 10 }, { 5, HANGAR, 0, 10 }, { 5, TAKEOFF, 0, 10 }, { 5, TO_ALL, 0, 5 },
	{  6, HELIPAD1, HELIPAD1_block, 6 }, { 6, HANGAR, TAXIWAY_BUSY_block, 9 }, { 6, HELITAKEOFF, 0, 35 },
	{  7, HELIPAD2, HELIPAD2_block, 7 }, { 7, HANGAR, TAXIWAY_BUSY_block, 10 }, { 7, HELITAKEOFF, 0, 36 },
	{  8, TERMGROUP, TAXIWAY_BUSY_block, 8 }, { 8, TAKEOFF, TAXIWAY_BUSY_block, 9 }, { 8, HANGAR, TAXIWAY_BUSY_block, 9 }, { 8, TERM1, TERM1_block, 3 }, { 8, TO_ALL, TAXIWAY_BUSY_block, 9 },
	{  9, TERMGROUP, TAXIWAY_BUSY_block, 9 }, { 9, TAKEOFF, TAXIWAY_BUSY_block, 10 }, { 9, HANGAR, TAXIWAY_BUSY_block, 10 }, { 9, TERM2, TERM2_block, 4 }, { 9, HELIPAD1, HELIPAD1_block, 6 }, { 9, HELITAKEOFF, HELIPAD1_block, 6 }, { 9, TERM1, TAXIWAY_BUSY_block, 8 }, { 9, TO_ALL, TAXIWAY_BUSY_block, 10 },
	{ 10, TERMGROUP, TAXIWAY_BUSY_block, 10 }, { 10, TERM3, TERM3_block, 5 }, { 10, HELIPAD1, 0, 9 }, { 10, HELIPAD2, HELIPAD2_block, 7 }, { 10, HELITAKEOFF, 0, 1 }, { 10, TAKEOFF, TAXIWAY_BUSY_block, 1 }, { 10, HANGAR, TAXIWAY_BUSY_block, 1 }, { 10, TO_ALL, TAXIWAY_BUSY_block, 9 },
	{ 11, TO_ALL, OUT_WAY_block, 12 },
	/* takeoff */
	{ 12, TAKEOFF, RUNWAY_IN_OUT_block, 13 },
	{ 13, TO_ALL, RUNWAY_IN_OUT_block, 14 },
	{ 14, STARTTAKEOFF, RUNWAY_IN_OUT_block, 15 },
	{ 15, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 16, FLYING, NOTHING_block, 21 }, { 16, LANDING, IN_WAY_block, 17 }, { 16, HELILANDING, 0, 25 },
	{ 17, LANDING, RUNWAY_IN_OUT_block, 18 },
	{ 18, TO_ALL, RUNWAY_IN_OUT_block, 19 },
	{ 19, TO_ALL, RUNWAY_IN_OUT_block, 20 },
	{ 20, ENDLANDING, IN_WAY_block, 2 },
	/* In Air */
	{ 21, TO_ALL, NOTHING_block, 22 },
	{ 22, TO_ALL, NOTHING_block, 23 },
	{ 23, TO_ALL, NOTHING_block, 24 },
	{ 24, TO_ALL, NOTHING_block, 16 },
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{ 25, HELILANDING, PRE_HELIPAD_block, 26 },
	{ 26, HELIENDLANDING, PRE_HELIPAD_block, 26 }, { 26, HELIPAD1, 0, 27 }, { 26, HELIPAD2, 0, 28 }, { 26, HANGAR, 0, 33 },
	{ 27, TO_ALL, NOTHING_block, 29 }, // helipad1 approach
	{ 28, TO_ALL, NOTHING_block, 30 },
	/* landing */
	{ 29, TERMGROUP, NOTHING_block, 0 }, { 29, HELIPAD1, HELIPAD1_block, 6 },
	{ 30, TERMGROUP, NOTHING_block, 0 }, { 30, HELIPAD2, HELIPAD2_block, 7 },
	/* Helicopter -- takeoff */
	{ 31, HELITAKEOFF, NOTHING_block, 0 },
	{ 32, HELITAKEOFF, NOTHING_block, 0 },
	{ 33, TO_ALL, TAXIWAY_BUSY_block, 34 }, // need to go to hangar when waiting in air
	{ 34, TO_ALL, TAXIWAY_BUSY_block, 1 },
	{ 35, TO_ALL, HELIPAD1_block, 31 },
	{ 36, TO_ALL, HELIPAD2_block, 32 },
	{ 37, HELITAKEOFF, NOTHING_block, 0 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

static const HangarTileTable _airport_depots_city[] = { {{5, 0}, DIR_SE, 0} };
static const byte _airport_terminal_city[] = { 1, 3 };
static const byte _airport_entries_city[] = {26, 29, 27, 28};
static const AirportFTAbuildup _airport_fta_city[] = {
	{  0, HANGAR, NOTHING_block, 1 }, { 0, TAKEOFF, OUT_WAY_block, 1 }, { 0, TO_ALL, 0, 1 },
	{  1, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 1, HANGAR, 0, 0 }, { 1, TERM2, 0, 6 }, { 1, TERM3, 0, 6 }, { 1, TO_ALL, 0, 7 }, // for all else, go to 7
	{  2, TERM1, TERM1_block, 7 }, { 2, TAKEOFF, OUT_WAY_block, 7 }, { 2, TO_ALL, 0, 7 },
	{  3, TERM2, TERM2_block, 5 }, { 3, TAKEOFF, OUT_WAY_block, 6 }, { 3, TO_ALL, 0, 6 },
	{  4, TERM3, TERM3_block, 5 }, { 4, TAKEOFF, OUT_WAY_block, 5 }, { 4, TO_ALL, 0, 5 },
	{  5, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 5, TERM2, TERM2_block, 3 }, { 5, TERM3, TERM3_block, 4 }, { 5, TO_ALL, 0, 6 },
	{  6, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 6, TERM2, TERM2_block, 3 }, { 6, TERM3, 0, 5 }, { 6, HANGAR, 0, 1 }, { 6, TO_ALL, 0, 7 },
	{  7, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 7, TERM1, TERM1_block, 2 }, { 7, TAKEOFF, OUT_WAY_block, 8 }, { 7, HELITAKEOFF, 0, 22 }, { 7, HANGAR, 0, 1 }, { 7, TO_ALL, 0, 6 },
	{  8, 0, OUT_WAY_block, 9 },
	{  9, 0, RUNWAY_IN_OUT_block, 10 },
	/* takeoff */
	{ 10, TAKEOFF, RUNWAY_IN_OUT_block, 11 },
	{ 11, STARTTAKEOFF, NOTHING_block, 12 },
	{ 12, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 13, FLYING, NOTHING_block, 18 }, { 13, LANDING, 0, 14 }, { 13, HELILANDING, 0, 23 },
	{ 14, LANDING, RUNWAY_IN_OUT_block, 15 },
	{ 15, TO_ALL, RUNWAY_IN_OUT_block, 17 },
	{ 16, TO_ALL, RUNWAY_IN_OUT_block, 17 }, // not used, left for compatibility
	{ 17, ENDLANDING, IN_WAY_block, 7 },
	/* In Air */
	{ 18, TO_ALL, NOTHING_block, 25 },
	{ 19, TO_ALL, NOTHING_block, 20 },
	{ 20, TO_ALL, NOTHING_block, 21 },
	{ 21, TO_ALL, NOTHING_block, 13 },
	/* helicopter */
	{ 22, HELITAKEOFF, NOTHING_block, 0 },
	{ 23, HELILANDING, IN_WAY_block, 24 },
	{ 24, HELIENDLANDING, IN_WAY_block, 17 },
	{ 25, TO_ALL, NOTHING_block, 20},
	{ 26, TO_ALL, NOTHING_block, 19},
	{ 27, TO_ALL, NOTHING_block, 28},
	{ 28, TO_ALL, NOTHING_block, 19},
	{ 29, TO_ALL, NOTHING_block, 26},
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

static const HangarTileTable _airport_depots_metropolitan[] = { {{5, 0}, DIR_SE, 0} };
static const byte _airport_terminal_metropolitan[] = { 1, 3 };
static const byte _airport_entries_metropolitan[] = {20, 19, 22, 21};
static const AirportFTAbuildup _airport_fta_metropolitan[] = {
	{  0, HANGAR, NOTHING_block, 1 },
	{  1, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 1, HANGAR, 0, 0 }, { 1, TERM2, 0, 6 }, { 1, TERM3, 0, 6 }, { 1, TO_ALL, 0, 7 }, // for all else, go to 7
	{  2, TERM1, TERM1_block, 7 },
	{  3, TERM2, TERM2_block, 6 },
	{  4, TERM3, TERM3_block, 5 },
	{  5, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 5, TERM2, TERM2_block, 3 }, { 5, TERM3, TERM3_block, 4 }, { 5, TO_ALL, 0, 6 },
	{  6, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 6, TERM2, TERM2_block, 3 }, { 6, TERM3, 0, 5 }, { 6, HANGAR, 0, 1 }, { 6, TO_ALL, 0, 7 },
	{  7, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 7, TERM1, TERM1_block, 2 }, { 7, TAKEOFF, 0, 8 }, { 7, HELITAKEOFF, 0, 23 }, { 7, HANGAR, 0, 1 }, { 7, TO_ALL, 0, 6 },
	{  8, 0, OUT_WAY_block, 9 },
	{  9, 0, RUNWAY_OUT_block, 10 },
	/* takeoff */
	{ 10, TAKEOFF, RUNWAY_OUT_block, 11 },
	{ 11, STARTTAKEOFF, NOTHING_block, 12 },
	{ 12, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 13, FLYING, NOTHING_block, 19 }, { 13, LANDING, 0, 14 }, { 13, HELILANDING, 0, 25 },
	{ 14, LANDING, RUNWAY_IN_block, 15 },
	{ 15, TO_ALL, RUNWAY_IN_block, 16 },
	{ 16, TERMGROUP, RUNWAY_IN_block, 0 }, { 16, ENDLANDING, IN_WAY_block, 17 },
	{ 17, TERMGROUP, RUNWAY_OUT_block, 0 }, { 17, ENDLANDING, IN_WAY_block, 18 },
	{ 18, ENDLANDING, IN_WAY_block, 27 },
	/* In Air */
	{ 19, TO_ALL, NOTHING_block, 20 },
	{ 20, TO_ALL, NOTHING_block, 21 },
	{ 21, TO_ALL, NOTHING_block, 22 },
	{ 22, TO_ALL, NOTHING_block, 13 },
	/* helicopter */
	{ 23, TO_ALL, NOTHING_block, 24 },
	{ 24, HELITAKEOFF, NOTHING_block, 0 },
	{ 25, HELILANDING, IN_WAY_block, 26 },
	{ 26, HELIENDLANDING, IN_WAY_block, 18 },
	{ 27, TERMGROUP, TAXIWAY_BUSY_block, 27 }, { 27, TERM1, TERM1_block, 2 }, { 27, TO_ALL, 0, 7 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

static const HangarTileTable _airport_depots_international[] = { {{0, 3}, DIR_SE, 0}, {{6, 1}, DIR_SE, 1} };
static const byte _airport_terminal_international[] = { 2, 3, 3 };
static const byte _airport_entries_international[] = { 38, 37, 40, 39 };
static const AirportFTAbuildup _airport_fta_international[] = {
	{  0, HANGAR, NOTHING_block, 2 }, { 0, TERMGROUP, TERM_GROUP1_block, 0 }, { 0, TERMGROUP, TERM_GROUP2_ENTER1_block, 1 }, { 0, HELITAKEOFF, AIRPORT_ENTRANCE_block, 2 }, { 0, TO_ALL, 0, 2 },
	{  1, HANGAR, NOTHING_block, 3 }, { 1, TERMGROUP, HANGAR2_AREA_block, 1 }, { 1, HELITAKEOFF, HANGAR2_AREA_block, 3 }, { 1, TO_ALL, 0, 3 },
	{  2, TERMGROUP, AIRPORT_ENTRANCE_block, 0 }, { 2, HANGAR, 0, 0 }, { 2, TERM4, 0, 12 }, { 2, TERM5, 0, 12 }, { 2, TERM6, 0, 12 }, { 2, HELIPAD1, 0, 12 }, { 2, HELIPAD2, 0, 12 }, { 2, HELITAKEOFF, 0, 51 }, { 2, TO_ALL, 0, 23 },
	{  3, TERMGROUP, HANGAR2_AREA_block, 0 }, { 3, HANGAR, 0, 1 }, { 3, HELITAKEOFF, 0, 52 }, { 3, TO_ALL, 0, 18 },
	{  4, TERM1, TERM1_block, 23 }, { 4, HANGAR, AIRPORT_ENTRANCE_block, 23 }, { 4, TO_ALL, 0, 23 },
	{  5, TERM2, TERM2_block, 24 }, { 5, HANGAR, AIRPORT_ENTRANCE_block, 24 }, { 5, TO_ALL, 0, 24 },
	{  6, TERM3, TERM3_block, 25 }, { 6, HANGAR, AIRPORT_ENTRANCE_block, 25 }, { 6, TO_ALL, 0, 25 },
	{  7, TERM4, TERM4_block, 16 }, { 7, HANGAR, HANGAR2_AREA_block, 16 }, { 7, TO_ALL, 0, 16 },
	{  8, TERM5, TERM5_block, 17 }, { 8, HANGAR, HANGAR2_AREA_block, 17 }, { 8, TO_ALL, 0, 17 },
	{  9, TERM6, TERM6_block, 18 }, { 9, HANGAR, HANGAR2_AREA_block, 18 }, { 9, TO_ALL, 0, 18 },
	{ 10, HELIPAD1, HELIPAD1_block, 10 }, { 10, HANGAR, HANGAR2_AREA_block, 16 }, { 10, HELITAKEOFF, 0, 47 },
	{ 11, HELIPAD2, HELIPAD2_block, 11 }, { 11, HANGAR, HANGAR2_AREA_block, 17 }, { 11, HELITAKEOFF, 0, 48 },
	{ 12, TO_ALL, TERM_GROUP2_ENTER1_block, 13 },
	{ 13, TO_ALL, TERM_GROUP2_ENTER1_block, 14 },
	{ 14, TO_ALL, TERM_GROUP2_ENTER2_block, 15 },
	{ 15, TO_ALL, TERM_GROUP2_ENTER2_block, 16 },
	{ 16, TERMGROUP, TERM_GROUP2_block, 0 }, { 16, TERM4, TERM4_block, 7 }, { 16, HELIPAD1, HELIPAD1_block, 10 }, { 16, HELITAKEOFF, HELIPAD1_block, 10 }, { 16, TO_ALL, 0, 17 },
	{ 17, TERMGROUP, TERM_GROUP2_block, 0 }, { 17, TERM5, TERM5_block, 8 }, { 17, TERM4, 0, 16 }, { 17, HELIPAD1, 0, 16 }, { 17, HELIPAD2, HELIPAD2_block, 11 }, { 17, HELITAKEOFF, HELIPAD2_block, 11 }, { 17, TO_ALL, 0, 18 },
	{ 18, TERMGROUP, TERM_GROUP2_block, 0 }, { 18, TERM6, TERM6_block, 9 }, { 18, TAKEOFF, 0, 19 }, { 18, HANGAR, HANGAR2_AREA_block, 3 }, { 18, TO_ALL, 0, 17 },
	{ 19, TO_ALL, TERM_GROUP2_EXIT1_block, 20 },
	{ 20, TO_ALL, TERM_GROUP2_EXIT1_block, 21 },
	{ 21, TO_ALL, TERM_GROUP2_EXIT2_block, 22 },
	{ 22, TO_ALL, TERM_GROUP2_EXIT2_block, 26 },
	{ 23, TERMGROUP, TERM_GROUP1_block, 0 }, { 23, TERM1, TERM1_block, 4 }, { 23, HANGAR, AIRPORT_ENTRANCE_block, 2 }, { 23, TO_ALL, 0, 24 },
	{ 24, TERMGROUP, TERM_GROUP1_block, 0 }, { 24, TERM2, TERM2_block, 5 }, { 24, TERM1, 0, 23 }, { 24, HANGAR, 0, 23 }, { 24, TO_ALL, 0, 25 },
	{ 25, TERMGROUP, TERM_GROUP1_block, 0 }, { 25, TERM3, TERM3_block, 6 }, { 25, TAKEOFF, 0, 26 }, { 25, TO_ALL, 0, 24 },
	{ 26, TERMGROUP, TAXIWAY_BUSY_block, 0 }, { 26, TAKEOFF, 0, 27 }, { 26, TO_ALL, 0, 25 },
	{ 27, TO_ALL, OUT_WAY_block, 28 },
	/* takeoff */
	{ 28, TAKEOFF, OUT_WAY_block, 29 },
	{ 29, TO_ALL, RUNWAY_OUT_block, 30 },
	{ 30, STARTTAKEOFF, NOTHING_block, 31 },
	{ 31, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 32, FLYING, NOTHING_block, 37 }, { 32, LANDING, 0, 33 }, { 32, HELILANDING, 0, 41 },
	{ 33, LANDING, RUNWAY_IN_block, 34 },
	{ 34, TO_ALL, RUNWAY_IN_block, 35 },
	{ 35, TO_ALL, RUNWAY_IN_block, 36 },
	{ 36, ENDLANDING, IN_WAY_block, 36 }, { 36, TERMGROUP, TERM_GROUP1_block, 0 }, { 36, TERMGROUP, TERM_GROUP2_ENTER1_block, 1 }, { 36, TERM4, 0, 12 }, { 36, TERM5, 0, 12 }, { 36, TERM6, 0, 12 }, { 36, TO_ALL, 0, 2 },
	/* In Air */
	{ 37, TO_ALL, NOTHING_block, 38 },
	{ 38, TO_ALL, NOTHING_block, 39 },
	{ 39, TO_ALL, NOTHING_block, 40 },
	{ 40, TO_ALL, NOTHING_block, 32 },
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{ 41, HELILANDING, PRE_HELIPAD_block, 42 },
	{ 42, HELIENDLANDING, PRE_HELIPAD_block, 42 }, { 42, HELIPAD1, 0, 43 }, { 42, HELIPAD2, 0, 44 }, { 42, HANGAR, 0, 49 },
	{ 43, TO_ALL, NOTHING_block, 45 },
	{ 44, TO_ALL, NOTHING_block, 46 },
	/* landing */
	{ 45, TERMGROUP, NOTHING_block, 0 }, { 45, HELIPAD1, HELIPAD1_block, 10 },
	{ 46, TERMGROUP, NOTHING_block, 0 }, { 46, HELIPAD2, HELIPAD2_block, 11 },
	/* Helicopter -- takeoff */
	{ 47, HELITAKEOFF, NOTHING_block, 0 },
	{ 48, HELITAKEOFF, NOTHING_block, 0 },
	{ 49, TO_ALL, HANGAR2_AREA_block, 50 }, // need to go to hangar when waiting in air
	{ 50, TO_ALL, HANGAR2_AREA_block, 3 },
	{ 51, HELITAKEOFF, NOTHING_block, 0 },
	{ 52, HELITAKEOFF, NOTHING_block, 0 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

/* intercontinental */
static const HangarTileTable _airport_depots_intercontinental[] = { {{0, 5}, DIR_SE, 0}, {{8, 4}, DIR_SE, 1} };
static const byte _airport_terminal_intercontinental[] = { 2, 4, 4 };
static const byte _airport_entries_intercontinental[] = { 44, 43, 46, 45 };
static const AirportFTAbuildup _airport_fta_intercontinental[] = {
	{  0, HANGAR, NOTHING_block, 2 }, { 0, TERMGROUP, HANGAR1_AREA_block | TERM_GROUP1_block, 0 }, { 0, TERMGROUP, HANGAR1_AREA_block | TERM_GROUP1_block, 1 }, { 0, TAKEOFF, HANGAR1_AREA_block | TERM_GROUP1_block, 2 }, { 0, TO_ALL, 0, 2 },
	{  1, HANGAR, NOTHING_block, 3 }, { 1, TERMGROUP, HANGAR2_AREA_block, 1 }, { 1, TERMGROUP, HANGAR2_AREA_block, 0 }, { 1, TO_ALL, 0, 3 },
	{  2, TERMGROUP, HANGAR1_AREA_block, 0 }, { 2, TERMGROUP, TERM_GROUP1_block, 0 }, { 2, TERMGROUP, TERM_GROUP1_block, 1 }, { 2, HANGAR, 0, 0 }, { 2, TAKEOFF, TERM_GROUP1_block, 27 }, { 2, TERM5, 0, 26 }, { 2, TERM6, 0, 26 }, { 2, TERM7, 0, 26 }, { 2, TERM8, 0, 26 }, { 2, HELIPAD1, 0, 26 }, { 2, HELIPAD2, 0, 26 }, { 2, HELITAKEOFF, 0, 74 }, { 2, TO_ALL, 0, 27 },
	{  3, TERMGROUP, HANGAR2_AREA_block, 0 }, { 3, HANGAR, 0, 1 }, { 3, HELITAKEOFF, 0, 75 }, {3, TAKEOFF, 0, 59}, { 3, TO_ALL, 0, 20 },
	{  4, TERM1, TERM1_block, 26 }, { 4, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 26 }, { 4, TO_ALL, 0, 26 },
	{  5, TERM2, TERM2_block, 27 }, { 5, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 27 }, { 5, TO_ALL, 0, 27 },
	{  6, TERM3, TERM3_block, 28 }, { 6, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 28 }, { 6, TO_ALL, 0, 28 },
	{  7, TERM4, TERM4_block, 29 }, { 7, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 29 }, { 7, TO_ALL, 0, 29 },
	{  8, TERM5, TERM5_block, 18 }, { 8, HANGAR, HANGAR2_AREA_block, 18 }, { 8, TO_ALL, 0, 18 },
	{  9, TERM6, TERM6_block, 19 }, { 9, HANGAR, HANGAR2_AREA_block, 19 }, { 9, TO_ALL, 0, 19 },
	{ 10, TERM7, TERM7_block, 20 }, { 10, HANGAR, HANGAR2_AREA_block, 20 }, { 10, TO_ALL, 0, 20 },
	{ 11, TERM8, TERM8_block, 21 }, { 11, HANGAR, HANGAR2_AREA_block, 21 }, { 11, TO_ALL, 0, 21 },
	{ 12, HELIPAD1, HELIPAD1_block, 12 }, { 12, HANGAR, 0, 70 }, { 12, HELITAKEOFF, 0, 72 },
	{ 13, HELIPAD2, HELIPAD2_block, 13 }, { 13, HANGAR, 0, 71 }, { 13, HELITAKEOFF, 0, 73 },
	{ 14, TO_ALL, TERM_GROUP2_ENTER1_block, 15 },
	{ 15, TO_ALL, TERM_GROUP2_ENTER1_block, 16 },
	{ 16, TO_ALL, TERM_GROUP2_ENTER2_block, 17 },
	{ 17, TO_ALL, TERM_GROUP2_ENTER2_block, 18 },
	{ 18, TERMGROUP, TERM_GROUP2_block, 0 }, { 18, TERM5, TERM5_block, 8 }, { 18, TAKEOFF, 0, 19 }, { 18, HELITAKEOFF, HELIPAD1_block, 19 }, { 18, TO_ALL, TERM_GROUP2_EXIT1_block, 19 },
	{ 19, TERMGROUP, TERM_GROUP2_block, 0 }, { 19, TERM6, TERM6_block, 9 }, { 19, TERM5, 0, 18 }, { 19, TAKEOFF, 0, 57 }, { 19, HELITAKEOFF, HELIPAD1_block, 20 }, { 19, TO_ALL, TERM_GROUP2_EXIT1_block, 20 }, // add exit to runway out 2
	{ 20, TERMGROUP, TERM_GROUP2_block, 0 }, { 20, TERM7, TERM7_block, 10 }, { 20, TERM5, 0, 19 }, { 20, TERM6, 0, 19 }, { 20, HANGAR, HANGAR2_AREA_block, 3 }, { 20, TAKEOFF, 0, 19 }, { 20, TO_ALL, TERM_GROUP2_EXIT1_block, 21 },
	{ 21, TERMGROUP, TERM_GROUP2_block, 0 }, { 21, TERM8, TERM8_block, 11 }, { 21, HANGAR, HANGAR2_AREA_block, 20 }, { 21, TERM5, 0, 20 }, { 21, TERM6, 0, 20 }, { 21, TERM7, 0, 20 }, { 21, TAKEOFF, 0, 20 }, { 21, TO_ALL, TERM_GROUP2_EXIT1_block, 22 },
	{ 22, TERMGROUP, TERM_GROUP2_block, 0 }, { 22, HANGAR, 0, 21 }, { 22, TERM5, 0, 21 }, { 22, TERM6, 0, 21 }, { 22, TERM7, 0, 21 }, { 22, TERM8, 0, 21 }, { 22, TAKEOFF, 0, 21 }, { 22, TO_ALL, 0, 23 },
	{ 23, TO_ALL, TERM_GROUP2_EXIT1_block, 70 },
	{ 24, TO_ALL, TERM_GROUP2_EXIT2_block, 25 },
	{ 25, TERMGROUP, TERM_GROUP2_EXIT2_block, 0 }, { 25, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 29 }, { 25, TO_ALL, 0, 29 },
	{ 26, TERMGROUP, TERM_GROUP1_block, 0 }, { 26, TERM1, TERM1_block, 4 }, { 26, HANGAR, HANGAR1_AREA_block, 27 }, { 26, TERM5, TERM_GROUP2_ENTER1_block, 14 }, { 26, TERM6, TERM_GROUP2_ENTER1_block, 14 }, { 26, TERM7, TERM_GROUP2_ENTER1_block, 14 }, { 26, TERM8, TERM_GROUP2_ENTER1_block, 14 }, { 26, HELIPAD1, TERM_GROUP2_ENTER1_block, 14 }, { 26, HELIPAD2, TERM_GROUP2_ENTER1_block, 14 }, { 26, HELITAKEOFF, TERM_GROUP2_ENTER1_block, 14 }, { 26, TO_ALL, 0, 27 },
	{ 27, TERMGROUP, TERM_GROUP1_block, 0 }, { 27, TERM2, TERM2_block, 5 }, { 27, HANGAR, HANGAR1_AREA_block, 2 }, { 27, TERM1, 0, 26 }, { 27, TERM5, 0, 26 }, { 27, TERM6, 0, 26 }, { 27, TERM7, 0, 26 }, { 27, TERM8, 0, 26 }, { 27, HELIPAD1, 0, 14 }, { 27, HELIPAD2, 0, 14 }, { 27, TO_ALL, 0, 28 },
	{ 28, TERMGROUP, TERM_GROUP1_block, 0 }, { 28, TERM3, TERM3_block, 6 }, { 28, HANGAR, HANGAR1_AREA_block, 27 }, { 28, TERM1, 0, 27 }, { 28, TERM2, 0, 27 }, { 28, TERM4, 0, 29 }, { 28, TERM5, 0, 14 }, { 28, TERM6, 0, 14 }, { 28, TERM7, 0, 14 }, { 28, TERM8, 0, 14 }, { 28, HELIPAD1, 0, 14 }, { 28, HELIPAD2, 0, 14 }, { 28, TO_ALL, 0, 29 },
	{ 29, TERMGROUP, TERM_GROUP1_block, 0 }, { 29, TERM4, TERM4_block, 7 }, { 29, HANGAR, HANGAR1_AREA_block, 27 }, { 29, TAKEOFF, 0, 30 }, { 29, TO_ALL, 0, 28 },
	{ 30, TO_ALL, OUT_WAY_block2, 31 },
	{ 31, TO_ALL, OUT_WAY_block, 32 },
	/* takeoff */
	{ 32, TAKEOFF, RUNWAY_OUT_block, 33 },
	{ 33, TO_ALL, RUNWAY_OUT_block, 34 },
	{ 34, STARTTAKEOFF, NOTHING_block, 35 },
	{ 35, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 36, TO_ALL, 0, 0 },
	{ 37, LANDING, RUNWAY_IN_block, 38 },
	{ 38, TO_ALL, RUNWAY_IN_block, 39 },
	{ 39, TO_ALL, RUNWAY_IN_block, 40 },
	{ 40, ENDLANDING, RUNWAY_IN_block, 41 },
	{ 41, TO_ALL, IN_WAY_block, 42 },
	{ 42, TERMGROUP, IN_WAY_block, 0 }, { 42, TERMGROUP, TERM_GROUP1_block, 0 }, { 42, TERMGROUP, TERM_GROUP1_block, 1 }, { 42, HANGAR, 0, 2 }, { 42, TO_ALL, 0, 26 },
	/* In Air */
	{ 43, TO_ALL, 0, 44 },
	{ 44, FLYING, 0, 45 }, { 44, HELILANDING, 0, 47 }, { 44, LANDING, 0, 69 }, { 44, TO_ALL, 0, 45 },
	{ 45, TO_ALL, 0, 46 },
	{ 46, FLYING, 0, 43 }, { 46, LANDING, 0, 76 }, { 46, TO_ALL, 0, 43 },
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{ 47, HELILANDING, PRE_HELIPAD_block, 48 },
	{ 48, HELIENDLANDING, PRE_HELIPAD_block, 48 }, { 48, HELIPAD1, 0, 49 }, { 48, HELIPAD2, 0, 50 }, { 48, HANGAR, 0, 55 },
	{ 49, TO_ALL, NOTHING_block, 51 },
	{ 50, TO_ALL, NOTHING_block, 52 },
	/* landing */
	{ 51, TERMGROUP, NOTHING_block, 0 }, { 51, HELIPAD1, HELIPAD1_block, 12 }, { 51, HANGAR, 0, 55 }, { 51, TO_ALL, 0, 12 },
	{ 52, TERMGROUP, NOTHING_block, 0 }, { 52, HELIPAD2, HELIPAD2_block, 13 }, { 52, HANGAR, 0, 55 }, { 52, TO_ALL, 0, 13 },
	/* Helicopter -- takeoff */
	{ 53, HELITAKEOFF, NOTHING_block, 0 },
	{ 54, HELITAKEOFF, NOTHING_block, 0 },
	{ 55, TO_ALL, HANGAR2_AREA_block, 56 }, // need to go to hangar when waiting in air
	{ 56, TO_ALL, HANGAR2_AREA_block, 3 },
	/* runway 2 out support */
	{ 57, TERMGROUP, OUT_WAY2_block, 0 }, { 57, TAKEOFF, 0, 58 }, { 57, TO_ALL, 0, 58 },
	{ 58, TO_ALL, OUT_WAY2_block, 59 },
	{ 59, TAKEOFF, RUNWAY_OUT2_block, 60 }, // takeoff
	{ 60, TO_ALL, RUNWAY_OUT2_block, 61 },
	{ 61, STARTTAKEOFF, NOTHING_block, 62 },
	{ 62, ENDTAKEOFF, NOTHING_block, 0 },
	/* runway 2 in support */
	{ 63, LANDING, RUNWAY_IN2_block, 64 },
	{ 64, TO_ALL, RUNWAY_IN2_block, 65 },
	{ 65, TO_ALL, RUNWAY_IN2_block, 66 },
	{ 66, ENDLANDING, RUNWAY_IN2_block, 0 }, { 66, TERMGROUP, 0, 1 }, { 66, TERMGROUP, 0, 0 }, { 66, TO_ALL, 0, 67 },
	{ 67, TO_ALL, IN_WAY2_block, 68 },
	{ 68, TERMGROUP, IN_WAY2_block, 0 }, { 68, TERMGROUP, TERM_GROUP2_block, 1 }, { 68, TERMGROUP, TERM_GROUP1_block, 0 }, { 68, HANGAR, HANGAR2_AREA_block, 22 }, { 68, TO_ALL, 0, 22 },
	{ 69, TERMGROUP, RUNWAY_IN2_block, 0 }, { 69, TO_ALL, RUNWAY_IN2_block, 63 },
	{ 70, TERMGROUP, TERM_GROUP2_EXIT1_block, 0 }, { 70, HELIPAD1, HELIPAD1_block, 12 }, { 70, HELITAKEOFF, HELIPAD1_block, 12 }, { 70, TO_ALL, 0, 71 },
	{ 71, TERMGROUP, TERM_GROUP2_EXIT1_block, 0 }, { 71, HELIPAD2, HELIPAD2_block, 13 }, { 71, HELITAKEOFF, HELIPAD1_block, 12 }, { 71, TO_ALL, 0, 24 },
	{ 72, TO_ALL, HELIPAD1_block, 53 },
	{ 73, TO_ALL, HELIPAD2_block, 54 },
	{ 74, HELITAKEOFF, NOTHING_block, 0 },
	{ 75, HELITAKEOFF, NOTHING_block, 0 },
	{ 76, TERMGROUP, RUNWAY_IN_block, 0 }, { 76, TO_ALL, RUNWAY_IN_block, 37 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

/* intercontinental */
static const HangarTileTable _airport_depots_terminus4[] = { {{0, 5}, DIR_SE, 0}, {{8, 4}, DIR_SE, 1} };
static const byte _airport_terminal_terminus4[] = { 2, 4, 4 };
static const byte _airport_entries_terminus4[] = { 44, 43, 46, 45 };
static const AirportFTAbuildup _airport_fta_terminus4[] = {
	{  0, HANGAR, NOTHING_block, 2 }, { 0, TERMGROUP, HANGAR1_AREA_block | TERM_GROUP1_block, 0 }, { 0, TERMGROUP, HANGAR1_AREA_block | TERM_GROUP1_block, 1 }, { 0, TAKEOFF, HANGAR1_AREA_block | TERM_GROUP1_block, 2 }, { 0, TO_ALL, 0, 2 },
	{  1, HANGAR, NOTHING_block, 3 }, { 1, TERMGROUP, HANGAR2_AREA_block, 1 }, { 1, TERMGROUP, HANGAR2_AREA_block, 0 }, { 1, TO_ALL, 0, 3 },
	{  2, TERMGROUP, HANGAR1_AREA_block, 0 }, { 2, TERMGROUP, TERM_GROUP1_block, 0 }, { 2, TERMGROUP, TERM_GROUP1_block, 1 }, { 2, HANGAR, 0, 0 }, { 2, TAKEOFF, TERM_GROUP1_block, 27 }, { 2, TERM5, 0, 26 }, { 2, TERM6, 0, 26 }, { 2, TERM7, 0, 26 }, { 2, TERM8, 0, 26 }, { 2, HELIPAD1, 0, 26 }, { 2, HELIPAD2, 0, 26 }, { 2, HELITAKEOFF, 0, 74 }, { 2, TO_ALL, 0, 27 },
	{  3, TERMGROUP, HANGAR2_AREA_block, 0 }, { 3, HANGAR, 0, 1 }, { 3, HELITAKEOFF, 0, 75 }, {3, TAKEOFF, 0, 59}, { 3, TO_ALL, 0, 20 },
	{  4, TERM1, TERM1_block, 26 }, { 4, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 26 }, { 4, TO_ALL, 0, 26 },
	{  5, TERM2, TERM2_block, 27 }, { 5, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 27 }, { 5, TO_ALL, 0, 27 },
	{  6, TERM3, TERM3_block, 28 }, { 6, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 28 }, { 6, TO_ALL, 0, 28 },
	{  7, TERM4, TERM4_block, 29 }, { 7, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 29 }, { 7, TO_ALL, 0, 29 },
	{  8, TERM5, TERM5_block, 18 }, { 8, HANGAR, HANGAR2_AREA_block, 18 }, { 8, TO_ALL, 0, 18 },
	{  9, TERM6, TERM6_block, 19 }, { 9, HANGAR, HANGAR2_AREA_block, 19 }, { 9, TO_ALL, 0, 19 },
	{ 10, TERM7, TERM7_block, 20 }, { 10, HANGAR, HANGAR2_AREA_block, 20 }, { 10, TO_ALL, 0, 20 },
	{ 11, TERM8, TERM8_block, 21 }, { 11, HANGAR, HANGAR2_AREA_block, 21 }, { 11, TO_ALL, 0, 21 },
	{ 12, HELIPAD1, HELIPAD1_block, 12 }, { 12, HANGAR, 0, 70 }, { 12, HELITAKEOFF, 0, 72 },
	{ 13, HELIPAD2, HELIPAD2_block, 13 }, { 13, HANGAR, 0, 71 }, { 13, HELITAKEOFF, 0, 73 },
	{ 14, TO_ALL, TERM_GROUP2_ENTER1_block, 15 },
	{ 15, TO_ALL, TERM_GROUP2_ENTER1_block, 16 },
	{ 16, TO_ALL, TERM_GROUP2_ENTER2_block, 17 },
	{ 17, TO_ALL, TERM_GROUP2_ENTER2_block, 18 },
	{ 18, TERMGROUP, TERM_GROUP2_block, 0 }, { 18, TERM5, TERM5_block, 8 }, { 18, TAKEOFF, 0, 19 }, { 18, HELITAKEOFF, HELIPAD1_block, 19 }, { 18, TO_ALL, TERM_GROUP2_EXIT1_block, 19 },
	{ 19, TERMGROUP, TERM_GROUP2_block, 0 }, { 19, TERM6, TERM6_block, 9 }, { 19, TERM5, 0, 18 }, { 19, TAKEOFF, 0, 57 }, { 19, HELITAKEOFF, HELIPAD1_block, 20 }, { 19, TO_ALL, TERM_GROUP2_EXIT1_block, 20 }, // add exit to runway out 2
	{ 20, TERMGROUP, TERM_GROUP2_block, 0 }, { 20, TERM7, TERM7_block, 10 }, { 20, TERM5, 0, 19 }, { 20, TERM6, 0, 19 }, { 20, HANGAR, HANGAR2_AREA_block, 3 }, { 20, TAKEOFF, 0, 19 }, { 20, TO_ALL, TERM_GROUP2_EXIT1_block, 21 },
	{ 21, TERMGROUP, TERM_GROUP2_block, 0 }, { 21, TERM8, TERM8_block, 11 }, { 21, HANGAR, HANGAR2_AREA_block, 20 }, { 21, TERM5, 0, 20 }, { 21, TERM6, 0, 20 }, { 21, TERM7, 0, 20 }, { 21, TAKEOFF, 0, 20 }, { 21, TO_ALL, TERM_GROUP2_EXIT1_block, 22 },
	{ 22, TERMGROUP, TERM_GROUP2_block, 0 }, { 22, HANGAR, 0, 21 }, { 22, TERM5, 0, 21 }, { 22, TERM6, 0, 21 }, { 22, TERM7, 0, 21 }, { 22, TERM8, 0, 21 }, { 22, TAKEOFF, 0, 21 }, { 22, TO_ALL, 0, 23 },
	{ 23, TO_ALL, TERM_GROUP2_EXIT1_block, 70 },
	{ 24, TO_ALL, TERM_GROUP2_EXIT2_block, 25 },
	{ 25, TERMGROUP, TERM_GROUP2_EXIT2_block, 0 }, { 25, HANGAR, HANGAR1_AREA_block | TERM_GROUP1_block, 29 }, { 25, TO_ALL, 0, 29 },
	{ 26, TERMGROUP, TERM_GROUP1_block, 0 }, { 26, TERM1, TERM1_block, 4 }, { 26, HANGAR, HANGAR1_AREA_block, 27 }, { 26, TERM5, TERM_GROUP2_ENTER1_block, 14 }, { 26, TERM6, TERM_GROUP2_ENTER1_block, 14 }, { 26, TERM7, TERM_GROUP2_ENTER1_block, 14 }, { 26, TERM8, TERM_GROUP2_ENTER1_block, 14 }, { 26, HELIPAD1, TERM_GROUP2_ENTER1_block, 14 }, { 26, HELIPAD2, TERM_GROUP2_ENTER1_block, 14 }, { 26, HELITAKEOFF, TERM_GROUP2_ENTER1_block, 14 }, { 26, TO_ALL, 0, 27 },
	{ 27, TERMGROUP, TERM_GROUP1_block, 0 }, { 27, TERM2, TERM2_block, 5 }, { 27, HANGAR, HANGAR1_AREA_block, 2 }, { 27, TERM1, 0, 26 }, { 27, TERM5, 0, 26 }, { 27, TERM6, 0, 26 }, { 27, TERM7, 0, 26 }, { 27, TERM8, 0, 26 }, { 27, HELIPAD1, 0, 14 }, { 27, HELIPAD2, 0, 14 }, { 27, TO_ALL, 0, 28 },
	{ 28, TERMGROUP, TERM_GROUP1_block, 0 }, { 28, TERM3, TERM3_block, 6 }, { 28, HANGAR, HANGAR1_AREA_block, 27 }, { 28, TERM1, 0, 27 }, { 28, TERM2, 0, 27 }, { 28, TERM4, 0, 29 }, { 28, TERM5, 0, 14 }, { 28, TERM6, 0, 14 }, { 28, TERM7, 0, 14 }, { 28, TERM8, 0, 14 }, { 28, HELIPAD1, 0, 14 }, { 28, HELIPAD2, 0, 14 }, { 28, TO_ALL, 0, 29 },
	{ 29, TERMGROUP, TERM_GROUP1_block, 0 }, { 29, TERM4, TERM4_block, 7 }, { 29, HANGAR, HANGAR1_AREA_block, 27 }, { 29, TAKEOFF, 0, 30 }, { 29, TO_ALL, 0, 28 },
	{ 30, TO_ALL, OUT_WAY_block2, 31 },
	{ 31, TO_ALL, OUT_WAY_block, 32 },
	/* takeoff */
	{ 32, TAKEOFF, RUNWAY_OUT_block, 33 },
	{ 33, TO_ALL, RUNWAY_OUT_block, 34 },
	{ 34, STARTTAKEOFF, NOTHING_block, 35 },
	{ 35, ENDTAKEOFF, NOTHING_block, 0 },
	/* landing */
	{ 36, TO_ALL, 0, 0 },
	{ 37, LANDING, RUNWAY_IN_block, 38 },
	{ 38, TO_ALL, RUNWAY_IN_block, 39 },
	{ 39, TO_ALL, RUNWAY_IN_block, 40 },
	{ 40, ENDLANDING, RUNWAY_IN_block, 41 },
	{ 41, TO_ALL, IN_WAY_block, 42 },
	{ 42, TERMGROUP, IN_WAY_block, 0 }, { 42, TERMGROUP, TERM_GROUP1_block, 0 }, { 42, TERMGROUP, TERM_GROUP1_block, 1 }, { 42, HANGAR, 0, 2 }, { 42, TO_ALL, 0, 26 },
	/* In Air */
	{ 43, TO_ALL, 0, 44 },
	{ 44, FLYING, 0, 45 }, { 44, HELILANDING, 0, 47 }, { 44, LANDING, 0, 69 }, { 44, TO_ALL, 0, 45 },
	{ 45, TO_ALL, 0, 46 },
	{ 46, FLYING, 0, 43 }, { 46, LANDING, 0, 76 }, { 46, TO_ALL, 0, 43 },
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{ 47, HELILANDING, PRE_HELIPAD_block, 48 },
	{ 48, HELIENDLANDING, PRE_HELIPAD_block, 48 }, { 48, HELIPAD1, 0, 49 }, { 48, HELIPAD2, 0, 50 }, { 48, HANGAR, 0, 55 },
	{ 49, TO_ALL, NOTHING_block, 51 },
	{ 50, TO_ALL, NOTHING_block, 52 },
	/* landing */
	{ 51, TERMGROUP, NOTHING_block, 0 }, { 51, HELIPAD1, HELIPAD1_block, 12 }, { 51, HANGAR, 0, 55 }, { 51, TO_ALL, 0, 12 },
	{ 52, TERMGROUP, NOTHING_block, 0 }, { 52, HELIPAD2, HELIPAD2_block, 13 }, { 52, HANGAR, 0, 55 }, { 52, TO_ALL, 0, 13 },
	/* Helicopter -- takeoff */
	{ 53, HELITAKEOFF, NOTHING_block, 0 },
	{ 54, HELITAKEOFF, NOTHING_block, 0 },
	{ 55, TO_ALL, HANGAR2_AREA_block, 56 }, // need to go to hangar when waiting in air
	{ 56, TO_ALL, HANGAR2_AREA_block, 3 },
	/* runway 2 out support */
	{ 57, TERMGROUP, OUT_WAY2_block, 0 }, { 57, TAKEOFF, 0, 58 }, { 57, TO_ALL, 0, 58 },
	{ 58, TO_ALL, OUT_WAY2_block, 59 },
	{ 59, TAKEOFF, RUNWAY_OUT2_block, 60 }, // takeoff
	{ 60, TO_ALL, RUNWAY_OUT2_block, 61 },
	{ 61, STARTTAKEOFF, NOTHING_block, 62 },
	{ 62, ENDTAKEOFF, NOTHING_block, 0 },
	/* runway 2 in support */
	{ 63, LANDING, RUNWAY_IN2_block, 64 },
	{ 64, TO_ALL, RUNWAY_IN2_block, 65 },
	{ 65, TO_ALL, RUNWAY_IN2_block, 66 },
	{ 66, ENDLANDING, RUNWAY_IN2_block, 0 }, { 66, TERMGROUP, 0, 1 }, { 66, TERMGROUP, 0, 0 }, { 66, TO_ALL, 0, 67 },
	{ 67, TO_ALL, IN_WAY2_block, 68 },
	{ 68, TERMGROUP, IN_WAY2_block, 0 }, { 68, TERMGROUP, TERM_GROUP2_block, 1 }, { 68, TERMGROUP, TERM_GROUP1_block, 0 }, { 68, HANGAR, HANGAR2_AREA_block, 22 }, { 68, TO_ALL, 0, 22 },
	{ 69, TERMGROUP, RUNWAY_IN2_block, 0 }, { 69, TO_ALL, RUNWAY_IN2_block, 63 },
	{ 70, TERMGROUP, TERM_GROUP2_EXIT1_block, 0 }, { 70, HELIPAD1, HELIPAD1_block, 12 }, { 70, HELITAKEOFF, HELIPAD1_block, 12 }, { 70, TO_ALL, 0, 71 },
	{ 71, TERMGROUP, TERM_GROUP2_EXIT1_block, 0 }, { 71, HELIPAD2, HELIPAD2_block, 13 }, { 71, HELITAKEOFF, HELIPAD1_block, 12 }, { 71, TO_ALL, 0, 24 },
	{ 72, TO_ALL, HELIPAD1_block, 53 },
	{ 73, TO_ALL, HELIPAD2_block, 54 },
	{ 74, HELITAKEOFF, NOTHING_block, 0 },
	{ 75, HELITAKEOFF, NOTHING_block, 0 },
	{ 76, TERMGROUP, RUNWAY_IN_block, 0 }, { 76, TO_ALL, RUNWAY_IN_block, 37 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

/* heliports, oilrigs don't have depots */
static const byte _airport_entries_heliport[] = { 7, 7, 7, 7 };
static const AirportFTAbuildup _airport_fta_heliport[] = {
	{ 0, HELIPAD1, HELIPAD1_block, 1 },
	{ 1, HELITAKEOFF, NOTHING_block, 0 }, // takeoff
	{ 2, TERMGROUP, AIRPORT_BUSY_block, 0 }, { 2, HELILANDING, 0, 3 }, { 2, HELITAKEOFF, 0, 1 },
	{ 3, HELILANDING, AIRPORT_BUSY_block, 4 },
	{ 4, HELIENDLANDING, AIRPORT_BUSY_block, 4 }, { 4, HELIPAD1, HELIPAD1_block, 0 }, { 4, HELITAKEOFF, 0, 2 },
	/* In Air */
	{ 5, TO_ALL, NOTHING_block, 6 },
	{ 6, TO_ALL, NOTHING_block, 7 },
	{ 7, TO_ALL, NOTHING_block, 8 },
	{ 8, FLYING, NOTHING_block, 5 }, { 8, HELILANDING, HELIPAD1_block, 2 }, // landing
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};
#define _airport_entries_oilrig _airport_entries_heliport
#define _airport_fta_oilrig _airport_fta_heliport

/* helidepots */
static const HangarTileTable _airport_depots_helidepot[] = { {{1, 0}, DIR_SE, 0} };
static const byte _airport_entries_helidepot[] = { 4, 4, 4, 4 };
static const AirportFTAbuildup _airport_fta_helidepot[] = {
	{  0, HANGAR, NOTHING_block, 1 },
	{  1, TERMGROUP, HANGAR2_AREA_block, 0 }, { 1, HANGAR, 0, 0 }, { 1, HELIPAD1, HELIPAD1_block, 14 }, { 1, HELITAKEOFF, 0, 15 }, { 1, TO_ALL, 0, 0 },
	{  2, FLYING, NOTHING_block, 3 }, { 2, HELILANDING, PRE_HELIPAD_block, 7 }, { 2, HANGAR, 0, 12 }, { 2, HELITAKEOFF, NOTHING_block, 16 },
	/* In Air */
	{  3, 0, NOTHING_block, 4 },
	{  4, 0, NOTHING_block, 5 },
	{  5, 0, NOTHING_block, 6 },
	{  6, 0, NOTHING_block, 2 },
	/* Helicopter -- stay in air in special place as a buffer to choose from helipads */
	{  7, HELILANDING, PRE_HELIPAD_block, 8 },
	{  8, HELIENDLANDING, PRE_HELIPAD_block, 8 }, { 8, HELIPAD1, 0, 9 }, { 8, HANGAR, 0, 12 }, { 8, TO_ALL, 0, 2 },
	{  9, 0, NOTHING_block, 10 },
	/* landing */
	{ 10, TERMGROUP, NOTHING_block, 10 }, { 10, HELIPAD1, HELIPAD1_block, 14 }, { 10, HANGAR, 0, 1 }, { 10, TO_ALL, 0, 14 },
	/* Helicopter -- takeoff */
	{ 11, HELITAKEOFF, NOTHING_block, 0 },
	{ 12, TO_ALL, HANGAR2_AREA_block, 13 }, // need to go to hangar when waiting in air
	{ 13, TO_ALL, HANGAR2_AREA_block, 1 },
	{ 14, HELIPAD1, HELIPAD1_block, 14 }, { 14, HANGAR, 0, 1 }, { 14, HELITAKEOFF, 0, 17 },
	{ 15, HELITAKEOFF, NOTHING_block, 0 }, // takeoff outside depot
	{ 16, HELITAKEOFF, 0, 14 },
	{ 17, TO_ALL, NOTHING_block, 11 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

/* helistation */
static const HangarTileTable _airport_depots_helistation[] = { {{0, 0}, DIR_SE, 0} };
static const byte _airport_entries_helistation[] = { 25, 25, 25, 25 };
static const AirportFTAbuildup _airport_fta_helistation[] = {
	{  0, HANGAR, NOTHING_block, 8 },    { 0, HELIPAD1, 0, 1 }, { 0, HELIPAD2, 0, 1 }, { 0, HELIPAD3, 0, 1 }, { 0, HELITAKEOFF, 0, 1 }, { 0, TO_ALL, 0, 0 },
	{  1, TERMGROUP, HANGAR2_AREA_block, 0 },  { 1, HANGAR, 0, 0 }, { 1, HELITAKEOFF, 0, 3 }, { 1, TO_ALL, 0, 4 },
	/* landing */
	{  2, FLYING, NOTHING_block, 28 },   { 2, HELILANDING, 0, 15 }, { 2, TO_ALL, 0, 28 },
	/* helicopter side */
	{  3, HELITAKEOFF, NOTHING_block, 0 }, // helitakeoff outside hangar2
	{  4, TERMGROUP, TAXIWAY_BUSY_block, 0 },  { 4, HANGAR, HANGAR2_AREA_block, 1 }, { 4, HELITAKEOFF, 0, 1 }, { 4, TO_ALL, 0, 5 },
	{  5, TERMGROUP, TAXIWAY_BUSY_block, 0 },  { 5, HELIPAD1, HELIPAD1_block, 6 }, { 5, HELIPAD2, HELIPAD2_block, 7 }, { 5, HELIPAD3, HELIPAD3_block, 8 }, { 5, TO_ALL, 0, 4 },
	{  6, HELIPAD1, HELIPAD1_block, 5 }, { 6, HANGAR, HANGAR2_AREA_block, 5 }, { 6, HELITAKEOFF, 0, 9 }, { 6, TO_ALL, 0, 6 },
	{  7, HELIPAD2, HELIPAD2_block, 5 }, { 7, HANGAR, HANGAR2_AREA_block, 5 }, { 7, HELITAKEOFF, 0, 10 }, { 7, TO_ALL, 0, 7 },
	{  8, HELIPAD3, HELIPAD3_block, 5 }, { 8, HANGAR, HANGAR2_AREA_block, 5 }, { 8, HELITAKEOFF, 0, 11 }, { 8, TO_ALL, 0, 8 },
	{  9, 0, HELIPAD1_block, 12 },
	{ 10, TO_ALL, HELIPAD2_block, 13 },
	{ 11, TO_ALL, HELIPAD3_block, 14 },
	{ 12, HELITAKEOFF, NOTHING_block, 0 },
	{ 13, HELITAKEOFF, NOTHING_block, 0 },
	{ 14, HELITAKEOFF, NOTHING_block, 0 },
	/* heli - in flight moves */
	{ 15, HELILANDING, PRE_HELIPAD_block, 16 },
	{ 16, HELIENDLANDING, PRE_HELIPAD_block, 16 }, { 16, HELIPAD1, 0, 17 }, { 16, HELIPAD2, 0, 18 }, { 16, HELIPAD3, 0, 19 }, { 16, HANGAR, 0, 23 },
	{ 17, TO_ALL, NOTHING_block, 20 },
	{ 18, TO_ALL, NOTHING_block, 21 },
	{ 19, TO_ALL, NOTHING_block, 22 },
	/* heli landing */
	{ 20, TERMGROUP, NOTHING_block, 0 }, { 20, HELIPAD1, HELIPAD1_block, 6 }, { 20, HANGAR, 0, 23 }, { 20, TO_ALL, 0, 6 },
	{ 21, TERMGROUP, NOTHING_block, 0 }, { 21, HELIPAD2, HELIPAD2_block, 7 }, { 21, HANGAR, 0, 23 }, { 21, TO_ALL, 0, 7 },
	{ 22, TERMGROUP, NOTHING_block, 0 }, { 22, HELIPAD3, HELIPAD3_block, 8 }, { 22, HANGAR, 0, 23 }, { 22, TO_ALL, 0, 8 },
	{ 23, TO_ALL, HANGAR2_AREA_block, 24 }, // need to go to helihangar when waiting in air
	{ 24, TO_ALL, HANGAR2_AREA_block, 1 },
	{ 25, TO_ALL, NOTHING_block, 26 },
	{ 26, TO_ALL, NOTHING_block, 27 },
	{ 27, TO_ALL, NOTHING_block, 2 },
	{ 28, TO_ALL, NOTHING_block, 29 },
	{ 29, TO_ALL, NOTHING_block, 30 },
	{ 30, TO_ALL, NOTHING_block, 31 },
	{ 31, TO_ALL, NOTHING_block, 32 },
	{ 32, TO_ALL, NOTHING_block, 25 },
	{ MAX_ELEMENTS, TO_ALL, 0, 0 } // end marker. DO NOT REMOVE
};

#endif /* AIRPORT_MOVEMENT_H */
