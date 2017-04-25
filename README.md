# Evolutionary-Algorithms-for-NES-Pinball
Code for the work presented in the paper "Evaluation of a Master-Slave Parallel Evolutionary Algorithm Applied to Artificial Intelligence for Games in the Xeon-Phi Many-Core Platform"

Dependencies:

In order to successfully compile the code the following packages are needed:

*Fceux depnencies (extracted from the official documentation):
	
	-libsdl1.2 - It is strongly recommended that you upgrade to the latest version of sdl (1.2.15 at the time of writing).
	-scons - Required to build fceux.
	-libgtk2.0 (optional) - version >= 2.24 recommended
	-libgtk3.0 (optional) - this compiles as of fceux 2.2.0
	-liblua5.1 (optional)
	-minizip (optional) - you may chose to use the version of minizip on your system by enabling SYSTEM_MINIZIP in the SConstruct
	-c++ compiler -- you may use g++ from gcc or clang++ from llvm.

*OpenMP

*Intel compiler and related libaries for cross-compilation for the k1om architecture

Compilation instructions:

1. Compile the Fceux headless binary using the install.sh script in the folder Linux/FceuxHeadlessSrc
2. Compile the visor binary using the install.sh script in the folder Linux/FceuxGUI
3. Compile the EA using the Makefile found on the OwnPinballAE folder

Each folder has a precompiled binary built under Fedora 23 for the x86_64 architecture
