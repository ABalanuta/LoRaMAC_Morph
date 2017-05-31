
/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */
// #include <ti/drivers/I2C.h>
#include <ti/drivers/PIN.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>
// #include <ti/drivers/Watchdog.h>

#include <driverlib/sys_ctrl.h> // SysCtrlSystemReset()

/* Board Header files */
#include "Board.h"

//#include "hal.h"

#define TIME_MS (1000/Clock_tickPeriod)

#include <string.h> // strlen in uartputs
#include "LoRaRadio/board.h"
#include "radio.h"

#define TASKSTACKSIZE   2048

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/* Pin driver handle */
static PIN_Handle ledPinHandle;
static PIN_State ledPinState;

static PIN_Handle btnPinHandle;
static PIN_State btnPinState;

/* UART driver handle */
static UART_Handle uartHandle;

/*
 * Application LED pin configuration table:
 *   - All LEDs board LEDs are off.
 */
PIN_Config ledPinTable[] = {
    Board_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    Board_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
    PIN_TERMINATE
};

PIN_Config btnPinTable[] = {
    Board_BTN | PIN_INPUT_EN | PIN_NOPULL | PIN_IRQ_NEGEDGE,
    PIN_TERMINATE
};

/**
 * Do a system hard reset when triggered
 */
static void btnIntCallback(PIN_Handle handle, PIN_Id pinId) {
    SysCtrlSystemReset();
}

static void setuppins() {
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if (ledPinHandle == NULL) {
        System_abort("Failed to open board LED pins\n");
    }
    btnPinHandle = PIN_open(&btnPinState, btnPinTable);
    if (btnPinHandle == NULL) {
        System_abort("Failed to open board BTN pin\n");
    }
    if (PIN_registerIntCb(btnPinHandle, btnIntCallback) != PIN_SUCCESS) {
        System_abort("Failed to register btn int callback\n");
    }
}

static void setupuart() {
    UART_Params uartParams;
    UART_Params_init(&uartParams);
    uartParams.baudRate = 115200; // 3000000
    uartHandle = UART_open(Board_UART, &uartParams);
    if (!uartHandle) {
        System_abort("Failed to open UART\n");
    }

//    // Initialize the logger output
//    UartLog_init(hUart);
//    UartPrintf_init()
}

/**
 * Print the given C string to UART followed by carriage return and new line.
 *
 * We define this little function, since we want the primary
 * logging to be over cJTAG, not UART.
 * @param str The C string to print
 */
static void uartputs(const char *str) {
    if (UART_write(uartHandle, str, strlen(str)) == UART_ERROR) {
        System_abort("Failed to write str to uart\n");
    }
    if (UART_write(uartHandle, "\r\n", 2) == UART_ERROR) {
        System_abort("Failed to write CRLR to uart\n");
    }
}

static void setLed(PIN_Id pin, uint_t value)
{
    if (PIN_setOutputValue(ledPinHandle, pin, value) != PIN_SUCCESS)
    {
        System_abort("Failed to set pin value\n");
    }
}

static void toggleLed(PIN_Id pin)
{
    if (PIN_setOutputValue(ledPinHandle, pin,
                           !PIN_getOutputValue(pin)) != PIN_SUCCESS)
    {
        System_abort("Failed to toggle pin value\n");
    }
}

void printstate()
{
    PIN_Status pstatus;
    RadioState_t state = Radio.GetStatus();
    switch (state)
    {
    case RF_IDLE:
        pstatus = PIN_setOutputValue(ledPinHandle, Board_RLED, 0);
        printf("RF_IDLE\n");
        break;
    case RF_RX_RUNNING:
        pstatus = PIN_setOutputValue(ledPinHandle, Board_RLED, 1);
        printf("RF_RX_RUNNING\n");
        break;
    case RF_TX_RUNNING:
        pstatus = PIN_setOutputValue(ledPinHandle, Board_RLED, 1);
        printf("RF_TX_RUNNING\n");
        break;
    case RF_CAD:
        pstatus = PIN_setOutputValue(ledPinHandle, Board_RLED, 1);
        printf("RF_CAD\n");
        break;
    }
    if (pstatus != PIN_SUCCESS) {
        System_abort("Failed to set Red LED value\n");
    }
}


void maintask(UArg arg0, UArg arg1)
{
    uint8_t i;

    // Target board initialization
    BoardInitMcu();
    BoardInitPeriph();


}


/*
 *  ======== main ========
 */
int main(void)
{
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    // Board_initI2C();
    Board_initSPI();
    Board_initUART();
    // Board_initWatchdog();

    /* Construct heartBeat Task  thread */
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000000 / Clock_tickPeriod;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr) maintask, &taskParams,
                   NULL);

    /* Open LED pins */
    setuppins();

    /* Open UART */
    setupuart();

    /* Start BIOS */
    BIOS_start();

    return (0);
}
