# STM32F4-IntelligentAlarm
STM32F407-智能报警系统

设备：STM32F407、MQ-2烟雾传感器
功能模块：MQ-2烟雾传感器模块、STM32：内部温度传感器、LCD、LED、KEY（按键）、红外模块（自带遥控器）
接线：MQ-2：VCC——STM32电路板3.3V正极 GND——STM32 GND AO-STM32电路板PF7

功能描述：开机后，屏幕显示当前温度和烟雾浓度，当浓度高度设定值便开启蜂鸣器
KEY1、遥控器按键‘1’———控制LED0
KEY2、遥控器按键‘2’———控制蜂鸣器
KEY_UP遥控器按键‘ALIENTEK’————控制LCD屏幕的亮灭