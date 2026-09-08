#include "motion.h"
#include "servo.h"
#include "stm32f1xx_hal.h"  // 根据实际使用的MCU头文件修改

// 定义姿态帧序列的最大长度
#define MAX_FRAME_COUNT 50

// 插值计算两个姿态之间的中间姿态
static void interpolate_pose(const Robot_PoseTypeDef *start, const Robot_PoseTypeDef *end, 
                            Robot_PoseTypeDef *result, float ratio) {
    result->servo1 = start->servo1 + (int16_t)((end->servo1 - start->servo1) * ratio);
    result->servo2 = start->servo2 + (int16_t)((end->servo2 - start->servo2) * ratio);
    result->servo3 = start->servo3 + (int16_t)((end->servo3 - start->servo3) * ratio);
    result->servo4 = start->servo4 + (int16_t)((end->servo4 - start->servo4) * ratio);
}

// 从当前姿态平滑过渡到目标姿态
static void transition_to_pose(Motion_HandleTypeDef *hMotion, const Robot_PoseTypeDef *target, uint32_t duration) {
    if (hMotion->state == MOTION_ERROR) return;
    
    hMotion->target_pose = *target;
    hMotion->state = MOTION_RUNNING;
    
    // 根据速度调整过渡时间，速度越快，时间越短
    uint32_t actual_duration = duration * (100 - hMotion->current_speed) / 100;
    if (actual_duration < 100) actual_duration = 100;  // 最小过渡时间
    
    uint32_t start_time = HAL_GetTick();  // 使用HAL库函数获取系统时间
    uint32_t elapsed;
    float ratio;
    
    Robot_PoseTypeDef current_intermediate;
    
    // 堵塞式过渡
    while ((elapsed = HAL_GetTick() - start_time) < actual_duration) {
        ratio = (float)elapsed / actual_duration;
        interpolate_pose(&hMotion->current_pose, &hMotion->target_pose, &current_intermediate, ratio);
        
        // 设置舵机角度，注意将角度转换为适合舵机的范围(-90~90)
        servo_set_angles(
            current_intermediate.servo1 ,
            current_intermediate.servo2 ,
            current_intermediate.servo3 ,
            current_intermediate.servo4 
        );
        
        HAL_Delay(20);  // 使用HAL库延时函数
    }
    
    // 确保最终达到目标姿态
    hMotion->current_pose = *target;
    servo_set_angles(
        target->servo1 ,
        target->servo2 ,
        target->servo3 ,
        target->servo4 
    );
    
    hMotion->state = MOTION_STOP;
}

// 执行姿态帧序列
static void execute_pose_frames(Motion_HandleTypeDef *hMotion, const Robot_PoseFrameTypeDef *frames, uint8_t frame_count) {
    if (hMotion->state == MOTION_ERROR || frame_count == 0) return;
    
    hMotion->state = MOTION_RUNNING;
    
    // 依次执行每个姿态帧
    for (uint8_t i = 0; i < frame_count; i++) {
        // 过渡到当前帧的姿态
        transition_to_pose(hMotion, &frames[i].pose, frames[i].duration / 2);
        
        // 保持当前姿态剩余时间
        HAL_Delay(frames[i].duration / 2);
    }
    
    hMotion->state = MOTION_STOP;
}

// 姿态定义：根据实际机器人结构调整这些角度值
static const Robot_PoseTypeDef pose_stand = {0, 0, 0, 0};                  // 站立姿态
static const Robot_PoseTypeDef pose_sit = {0, 0, 60, 60};                // 坐姿姿态
static const Robot_PoseTypeDef pose_lie_down = {80, 80, 80, 80};        // 趴下姿态
static const Robot_PoseTypeDef pose_relax_lie_down = {60, 60, 60, 60};  // 放松趴下姿态

// 初始化运动控制器
void motion_init(Motion_HandleTypeDef *hMotion) {
    if (hMotion == NULL) return;
    
    hMotion->state = MOTION_STOP;
    hMotion->current_speed = 50;  // 中等初始速度
    hMotion->current_pose = pose_stand;
    hMotion->target_pose = pose_stand;
    
    // 初始化为站立姿态
    servo_set_angles(90, 90, 90, 90);  // 0度对应舵机90度
}

// 设置运动速度
void motion_set_speed(Motion_HandleTypeDef *hMotion, uint8_t speed) {
    if (hMotion == NULL) return;
    
    // 限制速度范围在0-100
    if (speed > 100) speed = 100;
    hMotion->current_speed = speed;
}

// 设置机器人姿态
void motion_set_pose(Motion_HandleTypeDef *hMotion, Motion_PoseTypeDef pose) {
    if (hMotion == NULL) return;
    
    const Robot_PoseTypeDef *target_pose;
    
    // 根据姿态名称选择目标姿态
    switch (pose) {
        case POSE_STAND:
            target_pose = &pose_stand;
            break;
        case POSE_SIT:
            target_pose = &pose_sit;
            break;
        case POSE_LIE_DOWN:
            target_pose = &pose_lie_down;
            break;
        case POSE_RELAX_LIE_DOWN:
            target_pose = &pose_relax_lie_down;
            break;
        default:
            target_pose = &pose_stand;
            break;
    }
    
    // 过渡到目标姿态，基础时间500ms，会根据速度调整
    transition_to_pose(hMotion, target_pose, 500);
}

// 移动运动实现
void motion_move(Motion_HandleTypeDef *hMotion, Motion_DirectionTypeDef direction, uint16_t steps) {
    if (hMotion == NULL || steps == 0) return;
    
    // 定义步态帧序列，根据不同方向使用不同的帧
    Robot_PoseFrameTypeDef frames[MAX_FRAME_COUNT];
    uint8_t frame_count = 0;
    
    // 根据运动方向生成不同的步态帧
    switch (direction) {
        case FORWARD:
            // 前进步态帧定义（8帧）
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 30, 30, 0}, 50};  // 轻微迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 30, 30, -30}, 50};  // 进一步迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 0, 0, -30}, 50};  // 最大迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 50};  // 收回一点
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{30, 0, 0, 30}, 50};  // 后腿前迈
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{30, -30, -30, 30}, 50};  // 后腿进一步前迈
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, -30, -30, 0}, 50};  // 后腿最大迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 50};  // 后腿收回一点
            break;
            
        case BACKWARD:
            // 后退步态帧定义
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, -30, -30, 0}, 50};  // 轻微迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{30, -30, -30, 30}, 50};  // 进一步迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{30, 0, 0, 30}, 50};  // 最大迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 50};  // 收回一点
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 0, 0, -30}, 50};  // 后腿前迈
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 30, 30, -30}, 50};  // 后腿进一步前迈
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 30, 30, 0}, 50};  // 后腿最大迈步
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 50};  // 后腿收回一点
            break;
            
        case ROTATE_LEFT:
            // 向左旋转步态帧定义
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 0, 0, 30}, 200};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, -30, 30, 30}, 200};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, -30, 30, 0}, 200};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 200};
            break;
            
        case ROTATE_RIGHT:
            // 向右旋转步态帧定义
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, -30, 30, 0}, 200};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, -30, 30, 30}, 200};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 0, 0, 30}, 200};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 200};
            break;
            
        case JUMP_FORWARD:
            // 向前跳帧定义
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{45, 45, 45, 45}, 100};  // 蹲下
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-60, -60, -60, -60}, 1000};  // 伸展跳跃
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 100};  // 回到站立
            steps = 1;  // 跳跃动作只执行一次
            break;
            
        case JUMP_BACKWARD:
            // 向后跳帧定义
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{45, 45, 45, 45}, 100};  // 蹲下
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{60, 60, 60, 60}, 200};  // 伸展跳跃
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 100};  // 回到站立
            steps = 1;  // 跳跃动作只执行一次
            break;
            
        default:
            return;
    }
    
    // 执行步态序列，重复steps次
    for (uint16_t i = 0; i < steps; i++) {
        execute_pose_frames(hMotion, frames, frame_count);
    }
}

// 执行特定动作
void motion_perform_action(Motion_HandleTypeDef *hMotion, Motion_ActionTypeDef action) {
    if (hMotion == NULL) return;
    
    // 定义动作帧序列
    Robot_PoseFrameTypeDef frames[MAX_FRAME_COUNT];
    uint8_t frame_count = 0;
    
    // 根据动作类型生成不同的帧序列
    switch (action) {
        case ACTION_RAISE_FRONT:
            // 抬起前腿动作
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{60, 60, 0, 0}, 500};  // 抬起前腿
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 500};    // 放下前腿
            break;
            
        case ACTION_RAISE_REAR:
            // 抬起后腿动作
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 60, 60}, 500};  // 抬起后腿
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 500};    // 放下后腿
            break;
            
        case ACTION_SWING:
            // 摇摆动作
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{30, -30, 30, -30}, 300};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 30, -30, 30}, 300};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{30, -30, 30, -30}, 300};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{-30, 30, -30, 30}, 300};
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 300};
            break;
            
        case ACTION_GREETING:
            // 打招呼动作
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 80, 0, 0}, 500};   // 抬起右前腿
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 50, 0, 0}, 300};   // 放下一点
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 80, 0, 0}, 300};   // 再抬起
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 500};    // 放下
            break;
            
        case ACTION_WAVE_HAND:
            // 摇手动作
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{80, 80, 80, 80}, 500};   // 抬起
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{60, 90, 80, 80}, 300};  // 向右
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{90, 60, 80, 80}, 300}; // 向左
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{60, 90, 80, 80}, 300};  // 向右
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{90, 60, 80, 80}, 300}; // 向左
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{60, 90, 80, 80}, 300};  // 向右
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{90, 60, 80, 80}, 800}; // 向左
            frames[frame_count++] = (Robot_PoseFrameTypeDef){{0, 0, 0, 0}, 300};    // 放下
            break;
            
        default:
            return;
    }
    
    // 执行动作帧序列
    execute_pose_frames(hMotion, frames, frame_count);
}

// 停止当前运动
void motion_stop(Motion_HandleTypeDef *hMotion) {
    if (hMotion == NULL) return;
    
    hMotion->state = MOTION_STOP;
    // 回到站立姿态
    motion_set_pose(hMotion, POSE_STAND);
}

