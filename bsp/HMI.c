#include "HMI.h"
#include "dog.h"

volatile uint8_t HMI_RxFlag = 0;
#define TX_BUFFER_SIZE 256         // 发送缓冲区大小
uint8_t TxBuffer[TX_BUFFER_SIZE] = {0x00};
uint8_t RxBuffer[HMI_DMA_LENGTH] = {0x00};
uint8_t RxCount = 0;


extern UART_HandleTypeDef HMI_UART;
extern DMA_HandleTypeDef HMI_DMA;

//                              采用JavaDoc 风格注释，同时满足 Doxygen 自动生成文档的格式规范，便于团队协作和后期维护。
//分层结构：使用@brief（简要描述）+@details（详细说明）的两级结构
//参数规范：
////每个参数单独说明
////明确取值范围和限制条件
////指出特殊处理逻辑（如自动截断、精度转换）
//错误处理：
////提示潜在风险（缓冲区溢出、显示异常）
////说明依赖条件（HMI 配置要求）
//代码示例：
////每个函数提供典型使用示例
////包含参数边界值测试
//交叉引用：
////使用@see关联相关函数和变量
////指出函数间依赖关系
/**
  * @brief  初始化HMI通信接口
  * @details 
  *   配置HMI串口通信参数并启动DMA接收
  *   - 使能UART空闲中断用于检测消息结束
  *   - 配置DMA循环模式接收数据
  *   - 缓冲区溢出处理依赖于应用层实现
  * 
  * @note 
  *   1. 需确保HMI_UART已正确初始化
  *   2. RxBuffer必须足够大以容纳最长的HMI响应
  *   3. 建议在系统初始化阶段调用此函数
  * 
  * @retval 无
  * 
  * @see HMI_UART_HandleTypeDef
  * @see RxBuffer
  * @see HMI_DMA_LENGTH
  */
void HMI_Init(void)
{
  // 使能空闲中断
  __HAL_UART_ENABLE_IT(&HMI_UART, UART_IT_IDLE);
  // 开启接收DMA
  HAL_UART_Receive_DMA(&HMI_UART, (uint8_t *)RxBuffer, HMI_DMA_LENGTH);
}


/**
 * @brief 发送十六进制数值到UART
 * @param data 要发送的十六进制数据数组
 * @param length 数据长度（字节数）
 * @return HAL_StatusTypeDef 发送结果
 */
HAL_StatusTypeDef HMI_send_hex_values(const uint8_t *data, size_t length)
{
    // 参数校验
    if (data == NULL || length == 0)
    {
        return HAL_ERROR;
    }
    
    // 检查缓冲区是否足够
    if (length > TX_BUFFER_SIZE)
    {
        return HAL_ERROR;
    }
    
    // 将数据复制到发送缓冲区
    for (size_t i = 0; i < length; i++)
    {
        TxBuffer[i] = data[i];
    }
    
    // 发送数据
    return HAL_UART_Transmit(&huart2, TxBuffer, length, 100);
}
// 示例用法
//void example_usage(void)
//{
//    // 要发送的十六进制数值
//    uint8_t hex_data[] = {0x01, 0xAB, 0x3C, 0xFF, 0x00};
//    size_t data_length = sizeof(hex_data) / sizeof(hex_data[0]);
//    
//    // 发送十六进制数值
//    HAL_StatusTypeDef status = send_hex_values(hex_data, data_length);
//    
//    // 检查发送状态
//    if (status != HAL_OK)
//    {
//        // 处理发送错误
//    }
//}

/**
  * @brief  处理从HMI接收的数据
  * @details 
  *   检查接收标志位，处理HMI发送过来的指令
  *   支持两种指令格式:
  *   1. 2字节指令: [包头][指令码]
  *   2. 长度>3的指令: 目前作为错误处理
  * 
  * @note 
  *   1. 需在USART2空闲中断中置位RxFlag
  *   2. 处理完成后自动重启DMA接收
  *   3. 调试模式下会打印接收数据的详细信息
  * 
  * @retval 无
  * 
  * @see USART2_IRQHandler()
  * @see HMI_TO_STM32_HEADER
  */
void HMI_process_information(void)
{
  // 在USART2_IRQHandler中空闲中断时，置位RxFlag为1
  if(HMI_RxFlag)
  {
    HMI_RxFlag = 0;
    HAL_UART_DMAStop(&HMI_UART);
    RxCount = HMI_DMA_LENGTH - __HAL_DMA_GET_COUNTER(&HMI_DMA);
    
    #if DEBUG_HMI_Printf == 1
    printf("<HMI UART2 Rx Flag!>\n");
    #endif
    uart_to_51();
    // 根据接收数据长度处理不同类型的指令
    if(RxCount == 2)
    {
      
      // 首字节可以用做包头校验，可以填宏 ：HMI_TO_STM32_HEADER
      if(RxBuffer[0] == HMI_TO_STM32_HEADER)
      {
        
      }
      else if(RxBuffer[0] == HMI_TO_STM32_HEADER &&  RxBuffer[1] == 0x01)
      {
      }
    }
    else if(RxCount > 3)
    {
      printf("Instruction error!\n");
    }
    
    #if DEBUG_HMI_Printf == 1
    // 打印接收到的指令，留着下面代码提供调试信息
    for(uint8_t i = 0; i<RxCount; i++)
    {
      if((i%4) == 0 && i != 0) {         // 每四个换一次行
        printf("\n");
      }
      printf("%02X ",RxBuffer[i]);      // %02X：输出两位十六进制数，位数不足时前面补 0
    }
    printf("\n");
    #endif
    
    // 重启DMA继续接收数据
    HAL_UART_Receive_DMA(&HMI_UART, (uint8_t *)RxBuffer, HMI_DMA_LENGTH);
  }
}






