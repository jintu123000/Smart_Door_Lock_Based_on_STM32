#include "oled.h"
#include "menu.h"
#include "key.h"
#include "stmflash.h"

extern u32 password;
extern u8 lock_state;

u8 bit_view(u8 view_data, u8 view_bit)//位查看函数,view_data输入要查看的字节,view_bit输入要查看的位(0-7)
{
	u8 temp;
	temp=1<<view_bit;
	view_data=view_data&temp;
	view_data=view_data>>view_bit;
	return view_data;
}

u8 depth_view(u8 view_data)//菜单深度查看函数,view_data输入要查看的字节
{
	if(0x10>view_data)
		return 0;
	else if(0x40>view_data)
		return 1;
	else
		return 2;
}

u8 menu_entermsp(u8 state, u8 depth)//菜单进入调用函数	
{
	u8 highlight_state;
	highlight_state=state&0x03;
	if(menu_entercallback(state,depth)==1)
	{
		if(depth==0)
		{
			highlight_state=highlight_state<<4;
			state=state|highlight_state;
			return (state&0xfc)+1;
		}
		else if(depth==1)
		{
			highlight_state=highlight_state<<6;
			state=state|highlight_state;
			return (state&0xfc)+1;
		}
		else if (depth==2)
		{
			return (state&0xfc)+1;
		}
		else
			return state;
	}
	else
		return state;
}

u8 menu_entercallback(u8 state_in, u8 depth_in)//菜单进入回调函数,返回0不进入下一层菜单,1进入下一步菜单
{
	if(depth_in==2)
	{
		return 0;
	}
	else
	{
		if(state_in==0x06)//主界面上锁触发
		{
			if(lock_state==1)
			{
				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);//上锁
				OLED_Content_Clear();
				OLED_ShowString(0,0,"LOCKED ALREADY!",16,1);
				OLED_Refresh();
				HAL_Delay(1000);
			}
			else
			{
				lock_state=1;
				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);//上锁
				printf("Action triggered, LOCKED!\n");
				OLED_Content_Clear();
				OLED_ShowString(0,0,"LOCKED!",16,1);
				OLED_Refresh();
				HAL_Delay(1000);
			}
			return 0;
		}
		if(state_in==0x05)//主界面Unlock触发
		{
			u32 password_temp=0;
			u8 password_count=0;
			OLED_Content_Clear();
			OLED_ShowString(0,0,"Enter Password:",16,1);
			OLED_Refresh();
			while(1)//Unlock界面exit退出
			{
				u8 key_return=key_scan();
				if(key_return==11)
					break;
				else if(key_return!=12&&key_return!=10)
				{
					OLED_Content_Clear();
					if(password_count<6)
					{
					password_temp=password_temp*10+key_return;
					password_count++;
					OLED_ShowString(0,0,"Enter Password:",16,1);
					if(password_count==1)
						OLED_ShowString(0,15,"*",16,1);
					if(password_count==2)
						OLED_ShowString(0,15,"**",16,1);
					if(password_count==3)
						OLED_ShowString(0,15,"***",16,1);
					if(password_count==4)
						OLED_ShowString(0,15,"****",16,1);
					if(password_count==5)
						OLED_ShowString(0,15,"*****",16,1);
					if(password_count==6)
						OLED_ShowString(0,15,"******",16,1);
					OLED_Refresh();
					}
				}
				else if(key_return==10)
				{
					if(password_temp==password)
					{
						lock_state=0;
						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);//解锁
						printf("Right password, UNLOCKED!\n");
						OLED_Content_Clear();
						OLED_ShowString(0,0,"UNLOCKED!",16,1);
						OLED_Refresh();
						HAL_Delay(1000);
						password_temp=0;
						password_count=0;
						break;
					}
					else
					{
						lock_state=1;
						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);//上锁
						printf("Wrong password, LOCKED!\n");
						OLED_Content_Clear();
						OLED_ShowString(0,0,"Wrong Password!",16,1);
						OLED_Refresh();
						HAL_Delay(1000);
						password_temp=0;
						password_count=0;
						break;
						
					}
				}			
			}

			return 0;//解锁页面无需下一级菜单
		}
		if (state_in==0x07)//主界面重设密码触发
		{
			u32 password_temp=0;
			u8 password_count=0;
			u8 password_right=0;
			OLED_Content_Clear();
			OLED_ShowString(0,0,"Current Password:",16,1);
			OLED_Refresh();
			while(1)//重设密码界面exit退出
			{
				u8 key_return=key_scan();
				if(key_return==11)
					break;
				else if(key_return!=12&&key_return!=10)
				{
					OLED_Content_Clear();
					if(password_count<6)
					{
					password_temp=password_temp*10+key_return;
					password_count++;
					OLED_ShowString(0,0,"Current Password:",16,1);
					if(password_count==1)
						OLED_ShowString(0,15,"*",16,1);
					if(password_count==2)
						OLED_ShowString(0,15,"**",16,1);
					if(password_count==3)
						OLED_ShowString(0,15,"***",16,1);
					if(password_count==4)
						OLED_ShowString(0,15,"****",16,1);
					if(password_count==5)
						OLED_ShowString(0,15,"*****",16,1);
					if(password_count==6)
						OLED_ShowString(0,15,"******",16,1);
					OLED_Refresh();
					}
				}
				else if(key_return==10)
				{
					if(password_temp==password)
					{
						password_right=1;
						OLED_Content_Clear();
						OLED_ShowString(0,0,"New Password:",16,1);
						OLED_Refresh();
						password_temp=0;
						password_count=0;
						break;
					}
					else
					{
						lock_state=1;
						HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);//上锁
						printf("Wrong password, LOCKED!\n");
						OLED_Content_Clear();
						OLED_ShowString(0,0,"Wrong Password!",16,1);
						OLED_Refresh();
						HAL_Delay(1000);
						password_temp=0;
						password_count=0;
						break;
						
					}
				}			
			}
			if(password_right==1)
			{
			while(1)//重设密码界面exit退出
			{
				u8 key_return=key_scan();
				if(key_return==11)
					break;
				else if(key_return!=12&&key_return!=10)
				{
					OLED_Content_Clear();
					if(password_count<6)
					{
					password_temp=password_temp*10+key_return;
					password_count++;
					OLED_ShowString(0,0,"New Password:",16,1);
					if(password_count==1)
						OLED_ShowString(0,15,"*",16,1);
					if(password_count==2)
						OLED_ShowString(0,15,"**",16,1);
					if(password_count==3)
						OLED_ShowString(0,15,"***",16,1);
					if(password_count==4)
						OLED_ShowString(0,15,"****",16,1);
					if(password_count==5)
						OLED_ShowString(0,15,"*****",16,1);
					if(password_count==6)
						OLED_ShowString(0,15,"******",16,1);
					OLED_Refresh();
					}
				}
				else if(key_return==10)
				{
					password=password_temp;
					passwd_flash_erase();
					passwd_flash_write(password);
					lock_state=1;
					HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);//上锁
					printf("Password reset, LOCKED!\n");
					OLED_Content_Clear();
					OLED_ShowString(0,0,"PASSWORD RESET!",16,1);
					OLED_Refresh();
					HAL_Delay(1000);
					password_temp=0;
					password_count=0;
					password_right=0;
					break;
				}			
			}
			}

			return 0;//解锁页面无需下一级菜单
		}
		
		return 1;
	}
}

u8 menu_statemachine(u8 current_state, u8 input)//菜单状态机函数
{
	if(current_state==0x00)//初始状态
	{
		if (bit_view(input,1)==1)//enter button按下
		{
			return 0x05;//首个状态
		}
		else
		{
			return 0x00;//按下其他按钮时状态不变
		}
	}
	else
	{
		if(bit_view(input,2)==1)//up button按下
		{
			if ((current_state&0x03)==0x01)//状态最后2位为01,即达到最上端
			{
				return (current_state|0x03);//返回至最下端
			}
			else
			{
				return current_state-1;//向上移一位
			}
		}
		else if(bit_view(input,3)==1)//down button按下
		{
			if((current_state&0x03)==0x03)//状态最后2位为11,即达到最下端
			{
				return (current_state&0xfc)+1;//返回至最上端
			}
			else
			{
				return current_state+1;//向下移一位
			}
		}
		else if(bit_view(input,1)==1)//enter button按下
		{
			return menu_entermsp(current_state,depth_view(current_state));
		}
		else if(bit_view(input,4)==1)//back button按下
		{
			if(depth_view(current_state)==2)
			{
				current_state=current_state&0x3f;
				return (current_state&0xfc)+1;
			}
			else if(depth_view(current_state)==1)
			{
				current_state=current_state&0x0f;
				return (current_state&0xfc)+1;
			}
			else if(depth_view(current_state)==0)
			{
				return 0x00;
			}
			else
				return current_state;	
		}
		else//其他情况,如按下menu button
		{
			return current_state;
		}
	}
}
