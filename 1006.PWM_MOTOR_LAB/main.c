#include "device_driver.h"

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);
	Key_Poll_Init();
	Control_Init();
}

#if 1

int dir = 0;
int speed = 1;

void cmd_process(char x)
{
	volatile int i;

	if ((x >= '1') && (x <= '9')) speed = x - '0';
	else
	{
		switch (x)
		{
			case 'F':
				dir = 1;
				break;
			case 'R':
				dir = -1;
				break;
			case 'S':
				dir = 0;
				break;
			default:
				return;
		}
	}

	for (i = 0; i < 10000; i++);
	PWM_Motor_Run(dir, 1000, 30 + 9*(speed - 1));
	Uart1_Printf("%d, %d\n", dir, speed);
}

void Main(void)
{
	Sys_Init();
	Uart_Printf("Motor Pace Control Test\n");

	for(;;)
	{
		char x = Uart1_Get_Pressed();

		if (x != 0)
		{
			Uart1_Printf("%c\n", x);
			cmd_process(x);
		}
	}
}

#endif

#if 0

void Main(void)
{
	Sys_Init();
	TIM3_Out_Init();
	TIM3_Out_Freq_Generation(130);
}

#endif

#if 0

#define BASE  (500) //msec

static void Buzzer_Beep(unsigned char tone, int duration)
{
	const static unsigned short tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987};

	TIM3_Out_Freq_Generation(tone_value[tone]);
	TIM2_Delay(duration);
	TIM3_Out_Stop();
}

void Main(void)
{
	int i;

	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2};
	enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};
	const int song1[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2}};
	const char * note_name[] = {"C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1", "C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2"};

	Sys_Init();
	Uart1_Printf("Buzzer Test!!\n");

	TIM3_Out_Init();

	Uart1_Printf("%s ", note_name[C1]);
	Buzzer_Beep(C1,N4);
	Uart1_Printf("%s ", note_name[D1]);
	Buzzer_Beep(D1,N4);
	Uart1_Printf("%s ", note_name[E1]);
	Buzzer_Beep(E1,N4);
	Uart1_Printf("%s ", note_name[F1]);
	Buzzer_Beep(F1,N4);
	Uart1_Printf("%s ", note_name[G1]);
	Buzzer_Beep(G1,N4);
	Uart1_Printf("%s ", note_name[A1]);
	Buzzer_Beep(A1,N4);
	Uart1_Printf("%s ", note_name[B1]);
	Buzzer_Beep(B1,N4);
	Uart1_Printf("%s ", note_name[C2]);
	Buzzer_Beep(C2,N4);

	Uart1_Printf("\nSong Play\n");

	for(i=0; i<(sizeof(song1)/sizeof(song1[0])); i++)
	{
		Uart1_Printf("%s ", note_name[song1[i][0]]);
		Buzzer_Beep(song1[i][0], song1[i][1]);
	}
}

#endif