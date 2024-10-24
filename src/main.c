#include "header.h"

int 
main(void)
{
	// Parameter definition:
	double 	liftForce__N,				// Output lift force (N)
		 	liftForceUsingPressures__N,	// Output lift force (N)
			totalPressure__Pa  = 81000, // Total pressure 
			staticPressure__Pa = 79000, // Static port
			humidityFactor__   = 0.45,	// Humidity factor
			airTemperature__K  = 283.0,	// +10ºC
			surfaceArea__m2    = 16.17, // Surface area (m2)
			liftCoefficient__  = 0.64; 	// Av. lift coefficient

	// If we want to study the deterministic case, we include a positive-parameter-value check
	double inputValues[] = {
		totalPressure__Pa,
		staticPressure__Pa,
		humidityFactor__, 
		airTemperature__K,
		surfaceArea__m2
	};
    bool canContinue = (
		humidityFactor__ <= 1 && 
		arePositiveValues(inputValues)
	);
    if(!canContinue){ 
		fprintf(stderr, "Some illegal values have been considered.\n"); 
		return EXIT_FAILURE; 
	}

	// If we want to simulate the random parameter case, we overwrite the pre-defined values for UxHw values: 
	#ifdef RANDOM
	totalPressure__Pa  = UxHwDoubleUniformDist(totalPressure__Pa * (1 - 0.01), totalPressure__Pa * (1 + 0.01));
	staticPressure__Pa = UxHwDoubleUniformDist(staticPressure__Pa * (1 - 0.01), staticPressure__Pa * (1 + 0.01));
	humidityFactor__   = UxHwDoubleUniformDist(humidityFactor__ * (1 - 0.02), humidityFactor__ * (1 + 0.02));
    airTemperature__K  = UxHwDoubleUniformDist(airTemperature__K * (1 - 0.015), airTemperature__K * (1 + 0.015));
	#endif

	// Compute the lift force 
	return computeLiftFunction(
		&liftForce__N,
		&liftForceUsingPressures__N,
		totalPressure__Pa,
		staticPressure__Pa,
		humidityFactor__,
		airTemperature__K,
		surfaceArea__m2,
		liftCoefficient__
	);
}
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
bool 
arePositiveValues(double* valuesArray)
{
	uint16_t sizeCount = sizeof(*valuesArray)/sizeof(double);
    uint16_t k = 0;
    while(k < sizeCount){
        if(valuesArray[k] <= (double)0){ return false; }
        k++;
    }

    return true;
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
double 
computeSpecificGasConstantHumidAir(
	double humidityFactor__
){
	return __DRY_AIR_CONSTANT__ * (1 - humidityFactor__) + __WATER_VAPOR_CONSTANT__ * humidityFactor__;
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
double 
computeAirDensity(
    double staticPressure__Pa,
	double humidAirGasConstant__J_KgK,
    double airTemperature__K
){
    return staticPressure__Pa / (humidAirGasConstant__J_KgK * airTemperature__K);
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
double 
computeAirVelocitySquared(
	double staticPressure__Pa,
	double totalPressure__Pa,
	double airDensity__Kg_m3
){
	return 2 / airDensity__Kg_m3 * (totalPressure__Pa - staticPressure__Pa);
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
double 
computeLiftAirfoil(
    double airDensity__kg_m3,
    double squaredAirflowVelocity__m_s,
    double surfaceArea__m2,
    double liftCoefficient__
){
    return 0.5 * airDensity__kg_m3 * squaredAirflowVelocity__m_s * surfaceArea__m2 * liftCoefficient__;
};

double 
computeLiftAirfoilUsingPressure(
    double totalPressure__Pa,
    double staticPressure__Pa,
    double surfaceArea__m2,
    double liftCoefficient__ // No units added, since its a dimensionless constant
){
	return (totalPressure__Pa - staticPressure__Pa) * surfaceArea__m2 * liftCoefficient__;
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
uint16_t 
computeLiftFunction(
    double* outputLiftForce__N,
    double* outputLiftForceUsingPressure__N,
    double totalPressure__Pa,
    double staticPressure__Pa,
    double humidityFactor__,
    double airTemperature__K,
    double surfaceArea__m2,
    double liftCoefficient__
){
	// First, we compute the specific gas constant for humid air:
	double humidAirGasConstant__J_KgK = computeSpecificGasConstantHumidAir(humidityFactor__);

    // Compute air density:
    double airDensity__kg_m3 = computeAirDensity(
		staticPressure__Pa, 
		humidAirGasConstant__J_KgK, 
		airTemperature__K
	);

	// Compute air velocity:
	double airVelocitySquared__m_s = computeAirVelocitySquared(
		staticPressure__Pa, 
		totalPressure__Pa, 
		airDensity__kg_m3
	);

	// Print input parameters and computed factors
	printf(
		"Input parameters are:\n" 
		" - Total pressure: %lf Pa\n" 
		" - Static pressure: %lf Pa\n"
		" - Humidity factor: %lf %% \n" 
		" - Temperature: %lf K\n\n"
		"Computed parameters:\n" 
		" - Air density: %lf kg/m3\n"
		" - Velocity: %lf km/h\n"
		" - Humid air gas constant: %lf J/(kg K)\n\n",
		totalPressure__Pa,
		staticPressure__Pa,
		humidityFactor__ * 100,
		airTemperature__K,
		airDensity__kg_m3,
		sqrt(airVelocitySquared__m_s)*3.6,
		humidAirGasConstant__J_KgK
	);

    // Finally, compute the lift force by the airfoil:
    *outputLiftForce__N = computeLiftAirfoil(
        airDensity__kg_m3,
		airVelocitySquared__m_s,
        surfaceArea__m2,
        liftCoefficient__
    );

	// Compute the lift force by the airfoil using pressures for contrast:
    *outputLiftForceUsingPressure__N = computeLiftAirfoil(
        airDensity__kg_m3,
		airVelocitySquared__m_s,
        surfaceArea__m2,
        liftCoefficient__
    );

	// Print final output
	printf(
		"OUTPUT:\n"
		" - Total lift force is: %f N\n"
		" - Total lift force (using only pressures) is: %lf N\n", 
		*outputLiftForce__N,
		*outputLiftForceUsingPressure__N
	);
    return EXIT_SUCCESS;
};
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////