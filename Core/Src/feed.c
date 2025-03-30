#include "main.h"
#include "feed.h"

extern uint16_t feed_mm, move_sp;
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

void reset_feed(){
    step=0;
    passes_done=0;
}

void feed_ext_left(){
    if(step==0){
        stopx();
        stopz();

        if(is_rlim && llim && rlim){
            step=1;
            ulpos=xpos;
            ulim=1;
            ledu(1);
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
        go_left(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
        step=2;
    }
    else if (step==2 && is_llim==0){
        go_left(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
    }
    else if (step==2 && is_llim==1){
        dlpos=xpos-offset;
        dlim=1;
        ledd(1);
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
}//void feed_ext_left()

void feed_ext_right(){
    if(step==0){
        stopx();
        stopz();

        if(is_llim && llim && rlim){
            step=1;
            ulpos=xpos;
            ulim=1;
            ledu(1);
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
        go_right(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
        step=2;
    }
    else if (step==2 && is_rlim==0){
        go_right(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
    }
    else if (step==2 && is_rlim==1){
        dlpos=xpos-offset;
        dlim=1;
        ledd(1);
        go_down(mm_min_to_clk(move_sp, tickx_nm));
        step=3;
    }
    else if(step==3 && is_dlim==1){
        step=4;
        go_left(mm_min_to_clk(move_sp, tickz_nm));
    }
    else if((step==4 && is_llim==1) || step==7){
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
}//void feed_ext_right()

void feed_int_left(){
    if(step==0){
        stopx();
        stopz();

        if(is_rlim && llim && rlim){
            step=1;
            dlpos=xpos;
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
        go_left(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
        step=2;
    }
    else if (step==2 && is_llim==0){
        go_left(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
    }
    else if (step==2 && is_llim==1){
        ulpos=xpos+offset;
        ulim=1;
        ledu(1);
        go_up(mm_min_to_clk(move_sp, tickx_nm));
        step=3;
    }
    else if(step==3 && is_ulim==1){
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
        dlpos=dlpos-slice;
        go_down(mm_min_to_clk(move_sp, tickz_nm));
        step=6;
    }
    else if(step==6 && is_dlim){
        step=1;
    }
    
}//void feed_int_left()

void feed_int_right(){
    if(step==0){
        stopx();
        stopz();

        if(is_llim && llim && rlim){
            step=1;
            ulpos=xpos;
            ulim=1;
            ledu(1);
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
        go_right(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
        step=2;
    }
    else if (step==2 && is_rlim==0){
        go_right(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm));
    }
    else if (step==2 && is_rlim==1){
        ulpos=xpos+offset;
        ulim=1;
        ledu(1);
        go_up(mm_min_to_clk(move_sp, tickx_nm));
        step=3;
    }
    else if(step==3 && is_ulim==1){
        step=4;
        go_left(mm_min_to_clk(move_sp, tickz_nm));
    }
    else if((step==4 && is_llim==1) || step==7){
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
        dlpos=dlpos-slice;
        go_down(mm_min_to_clk(move_sp, tickz_nm));
        step=6;
    }
    else if(step==6 && is_dlim){
        step=1;
    }
}//void feed_int_right()