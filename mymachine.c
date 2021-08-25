#include "qemu/osdep.h"
#include "qapi/error.h"
#include "hw/sysbus.h"
#include "hw/ssi/ssi.h"
#include "hw/arm/boot.h"
#include "qemu-common.h"
#include "hw/arm/armv7m.h"
#include "qemu/timer.h"
#include "hw/boards.h"
#include "qemu/log.h"
#include "exec/address-spaces.h"
#include "sysemu/sysemu.h"
#include "hw/misc/unimp.h"
#include "chardev/char-fe.h"


#define NUM_IRQ_LINES 1

static void my_hw_init(MachineState *machine)
{
  DeviceState *nvic;
 
  MemoryRegion *sram = g_new(MemoryRegion, 1);
  MemoryRegion *flash = g_new(MemoryRegion, 1);
  MemoryRegion *system_memory = get_system_memory();

  int flash_size = 0x1000000;
  int sram_size =  0x1000000; 

  memory_region_init_ram(flash, NULL, "mymachine.flash", flash_size, &error_fatal);
  memory_region_set_readonly(flash, true);
  memory_region_add_subregion(system_memory, 0, flash);
  memory_region_init_ram(sram, NULL, "mymachine.sram", sram_size, &error_fatal);
  memory_region_add_subregion(system_memory, 0x20000000, sram);

 
  nvic = qdev_create(NULL, TYPE_ARMV7M);
  qdev_prop_set_uint32(nvic, "num-irq", NUM_IRQ_LINES);
  qdev_prop_set_string(nvic, "cpu-type", machine->cpu_type);
  object_property_set_link(OBJECT(nvic), OBJECT(get_system_memory()),
                                     "memory", &error_abort);

    /* This will exit with an error if the user passed us a bad cpu_type */
    qdev_init_nofail(nvic);

    system_clock_scale = 100;

    armv7m_load_kernel(ARM_CPU(first_cpu), machine->kernel_filename, flash_size);
	
}


static void my_init(MachineState *machine)
{
    my_hw_init(machine);
}


static void my_machine_init(MachineClass *mc)
{
    mc->desc = "MY MACHINE";
    mc->init = my_init;
   
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("cortex-m3");
}


DEFINE_MACHINE("mymachine", my_machine_init) 













