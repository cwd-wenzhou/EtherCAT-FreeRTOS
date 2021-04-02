
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
/*本程序为电磁阀的驱动程序
陈韦达 2021 3 16
*/
////////////////////////////////////////////////////////////////////////////////// 	

	 				    
#define RELAY1 PFout(4)	// 电磁阀CH1控制信号
#define RELAY2 PFout(5)	// 电磁阀CH2控制信号
#define RELAY3 PFout(6)	// 电磁阀CH3控制信号
#define RELAY4 PFout(7)	// 电磁阀CH4控制信号

#define FBO1 PEin(2)	// 电磁阀CH1读取是否动作的信号
#define FBO2 PEin(3)	// 电磁阀CH1读取是否动作的信号
#define FBO3 PEin(4)	// 电磁阀CH1读取是否动作的信号
#define FBO4 PEin(5)	// 电磁阀CH1读取是否动作的信号

void Diancifa_Init(void);//初始化	