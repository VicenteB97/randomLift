#ifndef ___SIGNALOID_PROJ_HEADER_H___
#define ___SIGNALOID_PROJ_HEADER_H___

// Include headers that will be used throughout the project
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
// Only include uxhw.h if we are in the Singlaoid platform
// (verify -DRANDOM exists in config.mk)
#ifdef RANDOM
#include <uxhw.h>
#endif

// Commonly defined in stdlib.h, but include them just in case
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

// Physical constants required for intermediate computations
const double __DRY_AIR_CONSTANT__ = 287.05;
const double __WATER_VAPOR_CONSTANT__ = 461.495;

/// @brief Checks if all elements of a double-type array are nonnegative
/// @param valuesArray Double-type array
/// @param numValuesCount Number of elements in the array/Number of values to check
/// @returns Status code (0 success, 1 failure)
bool 
arePositiveValues(double* valuesArray);

/// @brief Computes the specific gas constant from humidity factor in ambient air
/// @param humidityFactor__ a number between 0 and 1 that shows the water vapor/air mix
/// @returns Specific gas constant in J/(Kg K)
double 
computeSpecificGasConstantHumidAir(
	double humidityFactor__
);

/// @brief Computes the air density from pressure and density of surronding
/// air by the ideal gas law.
/// @param staticPressure__Pa Air pressure (Pa)
/// @param humidAirGasConstant__J_KgK Specific gas constant for humid air (J/Kg*K)
/// @param airTemperature__K Air temperature (K)
/// @return Air density (kg/m3)
double 
computeAirDensity(
    double staticPressure__Pa,
	double humidAirGasConstant__J_KgK,
    double airTemperature__K
);

/// @brief Computes the squared velocity (m/s) as measured by a Pitot Tube.
/// @param staticPressure__Pa Static ambient pressure
/// @param totalPressure__Pa Total (stagnation pressure) measured at Pitot tube
/// @param airDensity__Kg_m3 Ambient air density
/// @returns Squared velocity (m/s)
double 
computeAirVelocitySquared(
	double staticPressure__Pa,
	double totalPressure__Pa,
	double airDensity__Kg_m3
);

/// @brief Computes the lift force (N).
/// @param airDensity__kg_m3 Air density (kg/m3)
/// @param squaredAirflowVelocity__m_s Airflow velocity measured by Pitot tube (m/s)
/// @param surfaceArea__m2 Surface area of airfoil (m2)
/// @param liftCoefficient__ Lift coefficient of airfoil (dimensionless)
/// @return Lift force (N)
double 
computeLiftAirfoil(
    double airDensity__kg_m3,
    double squaredAirflowVelocity__m_s,
    double surfaceArea__m2,
    double liftCoefficient__ // No units added, since its a dimensionless constant
);

/// @brief This function computes the lift force (N) of an airfoil using
/// pressure, temperature and its geometry.
/// @param outputLiftForce__N Lift force from the airfoil
/// @param totalPressure__Pa Total pressure (Pa) measured by Pitot tube
/// @param staticPressure__Pa Static pressure (Pa) measured by Pitot tube
/// static port.
/// @param airTemperature__K Ambient air temperature (K)
/// @param humidityFactor__ Humidity factor (0-1)
/// @param surfaceArea__m2 Surface area (m2) of airfoil
/// @param liftCoefficient__ Airfoil lift coefficient
/// @returns Status code (0 success, 1 failure)
uint16_t 
computeLiftFunction(
    double* outputLiftForce__N,
    double totalPressure__Pa,
    double staticPressure__Pa,
    double humidityFactor__,
    double airTemperature__K,
    double surfaceArea__m2,
    double liftCoefficient__
);

#endif