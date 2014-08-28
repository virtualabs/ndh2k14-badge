Nuit du Hack 2014 Collector Badge
=================================

This is the source code of the firmware installed in the collector badge, including
funny things available through the minimal serial interface implemented from scratch
through V-USB (http://www.obdev.at/vusb/). 

Console.py is required to interact with the firmware.


1. Compiling
------------

$ make

This command produces a `main.hex` firmware file. If avrdude is installed on your
machine, you can plug in the badge in program mode (plug the badge, hold both reset
and boot buttons, then released the reset button, the led is on) and type:

$ make flash


2. USB interface
----------------

USB interface uses a bit of bitbanging implemented by people at Objective Development,
for more information see http://www.obdev.at/vusb/.


3. VM-related stuff
-------------------

I did not include the VM assembler I developed for this firmware, but basically the vm
is a stack-based one with a few operations. More details in `vm/vm.h` .

Serial I/O operations are also implemented in the virtual machine code.
