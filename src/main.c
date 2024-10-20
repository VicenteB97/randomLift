#include "header.h"

int main(void){
	// Parameter definition:
	double liftForce__N,
		totalPressure__Pa = 102233.75, // approx. pressure flying at 150 km/h 
		airTemperature__K = 263.0,	// -10ºC
		surfaceArea__m2 = 16.17, 
		liftCoefficient__ = 0.64, // Av. lift coefficient
		altitude__m = 2500,
		humidityFactor__ = 0.45;

	// If we want to study the deterministic case, we include a positive-parameter-value check
	#ifndef RANDOM
	double inputValues[] = {
		totalPressure__Pa,
		airTemperature__K, 
		surfaceArea__m2, 
		liftCoefficient__,
		altitude__m,
		humidityFactor__
	};
    bool canContinue = (
		humidityFactor__ <= 1 && 
		arePositiveValues(inputValues)
	);
    if(!canContinue){ return EXIT_FAILURE; }
	#endif

	// If we want to simulate the random parameter case, we overwrite the pre-defined values for UxHw values: 
	#ifdef RANDOM
	totalPressure__Pa = UxHwDoubleUniformDist(102230.75, 102235.75);
    airTemperature__K = UxHwDoubleUniformDist(262.8, 263.2);
    liftCoefficient__ = UxHwDoubleUniformDist(1.4, 1.8);
	altitude__m = UxHwDoubleUniformDist(2499.8, 2500.2);
	humidityFactor__ = UxHwDoubleUniformDist(0.44, 0.56);
	#endif

	return computeLiftFunction(
		&liftForce__N,
		totalPressure__Pa,
		altitude__m,
		humidityFactor__,
		airTemperature__K,
		surfaceArea__m2,
		liftCoefficient__
	);
}

uint16_t computeLiftFunction(
    double* outputLiftForce__N,
    double totalPressure__Pa,
    double altitude__m,
    double humidityFactor__,
    double airTemperature__K,
    double surfaceArea__m2,
    double liftCoefficient__
){
	// First, we compute the specific gas constant for humid air:
	double humidAirGasConstant__J_KgK = computeSpecificGasConstantHumidAir(humidityFactor__);

	// Compute the static pressure at cruise altitude
    double staticPressure__Pa = computeAltitudePressure(
		altitude__m,
		airTemperature__K,
		humidAirGasConstant__J_KgK,
		humidityFactor__
	);

    // Compute air density:
    double airDensity__kg_m3 = computeAirDensity(
		staticPressure__Pa, humidAirGasConstant__J_KgK, airTemperature__K);

	// Compute air velocity:
	double airVelocitySquared__m_s = computeAirVelocitySquared(staticPressure__Pa, totalPressure__Pa, airDensity__kg_m3);

    // Finally, compute the lift force by the airfoil:
    *outputLiftForce__N = computeLiftAirfoil(
        airDensity__kg_m3,
		airVelocitySquared__m_s,
        surfaceArea__m2,
        liftCoefficient__
    );

	#ifdef RANDOM
	printf("Total lift force is: %lf N\n", *outputLiftForce__N);
	#else
	printf("Total lift force is: %f N\n", *outputLiftForce__N);
	#endif
    return EXIT_SUCCESS;
};

double computeSpecificGasConstantHumidAir(
	double humidityFactor__
){
	return __DRY_AIR_CONSTANT__ * (1 + humidityFactor__ * (__WATER_VAPOR_CONSTANT__ / __DRY_AIR_CONSTANT__ - 1));
};

double computeAltitudePressure(
	double altitude__m,
	double temperature__K,
	double humidAirGasConstant__J_KgK,
	double humidityFactor__
){
	double molarMassHumidAir__kg_mol = __MOLAR_MASS_DRY_AIR__ * (1-humidityFactor__) + humidityFactor__ * __MOLAR_MASS_WATER_VAPOR__;

	return __PRESSURE_SEA_LEVEL__ * exp(-__GRAV_CONSTANT__ * molarMassHumidAir__kg_mol * altitude__m / humidAirGasConstant__J_KgK / temperature__K);
};

double computeAirDensity(
    double staticPressure__Pa,
	double humidAirGasConstant__J_KgK,
    double airTemperature__K
){
    return staticPressure__Pa / (humidAirGasConstant__J_KgK * airTemperature__K);
};

double computeAirVelocitySquared(
	double staticPressure__Pa,
	double totalPressure__Pa,
	double airDensity__Kg_m3
){
	return 2 / airDensity__Kg_m3 * (totalPressure__Pa - staticPressure__Pa);
};

double computeLiftAirfoil(
    double airDensity__kg_m3,
    double squaredAirflowVelocity__m_s,
    double surfaceArea__m2,
    double liftCoefficient__ // No units added, since its a dimensionless constant
){
    return 0.5 * airDensity__kg_m3 * squaredAirflowVelocity__m_s * surfaceArea__m2 * liftCoefficient__;
};

bool arePositiveValues(double* valuesArray){
	uint16_t sizeCount = sizeof(*valuesArray)/sizeof(double);
    uint16_t k = 0;
    while(k < sizeCount){
        if(valuesArray[k] <= (double)0){ return false; }
        k++;
    }

    return true;
};