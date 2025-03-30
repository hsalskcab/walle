#include "ui.h"
#include "main.h"
#include "lcd.h"
#include <stdio.h>

//extern vars/////////////////////////////////////////////
extern uint8_t ui_changed;

extern modes_t mode;
extern submodes_t submode;

extern uint32_t rpm;
extern uint16_t afeed_mm, feed_mm;
extern uint8_t passes_done, passes_total, passes_finish;
extern uint16_t slice;
extern uint16_t screw_step;

extern uint32_t cone_angle;

extern uint32_t xpos,zpos;

extern uint8_t err_no_lim, err_is_not_on_lim;
extern uint8_t completed;


extern uint32_t spindle_pos;
extern uint32_t spindle_enc_lines;
extern uint16_t sectors,sector_rn,old_sector;
//////////////////////////////////////////////////////////
extern uint32_t new_spx, new_spz;
extern uint16_t spx_low, spx_high, tickx_nm;
extern int32_t travel_distx, travel_distz;
//vars////////////////////////////////////////////////////
uint32_t last_time_updated=0;
char st[4][21];
//////////////////////////////////////////////////////////

extern int32_t travel_distx;

void ui(){
    if(millis()-last_time_updated>=100 && ui_changed){
        ui_changed=0;
        last_time_updated=millis();
        uint32_t rpm_int = (uint32_t)rpm;

        
        if(mode==afeed){
          if (submode==ex){
            snprintf(st[0], 21, "\x41\x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1 c\xBD""apy\xB6\xB8       ");  // СИНХРОННАЯ снаружи
            snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %3d \xBC\xBC\x2F\xBC\xB8\xBD    ", afeed_mm);
            snprintf(st[2], 21, "\xBE""\x70\x6F\x78\x6F\xE3\x6F\xB3"" %2d       <>", passes_total - passes_done); //проходов
            snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
          else if (submode==in){
        	  snprintf(st[0], 21, "\x41\x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1 \xB3\xBD""y\xBF""p\xB8  ");  // АСИНХРОННАЯ внутри
        	  snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %3d \xBC\xBC\x2F\xBC\xB8\xBD  ", afeed_mm);
        	  snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
        	  snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
          else if (submode==man){
        	  snprintf(st[0], 21, "\x41\x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1                               ");  //АСИНХРОННАЯ
        	  snprintf(st[1], 21, "                                          ");
        	  snprintf(st[2], 21, "\xBE""o\xE3""a\xC0""a  %3d \xBC\xBC\x2F\xBC\xB8\xBD  ", afeed_mm);
        	  snprintf(st[3], 21, "%10d                                           ", travel_distx);
          }
        }

        else if (mode==feed){
          if (submode==ex){
          snprintf(st[0], 21, " \x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1 c\xBD""apy\xB6\xB8 ");  // СИНХРОННАЯ снаружи
          snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100);
          snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
          snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
          else if (submode==man){
          snprintf(st[1-1], 21, "    \x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1      ");  // СИНХРОННАЯ
          snprintf(st[2-1], 21, "                    ");
          snprintf(st[3-1], 21, " \xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2 ", feed_mm / 100, feed_mm % 100);
          snprintf(st[4-1], 21, "        %5ld O\xA0""/M\xA5""H", rpm_int); //  0 витков
          }
          else if (submode==in){
          snprintf(st[1-1], 21, " \x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1  \xB3\xBD""y\xBF""p\xB8 ");  // СИНХРОННАЯ внутри
          snprintf(st[2-1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100);
          snprintf(st[3-1], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
          snprintf(st[4-1], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
        }

        else if (mode==sfeed){
          if (submode==ex){
          snprintf(st[1-1], 21, " \x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1 c\xBD""apy\xB6\xB8 ");  // СИНХРОННАЯ снаружи
          snprintf(st[2-1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100);
          snprintf(st[3-1], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
          snprintf(st[4-1], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
          else if (submode==man){
          snprintf(st[1-1], 21, "\x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1(CKOP.PE\xA4)                                             ");                             // СИНХРОННАЯ
          snprintf(st[2-1], 21, "O\xA0""/M\xA5""H: %1d                                        ", rpm_int);
          snprintf(st[3-1], 21, "C""K""OPOCTb"" PE""\xA4"".: %1d  <>                               ", rpm_int);  // скор. рез.
          snprintf(st[4-1], 21, "\xE0\xA5""AMETP: %1d.%02d     \xDA\xD9                       ", rpm_int / 100, rpm_int % 100);
          }
          else if (submode==in){
          snprintf(st[1-1], 21, " \x43\xA5\x48\x58\x50\x4F\x48\x48\x41\xB1  \xB3\xBD""y\xBF""p\xB8 ");  // СИНХРОННАЯ внутри
          snprintf(st[2-1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100);
          snprintf(st[3-1], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
          snprintf(st[4-1], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
        }

        else if (mode==divider){
            uint32_t angle=(spindle_pos*36000)/spindle_enc_lines;
            sector_rn=angle/(36000/sectors)+1;
            uint32_t sector_angle = angle - (36000/sectors*(sector_rn-1));
            snprintf(st[1-1], 21, "\x79\xB4\x6F\xBB \xC1\xBE\xB8\xBD\xE3""e\xBB\xC7 %3ld.%02ld ", angle / 100, angle % 100); //угол шпинделя
            snprintf(st[2-1], 21, "\xB3""ce\xB4""o ce\xBA\xBF""o\x70""o\xB3 \xDA\xD9%3d", sectors); //всего секторов
            snprintf(st[3-1], 21, "\xB3\xC3\xB2\x6F\x70 ce\xBA\xBF""o\x70""a    %3d", sector_rn);  // выбор сектора
            snprintf(st[4-1], 21, "\x79\xB4\x6F\xBB  ce\xBA\xBF""o\x70""a %3ld.%02ld ", sector_angle / 100, sector_angle % 100); //угол сектора*/
        }

        else if (mode==screw){
          if(submode==man){
            snprintf(st[0], 21, "      \x50\x45\xA4\x62\xA0\x41        ");       //РЕЗЬБА
            snprintf(st[2], 21, "   \xC1""a\xB4  %1u.%02u \xBC\xBC                 ", screw_step/100, screw_step%100); // шаг в мм
            snprintf(st[1], 21, "                                                      ");
            snprintf(st[3], 21, "                                                      ");
          }
          else if (submode == in){
            snprintf(st[0], 21, "\x50\x45\xA4\x62\xA0\x41  \xB3\xBD\x79\xBF\x70\x65\xBD\xBD\xC7\xC7 ");  //РЕЗЬБА внутренняя
            snprintf(st[1], 21, "\xC1""a\xB4   %1u.%02u \xBC\xBC                 ", screw_step/100, screw_step%100); // шаг в мм
            snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
        	  snprintf(st[3], 21, "final:  %2d     \xDA\xD9", passes_finish); // съём/R:
          }
          else if (submode==ex){
            snprintf(st[0], 21, "\x50\x45\xA4\x62\xA0\x41   \xBD""apy\xB6\xBD""a\xC7  ");                //РЕЗЬБА наружная
            snprintf(st[1], 21, "\xC1""a\xB4  %1u.%02u \xBC\xBC                 ", screw_step/100, screw_step%100); // шаг в мм
            snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //проходов
        	  snprintf(st[3], 21, "final: %2d         \xDA\xD9", passes_finish); // съём/R:
          }
        }

        else if (mode == lcone){
          if(submode==man){
            snprintf(st[0], 21, "   \x4B\x4F\x48\xA9\x43  <            ");
            snprintf(st[1], 21, "\xA9\xA1""O\xA7"" KOH\xA9""CA"" %2d.%02d <>             ", cone_angle / 100, cone_angle % 100);
            snprintf(st[2], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100);
            snprintf(st[3], 21, "                                         ");
          }
          else if (submode == in){
            snprintf(st[0], 21, " \x4B\x4F\x48\xA9\x43 <      \xB3\xBD""y\xBF""p\xB8");  //конус <
            snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100); //подача
            snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //  проходов осталось
            snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
          else if (submode==ex){
            snprintf(st[0], 21, " \x4B\x4F\x48\xA9\x43 <     c\xBD""apy\xB6\xB8");  //конус <
            snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100); //подача
            snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //  проходов осталось
            snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
        }

        else if (mode == rcone){
          if(submode==man){
            snprintf(st[0], 21, "   \x4B\x4F\x48\xA9\x43  >             ");
            snprintf(st[1], 21, "                                         ");
            snprintf(st[2], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100);
            snprintf(st[3], 21, "                                         ");
          }
          else if (submode == in){
            snprintf(st[0], 21, " \x4B\x4F\x48\xA9\x43 >      \xB3\xBD""y\xBF""p\xB8");  //конус <
            snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100); //подача
            snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //  проходов осталось
            snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
          else if (submode==ex){
            snprintf(st[0], 21, " \x4B\x4F\x48\xA9\x43 >    c\xBD""apy\xB6\xB8");  //конус <
            snprintf(st[1], 21, "\xBE""o\xE3""a\xC0""a  %1d.%02d \xBC\xBC\x2F\x6F\xB2  ", feed_mm / 100, feed_mm % 100); //подача
            snprintf(st[2], 21, "\xBE\x70\x6F\x78\x6F\xE3\x6F\xB3 %2d       <>", passes_total - passes_done); //  проходов осталось
            snprintf(st[3], 21, "\x63\xC2\xB5\xBC/R    %1d.%02d\xBC\xBC  \xDA\xD9", slice / 100, slice % 100); // съём/R:
          }
        }

        else if (mode == sphere){
            snprintf(st[0], 21, "EMPTY                                    ");
            snprintf(st[1], 21, "mode sphere will be                  ");
            snprintf(st[2], 21, "done in one of the            ");
            snprintf(st[3], 21, "upcoming updates                                         ");
        }

        else if (mode == settings){
            snprintf(st[0], 21, "settings                                    ");
            snprintf(st[1], 21, "                                              ");
            snprintf(st[2], 21, "                                           ");
            snprintf(st[3], 21, "                                         ");
        }



        else {
        snprintf(st[1-1], 21, "UNKNOWN                                                "); 
        snprintf(st[2-1], 21, "                                                       "); 
        snprintf(st[3-1], 21, "                                                       ");  
        snprintf(st[4-1], 21, "                                                       "); 
      }

        if(err_no_lim){
          snprintf(st[0], 21, "                                       ");
          snprintf(st[1], 21, "       \xA9\xA8""OP\xAE        ");
          snprintf(st[2], 21, "   \xBD\x65 \x79\x63\xBF\x61\xBD\x6F\xB3\xBB\x65\xBD\xC3   ");
          snprintf(st[3], 21, "                                       ");
        }
        else if(err_is_not_on_lim){
          snprintf(st[0], 21, "                               ");
          snprintf(st[1], 21, "      C\xA9\xA8\xA8""OPT       "); 
          snprintf(st[2], 21, "\xE3""o\xBB\xB6""e\xBD \xB2\xC3\xBF\xC4 \xBD\x61 \xA9\xA8""OPE"); 
          snprintf(st[3], 21, "                               ");
        }
        else if(completed){
          snprintf(st[0], 21, "                               ");
          snprintf(st[1], 21, " O\xA8""EPA\xE1\xA5\xB1 \xA4""ABEP\xAC""EHA "); //" OПEPAЦИЯ 3ABEPШEHA "
          snprintf(st[2], 21, "                               ");
          snprintf(st[3], 21, "                               ");
        }

    setCursor(0,0);
    lcdPrint(st[0]);
    setCursor(0,1);
    lcdPrint(st[1]);
    setCursor(0,2);
    lcdPrint(st[2]);
    setCursor(0,3);
    lcdPrint(st[3]);

    // if(err_no_lim || err_is_not_on_lim || completed){
    //   completed=0;
    //   err_no_lim=0;
    //   err_is_not_on_lim=0;
    //   ui_changed=1;
    // }
    }//if(millis()-last_time_updated>=100 && ui_changed)
}


