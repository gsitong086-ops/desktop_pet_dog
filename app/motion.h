#ifndef __MOTION_H
#define __MOTION_H

#include <stdint.h>

// 运动状态枚举
typedef enum {
    MOTION_STOP,
    MOTION_RUNNING,
    MOTION_ERROR
} Motion_StateTypeDef;

// 姿态结构体：存储四个舵机的角度(-90~90度)
// 前腿为servo1、servo2，后腿为servo3、servo4
typedef struct {
    int16_t servo1;  // 第一个舵机角度
    int16_t servo2;  // 第二个舵机角度
    int16_t servo3;  // 第三个舵机角度
    int16_t servo4;  // 第四个舵机角度
} Robot_PoseTypeDef;

// 姿态帧结构体：包含姿态和持续时间(ms)
typedef struct {
    Robot_PoseTypeDef pose;      // 姿态
    uint32_t duration;           // 持续时间(毫秒)
} Robot_PoseFrameTypeDef;

// 机器人姿态类型枚举
typedef enum {
    POSE_STAND,             // 站立姿态：四肢自然支撑，身体水平
    POSE_SIT,               // 坐姿姿态
    POSE_LIE_DOWN,          // 趴下姿态：四肢伸展，身体贴近地面（紧凑）
    POSE_RELAX_LIE_DOWN     // 放松趴下姿态：四肢微曲，身体较放松
} Motion_PoseTypeDef;

// 运动方向枚举
typedef enum {
    FORWARD,        // 前进
    BACKWARD,       // 后退
    ROTATE_LEFT,    // 向左旋转
    ROTATE_RIGHT,   // 向右旋转
    JUMP_FORWARD,   // 向前跳
    JUMP_BACKWARD   // 向后跳
} Motion_DirectionTypeDef;

// 动作类型枚举
typedef enum {
    ACTION_RAISE_FRONT, // 抬起前腿
    ACTION_RAISE_REAR,  // 抬起后腿
    ACTION_SWING,       // 摇摆
    ACTION_GREETING,    // 打招呼 
    ACTION_WAVE_HAND    // 摇手
} Motion_ActionTypeDef;

// 运动控制结构体
typedef struct {
    Motion_StateTypeDef state;              // 当前运动状态
    uint8_t current_speed;                  // 当前速度(0-100)
    Robot_PoseTypeDef current_pose;         // 当前姿态
    Robot_PoseTypeDef target_pose;          // 目标姿态
} Motion_HandleTypeDef;

// 函数声明
void motion_init(Motion_HandleTypeDef *hMotion);
void motion_set_speed(Motion_HandleTypeDef *hMotion, uint8_t speed);
void motion_set_pose(Motion_HandleTypeDef *hMotion, Motion_PoseTypeDef pose);
void motion_move(Motion_HandleTypeDef *hMotion, Motion_DirectionTypeDef direction, uint16_t steps);
void motion_perform_action(Motion_HandleTypeDef *hMotion, Motion_ActionTypeDef action);
void motion_stop(Motion_HandleTypeDef *hMotion);

#endif /* __MOTION_H */

