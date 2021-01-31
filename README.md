# arm-simulator
This is a CPU *simulator* for the ARMv7 instruction set.

I built it because of [QEMU's](https://www.qemu.org/) approach of patching target assembly into host assembly, and executing that.
For our computer architectures proseminar we want more granular control and an actual simulation of the ARM CPU.

It can decode and execute all instructions present in the ARMv7a architecture reference manual.
The current [main.cpp](main.cpp) can even parse ELF files and allocates memory correctly.

Because we found a better alternative development has currently seized.
I am in the process of publishing a bachelor's thesis of rebuilding this project in TypeScript so we can supply it to our first semester students.
