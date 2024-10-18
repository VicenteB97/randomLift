[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-dark-v6.png#gh-dark-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-dark-mode-only)
[<img src="https://assets.signaloid.io/add-to-signaloid-cloud-logo-light-v6.png#gh-light-mode-only" alt="[Add to signaloid.io]" height="30">](https://signaloid.io/repositories?connect=https://github.com/VicenteB97/randomLift.git#gh-light-mode-only)

# Computing the lift force of an airfoil with the Berniouilli equation
This is a minimal example that computes the lift force of an airfoil subject to randomly distributed parameters. 
The repository contains a C source file in the `src` folder which gets compiled and executed by the  [Singaloid Cloud Developer Platform](http://signaloid.io).

Specifically, we use the following mathematical model:
$$ 
L = \frac{1}{2}\rho v^2 S C_L,
$$
where $\rho$ is the air density, $v$ is air velocity, $S$ is the surface area of the airfoil and $C_L$ is the lift coefficient. Also, the air density and velocity are computed as follows:
$$
\rho = \frac{P_{static}}{R\,T},\quad v^2 = 2\frac{P_{dynamic}}{\rho},
$$
where $P$ are the pressures, $R$ is the specific gas constant and $T$ is the air temperature. The equation used for velocity is obtained as measured by a Pitot tube.

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