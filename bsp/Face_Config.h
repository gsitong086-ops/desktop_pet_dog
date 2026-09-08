/**
  ******************************************************************************
  * @file    Face_Config.h
  * @author  SZTU OSHA  HFH、KRM、LZS
  * @brief   This file contains all the function prototypes for
  *          the Face_Config.c file
  ******************************************************************************
  * @attention
  *
  * 
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FACE_CONFIG_H__
#define __FACE_CONFIG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* SZTU OSHA  Includes--------------------------------------------------------*/
#include "main.h"
#include "OLED.h"

/* SZTU OSHA  instructions macro----------------------------------------------*/
// 面部表情枚举
typedef enum {
    FACE_SLEEP = 0,
    FACE_STARE,
    FACE_HAPPY,
    FACE_MANIA,
    FACE_VERY_HAPPY,
    FACE_EYES,
    FACE_HELLO,
    FACE_MAX
} FaceMode;



/* SZTU OSHA  Private defines-------------------------------------------------*/
// 动作模式枚举
typedef enum {
    MODE_RELAXED_GETDOWN = 0,  // 放松趴下
    MODE_SIT,                  // 坐下
    MODE_UPRIGHT,              // 站立
    MODE_GETDOWN,              // 趴下
    MODE_ADVANCE,              // 前进
    MODE_BACK,                 // 后退
    MODE_LROTATION,            // 左转
    MODE_RROTATION,            // 右转
    MODE_SWING,                // 摇摆
    MODE_SWING_TAIL,           // 摇尾巴
    MODE_JUMP_U,               // 向前跳
    MODE_JUMP_D,               // 向后跳
    MODE_HELLO,                // 打招呼
    MODE_STRETCH,              // 伸懒腰
    MODE_LSTRETCH,             // 后腿拉伸
    MODE_MAX                   // 模式总数（用于边界检查）
} ActionMode;

/* SZTU OSHA  Prototypes------------------------------------------------------*/
void Face_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __Face_Config__ */

