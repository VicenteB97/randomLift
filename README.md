[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-light-mode-only)

# Computing the lift force of an airfoil with the Bernouilli equation

This is a minimal example that computes the lift force of an airfoil subject to randomly distributed parameters accordig to the Bernouilli equation, which assumes non-compressible air or low mach numbers regarding airflow. The repository contains a C source file in the `src` folder which gets compiled and executed by the  [Singaloid Cloud Developer Platform](http://signaloid.io).

## Mathematical model
Specifically, we use the following [mathematical model](https://www.grc.nasa.gov/www/k-12/VirtualAero/BottleRocket/airplane/lifteq.html):

```math 
L = \frac{1}{2}\rho v^2 S C_L,
```

where $\rho$ is the air density, $v$ is air velocity, $S$ is the surface area of the airfoil and $C_L$ is the lift coefficient. Also, the air density and [velocity](https://www.princeton.edu/~asmits/Bicycle_web/Bernoulli.html) are computed as follows:

```math
\rho = \frac{P_{static}}{R\,T},\quad v^2 = 2\frac{P_{total} - P_{static}}{\rho},
```

where $P_{total}$ and $P_{static}$ are the pressures obtained by the dynamic and the static port of the [Pitot tube](http://www.erbman.org/Pitot%20Statics%20and%20the%20Standard%20Atmosphere%205th%20ed%20signed.pdf), respectively. $T$ is the ambient air temperature and $R$ is the [specific gas constant](https://blogs.millersville.edu/adecaria/files/2021/11/esci340-Lesson02-Thermodynamics.pdf) which is computed as follows:
```math
R = R_{d}(1-f) + f~R_v,
```

where $f$ is the humidity factor, and $R_d$ and $R_v$ are the dry and water vapor specific gas constants, respectively.

## Specific simulation parameters
Taking into account all equations, the input parameters for the model are: $C_L, S, P_{total}, P_{static}, T$ and $f$. As a concrete example, we consider a [Cessna 172](http://dx.doi.org/10.13140/RG.2.2.27040.51205), which has the following characteristics:

- $C_L = 0.64$ is the average lift coefficient.
- $S = 16.17~m^2$ is the wing area.

Now, we consider the simulation of a flight with the following information that we assume is obtained from its onboard sensors:

- $P_{static} \sim U(79000-1\%, 79000+1\%)$ is the total pressure ($Pa$) [calculated from the static port of the pitot tube](https://www.omega.com/en-us/resources/pitot-tube), which is itself obtained in-flight from several Pitot tubes. The expected value has been obtained from [standard pressure tables](https://www.mide.com/air-pressure-at-altitude-calculator).
- $P_{total} \sim U(81000 - 1\%, 81000+1\%)$ is the total pressure ($Pa$) [calculated from the pitot tube](https://www.omega.com/en-us/resources/pitot-tube), which is also obtained in-flight from several Pitot tubes. Expected value is obtained by considering $2000~Pa$ of dynamic pressure.
- $T \sim U(283-1.5\%, 283+1.5\%)$ is the static air temperature ($K$) according to [sensor standards](https://temperaturesensors.blogspot.com/2017/03/tolerances-on-temperature-reading.html).
- $f \sim U(0.45-2\%, 0.45+2\%)$ according to the chart of [measurement tolerance](https://www.rotronic.com/media/productattachments/files/h/u/humidity-accuracy-demystified.pdf).

### Note on the distribution choice
Other distributions could have been used, but we have chosen uniform distributions mainly because of two reasons:

- All parameters must be positive and upper-bounded for technical and physical reasons. Compact-support distributions are perfect for this task.
- We have not been able to find enough information to consider more informative distributions.

### An interesing note regarding velocity
Considering total and static pressures as random variables imply that velocity is also a random variable $v(\omega)$. 
However, note that $v(\omega)$ will only be well defined if
```math
\mathbb{P}\left(\{\omega \in\Omega : P_{total}(\omega)\geq P_{static}(\omega)\}\right) = 1,
``` 
where $(\Omega,\mathcal{F},\mathbb{P})$ is a complete probability space.
In our particluar case, we have built $P_{total}(\omega),~P_{static}(\omega)$ such that the support of their distributions do not overlap, so $v(\omega)$ is well-defined.

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