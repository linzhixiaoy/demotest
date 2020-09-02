/********************************************************/
// CPU需要：STM32F103--RAM内存不小于64K	Flash内存不小于128K
// 本代码已在STM32F103RDT6、VET6测试通过
// 编辑日期： 20150909
// editor by 传人记
// 网店：     shop148504253.taobao.com
/********************************************************/

//http://wenku.baidu.com/link?url=CMU4nDy-NriXz4F-Mip0Kx2JbzBrK3j0IovrxB6dGXPP0B-qFAFHVzGXRw6OAFQCyEt9ZHeXIj-0Pnpk9ytDibzLBHNBFLQmNFYpMSL7pKC
//ADCx对应的引脚
//ADC123_IN0->PA0；ADC123_IN1->PA1；ADC123_IN2->PA2；ADC123_IN3->PA3

//ADC12_IN4->PA4  //ADC3_IN4->PF6
//ADC12_IN5->PA5  //ADC3_IN5->PF7
//ADC12_IN6->PA6  //ADC3_IN6->PF8
//ADC12_IN7->PA7  //ADC3_IN7->PF9
//ADC12_IN8->PB0  //ADC3_IN8->PF10
//ADC12_IN9->PB1

//ADC123_IN10->PC0；ADC123_IN11->PC1；ADC123_IN12->PC2；ADC123_IN13->PC3；
//ADC12_IN14->PC4；ADC12_IN15->PC5

#include "stm32f10x.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include <stdio.h>   
#include "PLC_Dialogue.h"
#include "plc_conf.h"

#if ADC_FUNC

/* Private typedef -----------------------------------------------------------*/
// NTC 5K, 1%
//const U32 NTCTab[] = 
//{
///*       -10度    -9度    -8度    -7度    -6度    -5度    -4度    -3度    -2度    -1度 */
///*-20*/  36794,  35017,  33314,  31677,  30096,  28567,  27332,  26094,  24881,  23715,
///*-10*/  22610,  21571,  20604,  19707,  18879,  18115,  17167,  16283,  15459,  14688,
///*       0度     1度     2度     3度     4度     5度     6度     7度     8度     9度 */
///*0  */  13967,  13292,  12659,  12067,  11511,  10988,  10498,  10036,  9602,   9193,
///*10 */  8808,   8444,   8101,   7777,   7471,   7182,   6908,   6648,   6403,   6170,
///*20 */  5949,   5739,   5540,   5351,   5171,   5000,   4771,   4562,   4369,   4191,
///*30 */  4025,   3870,   3726,   3590,   3462,   3341,   3226,   3117,   3013,   2914,
///*40 */  2819,   2728,   2640,   2556,   2475,   2397,   2321,   2248,   2178,   2110,
///*50 */  2044,   1980,   1917,   1857,   1799,   1742,   1688,   1634,   1583,   1533,
///*60 */  1485,   1438,   1392,   1348,   1305,   1264,   1224,   1185,   1148,   1111,
///*70 */  1076,   1042,   1010,   978,    947,    918,    889,    862,    835,    809,
///*80 */  784,    760,    737,    715,    693,    673,    652,    633,    614,    596,
///*90 */  579,    562,    546,    531,    515,    501,    487,    473,    460,    448
//};

// NTC 10K, 1%
const u32 NTC10KTab[] = 
{
/*-20*/  95337,90058,85101,80444,76068,71956,68088,64450,61026,57804,
/*-10*/  54769,51911,49217,46679,44285,42027,39898,37887,35989,34197,
/*0  */  32503,30903,29390,27959,26660,25325,24114,22966,21880,20850,
/*10*/   19875,18950,18073,17242,16453,15705,14994,14320,13679,13071,
/*20*/   12492,11943,11420,10923,10450,10000, 9572, 9164, 8776, 8406,
/*30*/    8054, 7718, 7399, 7094, 6803, 6525, 6260, 6008, 5767, 5536,
/*40*/    5316, 5106, 4906, 4714, 4531, 4355, 4188, 4028, 3874, 3728,
/*50*/    3587, 3453, 3324, 3201, 3082, 2969, 2861, 2757, 2657, 2562,
/*60*/    2470, 2382, 2298, 2217, 2139, 2064, 1993, 1924, 1858, 1794,
/*70*/    1733, 1675, 1618, 1564, 1512, 1462, 1414, 1367, 1323, 1280,
/*80*/    1238, 1198, 1160, 1123, 1087, 1053, 1020,  988,  957,  928,
/*90*/     899,  872,  845,  819,  795,  771,  748,  725,  704,  683
};


// NTC 100K, 1% 单位：欧
const s32 NTC100KTab[] = 
{
/*        -9度    -8度    -7度    -6度    -5度    -4度    -3度    -2度    -1度    0度 */
/*-40*/										                                                        3225000,
/*-30*/   3026000,2840000,2665000,2501000,2348000,2204000,2070000,1945000,1828000,1718000,
/*-20*/   1614000,1518000,1428000,1344000,1267000,1194000,1127000,1063000,1004000,948800,
/*-10*/   896300, 847100, 801000, 757800, 717200, 679000, 643200, 609600, 577900, 548100,
/*0*/     519700, 492800, 467500, 443500, 420900, 399500, 376300, 360200, 342200, 
/*        0度     1度     2度     3度     4度     5度     6度     7度     8度     9度 */    
/*0*/ 	  325100, 309400, 294500, 280400, 267000, 254300, 242200, 230700, 219900, 209500,
/*10*/    199700, 190400, 181600, 173200, 165200, 157700, 150500, 143700, 137200, 131000,
/*20*/    125200, 119600, 114300, 109300, 104500, 100000, 95600,  91570,  87660,  83930,
/*30*/    80390,  77010,  73790,  70730,  67810,  65030,  62380,  59840,  57430,  55130,
/*40*/    52930,  50830,  48830,  46920,  45090,  43340,  41670,  40080,  38550,  37090,
/*50*/    35700,  34360,  33080,  31860,  30680,  29560,  28480,  27450,  26460,  25510,
/*60*/    24600,  23730,  22890,  22090,  21320,  20580,  19860,  19180,  18520,  17890,
/*70*/    17290,  16700,  16140,  15600,  15080,  14580,  14100,  13640,  13190,  12760,
/*80*/    12350,  11950,  11560,  11190,  10830,  10490,  10150,  9830,   9518,   9217,
/*90*/    8927,   8648,   8380,   8121,   7871,   7630,   7398,   7175,   6959,   6751,
/*        0度     1度     2度     3度     4度     5度     6度     7度     8度     9度 */
/*100*/   6550,   6357,   6170,   5989,   5815,   5647,   5485,   5328,   5176,   5030,
/*110*/   4888,   4751,   4619,   4491,   4367,   4247,   4131,   4018,   3909,   3804,
/*120*/   3702,   3602,   3506,   3413,   3322,   3234,   3144,   3057,   2973,   2891,
/*130*/   2812,   2735,   2661,   2589,   2519,   2452,   2387,   2323,   2262,   2203,
/*140*/   2146,   2091,   2037,   1985,   1935,   1886,   1839,   1793,   1749,   1706,
/*150*/   1665,   1625,   1586,   1548,   1511,   1476,   1442,   1408,   1376,   1344,
/*160*/   1314,   1284,   1255,   1227,   1200,   1173,   1148,   1123,   1098,   1074,
/*170*/   1051,   1029,   1007,   985,    964,    944,    923,    902,    881,    862, 
/*180*/   842,    824,    805,    788,    771,    754,    738,    722,    706,    691,
/*190*/   677,    663,    649,    636,    623,    610,    598,    586,    574,    563, 
/*        0度     1度     2度     3度     4度     5度     6度     7度     8度     9度 */
/*200*/   552,    541,    531,    521,    511,    500,    492,    483,    474,    465, 
/*210*/   457,    448,    440,    432,    425,    417,    410,    403,    396,    389,
/*220*/   382,    375,    369,    363,    356,    350,    344,    337,    331,    325,
/*230*/   319,    313,    307,    302,    297,    291,    286,    281,    276,    271,
/*240*/   266,    262,    257,    252,    248,    244,    240,    235,    231,    228,
/*250*/   224,    220,    216,    213,    209,    206,    202,    199,    196,    193,
/*260*/   189,    186,    183,    180,    178,    175,    172,    169,    167,    164,
/*270*/   162,    159,    157,    154,    152,    150,    147,    145,    143,    141,
/*280*/   139,    137,    135,    132,    130,    129,    127,    125,    123,    121,
/*290*/   119,    117,    116,    114,    112,    111,    109,    107,    106,    104,
/*300*/   102
};

// NTC 10K, 1% 单位：欧
//const s32 NTC10KTab[] = 
//{
///*        -9度    -8度    -7度    -6度    -5度    -4度    -3度    -2度    -1度    0度 */
///*-40*/										                                                        339100,
///*-30*/   317140, 296740, 277790, 260170, 243790, 228540, 214340, 201120, 188800, 177310,
///*-20*/   166590, 156590, 147260, 138540, 130390, 122770, 115640, 108980, 102740, 96891,     
///*-10*/   91415,  86283,  81471,  76957,  72720,  68743,  65008,  61499,  58200,  55099,     
///*0*/     52181,  49436,  46852,  44418,  42126,  39965,  37928,  36007,  34195,  
///*        0度     1度     2度     3度     4度     5度     6度     7度     8度     9度 */    
///*0*/     32485,  30871,  29346,  27937,  26584,  25303,  24092,  22945,  21895,  20830,
///*10*/    19856,  18932,  18057,  17227,  16439,  15692,  14983,  14310,  13670,  13063,
///*20*/    12486,  11938,  11416,  10920,  10449,  10000,  9573,   9166,   8779,   8411, 
///*30*/    8059,   7724,   7405,   7101,   6811,   6534,   6270,   6018,   5778,   5548, 
///*40*/    5328,   5119,   4919,   4727,   4544,   4369,   4192,   4031,   3877,   3729,
///*50*/    3588,   3453,   3324,   3201,   3083,   2970,   2861,   2758,   2658,   2563,
///*60*/    2472,   2384,   2300,   2219,   2142,   2068,   1997,   1928,   1863,   1800,
///*70*/    1739,   1681,   1625,   1571,   1519,   1470,   1422,   1376,   1331,   1289,
///*80*/    1248,   1208,   1170,   1133,   1098,   1064,   1031,   1000,   969,    938,
///*90*/    910,    882,    856,    830,    805,    781,    758,    736,    715,    694,
///*100*/   674,    655,    636,    618,    601,    584,    568,    552,    537,    522,
///*110*/   508,    494,    481,    468,    456,    444,    432,    421,    410,    399,
///*120*/   389,    379 	
//};
/* Private define ------------------------------------------------------------*/
// 求数组元素个数宏
#define countof(arr)				  (sizeof(arr)/sizeof(arr[0]))
//#define NTC_R_PULLUP          (33000uL)
#define NTC_R_PULLUP          (10100000)

#define START_VALUE_NTC       (s32)(-20)

#define ADC1_DR_Address    ((uint32_t)0x4001244C)  //ADC1 DR寄存器基地址
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//////////////////////////////////////////////////////////////
/*  传人记，20170922新增 */
#define ADC_NUM      2                // 共2个通道
#define BUF_SIZE     10                // 每个通道采样次数


uint16_t adcDmaBuf[ADC_NUM];           // DMA缓冲区
uint16_t adcBuf[ADC_NUM][BUF_SIZE];    // ADC通道排序环节滤波缓冲区
uint32_t adcSum[ADC_NUM];              // 平均值滤波环节的总数缓存

__IO uint16_t AdcAverage[ADC_NUM];
__IO uint8_t sampled = 0;

///////////////////////////////////////////////////////////////
void ADC_init(void)
{
	ADC_InitTypeDef  ADC_InitStructure;      //ADC初始化结构体声明
	DMA_InitTypeDef  DMA_InitStructure;      //DMA初始化结构体声明  
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable ADC1 and GPIOC clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB, ENABLE);  

	/* Enable DMA1 clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
  // 传人记，20170309修改                      
  // 传人记，20170309修改                      
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
  /* DMA1的Channel1通道初始化 */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;   // 配置外设数据寄存器地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adcDmaBuf;  // 配置用户存储区首地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  // DMA方向：外设是源头
  DMA_InitStructure.DMA_BufferSize = ADC_NUM;               // 缓冲区大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 外设地址增长使能
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 存储区地址增长使能
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据单元大小
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 存储区单元大小
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;     // DMA模式：循环
  DMA_InitStructure.DMA_Priority = DMA_Priority_High; // DMA优先级：高
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;        // M2M模式：不使能
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);        // 调用DMA初始化函数初始化DMA1的Channel1通道
  
  DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
  DMA_Cmd(DMA1_Channel1, ENABLE);
  
  /* ADC的配置 */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;        // 使能扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 失能连续模式，采用外部触发了
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;// 右对齐
  ADC_InitStructure.ADC_NbrOfChannel = ADC_NUM;             // 规则通道数
  ADC_Init(ADC1, &ADC_InitStructure);
  
  /* ADC规则通道序列和采样时间配置 */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
  
  /* 使能 ADC */
  ADC_Cmd(ADC1, ENABLE);
  /* 复位校准寄存器 */
  ADC_ResetCalibration(ADC1);
  /* 检查复位校准寄存器是否已经完成初始化 */
  while(ADC_GetResetCalibrationStatus(ADC1));
  /* 启动校准 */
  ADC_StartCalibration(ADC1);
  /* 检查校准是否完成 */
  while(ADC_GetCalibrationStatus(ADC1));

  ADC_DMACmd(ADC1, ENABLE);

  {
		NVIC_InitTypeDef NVIC_InitStructure;
    uint8_t i;
    for (i=0; i<ADC_NUM; i++)
    {
      adcSum[i] = 0;
    }
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
  }
	
}

void DMA1_Channel1_IRQHandler(void)
{
  static uint8_t cnt = 0;
  uint8_t i;
  DMA_ClearITPendingBit(DMA1_IT_TC1);
  if (!sampled)
  {
    for (i=0; i<ADC_NUM; i++)
    {
      adcBuf[i][cnt] = adcDmaBuf[i];
    }
    if (++cnt >= BUF_SIZE)
    {
      cnt = 0;
      sampled = 1;
    }
  }
}


// 获取NTC通道上的检测温度，精确到0.1度
s16 ClacNTC(u32 val) 
{
	s32 i;
	s32 temp;
	
	if (val > NTC10KTab[0])
		return START_VALUE_NTC*10;
	if (val < NTC10KTab[countof(NTC10KTab)-1])
		return ((s16)countof(NTC10KTab)+START_VALUE_NTC)*10;
	
	for (i=0; i<countof(NTC10KTab); i++)
	{
    if (val == NTC10KTab[i])
			return (i+START_VALUE_NTC)*10;
		
		if ((val > NTC10KTab[i+1]) && (val < NTC10KTab[i]))
			break;
	}
	temp = ((val-NTC10KTab[i+1])*10) / (NTC10KTab[i]-NTC10KTab[i+1]);
	
	return ((i+2)+START_VALUE_NTC)*10 - temp;
}

s16 GetNTC(u16 adcVal)
{
		u32 tmpV, Rntc;
		s16 ret;
		
		tmpV = (3000uL*adcVal) / 4095uL; // 计算ADC对应多少的分压
		Rntc = (NTC_R_PULLUP*tmpV) / (3001uL-tmpV); // 计算热敏电阻的阻值

		// 判断NTC是否在合理范围之内
		if ((tmpV>20) && (tmpV<2800))
		{
			ret = ClacNTC(Rntc); // 查RT表，计算温度，精度0.1度
		}
		else
		{
			ret = 32767;
		} 
		return ret;
}

// 传人记，20170920
void ADC_Sample(void)
{
  if (sampled)
  {
    uint8_t i, j;    
    for (i=0; i<ADC_NUM; i++)
    {
				for(j=0;j < BUF_SIZE;j++)
					adcSum[i] += adcBuf[i][j];

				AdcAverage[i] = adcSum[i] / BUF_SIZE;

//					if(i <4)
						plc_16BitBuf[0X1000 + 6030 + i] =AdcAverage[i];
//					else
//					  plc_16BitBuf[0X1000 + 6030 + i] = GetNTC(AdcAverage[i]);

				adcSum[i] = 0;
    }  
    sampled = 0;
  }
}

#endif 
