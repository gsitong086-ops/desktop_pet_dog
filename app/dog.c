#include "dog.h"
#include "OLED.h"
#include "motion.h"
#include "face.h"
#include "servo.h"

extern uint8_t RxBuffer[100];
extern Face_HandleTypeDef hface;
extern Motion_HandleTypeDef hMotion;


extern uint16_t time_cnt;

void dog_Init(void)
{
  
}
void uart_to_51(void)
{
    // 根据串口接收的指令字节进行分支处理
    switch(RxBuffer[0])
    {
        case 0x29:  // 躺平
            
            OLED_ShowImage(0, 0, 128, 64, BMP1);
            OLED_Update();
            motion_set_pose(&hMotion, POSE_RELAX_LIE_DOWN);
            break;
            
        case 0x30:  // 蹲下
            
            OLED_ShowImage(0, 0, 128, 64, BMP1);
            OLED_Update();
            motion_set_pose(&hMotion, POSE_SIT);
            break;
            
        case 0x31:  // 起立
            
            OLED_ShowImage(0, 0, 128, 64, BMP1);
            OLED_Update();
            motion_set_pose(&hMotion, POSE_STAND);
            break;
             
        case 0x32:  // 趴下
            
            OLED_ShowImage(0, 0, 128, 64, BMP1);
            OLED_Update();
            motion_set_pose(&hMotion, POSE_LIE_DOWN);
            break;
            
        case 0x33:  // 前进
            
            OLED_ShowImage(0, 0, 128, 64, BMP2);
            OLED_Update();
            motion_move(&hMotion, FORWARD, 2);
            break;
        case 0x34:  // 后退
           
            OLED_ShowImage(0, 0, 128, 64, BMP2);
            OLED_Update();
            motion_move(&hMotion, BACKWARD, 2);
            break;
        case 0x35:  // 左转
            OLED_ShowImage(0, 0, 128, 64, BMP3);
            OLED_Update();
            motion_move(&hMotion, ROTATE_LEFT, 2);
            break;
        case 0x36:  // 右转
            
            OLED_ShowImage(0, 0, 128, 64, BMP4);
            OLED_Update();
            motion_move(&hMotion, ROTATE_RIGHT, 2);
            break;
        case 0x41:  // 向前跳
            OLED_ShowImage(0, 0, 128, 64, BMP2);
            OLED_Update();
            motion_move(&hMotion, JUMP_FORWARD, 1);  // 向前跳
            break;
        case 0x42:  // 向后跳
            OLED_ShowImage(0, 0, 128, 64, BMP2);
            OLED_Update();
            motion_move(&hMotion, JUMP_BACKWARD, 1); // 向后跳
            break;
        case 0x37:  // 摇摆
            
            OLED_ShowImage(0, 0, 128, 64, BMPaa);
            OLED_Update();
            motion_perform_action(&hMotion, ACTION_SWING);
            break;
            
        case 0x38:  // 减少移动延迟，增加移动速度
            motion_set_speed(&hMotion, 90);
            break;
            
        case 0x39:  //摇手
            OLED_ShowImage(0, 0, 128, 64, BMPaa);
            OLED_Update();
            motion_perform_action(&hMotion, ACTION_WAVE_HAND);
            break;
            
        case 0x43:  // 打招呼
            
            OLED_ShowImage(0, 0, 128, 64, BMPbb);
            OLED_Update();
            motion_perform_action(&hMotion, ACTION_GREETING);
            break;
        case 0x44:  // 前进五步
            
            OLED_ShowImage(0, 0, 128, 64, BMP1);
            OLED_Update();
            motion_move(&hMotion, FORWARD, 5);
            break;
        case 0xAA:  // 矫正
            OLED_Clear();
            OLED_ShowImage(4, 0, 55, 64, CantabImg);
            OLED_ShowImage(70, 0, 53, 63, SZTUImg1);
            OLED_Update();
            time_cnt = 0;
            servo_adjust();
            break;
        case 0xBB:  // 矫正完成
            motion_perform_action(&hMotion, ACTION_SWING);
            
            break;
        case 0xCC: 
            OLED_Clear();
            OLED_ShowImage(22, 0, 86, 64, OSHAImg);
            OLED_Update();
            time_cnt = 0;
            motion_set_pose(&hMotion, POSE_SIT);
            break;
        
        // 以下为注释掉的预留功能，保留结构便于后续扩展
        /*
        case 0x44:  // 开启灯光
            AllLed = 1;
            break;
            
        case 0x45:  // 关闭灯光
            AllLed = 0;
            break;
            
        case 0x46:  // 开启呼吸灯
            BreatheLed = 1;
            break;
            
        case 0x47:  // 关闭呼吸灯
            BreatheLed = 0;
            break;
        */
            
        default:    // 未知指令，不执行操作
            return;
    }
}


