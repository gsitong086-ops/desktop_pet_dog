
/**
  ******************************************************************************
  * @file    face.h
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   面部表情显示模块：负责OLED屏幕表情图像的初始化、切换与管理
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * - 依赖OLED模块正常工作，需确保OLED_Init()已提前调用
  * - 图像数据（如Face_sleep）需在OLED.h中正确定义
  * - 模块使用TIM6定时器实现表情动画刷新（可选功能）
  ******************************************************************************
  * @CubeMXConfiguration
  * - 引脚分配：
  *   - GPIOB_Pin8: SCL 上拉，开漏
  *   - GPIOB_Pin9: SDA 上拉，开漏
  * 
  * 注意： | Note:
  * 1. IIC通信，SCL和SDA都是上拉，开漏
  ******************************************************************************
  * @usage
  * 1. 初始化: Face_Init() 
  * 2. 设置参数: Face_SetMode(FACE_HAPPY) 
  * 3. 执行操作: Face_GetMode() 
  * 
  * 注意： | Note:
  * 1. 调用Face_SetMode()前必须确保Face_Init()已执行成功
  * 2. 表情切换频率不宜超过50Hz，避免OLED刷新冲突
  ******************************************************************************
  * @compatibility
  * - STM32CubeMX v6.8.1+
  * - HAL库 v1.18.0+ 
  * 
  * 依赖： | Dependencies:
  * - stm32fxx_hal.h
  * - main.h
  * - OLED.h
  ******************************************************************************
  */

/* 防止递归包含保护 | Prevent recursive inclusion */
#ifndef __FACE_H__
#define __FACE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* 包含头文件   | Includes ----------------------------------------------------*/
#include "main.h"

/* 宏定义       | Macros ------------------------------------------------------*/
#define FACE_MODULE_VERSION    "1.0.0"  // 模块版本号
#define FACE_OLED_WIDTH        128      // 表情图像宽度（像素）
#define FACE_OLED_HEIGHT       64       // 表情图像高度（像素）
#define FACE_DEFAULT_MODE      FACE_SLEEP  // 上电默认表情

/* 状态码定义   | Status code definitions -------------------------------------*/
// 与HAL库状态码兼容（便于混合使用）
#define FACE_OK                HAL_OK         // 操作成功
#define FACE_ERROR             HAL_ERROR      // 操作失败
#define FACE_BUSY              HAL_BUSY       // 模块忙
#define FACE_TIMEOUT           HAL_TIMEOUT    // 操作超时
#define FACE_INVALID_PARAM     0x04U          // 参数无效（自定义状态码）

/* 类型定义     | Types -------------------------------------------------------*/
/**
 * @brief 面部表情模式枚举
 * @note 新增表情需在此处扩展，并在face.c的映射表中添加对应图像
 */
typedef enum {
    FACE_SLEEP = 0,        // 睡觉表情（闭眼状态）
    FACE_STARE,            // 瞪大眼表情（惊讶状态）
    FACE_HAPPY,            // 快乐表情（微笑状态）
    FACE_MANIA,            // 狂热表情（兴奋状态）
    FACE_VERY_HAPPY,       // 非常快乐表情（大笑状态）
    FACE_EYES,             // 眼睛表情（正常注视状态）
    FACE_HELLO,            // 打招呼表情（挥手状态）
    FACE_MAX               // 表情模式总数（用于边界检查）
} FaceMode;

/**
 * @brief 面部表情模块句柄结构
 */
typedef struct {
    FaceMode current_mode;      // 当前表情模式
    uint8_t is_initialized;     // 初始化状态标志
    TIM_HandleTypeDef *htim;    // 定时器句柄指针
} Face_HandleTypeDef;

/* 公有函数声明 | Public function prototypes ----------------------------------*/
/**
  * @brief  初始化面部表情模块
  * @param  hface: 面部表情模块句柄指针
  * @param  htim: 定时器句柄指针（用于动画刷新）
  * @retval FACE_OK: 初始化成功; FACE_ERROR: 初始化失败
  */
uint8_t Face_Init(Face_HandleTypeDef *hface, TIM_HandleTypeDef *htim);

/**
  * @brief  设置并显示指定的面部表情
  * @param  hface: 面部表情模块句柄指针
  * @param  mode: 目标表情模式（取值范围：0 ~ FACE_MAX-1）
  * @retval FACE_OK: 设置成功; FACE_INVALID_PARAM: 无效模式; FACE_ERROR: OLED操作失败
  */
uint8_t Face_SetMode(Face_HandleTypeDef *hface, FaceMode mode);

/**
  * @brief  获取当前显示的表情模式
  * @param  hface: 面部表情模块句柄指针
  * @retval 当前表情模式（FaceMode枚举值）
  */
FaceMode Face_GetMode(Face_HandleTypeDef *hface);

#ifdef __cplusplus
}
#endif

#endif /* __FACE_H__ */




