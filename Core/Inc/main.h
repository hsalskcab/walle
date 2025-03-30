/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

typedef enum
{
    settings,
    afeed,
    sfeed,
    divider,
    sphere,
    rcone,
    lcone,
    screw,
    feed,
    reset
} modes_t;

typedef enum{
  ex,
  man,
  in
} submodes_t;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

//helpful macros//////////////////////////////////////////////
#define sgn(x) (x>0 ? 1 : x<0 ? -1 : 0)
#define constrain(n, l , u) n = (n > u+110 || n<l) ? u : ((n>u) ? l : n)
#define mcm_to_ticks(x, y) ((x*1000)/y)
//////////////////////////////////////////////////////////////

//time macros/////////////////////////////////////////////////
#define micros() (TIM2->CNT)
#define usDelay(x) micros()=0;while(micros()<x)
#define delay(x) HAL_Delay(x)
#define millis() HAL_GetTick()
//////////////////////////////////////////////////////////////

//feed speed encoder button///////////////////////////////////
#define fs_btn ((FEED_SPEED_ENC_BTN_GPIO_Port -> IDR & FEED_SPEED_ENC_BTN_Pin) ? 0 : 1)
//////////////////////////////////////////////////////////////

//limit LEDs mascros//////////////////////////////////////////
#define ledd(x)   LED_L_GPIO_Port->BSRR=LED_L_Pin << (x ? 0 : 16);delay(1)
#define ledl(x)   LED_R_GPIO_Port->BSRR=LED_R_Pin << (x ? 0 : 16);delay(1) 
#define ledr(x)   LED_U_GPIO_Port->BSRR=LED_U_Pin << (x ? 0 : 16);delay(1) 
#define ledu(x)   LED_D_GPIO_Port->BSRR=LED_D_Pin << (x ? 0 : 16);delay(1) 
//////////////////////////////////////////////////////////////

//limit buttons macros////////////////////////////////////////
#define limit_d (GPIOC->IDR&(1<<8))
#define limit_l (GPIOC->IDR&(1<<9))
#define limit_r (GPIOA->IDR&(1<<8))
#define limit_u (GPIOA->IDR&(1<<9))
//////////////////////////////////////////////////////////////

//menu buttons macros/////////////////////////////////////////
#define menu_u (MENU_U_GPIO_Port -> IDR & MENU_U_Pin)
#define menu_d (MENU_D_GPIO_Port -> IDR & MENU_D_Pin)
#define menu_r (MENU_R_GPIO_Port -> IDR & MENU_R_Pin)
#define menu_l (MENU_L_GPIO_Port -> IDR & MENU_L_Pin)
#define menu_s (MENU_S_GPIO_Port -> IDR & MENU_S_Pin)
//////////////////////////////////////////////////////////////

//joystick macros/////////////////////////////////////////////
#define joy_r     (JOY_U_GPIO_Port -> IDR & JOY_U_Pin)
#define joy_l     (JOY_D_GPIO_Port -> IDR & JOY_D_Pin)
#define joy_u     (JOY_R_GPIO_Port -> IDR & JOY_R_Pin)
#define joy_d     (JOY_L_GPIO_Port -> IDR & JOY_L_Pin)
#define joy_rapid (JOY_RAPID_GPIO_Port -> IDR & JOY_RAPID_Pin)
//////////////////////////////////////////////////////////////

//submode switcher macros/////////////////////////////////////
#define submode_ex  (SUBMODE_EX_GPIO_Port -> IDR & SUBMODE_EX_Pin)
#define submode_in  (SUBMODE_IN_GPIO_Port -> IDR & SUBMODE_IN_Pin)
#define submode_man (SUBMODE_MAN_GPIO_Port -> IDR & SUBMODE_MAN_Pin)
//////////////////////////////////////////////////////////////

//mode switcher///////////////////////////////////////////////
#define mode_afeed (MODE_SETTINGS_GPIO_Port -> IDR & MODE_SETTINGS_Pin)
#define mode_settings    (MODE_AFEED_GPIO_Port -> IDR & MODE_AFEED_Pin)
#define mode_sfeed    (MODE_SFEED_GPIO_Port -> IDR & MODE_SFEED_Pin)
#define mode_divider  (MODE_DIVIDER_GPIO_Port -> IDR & MODE_DIVIDER_Pin)
#define mode_screw   (MODE_SPHERE_GPIO_Port -> IDR & MODE_SPHERE_Pin)
#define mode_rcone    (MODE_RCONE_GPIO_Port -> IDR & MODE_RCONE_Pin)
#define mode_lcone    (MODE_LCONE_GPIO_Port -> IDR & MODE_LCONE_Pin)
#define mode_sphere    (MODE_SCREW_GPIO_Port -> IDR & MODE_SCREW_Pin)
#define mode_feed     (!mode_afeed && !mode_sfeed && !mode_divider &&\
                       !mode_lcone && !mode_rcone && !mode_sphere \
                       && !mode_settings && !mode_screw)
//////////////////////////////////////////////////////////////

//manual encoder//////////////////////////////////////////////
#define mig_x1       ((MANUAL_ENC_X1_GPIO_Port->IDR&MANUAL_ENC_X1_Pin) ? 0 : 1)
#define mig_x10      ((MANUAL_ENC_X10_GPIO_Port->IDR&MANUAL_ENC_X10_Pin) ? 0 : 1)
#define mig_z        ((MANUAL_ENC_Z_GPIO_Port->IDR&MANUAL_ENC_Z_Pin) ? 0 : 1)
#define mig_x        ((MANUAL_ENC_X_GPIO_Port->IDR&MANUAL_ENC_X_Pin) ? 0 : 1)
//////////////////////////////////////////////////////////////

//motor controls//////////////////////////////////////////////
extern uint8_t dirx, dirz;
#define setx(x)       STEP_X_GPIO_Port->BSRR=STEP_X_Pin << (x ? 0 : 16) 
#define invertx()     STEP_X_GPIO_Port->ODR^=STEP_X_Pin
#define set_dirx(z)   DIR_X_GPIO_Port->BSRR=DIR_X_Pin << (z ? 0 : 16);dirx=z
#define stopx()       (TIM1->CR1&=0)
#define startx()      (TIM1->CR1|=1)

#define setz(y)       STEP_Z_GPIO_Port->BSRR=STEP_Z_Pin << (y ? 0 : 16)
#define invertz()     STEP_Z_GPIO_Port->ODR^=STEP_Z_Pin
#define set_dirz(w)   DIR_Z_GPIO_Port->BSRR=DIR_Z_Pin << (w ? 16 : 0);dirz=w
#define stopz()       (TIM8->CR1&=0)
#define startz()      (TIM8->CR1|=1)

extern uint32_t new_spz, new_spx;
#define go_left(x)    set_dirz(1);new_spz=x;startz()

#define go_right(x)   set_dirz(0);new_spz=x;startz()

#define go_up(x)      set_dirx(1);new_spx=x;startx()

#define go_down(x)    set_dirx(0);new_spx=x;startx()

#define freq_ref 500000.0
#define ccr_ref 168.0
#define mm_min_to_clk(x, y) (uint32_t)(ccr_ref*(freq_ref/(((double)(x)/60.0)*1000000.0/(double)(y))))
#define clk_to_mm_min(x, y) (uint32_t)( 168.0 * 30.0 * ((double)(y)) / ((double)(x)) )
//////////////////////////////////////////////////////////////

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPINDLE_ENCA_Pin GPIO_PIN_2
#define SPINDLE_ENCA_GPIO_Port GPIOE
#define SPINDLE_ENCA_EXTI_IRQn EXTI2_IRQn
#define SPINDLE_ENCB_Pin GPIO_PIN_3
#define SPINDLE_ENCB_GPIO_Port GPIOE
#define MANUAL_ENC_X1_Pin GPIO_PIN_4
#define MANUAL_ENC_X1_GPIO_Port GPIOE
#define MANUAL_ENC_X10_Pin GPIO_PIN_5
#define MANUAL_ENC_X10_GPIO_Port GPIOE
#define MANUAL_ENC_Z_Pin GPIO_PIN_6
#define MANUAL_ENC_Z_GPIO_Port GPIOE
#define MANUAL_ENC_X_Pin GPIO_PIN_13
#define MANUAL_ENC_X_GPIO_Port GPIOC
#define SUBMODE_IN_Pin GPIO_PIN_4
#define SUBMODE_IN_GPIO_Port GPIOA
#define SUBMODE_MAN_Pin GPIO_PIN_5
#define SUBMODE_MAN_GPIO_Port GPIOA
#define SUBMODE_EX_Pin GPIO_PIN_6
#define SUBMODE_EX_GPIO_Port GPIOA
#define MODE_SETTINGS_Pin GPIO_PIN_7
#define MODE_SETTINGS_GPIO_Port GPIOA
#define MODE_DIVIDER_Pin GPIO_PIN_4
#define MODE_DIVIDER_GPIO_Port GPIOC
#define MODE_SPHERE_Pin GPIO_PIN_5
#define MODE_SPHERE_GPIO_Port GPIOC
#define MODE_RCONE_Pin GPIO_PIN_0
#define MODE_RCONE_GPIO_Port GPIOB
#define MODE_LCONE_Pin GPIO_PIN_1
#define MODE_LCONE_GPIO_Port GPIOB
#define MODE_SFEED_Pin GPIO_PIN_7
#define MODE_SFEED_GPIO_Port GPIOE
#define MODE_SCREW_Pin GPIO_PIN_8
#define MODE_SCREW_GPIO_Port GPIOE
#define MODE_AFEED_Pin GPIO_PIN_9
#define MODE_AFEED_GPIO_Port GPIOE
#define FEED_SPEED_ENC_BTN_Pin GPIO_PIN_11
#define FEED_SPEED_ENC_BTN_GPIO_Port GPIOE
#define MENU_S_Pin GPIO_PIN_13
#define MENU_S_GPIO_Port GPIOE
#define MENU_D_Pin GPIO_PIN_14
#define MENU_D_GPIO_Port GPIOE
#define MENU_U_Pin GPIO_PIN_15
#define MENU_U_GPIO_Port GPIOE
#define MENU_R_Pin GPIO_PIN_10
#define MENU_R_GPIO_Port GPIOB
#define MENU_L_Pin GPIO_PIN_11
#define MENU_L_GPIO_Port GPIOB
#define JOY_L_Pin GPIO_PIN_12
#define JOY_L_GPIO_Port GPIOB
#define JOY_R_Pin GPIO_PIN_13
#define JOY_R_GPIO_Port GPIOB
#define JOY_U_Pin GPIO_PIN_14
#define JOY_U_GPIO_Port GPIOB
#define JOY_D_Pin GPIO_PIN_15
#define JOY_D_GPIO_Port GPIOB
#define JOY_RAPID_Pin GPIO_PIN_8
#define JOY_RAPID_GPIO_Port GPIOD
#define HAND_ENC_B_Pin GPIO_PIN_12
#define HAND_ENC_B_GPIO_Port GPIOD
#define HAND_ENC_A_Pin GPIO_PIN_13
#define HAND_ENC_A_GPIO_Port GPIOD
#define LIM_L_Pin GPIO_PIN_8
#define LIM_L_GPIO_Port GPIOC
#define LIM_R_Pin GPIO_PIN_9
#define LIM_R_GPIO_Port GPIOC
#define LIM_U_Pin GPIO_PIN_8
#define LIM_U_GPIO_Port GPIOA
#define LIM_D_Pin GPIO_PIN_9
#define LIM_D_GPIO_Port GPIOA
#define LED_L_Pin GPIO_PIN_10
#define LED_L_GPIO_Port GPIOA
#define LED_R_Pin GPIO_PIN_11
#define LED_R_GPIO_Port GPIOA
#define LED_D_Pin GPIO_PIN_15
#define LED_D_GPIO_Port GPIOA
#define LED_U_Pin GPIO_PIN_12
#define LED_U_GPIO_Port GPIOC
#define STEP_X_Pin GPIO_PIN_1
#define STEP_X_GPIO_Port GPIOD
#define DIR_X_Pin GPIO_PIN_2
#define DIR_X_GPIO_Port GPIOD
#define STEP_Z_Pin GPIO_PIN_3
#define STEP_Z_GPIO_Port GPIOD
#define DIR_Z_Pin GPIO_PIN_4
#define DIR_Z_GPIO_Port GPIOD
#define SPINDLE_ENC_B_Pin GPIO_PIN_4
#define SPINDLE_ENC_B_GPIO_Port GPIOB
#define SPINDLE_ENC_A_Pin GPIO_PIN_5
#define SPINDLE_ENC_A_GPIO_Port GPIOB
#define LCD_SCL_Pin GPIO_PIN_6
#define LCD_SCL_GPIO_Port GPIOB
#define LCD_SDA_Pin GPIO_PIN_7
#define LCD_SDA_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
