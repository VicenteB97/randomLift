[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-light-mode-only)

# Computing the lift force of an airfoil with the Berniouilli equation
This is a minimal example that computes the lift force of an airfoil subject to randomly distributed parameters. 
The repository contains a C source file in the `src` folder which gets compiled and executed by the  [Singaloid Cloud Developer Platform](http://signaloid.io).

Specifically, we use the following [mathematical model](https://www.grc.nasa.gov/www/k-12/VirtualAero/BottleRocket/airplane/lifteq.html):
```math 
L = \frac{1}{2}\rho v^2 S C_L,
```
where $\rho$ is the air density, $v$ is air velocity, $S$ is the surface area of the airfoil and $C_L$ is the lift coefficient. Also, the air density and velocity are computed as follows:
```math
\rho = \frac{P_{static}}{R\,T},\quad v^2 = 2\frac{P_{total} - P_{static}}{\rho},
```
where $P$ are the pressures, $R$ is the specific gas constant and $T$ is the air temperature. The equation used for velocity is obtained as measured by a Pitot tube. 

Also, $P_{static}$ and $R$ depend on the flight altitude, and $\rho$ also depends on the air humidity by the following equations:
```math
P_{static} = P_{0}\exp\left( - \frac{gMh}{RT}\right),
```
where $P_{0}$ is static pressure at sea level, $h$ is altitude, $T$ is the air temperature, and $M$ is the molar mass of ambient air and $R$ is the specific gas constant. These values are computed as follows:
```math
M = M_d(1-f) + M_{v}f, \quad 
R = R_{d}(1 + f(R_v/R_d - 1)),
```
where $f$ is the humidity factor, $R_d$ and $R_v$ are the dry and water vapor molar masses, and $R_d$ and $R_v$ are the dry and water vapor specific gas constants, respectively.

Taking into account all equations, the set of input parameters to the model is: $C_L, S, P_{total}, T, h$ and $f$. Based on how these parameters are obtained, we can assume the following parameter distributions:
- $C_L \sim U()$, because it is determined expermientally.
- $P_{total} \sim U()$ is the total pressure calculated from the pitot tube, which is itself obtained in-flight from several Pitot tubes.
- $h$

## Repository Structure
This repository is very simple. Folder `src` has three files: `config.mk`, `header.h` and `main.c`.
```
src/
├── config.mk
├── header.h
└── main.c
```

File `config.mk` contains the following variables:
- `SOURCES`, a list of C source files to compile;
- `CFLAGS`, the options that are passed to the C compiler. In our case, we can define two variables: `DEBUG` (if we compile in Debug mode) and `RANDOM` (if we use the Signaloid platform).

File `header.h` contains the include files and function definitions. Finally, file `main.c` contains function implementations. 

### Libraries
During the build process, the Signaloid Cloud Developer Platform links the compiled object files against the following libraries:
- `libc`: the C standard library;
- `libm`: the C math library (with function prototypes and other definitions in `math.h`);
- `UxHw`: a library that exposes functionalities of uncertainty-tracking computer architectures (with function prototypes and other definitions in `uxhw.h`).

`UxHw` allows applications to access and inject uncertainty information into the underlying microarchitectures. You can find out more about `UxHw` in the [UxHw documentation](https://docs.signaloid.io/).