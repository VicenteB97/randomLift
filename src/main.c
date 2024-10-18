#include "header.h"

int
main(int argc, char* argv[])
{
	double liftForce__N,
		dynamicPressure__Pa = 150325.0,
		staticPressure__Pa = 101325.0,
		airTemperature__K = 273.0,
		specificGasConstant__J_KgK = 287.05,
		surfaceArea__m2 = 0.3,
		liftCoefficient__ = 0.2;

	#ifdef RANDOM
	dynamicPressure__Pa = UxHwDoubleUniformDist(150324.8, 150325.2);
    staticPressure__Pa = UxHwDoubleUniformDist(101324.8, 101325.2);
    airTemperature__K = UxHwDoubleUniformDist(272.8, 273.2);
	specificGasConstant__J_KgK = 287.05;
    surfaceArea__m2 = 0.3;
    liftCoefficient__ = UxHwDoubleUniformDist(0.18, 0.24);
	#endif

	return computeLiftFunction(
		&liftForce__N,
		dynamicPressure__Pa,
		staticPressure__Pa,
		airTemperature__K,
		specificGasConstant__J_KgK,
		surfaceArea__m2,
		liftCoefficient__
	);
}

uint16_t computeLiftFunction(
    double* outputLiftForce__N,
    double dynamicPressure__Pa,
    double staticPressure__Pa,
    double airTemperature__K,
    double specificGasConstant__J_KgK,
    double surfaceArea__m2,
    double liftCoefficient__
){
    // First check that input values are in acceptable range:
    double inputValues[6] = {dynamicPressure__Pa, staticPressure__Pa, airTemperature__K, specificGasConstant__J_KgK, surfaceArea__m2, liftCoefficient__};
    bool canContinue = (liftCoefficient__ <= 1 && arePositiveValues(inputValues, 6));
    if(!canContinue){ return EXIT_FAILURE; }

    // Compute air density:
    double airDensity__kg_m3 = 0.0;
    computeAirDensity__kg_m3(
        &airDensity__kg_m3,
        staticPressure__Pa,
        specificGasConstant__J_KgK,
        airTemperature__K
    );

    // Using the recently computed air density, 
    // compute velocity as measured by a Pitot tube:
    double squaredVelocity__m_s = 0.0;
    computeSquaredVelocityPitotTube__m_s(
        &squaredVelocity__m_s,
        dynamicPressure__Pa,
        airDensity__kg_m3
    );

    // Finally, compute the lift force by the airfoil:
    computeLiftAirfoil__N(
        outputLiftForce__N,
        airDensity__kg_m3,
        squaredVelocity__m_s,
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

bool arePositiveValues(double* valuesArray, uint16_t numValuesCount){
    uint16_t k = 0;
    while(k < numValuesCount){
        if(valuesArray[k] <= (double)0){ return false; }
        k++;
    }

    return true;
};

int16_t computeSquaredVelocityPitotTube__m_s(
    double* outputSquaredVelocity__m_s,
    double dynamicPressure__Pa,
    double airDensity__kg_m3
){
    // First, let us check that the input values are positive
    double inputArray[2] = {dynamicPressure__Pa, airDensity__kg_m3};
    if(!arePositiveValues(inputArray, 2)){ 
        fprintf(stderr, "Not all values are positive at velocity computation.\n"); 
        return EXIT_FAILURE; 
    }

    // Compute the desired SQUARED velocity (we avoid computing the square root)
    *outputSquaredVelocity__m_s = 2 * dynamicPressure__Pa / airDensity__kg_m3;
    return EXIT_SUCCESS;
};

int16_t computeAirDensity__kg_m3(
    double* outputAirDensity__kg_m3,
    double staticPressure__Pa,
    double specificGasConstant__J_KgK,
    double airTemperature__K
){
    // Usual check that values are positive
    double inputArray[3] = {staticPressure__Pa, specificGasConstant__J_KgK, airTemperature__K};
    if(!arePositiveValues(inputArray, 3)){ 
        fprintf(stderr, "Not all values are positive at airDensity computation.\n"); 
        return EXIT_FAILURE; 
    }

    *outputAirDensity__kg_m3 = staticPressure__Pa / (specificGasConstant__J_KgK * airTemperature__K);
    return EXIT_SUCCESS;
};

int16_t computeLiftAirfoil__N(
    double* outputLiftAirfoil__N,
    double airDensity__kg_m3,
    double squaredAirflowVelocity__m_s,
    double surfaceArea__m2,
    double liftCoefficient__ // No units added, since its a dimensionless constant
){
    // Usual check that values are positive
    double inputArray[4] = {airDensity__kg_m3, squaredAirflowVelocity__m_s, surfaceArea__m2, liftCoefficient__};
    if(!arePositiveValues(inputArray, 4)){ 
        fprintf(stderr, "Not all values are positive at lift Force computation.\n"); 
        return EXIT_FAILURE; 
    }

    *outputLiftAirfoil__N = 0.5 * airDensity__kg_m3 * squaredAirflowVelocity__m_s * surfaceArea__m2 * liftCoefficient__;
    return EXIT_SUCCESS;
};