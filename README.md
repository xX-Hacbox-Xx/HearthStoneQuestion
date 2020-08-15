# HearthStoneQuestion
## 本文在[玩具盒](https://www.hacbox.studio/)同步更新！图片无法阅读请移步至玩具盒！
------------

#### 旧版的炉石传说天梯机制中，如果想要在天梯模式下”拿低保”需要拿到50个星。玩家每赢一局得一颗星，如果赢了之后总共达到三次及以上的连胜则当次再得一颗星即共得两颗星，而如果玩家输了则会失去一颗星。在0星基础上，给定玩家胜率和上低保需要的星数，如何求解拿低保的期望局数？


## 问题难点
### 1. 随机数的生成

在学习过程中我发现C语言的生成随机数函数`rand()`生成的数字并不“随机”

在查询资料后得知`rand()` 函数产生的随机数是伪随机数，是根据一个数值按照某个公式推算出来的，这个数值我们称之为“种子”

> 种子在每次启动计算机时是随机的，但是一旦计算机启动以后它就不再变化了；也就是说，每次启动计算机以后，种子就是定值了，所以根据公式推算出来的结果（也就是生成的随机数）就是固定的。

所以接下来我们会使用`srand()`函数对种子`“重新播种”`，这样种子数字就会发生变化

```c
void srand (unsigned int seed);
```

它需要一个 unsigned int 类型的参数。在实际开发中，我们可以用时间作为参数，只要每次播种的时间不同，那么生成的种子就不同，最终的随机数也就不同。
使用 `<time.h> `头文件中的 `time()` 函数即可得到当前的时间（精确到秒），就像下面这样

```c
srand((unsigned)time(NULL));
```

所以修改后的随机数生成代码如下：

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    int a;
    srand((unsigned)time(NULL));
    a = rand();
    printf("%d\n", a);
    return 0;
}
```

多次运行程序，会发现每次生成的随机数都不一样了，但是，这些随机数会有逐渐增大或者逐渐减小的趋势，这是因为我们以时间为种子。

##### 再次遇到问题

在第一版的代码中会发现生成的胜率总是一样的，比如说1000次模拟中胜率每次都是0.6，我排查了很久才发现问题的关键：
**生成的随机数使用时间（秒）为种子，但是计算机模拟100甚至1000次的时间都远远少于一秒**
这就是为什么1000次的模拟中胜率总是不变的。

##### 解决方案
这个问题我从网络上找了一个播种随机数的函数方便程序设计，出处见[C语言之实现随机数产生算法-CSDN](https://blog.csdn.net/morixinguan/article/details/50704925 "C语言之实现随机数产生算法-CSDN").

```c
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
```
> "不要重复造轮子"

## 源码展示

```c
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
```
