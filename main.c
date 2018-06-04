/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/

#include <stdio.h>
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "rl_net.h"

#ifdef RTE_Compiler_EventRecorder
#include "EventRecorder.h"
#endif

extern void MQTT_Demo(void);

const osThreadAttr_t app_main_attr = {
  .stack_size = 8192U
};

/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
void app_main (void *argument) {
  uint32_t addr;

  netInitialize();
  printf("Network Initialized\n");

  do {
   osDelay(500U);
   netIF_GetOption(NET_IF_CLASS_ETH | 0,
                   netIF_OptionIP4_Address,
                   (uint8_t *)&addr, sizeof (addr));
  } while (addr == 0U);
  printf("IP Address Assigned\n");

  MQTT_Demo();
}

int main (void) {

  // System Initialization
  SystemCoreClockUpdate();
#ifdef RTE_Compiler_EventRecorder
  // Initialize and start Event Recorder
  EventRecorderInitialize(EventRecordError, 1U);
#endif
  // ...

  osKernelInitialize();                         // Initialize CMSIS-RTOS
  osThreadNew(app_main, NULL, &app_main_attr);  // Create application main thread
  osKernelStart();                              // Start thread execution
  for (;;) {}
}
