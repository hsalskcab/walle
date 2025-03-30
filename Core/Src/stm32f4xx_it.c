/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include <math.h>
#include <stdlib.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ACCEL_TIME(y) ((double)( ((double)(y) / 60.0) / ((double)(accel)) ))
#define ACCEL_DIST(x) ((uint32_t)((double)( (((double)(x)) / 60.0) * ACCEL_TIME(x) / 2.0 ) * 1000000.0 / ((double)(tickx_nm))))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t statex=0, statez=0, spx_toggle=0, spz_toggle=0;
uint16_t spx_low=0, spz_low=0, spx_high=0, spz_high=0;
uint32_t new_spx=10000, new_spz=10000;
uint32_t tim_accel=0;

extern uint16_t sp_mig;
extern uint32_t accel;

extern uint16_t afeed_mm;
extern uint32_t xpos, zpos;
extern uint32_t ulpos, dlpos, rlpos, llpos;
extern uint8_t ulim, dlim, rlim, llim;
extern uint8_t is_ulim, is_dlim, is_rlim, is_llim;

extern uint8_t spindle_dir;
extern uint32_t spindle_pos;
extern uint32_t spindle_enc_lines;
extern uint32_t rpm;

extern int32_t travel_distx, travel_distz;
extern uint16_t tickx_nm, tickz_nm;

uint32_t accel_ticks=0;
uint8_t accel_mode=0;

uint32_t last_tick=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim8;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles EXTI line2 interrupt.
  */
void EXTI2_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_IRQn 0 */
  EXTI->PR=SPINDLE_ENCA_Pin;
  uint32_t tick=micros();
  rpm=60000.0/(((double)(tick-last_tick)*(double)spindle_enc_lines)/1000.0);
  last_tick=tick;
  /* USER CODE END EXTI2_IRQn 0 */
  //HAL_GPIO_EXTI_IRQHandler(SPINDLE_ENCA_Pin);
  /* USER CODE BEGIN EXTI2_IRQn 1 */
  if(SPINDLE_ENCB_GPIO_Port->IDR&SPINDLE_ENCB_Pin){
    spindle_dir=1;
    spindle_pos+=1;
  }
  else{
    spindle_dir=0;
    spindle_pos-=1;
  }
  constrain(spindle_pos, 0, spindle_enc_lines);
  //HAL_NVIC_DisableIRQ(EXTI2_IRQn);
  /* USER CODE END EXTI2_IRQn 1 */
}

/**
  * @brief This function handles TIM1 capture compare interrupt.
  */
void TIM1_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_CC_IRQn 0 */
  spx_low = new_spx%65536;
  spx_high = new_spx/65536;

  if(spx_high == 0)
    TIM1->CCR1+=spx_low;
  else{
    TIM1->CCR1 += spx_toggle == 1 ? spx_low : 0;
  }

  TIM1->DIER|=TIM_DIER_CC1IE;  //разрешаем прерывания таймера по CC
	TIM1->SR=0; //выключаем флажок прерывания

  /* USER CODE END TIM1_CC_IRQn 0 */
  //HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_CC_IRQn 1 */
  if(spx_high == 0){
    spx_toggle=0;
    statex=1-statex;
  }
  else{
    spx_toggle-=1;
    constrain(spx_toggle, 0, spx_high+1);
    statex = spx_toggle == 0 ? 1 : 0;
  }
  if(xpos<ulpos || ulim==0)is_ulim=0;
  if(xpos>dlpos || dlim==0)is_dlim=0;
  if(statex==0){
    setx(statex);
  }
  else{
    if(!joy_u && !joy_d && !joy_r && !joy_l){
      afeed_mm=accel_ticks;
      if(travel_distx==0){
        new_spx=0;
        accel_ticks=0;
        statex=0;
      }
      else if(new_spx==0){
          new_spx = mm_min_to_clk( (uint32_t)((double)accel /100.0 * 60.0) ,tickx_nm);
      }
      else{
        if(accel_ticks>=abs(travel_distx) && (millis()-tim_accel>=10)){
          new_spx = mm_min_to_clk( clk_to_mm_min(new_spx, tickx_nm) - (int32_t)((double)accel /100.0 * 60.0) ,tickx_nm);
          tim_accel=millis();
        }
        else if((clk_to_mm_min(new_spx, tickx_nm) + (uint32_t)((double)accel /100.0 * 60.0) <= sp_mig) && (millis()-tim_accel>=10)){
          new_spx = mm_min_to_clk( clk_to_mm_min(new_spx, tickx_nm) + (uint32_t)((double)accel /100.0 * 60.0) ,tickx_nm);
          if((clk_to_mm_min(new_spx, tickx_nm) + (uint32_t)((double)accel /100.0 * 60.0) > sp_mig)) accel_mode=1;
          else accel_mode=0;
          tim_accel=millis();
        }
      }
      if((accel_mode==0)  && ( accel_ticks<abs(travel_distx) )){
        accel_ticks++;
      }



      if((travel_distx>0)){
        set_dirx(1);
        xpos+=1;
        travel_distx-=1;
        setx(statex);
      }
      else if(travel_distx<0){
        set_dirx(0);
        xpos-=1;
        travel_distx+=1;
        setx(statex);
      }
      else{
        spx_high=0;
        spx_low=0;
        setx(0);
      }
    }
    else{
      if((ulim==0 && dirx==1) || (dlim==0 && dirx==0)){
        setx(statex);
        xpos=xpos + (dirx==1 ? 1 : -1);
      }
      else if(ulim==1 && dirx==1 && xpos>=ulpos){
        is_ulim=1;
      }
      else if(ulim==1 && dirx==1 && xpos<ulpos){
        setx(statex);
        xpos+=1;
      }
      else if(dlim==1 && dirx==0 && xpos<=dlpos){
        is_dlim=1;
      }
      else if(dlim==1 && dirx==0 && xpos>dlpos){
        setx(statex);
        xpos-=1;
      }
    }
  }
  /* USER CODE END TIM1_CC_IRQn 1 */
}

/**
  * @brief This function handles TIM8 capture compare interrupt.
  */
void TIM8_CC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM8_CC_IRQn 0 */;
  spz_low = new_spz%65536;
  spz_high = new_spz/65536;

  if(spz_high == 0)
    TIM8->CCR1+=spz_low;
  else{
    TIM8->CCR1 += spz_toggle == 1 ? spz_low : 0;
  }

	TIM8->DIER|=TIM_DIER_CC1IE;  //разрешаем прерывания таймера по CC
	TIM8->SR=0; //выключаем флажок прерывания
  /* USER CODE END TIM8_CC_IRQn 0 */
  //HAL_TIM_IRQHandler(&htim8);
  /* USER CODE BEGIN TIM8_CC_IRQn 1 */
  if(spz_high == 0){
    spz_toggle=0;
    statez=1-statez;
  }
  else{
    spz_toggle-=1;
    constrain(spz_toggle, 0, spz_high+1);
    statez  = spz_toggle == 0 ? 1 : 0;
  }

  if(zpos<llpos || llim==0)is_llim=0;
  if(zpos>rlpos || rlim==0)is_rlim=0;

  if(statez==0){
    setz(statez);
  }
  else{
    if(!joy_u && !joy_d && !joy_r && !joy_l){
      if(travel_distz>0){


        set_dirz(1);
        zpos+=1;
        travel_distz-=1;
        setz(statez);
      }
      else if(travel_distz<0){

        set_dirz(0);
        zpos-=1;
        travel_distz+=1;
        setz(statez);
      }
      else{
        setz(0);
      }
    }
    else {
      if((llim==0 && dirz==1) || (rlim==0 && dirz==0)){
        setz(statez);
        zpos=zpos + (dirz==1 ? 1 : -1);
      }
      else if(llim==1 && dirz==1 && zpos>=llpos){
        is_llim=1;
      }
      else if(llim==1 && dirz==1 && zpos<llpos){
        setz(statez);
        zpos+=1;
      }
      else if(rlim==1 && dirz==0 && zpos<=rlpos){
        is_rlim=1;
      }
      else if(rlim==1 && dirz==0 && zpos>rlpos){
        setz(statez);
        zpos-=1;
      }
    }
  }
  /* USER CODE END TIM8_CC_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
