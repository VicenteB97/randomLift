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

/// @brief This function computes the lift force (N) of an airfoil using pressure, temperature and its geometry
/// @param outputLiftForce__N 
/// @param dynamicPressure__Pa 
/// @param staticPressure__Pa 
/// @param airTemperature__K 
/// @param specificGasConstant__J_KgK 
/// @param surfaceArea__m2 
/// @param liftCoefficient__ 
/// @return status code (0 success, 1 failure)
uint16_t computeLiftFunction(
    double* outputLiftForce__N,
    double dynamicPressure__Pa,
    double staticPressure__Pa,
    double airTemperature__K,
    double specificGasConstant__J_KgK,
    double surfaceArea__m2,
    double liftCoefficient__
);

/// @brief Checks if all elements of a double-type array are nonnegative
/// @param valuesArray Double-type array
/// @param numValuesCount Number of elements in the array/Number of values to check
/// @return status code (0 success, 1 failure)
bool arePositiveValues(double* valuesArray, uint16_t numValuesCount);

/// @brief Computes the squared velocity (in m/s) as measured by a Pitot tube
/// @param outputSquaredVelocity__m_s Output squared velocity (in m/s)
/// @param dynamicPressure__Pa Dynamic pressure (Pa)
/// @param airDensity__kg_m3 Air density (kg/m3)
/// @return status code (0 success, 1 failure)
int16_t computeSquaredVelocityPitotTube__m_s(
    double* outputSquaredVelocity__m_s,
    double dynamicPressure__Pa,
    double airDensity__kg_m3
);

/// @brief Computes the air density from pressure and density of surronding air by the ideal gas law.
/// @param outputAirDensity__kg_m3 Output air density (kg/m3)
/// @param staticPressure__Pa Air pressure (Pa)
/// @param specificGasConstant__J_KgK Specific gas constant (J/Kg*K)
/// @param airTemperature__K Air temperature (K)
/// @return status code (0 success, 1 failure)
int16_t computeAirDensity__kg_m3(
    double* outputAirDensity__kg_m3,
    double staticPressure__Pa,
    double specificGasConstant__J_KgK,
    double airTemperature__K
);

/// @brief Computes the lift force (N).
/// @param outputLiftAirfoil__N Output lift force (N)
/// @param airDensity__kg_m3 Air density (kg/m3)
/// @param airflowVelocity__m_s Airflow velocity measured by Pitot tube (m/s)
/// @param surfaceArea__m2 Surface area of airfoil (m2)
/// @param liftCoefficient__ Lift coefficient of airfoil (dimensionless)
/// @return status code (0 success, 1 failure)
int16_t computeLiftAirfoil__N(
    double* outputLiftAirfoil__N,
    double airDensity__kg_m3,
    double squaredAirflowVelocity__m_s,
    double surfaceArea__m2,
    double liftCoefficient__ // No units added, since its a dimensionless constant
);

#endif