#include <Arduino.h>

//Definitions of all the constants necessary for interaction with PCA9633PW(https://www.nxp.com/part/PCA9633PW#/)

#define SWRST_ADDR 0x03
#define SWRST_MAGIC1 0xA5
#define SWRST_MAGIC2 0x5A

#define ADDR_AI2_bm 0x80
#define ADDR_AI1_bm 0x40
#define ADDR_AI0_bm 0x20

#define REG_MODE1 0x00

#define MODE1_AI2_bm 0x80
#define MODE1_AI1_bm 0x40
#define MODE1_AI0_bm 0x20
#define MODE1_SLEEP_bm 0x10
#define MODE1_SUB1_bm 0x08
#define MODE1_SUB2_bm 0x04
#define MODE1_SUB3_bm 0x02
#define MODE1_ALLCALL_bm 0x01

#define REG_MODE2 0x01

#define MODE2_OUTNE0_bm 0x01
#define MODE2_OUTNE1_bm 0x02
#define MODE2_OUTDRV_bm 0x04
#define MODE2_OCH_bm 0x08
#define MODE2_INVRT_bm 0x10
#define MODE2_DMBLNK_bm 0x20

#define REG_PWM0 0x02
#define REG_PWM1 0x03
#define REG_PWM2 0x04
#define REG_PWM3 0x05

#define REG_GRPPWM 0x06
#define REG_GRPFREQ 0x07

#define REG_LEDOUT 0x08

#define REG_SUBADR1 0x09
#define REG_SUBADR2 0x0A
#define REG_SUBADR3 0x0B

#define REG_ALLCALLADR 0x0C