#include "MPU6050.h"

void I2C_WriteByte(uint8_t reg, uint8_t data)
{
  HAL_I2C_Mem_Write(&hi2c2, MPU_6050_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
  HAL_Delay(5);
}
/* USER CODE END 1 */
uint8_t I2C_ReadByte(uint8_t reg)
{
  uint8_t data;
  HAL_I2C_Mem_Read(&hi2c2, MPU_6050_ADDRESS, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 100);
  HAL_Delay(5);
  return data;
}
void MPU_6050_Init(void)
{
  I2C_WriteByte(MPU6050_PWR_MGMT_1, 0x01);   // 电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
  I2C_WriteByte(MPU6050_PWR_MGMT_2, 0x00);   // 电源管理寄存器2，保持默认值0，所有轴均不待机
  I2C_WriteByte(MPU6050_SMPLRT_DIV, 0x09);   // 采样率分频寄存器，配置采样率
  I2C_WriteByte(MPU6050_CONFIG, 0x06);       // 配置寄存器，配置DLPF
  I2C_WriteByte(MPU6050_GYRO_CONFIG, 0x18);  // 陀螺仪配置寄存器，选择满量程为±2000°/s
  I2C_WriteByte(MPU6050_ACCEL_CONFIG, 0x18); // 加速度计配置寄存器，选择满量程为±16g
}
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ,
                     int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ)
{
  uint8_t DataH, DataL; // 定义数据高8位和低8位的变量

  DataH = I2C_ReadByte(MPU6050_ACCEL_XOUT_H); // 读取加速度计X轴的高8位数据
  DataL = I2C_ReadByte(MPU6050_ACCEL_XOUT_L); // 读取加速度计X轴的低8位数据
  *AccX = (DataH << 8) | DataL;                  // 数据拼接，通过输出参数返回

  DataH = I2C_ReadByte(MPU6050_ACCEL_YOUT_H); // 读取加速度计Y轴的高8位数据
  DataL = I2C_ReadByte(MPU6050_ACCEL_YOUT_L); // 读取加速度计Y轴的低8位数据
  *AccY = (DataH << 8) | DataL;                  // 数据拼接，通过输出参数返回

  DataH = I2C_ReadByte(MPU6050_ACCEL_ZOUT_H); // 读取加速度计Z轴的高8位数据
  DataL = I2C_ReadByte(MPU6050_ACCEL_ZOUT_L); // 读取加速度计Z轴的低8位数据
  *AccZ = (DataH << 8) | DataL;                  // 数据拼接，通过输出参数返回

  DataH = I2C_ReadByte(MPU6050_GYRO_XOUT_H); // 读取陀螺仪X轴的高8位数据
  DataL = I2C_ReadByte(MPU6050_GYRO_XOUT_L); // 读取陀螺仪X轴的低8位数据
  *GyroX = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回

  DataH = I2C_ReadByte(MPU6050_GYRO_YOUT_H); // 读取陀螺仪Y轴的高8位数据
  DataL = I2C_ReadByte(MPU6050_GYRO_YOUT_L); // 读取陀螺仪Y轴的低8位数据
  *GyroY = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回

  DataH = I2C_ReadByte(MPU6050_GYRO_ZOUT_H); // 读取陀螺仪Z轴的高8位数据
  DataL = I2C_ReadByte(MPU6050_GYRO_ZOUT_L); // 读取陀螺仪Z轴的低8位数据
  *GyroZ = (DataH << 8) | DataL;                // 数据拼接，通过输出参数返回
}
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float MPU6050_GetPitch(int16_t AccX, int16_t AccY, int16_t AccZ, int16_t GyroX, float dt)
{
  static float pitch = 0.0f; // 静态变量保存上一时刻的俯仰角
  float accelPitch;

  // 使用加速度计计算俯仰角
  accelPitch = atan2f((float)AccY, sqrtf((float)(AccX * AccX + AccZ * AccZ))) * 180.0f / M_PI;

  // 使用陀螺仪增量更新俯仰角
  pitch += (float)GyroX * dt / 131.0f; // 陀螺仪数据转换为角度

  // 融合加速度计和陀螺仪数据
  pitch = 0.98f * pitch + 0.02f * accelPitch;

  return pitch;
}
