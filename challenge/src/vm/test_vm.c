#include <stdio.h>
#include <stdlib.h>
#include "vm.h"

int main(int argc, char **argv)
{
    char bytecode[] = "0NAAAA0NAAAAD2";
    VMContext vm;
    
    vm_init(&vm, bytecode);
    printf("%08X\n", vm_run(&vm));

    return 0;
}
