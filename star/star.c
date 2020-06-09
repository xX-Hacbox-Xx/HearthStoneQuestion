#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
static unsigned int randseed;
long longRounds=0; /*游戏总局数*/
long longWin=0; /*玩家赢得次数*/
int intStar=0; /*玩家获得的星星数*/
int intWinTimes=0 ;/*连续赢的次数*/
double dbWinArea=0;//赢的局数平均值
/* 实现伪随机数的支持 */
unsigned int Curl_rand(void)
{
  unsigned int r;
  /* 返回一个无符号32位整型的伪随机数. */
  r = randseed = randseed * 1103515245 + 12345;
  return (r << 16) | ((r >> 16) & 0xFFFF);
}

void Curl_srand(void)
{
  /* 产生随机的伪随机数序列。 */
  randseed = (unsigned int) time(NULL);
  Curl_rand();
  Curl_rand();
  Curl_rand();
}


void gaming() /*一次游戏*/
{
	int result=Curl_rand() %2;
	longRounds++; /*总局数+1 */
	if (result==0 && intStar>0) /*玩家输*/
	{
		intStar--;
		intWinTimes=0; /*连赢局数清零*/
	}
	else{
		longWin++;/*玩家赢的总局数+1*/
		
		if (intWinTimes<2) intStar++; /*连续win的不足2局*/
		else /*连续win的不少于2局*/
			intStar+=2;
		intWinTimes++;
		/*连赢局数+1*/
	}
}

void play(int i) {
	while(10>1)
	{
		/*游戏进行*/
		gaming();
		if (intStar>=50) break;
	}
	printf("第%4d次模拟：总局数=%-6ld，胜局数=%-6ld，获得50颗星星的胜率=%5.2f ％\n",i,longRounds,longWin,100.00*longWin/longRounds);
}

int main() {
    int intRuns=1000; /*尝试模拟次数*/
	int i=1;
	Curl_srand();
	for (i=1;i<=intRuns;i++)
	{
		longRounds=0; /*游戏总局数*/
		longWin=0; /*玩家赢得次数*/
		intStar=0; /*玩家获得的星星数*/
		intWinTimes=0 ;/*连续赢的次数*/
		play(i);
		dbWinArea+=longWin;
	}
	printf("\n------------------------------------\n模拟%d次，获得50颗星星需要赢的次数平均值=%d",intRuns,(int)(dbWinArea/intRuns));
	getchar();
	return 0;
}