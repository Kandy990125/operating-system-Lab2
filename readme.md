# 实验简介

**实验目的:**

>对操作系统shell和系统调用的理解更加深刻。

>协调父子进程的创建与实现


# 实验设计

**参数设置**、**完整流程图**、**其他主要流程**

## 参数设置
|                |类型                        |说明
|----------------|-------------------------------|-----------------------------|
|args[MAX_LINE/2+1]|指针数组|命令行指针数组|
|should_run|整数型|退出shell的决定标志|
|history_real_num|整数型|记录一共输入了几个命令|
|num|整数型|记录命令&参数个数|
|ch|字符型|接收输入空格和换行符的参数|
|s[MAX_LINE]|字符数组|存储整个命令字符串的函数|
|all_i|整数型|记录命令的长度|
|flag|整数型|标记命令字符串的最后是否为&|
|flag2|整数型|标记是否运行了！命令和history|
## 实验流程图
![enter image description here](https://github.com/Kandy990125/operating-system-Lab2/blob/master/%E5%9B%BE%E7%89%871.png?raw=true)

# 实验说明
## !!命令
>执行上一条命令
>例如：输入ps、!!，则执行ps命令
>如果直接输入!!显示：历史上不存在这样的命令
## !+数字命令
>执行第i条命令
>如果第i条命令不存在的话，显示：历史上不存在这样的命令
## history命令
>显示最近用户输入的十条命令
## 父进程与子进程
基本思路是：
>输入命令的过程在子进程中进行

>直接输入命令，则父进程等子进程运行完之后继续
>输入命令+&，则父进程和子进程同时运行
