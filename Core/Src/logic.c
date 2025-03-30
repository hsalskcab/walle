#include "main.h"
#include "afeed.h"
#include "feed.h"
#include "screw.h"
#include "rcone.h"
#include "lcone.h"

//extern vars////////////////////////////////////
extern uint8_t ui_changed;
extern modes_t mode;
extern submodes_t submode;

extern uint16_t afeed_mm, feed_mm, move_sp, screw_step;
extern uint8_t passes_done, passes_total, passes_finish;
extern uint16_t slice, rpm;
extern uint16_t screw_step;
extern uint16_t offset;

extern uint32_t cone_angle;

extern uint8_t err_no_lim, err_is_not_on_lim;
extern uint8_t completed;

extern uint16_t tickx_nm, tickz_nm;

extern uint32_t new_spx, new_spz;

extern uint32_t accel;

extern int32_t travel_distx,travel_distz;
/////////////////////////////////////////////////

#define CONE_FEED_Z (mm_min_to_clk( ((double)(feed_mm*rpm)) / 100.0  , tickz_nm))
#define CONE_FEED_X (mm_min_to_clk( ( ((double)(feed_mm*rpm)) * tan( ( ((double)(cone_angle)) * 3.14 )/18000.0 ) ) /100.0  , tickx_nm))

//vars///////////////////////////////////////////

//rapid feed//////////////////////
uint8_t rapid_mode=0;
uint32_t rapid_speed=3000;
uint32_t spx_before_rapid=0, spz_before_rapid=0;
uint32_t rapid_tmr=0;
//////////////////////////////////

//mode switcher///////////////////
modes_t mode_old = reset;
submodes_t submode_old = man;
uint32_t mode_timer = 0;
uint8_t mode_changed=0;
//////////////////////////////////

//feed speed encoder//////////////
uint32_t fs_cnt_old=0;
//////////////////////////////////

//menu buttons////////////////////
uint8_t ud_pressed=0;
uint32_t ud_timer=0;
uint8_t rl_pressed=0;
uint32_t rl_timer=0;
uint8_t s_pressed=0;
//////////////////////////////////

//limits//////////////////////////
uint8_t lim_pressed=0;
uint32_t lim_tmr=0;

extern uint32_t new_spz, new_spx;

extern uint32_t xpos, zpos;
extern uint32_t ulpos, dlpos, rlpos, llpos;
extern uint8_t ulim, dlim, rlim, llim;
extern uint8_t is_ulim, is_dlim, is_rlim, is_llim;
//////////////////////////////////

//manual encoder//////////////////
uint32_t old_mig_cnt=0, mig_tmr=0;
extern uint16_t sp_mig;
uint32_t x1_dist_mcm=1000, x10_dist_mcm=10000;
uint8_t old_axis=2;
//////////////////////////////////

//joystick////////////////////////
uint8_t last_joystick=10;
//////////////////////////////////

extern uint16_t sectors;
uint32_t spindle_tim=0;

/////////////////////////////////////////////////

void logic(){

    //mode switcher//////////////////////////////////////////
    if(mode_settings) 
        mode=settings;
    else if(mode_afeed) 
        mode=afeed;
    else if(mode_sfeed)
        mode=sfeed;
    else if(mode_feed)
        mode=feed;
    else if(mode_divider) 
        mode=divider;
    else if(mode_sphere) 
        mode=sphere;
    else if(mode_rcone) 
        mode=rcone;
    else if(mode_lcone) 
        mode=lcone;
    else if(mode_screw) 
        mode=screw;

    if(!mode_changed && mode_old!=mode){
        mode_timer=millis();
        mode_changed=1;
    }
    else if(mode_changed && millis()-mode_timer>=100){
        mode_changed=0;
        mode_old = mode;
        ui_changed = 1;
    }
    /////////////////////////////////////////////////////////

    //submode switcher///////////////////////////////////////
    if(submode_ex) 
        submode=ex;
    else if(submode_man)
        submode=man;
    else if(submode_in)
        submode=in;

    if(submode_old!=submode){
        submode_old=submode;
        ui_changed=1;
    }
    /////////////////////////////////////////////////////////

    //spindle////////////////////////////////////////////////
    if(millis()-spindle_tim>=100){
        spindle_tim=millis();
        //EXTI->PR=SPINDLE_ENCA_Pin;
    }
    /////////////////////////////////////////////////////////

    //feed speed enc/////////////////////////////////////////
    if(TIM5->CNT!=fs_cnt_old){
        int32_t delta=((TIM5->CNT)-fs_cnt_old)*(fs_btn ? 100 : 1);
        fs_cnt_old=TIM5->CNT;

        if(mode==afeed){
            afeed_mm=afeed_mm+delta;
            if(afeed_mm > 10000+10000 || afeed_mm<1){
                afeed_mm=1;
            }
            else if(afeed_mm>10000){
                afeed_mm=10000;
            }
        }
        else if (mode==feed || mode==sfeed || mode==rcone || mode==lcone){
            feed_mm=feed_mm+delta;
            if(feed_mm > 10000+10000 || feed_mm<1){
                feed_mm=1;
            }
            else if(feed_mm>10000){
                feed_mm=10000;
            }
        }
        else if (mode==screw){
            screw_step=screw_step+delta;
            if(screw_step > 5000+10000 || screw_step<1){
                screw_step=1;
            }
            else if(screw_step>5000){
                screw_step=5000;
            }
        }

        ui_changed = 1;
    }
    /////////////////////////////////////////////////////////

    //menu buttons///////////////////////////////////////////
    if(!menu_u && !menu_d)
        ud_pressed=0;

    int8_t delta_ud = 0;
    if((menu_u || menu_d) && ud_pressed==0){
        ud_pressed=1;
        ud_timer=millis();
    }
    else if(ud_pressed==1 && millis()-ud_timer>50){
        delta_ud=delta_ud + (menu_u ? 1 : -1);
        ud_pressed=2;
    }
    else if(ud_pressed==2 && millis()-ud_timer>=500){
        ud_pressed=3;
        ud_timer=millis();
        delta_ud=delta_ud + (menu_u ? 1 : -1);
    }
    else if (ud_pressed==3 && millis()-ud_timer>=100){
        ud_timer=millis();
        delta_ud=delta_ud + (menu_u ? 1 : -1);
    }

    if(mode==feed || mode==afeed || mode==sfeed || mode==rcone || mode==lcone){
        if(submode!=man){
            slice+=delta_ud*5;
            constrain(slice, 5, 500);
        }
    }
    else if(mode==screw){
        if(submode!=man){
            passes_finish+=delta_ud;
            constrain(passes_finish,1,99);
        }
    }
    else if(mode==divider){
        sectors+=delta_ud;
        constrain(sectors,1,99);
    }

    if(!menu_r && !menu_l)
        rl_pressed=0;

    int8_t delta_rl = 0;
    if((menu_r || menu_l) && rl_pressed==0){
        rl_pressed=1;
        rl_timer=millis();
    }
    else if(rl_pressed==1 && millis()-rl_timer>50){
        delta_rl=delta_rl + (menu_r ? 1 : -1);
        rl_pressed=2;
    }
    else if(rl_pressed==2 && millis()-rl_timer>=500){
        rl_pressed=3;
        rl_timer=millis();
        delta_rl=delta_rl + (menu_r ? 1 : -1);
    }
    else if (rl_pressed==3 && millis()-rl_timer>=100){
        rl_timer=millis();
        delta_rl=delta_rl + (menu_r ? 1 : -1);
    }
    ui_changed += ((delta_ud+delta_rl)==0) ? 0 : 1;

    if(mode==feed || mode==afeed || mode==sfeed || mode==rcone || mode==lcone || mode==screw){
        if(submode!=man){
            passes_total+=delta_rl;
            constrain(passes_total, 1, 99);
        }
    }

    if(!menu_s){
        s_pressed=0;
    }
    else if(menu_s && s_pressed==0){
        s_pressed=1;

        reset_afeed();
        reset_feed();
        reset_screw();

        ui_changed=1;
    }
    /////////////////////////////////////////////////////////

    //joystick//////////////////////////////////////////////
    if((joy_u || joy_d || joy_l || joy_r)){
        last_joystick=0;
    }
    if(!joy_u && !joy_d && !joy_r && !joy_l){
        if(last_joystick!=1){
            new_spz=0;
            new_spx=0;
            stopx();
            stopz();
            last_joystick=1;
            rapid_mode=0;
            err_no_lim=0;
            err_is_not_on_lim=0;
        }

        if(completed){
            reset_afeed();
            reset_feed();
            reset_lcone();
            reset_rcone();
            reset_screw();
            completed=0;
        }

        ui_changed=1;
    }
    if((joy_u || joy_d || joy_l || joy_r) && joy_rapid && rapid_mode==0){
        rapid_mode=1;
        if(joy_u){
            go_up(mm_min_to_clk((uint32_t)((double)accel /100.0 * 60.0), tickx_nm));
        }
        else if(joy_d){
            go_down(mm_min_to_clk((uint32_t)((double)accel /100.0 * 60.0), tickx_nm));
        }
        else if(joy_r){
            go_right(mm_min_to_clk((uint32_t)((double)accel /100.0 * 60.0), tickz_nm));
        }
        else if(joy_l){
            go_left(mm_min_to_clk((uint32_t)((double)accel /100.0 * 60.0), tickz_nm));
        }
    }
    else if((joy_u || joy_d || joy_l || joy_r) && !joy_rapid && rapid_mode==0){
        rapid_mode=3;
    }
    else if(joy_u && !joy_rapid && (rapid_mode==0 || rapid_mode==3)){
        if(mode==afeed){
            if(submode==man) { go_up(mm_min_to_clk(afeed_mm, tickx_nm)); }
            else if(submode==ex) { afeed_ext_up(); }
        }
        else if(mode==feed || mode==rcone || mode==lcone){
            if(submode==man){ go_up(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickx_nm)); }
        }
        else if(mode==screw){
            if(submode==man) go_up(mm_min_to_clk(((double)(screw_step*rpm))/100, tickx_nm));
        }
    }
    else if(joy_d && !joy_rapid && (rapid_mode==0 || rapid_mode==3)){
        if(mode==afeed){
            if(submode==man) { go_down(mm_min_to_clk(afeed_mm, tickx_nm)); }
            else if(submode==ex) { afeed_ext_down(); }
        }
        else if(mode==feed || mode==rcone || mode==lcone){
            if(submode==man){ go_down(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickx_nm)); }
        }
        else if(mode==screw){
            if(submode==man){ go_down(mm_min_to_clk(((double)(screw_step*rpm))/100, tickx_nm)); }
        }
    }
    else if(joy_r && !joy_rapid && (rapid_mode==0 || rapid_mode==3)){
        if(mode==afeed){
            if(submode==man) { go_right(mm_min_to_clk(afeed_mm, tickz_nm)); }
            else if(submode==ex) { afeed_ext_right(); }
            else if(submode==in) { afeed_int_right(); }
        }
        else if(mode==feed){
            if(submode==man){ go_right(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm)); }
            else if(submode==ex){ feed_ext_right(); }
            else if(submode==in){ feed_int_right(); }
        }
        else if(mode==screw){
            if(submode==man){ go_right(mm_min_to_clk(((double)(screw_step*rpm))/100, tickz_nm)); }
            else if(submode==ex){ screw_ext_right(); }
            else if(submode==in){ screw_int_right(); }
        }
        else if(mode==lcone){
            if(submode==man){go_right(CONE_FEED_X);go_down(CONE_FEED_X);}
        }
        else if(mode==rcone){
            if(submode==man){go_right(CONE_FEED_X);go_up(CONE_FEED_X);}
        }
    }
    else if(joy_l && !joy_rapid && (rapid_mode==0 || rapid_mode==3)){
        if(mode==afeed){
            if(submode==man) { go_left(mm_min_to_clk(afeed_mm, tickz_nm)); }
            else if(submode==ex) { afeed_ext_left(); }
            else if(submode==in) { afeed_int_left(); }
        }
        else if(mode==feed){
            if(submode==man){ go_left(mm_min_to_clk(((double)(feed_mm*rpm))/100, tickz_nm)); }
            else if(submode==ex){ feed_ext_left(); }
            else if(submode==in){ feed_int_left(); }
        }
        else if(mode==screw){
            if(submode==man){ go_left(mm_min_to_clk(((double)(screw_step*rpm))/100, tickz_nm)); }
            else if(submode==ex){ screw_ext_left(); }
            else if(submode==in){ screw_int_left(); }
        }
        else if(mode==lcone){
            if(submode==man){go_left(CONE_FEED_X);go_up(CONE_FEED_X);}
        }
        else if(mode==rcone){
            if(submode==man){go_left(CONE_FEED_X);go_down(CONE_FEED_X);}
            else if(submode==ex){ rcone_ext_left(); }
        }
    }
    
    if(!joy_rapid && rapid_mode==1){
        rapid_mode=2;
    }


    if(rapid_mode==1 && millis()-rapid_tmr>=10){
        rapid_tmr=millis();

        if((clk_to_mm_min(new_spx, tickx_nm) < rapid_speed) ) new_spx=mm_min_to_clk(clk_to_mm_min(new_spx, tickx_nm) + (uint32_t)((double)accel /100.0 * 60.0), tickx_nm);
        if((clk_to_mm_min(new_spz, tickz_nm) < rapid_speed) ) new_spz=mm_min_to_clk(clk_to_mm_min(new_spz, tickz_nm) + (uint32_t)((double)accel /100.0 * 60.0), tickz_nm);
        

    }
    else if(rapid_mode==2 && millis()-rapid_tmr>=10){
        rapid_tmr=millis();

        uint16_t speed=0;
        if (mode==afeed) speed=afeed_mm;
        else if (mode==feed || mode==rcone || mode==lcone || mode==sfeed) speed=((double)(feed_mm*rpm))/100;
        else if (mode==screw) speed=((double)(screw_step*rpm))/100;
        
        if(clk_to_mm_min(new_spx,tickx_nm) > speed + (uint32_t)((double)accel /100.0 * 60.0) ) new_spx = mm_min_to_clk(clk_to_mm_min(new_spx, tickx_nm)-(uint32_t)((double)accel /100.0 * 60.0), tickx_nm);
        else{ 
            new_spx = mm_min_to_clk(speed, tickx_nm);
            rapid_mode=3;
        }

        if(clk_to_mm_min(new_spz,tickz_nm) > speed + (uint32_t)((double)accel /100.0 * 60.0) ) new_spz = mm_min_to_clk(clk_to_mm_min(new_spz, tickz_nm)-(uint32_t)((double)accel /100.0 * 60.0), tickz_nm);
        else{ 
            new_spz = mm_min_to_clk(speed, tickz_nm);
            rapid_mode=3;
        }
    }
    ////////////////////////////////////////////////////////

    //limits////////////////////////////////////////////////
    if(!limit_l && !limit_r && !limit_d && !limit_u){
        lim_pressed=0;
    }
    else if((limit_l || limit_r || limit_d || limit_u) && lim_pressed==0){
        lim_pressed=1;
        lim_tmr=millis();
    }
    else if(limit_l && millis()-lim_tmr>100 && lim_pressed==1){
        lim_pressed=2;
        llim=1-llim;
        llpos=zpos;
        ledl(llim);
    }
    else if(limit_r && millis()-lim_tmr>100 && lim_pressed==1){
        lim_pressed=2;
        rlim=1-rlim;
        rlpos=zpos;
        ledr(rlim);
    }
    else if(limit_d && millis()-lim_tmr>100 && lim_pressed==1){
        lim_pressed=2;
        dlim=1-dlim;
        dlpos=xpos;
        ledd(dlim);
    }
    else if(limit_u && millis()-lim_tmr>100 && lim_pressed==1){
        lim_pressed=2;
        ulim=1-ulim;
        ulpos=xpos;
        ledu(ulim);
    }
    ////////////////////////////////////////////////////////

    //manual impulse generator (MIG)////////////////////////
    if(!joy_u && !joy_d && !joy_r && !joy_l){
        if(TIM4->CNT!=old_mig_cnt){
            int32_t delta=(TIM4->CNT)-old_mig_cnt;
            old_mig_cnt=(TIM4->CNT);

            if(mig_z){
                travel_distz += delta * ((int32_t)( ((double)((mig_x1 ? x1_dist_mcm : x10_dist_mcm))) * 1000.0 / ((double)(tickz_nm)) ));
            }
            else if (mig_x){
                travel_distx += delta * ((int32_t)( ((double)((mig_x1 ? x1_dist_mcm : x10_dist_mcm))) * 1000.0 / ((double)(tickx_nm)) ));
            }
        }
        if(mig_z && old_axis!=0  && !joy_u && !joy_d && !joy_r && !joy_l){
            old_axis=0;
            stopx();
            travel_distx=0;
            startz();
        }
        else if(mig_x && old_axis!=1){
            old_axis=1;
            stopz();
            travel_distz=0;
            startx();
        }
        else if(!mig_x && !mig_z){
            travel_distx=0;
            travel_distz=0;
            stopx();
            stopz();
            old_axis=2;
        }
    }
    ////////////////////////////////////////////////////////

} //logic()
