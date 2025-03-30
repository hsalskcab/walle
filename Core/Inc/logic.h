#include "main.h"

uint8_t ui_changed=1;

modes_t mode=afeed;
submodes_t submode=man;

uint32_t rpm=0;
uint16_t afeed_mm = 300, feed_mm=300, move_sp=750;
uint16_t sp_mig=2000;
uint8_t passes_done=0, passes_total=1, passes_finish = 1;
uint16_t slice = 100;
uint16_t screw_step=100;
uint16_t offset=10000;
uint32_t cone_angle=4500;

uint32_t ulpos=0, dlpos=0, rlpos=0, llpos=0;
uint8_t ulim=0, dlim=0, rlim=0, llim=0;
uint8_t is_ulim=0, is_dlim=0, is_rlim=0, is_llim=0;

uint32_t xpos=(1<<31), zpos=(1<<31);

uint8_t err_no_lim=0, err_is_not_on_lim=0;
uint8_t completed=0;

uint8_t dirx=0, dirz=0;
uint16_t tickx_nm=500, tickz_nm=600;

uint8_t spindle_dir=0;
uint32_t spindle_pos=0;
uint32_t spindle_enc_lines=4000;

uint8_t step=0;

int32_t travel_distx=0,travel_distz=0;;


uint32_t accel=100;

uint16_t sectors=1,sector_rn=1,old_sector=1;
void logic();