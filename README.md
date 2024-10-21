[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-light-mode-only)

# Computing the lift force of an airfoil with the Bernouilli equation

This is a minimal example that computes the lift force of an airfoil subject to randomly distributed parameters accordig to the Bernouilli equation, which assumes our airplane is flying in non-compressible air or very low mach numbers.
The repository contains a C source file in the `src` folder which gets compiled and executed by the  [Singaloid Cloud Developer Platform](http://signaloid.io).

Specifically, we use the following [mathematical model](https://www.grc.nasa.gov/www/k-12/VirtualAero/BottleRocket/airplane/lifteq.html):

```math 
L = \frac{1}{2}\rho v^2 S C_L,
```

where $\rho$ is the air density, $v$ is air velocity, $S$ is the surface area of the airfoil and $C_L$ is the lift coefficient. Also, the air density and [velocity](https://www.princeton.edu/~asmits/Bicycle_web/Bernoulli.html) are computed as follows:

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

Taking into account all equations, the set of input parameters to the model is: $C_L, S, P_{total}, T, h$ and $f$. As a concrete example, we consider a [Cessna 172](http://dx.doi.org/10.13140/RG.2.2.27040.51205), which has the (assumed deterministic) parameters:

- $C_L = 0.64$ is the average lift coefficient in a cruising setting.
- $S = 16.17~m^2$ is the average wing area.

Now, we consider the following distributional parameters for a cruise flight at $h = 2000~m$ over sea level:

- $h \sim U(1980,2020)$ is an altitude ($m$) value based on [radar altimetry](https://avsi.aero/wp-content/uploads/2021/12/Radar-Altimeter-Overview-of-Design-and-Performance.pdf) assuming a $1\%$ tolerance error.
- $P_{total} \sim U(101211.4125, 103256.0875)$ is the total pressure ($Pa$) [calculated from the pitot tube](https://www.omega.com/en-us/resources/pitot-tube), which is itself obtained in-flight from several Pitot tubes assuming a $1\%$ measurement error tolerance.
- $f \sim U(0.44325, 0.45675)$ according to the chart of [measurement tolerance](https://www.rotronic.com/media/productattachments/files/h/u/humidity-accuracy-demystified.pdf).
- $T \sim U(262.9,263.1)$ is the static air temperature ($K$), which we have assumed has a $1\%$ maximum tolerance in the considered average temperature according to [sensor standards](https://temperaturesensors.blogspot.com/2017/03/tolerances-on-temperature-reading.html).

Other distributions could have been used, but we have chosen Uniform distributions mainly because of two reasons:

- All parameters must be positive and their possible values are upper-bounded for technical and physical reasons. Therefore we need compact-support distributions.
- We have not been able to find enough information to consider more informative distributions.

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