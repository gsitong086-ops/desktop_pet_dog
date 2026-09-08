/**
  ******************************************************************************
  * @file    face.c
  * @author  SZTU OSHA 深圳技术大学开源硬件社 [YourName]
  * @brief   面部表情显示模块实现：通过OLED驱动接口完成表情图像的存储、切换与屏幕刷新，支持多表情模式管理
  * @version 1.0.0
  * @date    [YYYY-MM-DD]
  * @license GPL-3.0-only
  ******************************************************************************
  * @attention
  * - 所有表情图像数据需在OLED.h中预定义，格式为128×64像素的点阵数据
  * - 定时器中断服务程序中调用表情刷新函数时需加锁保护，避免资源冲突
  ******************************************************************************
  * @implementation
  * - 实现原理: 通过枚举类型定义表情模式，使用映射表关联模式与图像数据，通过OLED驱动接口完成图像显示；内部维护状态标志确保操作合法性
  * - 算法描述: 采用状态机管理当前表情模式，通过模式有效性检查、重复刷新过滤等逻辑优化显示效率；刷新流程为"模式验证→数据查找→屏幕刷新→状态更新"
  * 
  * 注意： | Note:
  * 1. 新增表情时需同时扩展FaceMode枚举、映射表及对应的图像数据，三者需保持一致性
  * 2. 若需支持动画表情，可在映射表中增加帧序列数据，通过定时器实现帧切换
  ******************************************************************************
  */

/* 包含头文件   | Includes ---------------------------------------------------*/
#include "face.h"
#include "OLED.h"


/* 私有宏定义   | Private macros ---------------------------------------------*/
#define FACE_IMAGE_COUNT  (sizeof(s_face_image_maps) / sizeof(FaceImageMap))  // 表情图像总数

/* 私有类型定义 | Private types ----------------------------------------------*/
/**
 * @brief 表情模式与图像数据的映射结构
 * @note 用于关联表情枚举值与实际点阵数据，仅模块内部可见
 */
typedef struct {
    FaceMode mode;               // 表情模式枚举值
    const uint8_t* image_data;   // 对应表情的OLED点阵数据（需与屏幕尺寸匹配）
} FaceImageMap;

/* 模块私有变量 | Private variables ------------------------------------------*/
static Face_HandleTypeDef* s_face_handle = NULL;  // 模块句柄指针（全局唯一实例）
static const uint8_t* s_null_image = NULL;         // 空图像指针（用于错误处理）

/**
 * @brief 表情模式-图像映射表
 * @note 新增表情需在此处添加映射关系，顺序需与FaceMode枚举一致
 */
static const FaceImageMap s_face_image_maps[] = {
    {FACE_SLEEP,       Face_sleep},       // 睡觉表情（闭眼状态）
    {FACE_STARE,       Face_stare},       // 瞪大眼表情（惊讶状态）
    {FACE_HAPPY,       Face_happy},       // 快乐表情（微笑状态）
    {FACE_MANIA,       Face_mania},       // 狂热表情（兴奋状态）
    {FACE_VERY_HAPPY,  Face_very_happy},  // 非常快乐表情（大笑状态）
    {FACE_EYES,        Face_eyes},        // 眼睛表情（正常注视状态）
    {FACE_HELLO,       Face_hello}        // 打招呼表情（挥手状态）
};

/* 私有函数原型 | Private function prototypes --------------------------------*/
/**
 * @brief 检查表情模式是否在有效范围内
 * @param mode：待检查的表情模式
 * @retval FACE_OK-有效，FACE_INVALID_PARAM-无效
 */
static uint8_t face_check_mode_valid(FaceMode mode);

/**
 * @brief 根据表情模式查找对应的图像数据
 * @param mode：目标表情模式
 * @retval 图像数据指针（NULL表示查找失败）
 */
static const uint8_t* face_find_image(FaceMode mode);

/* 公有函数实现 | Public functions -------------------------------------------*/
/**
  * @brief  初始化面部表情模块
  * @param  hface: 面部表情模块句柄指针
  * @param  htim: 定时器句柄指针（用于动画刷新，可为NULL）
  * @retval FACE_OK: 初始化成功; FACE_ERROR: 初始化失败
  */
uint8_t Face_Init(Face_HandleTypeDef *hface, TIM_HandleTypeDef *htim) {
    // 参数合法性检查
    if (hface == NULL) {
        return FACE_INVALID_PARAM;
    }
    
//    // 检查OLED模块状态
//    if (OLED_GetStatus() != OLED_STATUS_INITIALIZED) {
//        return FACE_ERROR;
//    }
    OLED_Init();
    
    // 初始化句柄成员
    hface->current_mode = FACE_DEFAULT_MODE;
    hface->is_initialized = 1;
    hface->htim = htim;
    
    // 保存全局句柄
    s_face_handle = hface;
    
    // 显示默认表情
    return Face_SetMode(hface, FACE_DEFAULT_MODE);
}

/**
  * @brief  设置并显示指定的面部表情
  * @param  hface: 面部表情模块句柄指针
  * @param  mode: 目标表情模式（取值范围：0 ~ FACE_MAX-1）
  * @retval FACE_OK: 设置成功; FACE_INVALID_PARAM: 无效模式; FACE_ERROR: OLED操作失败
  */
uint8_t Face_SetMode(Face_HandleTypeDef *hface, FaceMode mode) {
    // 模块状态检查
    if (hface == NULL || hface->is_initialized == 0) {
        return FACE_INVALID_PARAM;
    }
    
    // 模式有效性检查
    if (face_check_mode_valid(mode) != FACE_OK) {
        return FACE_INVALID_PARAM;
    }
    
    // 过滤重复刷新
    if (mode == hface->current_mode) {
        return FACE_OK;
    }
    
    // 查找图像数据并刷新显示
    const uint8_t* image_data = face_find_image(mode);
    if (image_data == s_null_image) {
        return FACE_ERROR;
    }
    
    // 调用OLED驱动接口刷新屏幕
    OLED_Clear();
    OLED_ShowImage(0, 0, FACE_OLED_WIDTH, FACE_OLED_HEIGHT, image_data);
    OLED_Update();
    
    // 更新当前模式
    hface->current_mode = mode;
    return FACE_OK;
}

/**
  * @brief  获取当前显示的表情模式
  * @param  hface: 面部表情模块句柄指针
  * @retval 当前表情模式（FaceMode枚举值）
  */
FaceMode Face_GetMode(Face_HandleTypeDef *hface) {
    // 模块状态检查
    if (hface == NULL || hface->is_initialized == 0) {
        return FACE_MAX;  // 无效模式标记
    }
    return hface->current_mode;
}

/* 私有函数实现 | Private functions ------------------------------------------*/
static uint8_t face_check_mode_valid(FaceMode mode) {
    return (mode >= 0 && mode < FACE_MAX) ? FACE_OK : FACE_INVALID_PARAM;
}

static const uint8_t* face_find_image(FaceMode mode) {
    // 遍历映射表查找对应图像
    for (uint8_t i = 0; i < FACE_IMAGE_COUNT; i++) {
        if (s_face_image_maps[i].mode == mode) {
            return s_face_image_maps[i].image_data;
        }
    }
    return s_null_image;
}

/**
  * @brief  定时器中断服务程序中的表情刷新回调（可选功能）
  * @note   若启用动画表情，可在此函数中实现帧切换逻辑
  */
void Face_TimerCallback(void) {
    // 检查全局句柄
    if (s_face_handle == NULL || s_face_handle->htim == NULL) {
        return;
    }
    
    // 动画逻辑示例（需根据实际需求扩展）
    /*
    static uint8_t frame_count = 0;
    if (++frame_count >= ANIMATION_FRAME_INTERVAL) {
        frame_count = 0;
        // 切换到下一帧表情
        FaceMode next_mode = (s_face_handle->current_mode + 1) % FACE_MAX;
        Face_SetMode(s_face_handle, next_mode);
    }
    */
}

