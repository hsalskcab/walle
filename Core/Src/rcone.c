#include "main.h"
#include "rcone.h"
#include <math.h>

extern uint16_t feed_mm, move_sp, afeed_mm;
extern uint8_t passes_done, passes_total;
extern uint16_t slice, rpm;
extern uint16_t offset;

extern uint32_t xpos, zpos;
extern uint32_t ulpos, dlpos, rlpos, llpos;
extern uint8_t ulim, dlim, rlim, llim;
extern uint8_t is_ulim, is_dlim, is_rlim, is_llim;

extern uint16_t tickx_nm, tickz_nm;

extern uint8_t step;

extern uint8_t err_is_not_on_lim, err_no_lim;
extern uint8_t completed;
extern uint8_t ui_changed;

extern uint32_t cone_angle;

#define CONE_FEED_Z (mm_min_to_clk( ((double)(feed_mm*rpm)) / 100.0  , tickz_nm))
#define CONE_FEED_X (mm_min_to_clk( ( ((double)(feed_mm*rpm)) * tan( ( ((double)(cone_angle)) * 3.14 )/18000.0 ) ) /100.0  , tickx_nm))

void reset_rcone(){
    step=0;
    passes_done=0;
}

void rcone_ext_left(){
    if(step==0){
        stopx();
        stopz();

        if(is_rlim && llim && rlim){
            step=1;
            ulpos=xpos;
            ulim=1;
            ledu(1);
            dlpos=xpos-offset;
            dlim=1;
            ledd(1);
        }
        else if(llim==0 || rlim==0){
            err_no_lim=1;
            ui_changed=1;
        }
        else{
            err_is_not_on_lim=1;
            ui_changed=1;
        }
    }
    else if(step==1){
        go_left(CONE_FEED_Z);
        go_down(CONE_FEED_X);
        step=2;
    }
    else if (step==2 && is_llim==0 && is_dlim==0){
        go_left(CONE_FEED_Z);
        go_down(CONE_FEED_X);
    }
    else if (step==2 && ( is_llim==1 || is_dlim==1 )){
        stopx();
        stopz();
        go_down(mm_min_to_clk(move_sp, tickx_nm));
        step=3;
    }
    else if(step==3 && is_dlim==1){
        step=4;
        go_right(mm_min_to_clk(move_sp, tickz_nm));
    }
    else if((step==4 && is_rlim==1) || step==7){
        passes_done=passes_done + ((passes_done<passes_total) ? 1 : 0);
        if(passes_done>=passes_total){
            completed=1;
            step=7;
        }
        else{
            step=5;
        }
        ui_changed=1;
    }
    else if(step==5){
        ulpos=ulpos+slice;
        go_up(mm_min_to_clk(move_sp, tickz_nm));
        step=6;
    }
    else if(step==6 && is_ulim){
        step=1;
    }
}//void rcone_ext_left()

void rcone_ext_right(){
    if(step==0){
        stopx();
        stopz();

        if(is_llim && llim && rlim){
            step=1;
            ulpos=xpos;
            ulim=1;
            ledu(1);
            dlpos=xpos-offset;
            dlim=1;
            ledd(1);
        }
        else if(llim==0 || rlim==0){
            err_no_lim=1;
            ui_changed=1;
        }
        else{
            err_is_not_on_lim=1;
            ui_changed=1;
        }
    }
    else if(step==1){
        go_right(CONE_FEED_Z);
        go_up(CONE_FEED_X);
        step=2;
    }
    else if (step==2 && is_llim==0 && is_dlim==0){
        go_right(CONE_FEED_Z);
        go_up(CONE_FEED_X);
    }
    else if (step==2 &&  is_dlim==1 ){
        stopx();
        stopz();
        go_down(mm_min_to_clk(move_sp, tickx_nm));
        step=3;
    }
    else if(step==3 && is_dlim==1){
        step=4;
        go_left(mm_min_to_clk(move_sp, tickz_nm));
    }
    else if((step==4 && is_rlim==1) || step==7){
        passes_done=passes_done + ((passes_done<passes_total) ? 1 : 0);
        if(passes_done>=passes_total){
            completed=1;
            step=7;
        }
        else{
            step=5;
        }
        ui_changed=1;
    }
    else if(step==5){
        ulpos=ulpos+slice;
        go_up(mm_min_to_clk(move_sp, tickz_nm));
        step=6;
    }
    else if(step==6 && is_ulim){
        step=1;
    }
}//void rcone_ext_right()
