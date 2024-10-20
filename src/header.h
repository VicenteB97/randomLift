#ifndef ___SIGNALOID_PROJ_HEADER_H___
#define ___SIGNALOID_PROJ_HEADER_H___

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#ifdef RANDOM
#include <uxhw.h>
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

#define __GRAV_CONSTANT__ 9.80665
#define __MOLAR_MASS_DRY_AIR__ 0.028964
#define __MOLAR_MASS_WATER_VAPOR__ 0.018016
#define __UNVIERSAL_GAS_CONSTANT__ 8.3144598
#define __DRY_AIR_CONSTANT__ 287.05
#define __WATER_VAPOR_CONSTANT__ 461.495
#define __TEMPERATURE_SEA_LEVEL__ 288.15
#define __PRESSURE_SEA_LEVEL__ 101325
#define __TEMP_LAPSE_RATE__ 0.0065

/// @brief This function computes the lift force (N) of an airfoil using pressure, temperature and its geometry
/// @param outputLiftForce__N 
/// @param dynamicPressure__Pa 
/// @param staticPressure__Pa 
/// @param airTemperature__K 
/// @param specificGasConstant__J_KgK 
/// @param surfaceArea__m2 
/// @param liftCoefficient__ 
/// @returns Status code (0 success, 1 failure)
uint16_t computeLiftFunction(
    double* outputLiftForce__N,
    double totalPressure__Pa,
    double altitude__m,
    double humidityFactor__,
    double airTemperature__K,
    double surfaceArea__m2,
    double liftCoefficient__
);

/// @brief Computes the specific gas constant from humidity factor in ambient air
/// @param humidityFactor__ a number between 0 and 1 that shows the water vapor/air mix
/// @returns Specific gas constant in J/(Kg K)
double computeSpecificGasConstantHumidAir(
	double humidityFactor__
);

/// @brief Checks if all elements of a double-type array are nonnegative
/// @param valuesArray Double-type array
/// @param numValuesCount Number of elements in the array/Number of values to check
/// @returns Status code (0 success, 1 failure)
bool arePositiveValues(double* valuesArray);

/// @brief Computes the static pressure at cruising altitude.
/// @param altitude__m Cruising altitude
/// @param temperature__K Ambient air temperature (K)
/// @param humidAirGasConstant__J_KgK Specific ambient air constant (J/kg K)
/// @param humidityFactor__ Humidity factor (0 - 1)
/// @returns Static pressure (Pa)
double computeAltitudePressure(
	double altitude__m,
	double temperature__K,
	double humidAirGasConstant__J_KgK,
	double humidityFactor__
);

/// @brief Computes the air density from pressure and density of surronding air by the ideal gas law.
/// @param staticPressure__Pa Air pressure (Pa)
/// @param humidAirGasConstant__J_KgK Specific gas constant for humid air (J/Kg*K)
/// @param airTemperature__K Air temperature (K)
/// @return Air density (kg/m3)
double computeAirDensity(
    double staticPressure__Pa,
	double humidAirGasConstant__J_KgK,
    double airTemperature__K
);

/// @brief Computes the squared velocity (m/s) as measured by a Pitot Tube.
/// @param staticPressure__Pa Static ambient pressure
/// @param totalPressure__Pa Total (stagnation pressure) measured at Pitot tube
/// @param airDensity__Kg_m3 Ambient air density
/// @returns Squared velocity (m/s)
double computeAirVelocitySquared(
	double staticPressure__Pa,
	double totalPressure__Pa,
	double airDensity__Kg_m3
);

/// @brief Computes the lift force (N).
/// @param outputLiftAirfoil__N Output lift force (N)
/// @param airDensity__kg_m3 Air density (kg/m3)
/// @param airflowVelocity__m_s Airflow velocity measured by Pitot tube (m/s)
/// @param surfaceArea__m2 Surface area of airfoil (m2)
/// @param liftCoefficient__ Lift coefficient of airfoil (dimensionless)
/// @return Lift force (N)
double computeLiftAirfoil(
    double airDensity__kg_m3,
    double squaredAirflowVelocity__m_s,
    double surfaceArea__m2,
    double liftCoefficient__ // No units added, since its a dimensionless constant
);

#endif