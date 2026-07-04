#include "key.h"
#include "main.h"

//扫描列端口
u8 col_port_scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN)
{
    if (HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == 0)
    {
		HAL_Delay(10);//延时10ms
        if (HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == 0)
			return 1;
		else
			return 0;
    }
    else
    {
        return 0;
    }   
}

//扫描行端口
u8 row_port_scan(GPIO_TypeDef *GPIOx,uint16_t GPIO_PIN)
{
    if (HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == 0)
    {
		HAL_Delay(10);
        while (HAL_GPIO_ReadPin(GPIOx,GPIO_PIN) == 0);
		return 1;
    }
    else
    {
        return 0;
    }   
}

u8 col_scan(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  //列扫描GPIO设定

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = ROW1_Pin|ROW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  /*Configure GPIO pins : PFPin PFPin */
  GPIO_InitStruct.Pin = ROW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  //所有行置低
  HAL_GPIO_WritePin(ROW3_GPIO_Port, ROW3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, ROW1_Pin|ROW2_Pin, GPIO_PIN_RESET);
  
  //所有列上拉输入
  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = COL1_Pin|COL2_Pin|COL3_Pin|COL4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  
  //扫描各列
  if (col_port_scan(GPIOA, COL1_Pin) == 1)
	  return 1;
  else if (col_port_scan(GPIOA, COL2_Pin) == 1)
	  return 2;
  else if (col_port_scan(GPIOA, COL3_Pin) == 1)
	  return 3;
  else if (col_port_scan(GPIOA, COL4_Pin) == 1)
	  return 4;
  else
	  return 0;
}

u8 row_scan(u8 col_return)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  //行扫描GPIO设定
  
  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = COL1_Pin|COL2_Pin|COL3_Pin|COL4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  //所有列先置高
  HAL_GPIO_WritePin(GPIOA, COL1_Pin|COL2_Pin|COL3_Pin|COL4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = ROW3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  
  /*Configure GPIO pins : PAPin PAPin PAPin PAPin */
  GPIO_InitStruct.Pin = ROW1_Pin|ROW2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  
  //扫描各行
  if(col_return==1)
  {
	  HAL_GPIO_WritePin(GPIOA, COL1_Pin, GPIO_PIN_RESET);
	  if (row_port_scan(GPIOA, ROW1_Pin) == 1)
		  return 1;
	  else if (row_port_scan(GPIOA, ROW2_Pin) == 1)
		  return 4;
	  else if (row_port_scan(GPIOF, ROW3_Pin) == 1)
		  return 7;
	  else
		  return 12;
  }
    else if(col_return==2)
  {
	  HAL_GPIO_WritePin(GPIOA, COL2_Pin, GPIO_PIN_RESET);
	  if (row_port_scan(GPIOA, ROW1_Pin) == 1)
		  return 2;
	  else if (row_port_scan(GPIOA, ROW2_Pin) == 1)
		  return 5;
	  else if (row_port_scan(GPIOF, ROW3_Pin) == 1)
		  return 8;
	  else
		  return 12;
  }
    else if(col_return==3)
  {
	  HAL_GPIO_WritePin(GPIOA, COL3_Pin, GPIO_PIN_RESET);
	  if (row_port_scan(GPIOA, ROW1_Pin) == 1)
		  return 3;
	  else if (row_port_scan(GPIOA, ROW2_Pin) == 1)
		  return 6;
	  else if (row_port_scan(GPIOF, ROW3_Pin) == 1)
		  return 9;
	  else
		  return 12;
  }
    else if(col_return==4)
  {
	  HAL_GPIO_WritePin(GPIOA, COL4_Pin, GPIO_PIN_RESET);
	  if (row_port_scan(GPIOA, ROW1_Pin) == 1)
		  return 0;
	  else if (row_port_scan(GPIOA, ROW2_Pin) == 1)
		  return 10;
	  else if (row_port_scan(GPIOF, ROW3_Pin) == 1)
		  return 11;
	  else
		  return 12;
  }
    else
		return 12;//无按键按下
}

u8 key_scan(void)
{
	u8 key_return=0;
	key_return=row_scan(col_scan());
	//printf("%d\n",key_return);
	return key_return;
}
