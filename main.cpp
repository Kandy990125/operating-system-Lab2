#include <stdio.h>

#include <unistd.h>

#include <string.h>

#include <malloc.h>

#include <stdlib.h>

#include <errno.h>

#include <sys/wait.h>   /* for wait() */

#define MAX_LINE 80     /* 命令最大长度限制 */

char history_array[10][MAX_LINE] = { { '\0' } };

int history_len[10];

void history( int num )

{

	int i, j;

	for ( i = num - 1; i >= 0; i-- )

	{

		printf( "%d ", i + 1 );

		for ( j = 0; j < history_len[i]; j++ )

		{

			putchar( history_array[i][j] );

		}

		printf( "\n" );

	}

}

int main()

{

	char *args[MAX_LINE / 2 + 1];   /* 命令行指针数组 */

	int should_run = 1;             /* 退出shell的决定标志 */

	int the_last_one = 0;

	int history_real_num = 0;       /* 记录一共输入了几个命令 */

	while ( should_run )

	{

		printf( "osh>" );

		fflush( stdout );

		int	num	= 0;	//记录命令&参数个数

		char ch;	//接收输入空格和换行符的临时参数

		ch = ' ';
	
		char s[MAX_LINE];	//存储整个命令字符串的函数

		int	i = 0, j = 0;

		int	all_i = 0;	//记录命令的长度

		int flag = 1;	//标记命令字符串的最后是否为&		//如果是&，flag置为0

		int flag2 = 1;	//标记是否运行了！命令和history，执行过的话，flag2置为0

		while ( 1 )

		{

			char *temp;	//定义临时指针temp

			temp = (char *) malloc( sizeof(char) * MAX_LINE );	//为temp申请存储空间

			scanf( "%s", temp );	//读入字符串，并存储在temp中
			/*
				C语言中默认读到空格标志着一个字符串结束
			*/

			args[num] = temp;	//args数组中的第num个指针指向temp临时指针

			scanf( "%c", &ch );	//读取一个字符串后面的一个字符，直到读到回车，程序结束

			int temp_len = strlen( temp );	//temp_len用来存储temp字符串的长度

			for ( i = 0; i < temp_len; i++, all_i++ )

			{

				s[all_i] = temp[i];	//数组s中的第all_i个元素等于temp数组中的第i个元素

			}

			num++;

			if ( ch == '\n' )

			{

				break;

			}

			s[all_i] = ' ';

			all_i++;

		}

		if( args[num-1][0] == '&' )

		{

			flag = 0;
 
			args[num-1][0] = '\0';

			args[num-1] = NULL;

			num --;

		}


		if ( strcmp( args[0], "exit" ) == 0 )

		{

			should_run=0;

			break;

		}

		if ( s[0] == '!'&& s[1] >= '0' && s[1] <= '9' )

		{
			flag2 = 0;

			int command_num = int(s[1] - '0');

			if ( command_num < history_real_num )

			{

				//printf( "%d\n", history_real_num );

				char *temp_args[MAX_LINE / 2 + 1];

				temp_args[0] = (char *) malloc( sizeof(char) * MAX_LINE );

				i = 0;

				for ( j = 0; j < history_len[command_num]; j++ )

				{

					if ( history_array[command_num][j] == ' ' )

					{

						*(temp_args[i] + j) = '\0';

						i++;

						temp_args[i] = (char *) malloc( sizeof(char) * MAX_LINE );

					}

					else 

					{

						*(temp_args[i] + j) = history_array[command_num][j];

					}

				}

				*(temp_args[i] + j) = '\0';

				execvp( temp_args[0], temp_args );

			}

			else 

			{

				printf( "历史上没有这样的命令\n" );

			}

			
		}

		else if ( num == 1 && strcmp( args[0], "!!" ) == 0 )
	
		{

			//printf("!!\n");

			//printf("%d,%d\n",the_last_one,history_real_num);

			flag2 = 0;

			if ( history_real_num == 0 )

			{

				printf( "历史记录中没有命令\n" );

			}

			else 

			{

				char *temp_args[MAX_LINE / 2 + 1];

				temp_args[0] = (char *) malloc( sizeof(char) * MAX_LINE );

				i = 0;

				for ( j = 0; j < history_len[the_last_one-1]; j++ )

				{

					if ( history_array[the_last_one-1][j] == ' ' )

					{

						*(temp_args[i] + j) = '\0';

						i++;

						temp_args[i] = (char *) malloc( sizeof(char) * MAX_LINE );

					}

					else 

					{

						*(temp_args[i] + j) = history_array[the_last_one-1][j];

					}

				}

				*(temp_args[i] + j) = '\0';

				execvp( temp_args[0], temp_args );

			}

		}

		/* 判断是否是history命令 */

		else if ( num == 1 && strcmp( args[0], "history" ) == 0 )

		{

			flag2 = 0;

			//printf("history:%d\n",history_real_num);

			if ( history_real_num >= 10 )

			{

				history( 10 );

				for ( i = 0; i < 9; i++ )

					for ( j = 0; j < history_len[i + 1]; j++ )

						history_array[i][j] = history_array[i + 1][j];

				for ( i = 0; i < 9; i++ )

					history_len[i] = history_len[i + 1];

			}

			else 

			{

				history( history_real_num );

			}

		}

		else

		{

			if ( history_real_num >= 10 )

			{

				for ( i = 0; i < 9; i++ )

					for ( int j = 0; j < history_len[i + 1]; j++ )

						history_array[i][j] = history_array[i + 1][j];

				for ( i = 0; i < 9; i++ )

					history_len[i] = history_len[i + 1];

			}

			//printf("%s\n",args[0]);

		}

		for ( j = 0; j < all_i; j++ )

				
			history_array[the_last_one][j] = s[j];


		history_len[the_last_one] = all_i;

		the_last_one++;


		if ( the_last_one == 10 )

		{

			the_last_one = 9;

		}

		history_real_num++;



		pid_t fpid;

		fpid = fork();  // fork子进程

		if ( fpid < 0 )		// 如果当前进程的进程号<0，则证明发生了错误

		{

			printf( "\r\n error" );

		}

		else if ( 0 == fpid )	
		// 如果当前进程的进程号=0，则当前进程是子进程
			//并发执行

		{

			if( flag2 )
			{

				execvp( args[0], args );	

			}

			break;

			flag2 = 1;

			//exit( 0 );

		}

		// 如果当前进程的进程号=0，则当前进程是父进程
			//等待运行完毕后运行

		else
		{

			if( flag != 0 )

			{
				while (fpid != wait(NULL));

			}

			flag = 1;

		}

		
	}

	return(0);

}