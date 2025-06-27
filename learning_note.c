/*===== 寄存器映射 =======*/

//一个地址0x12345678 若直接对此地址赋值，1.强转为某一类型的指针、2.取内容赋值
*(unsigned int *)0x12345678 = 0xff;

//可定义为宏定义，更加方便赋值
#define *(unsigned int *)0x12345678 GPIOA_MODER 
GPIOA_MODER = 0xff;

//由于在AHB1总线搭载的GPIO外设有许多接口：GPIOA、GPIOB……，每种接口都有9种寄存器，单一定义太过繁琐，采用结构体定义
//正常操作是采用地址偏移，如下，每种接口都有9种寄存器，每个接口的基地址也不同
*(uint32_t*)GPIOE_BASE = 0xFF;     // 只能访问第一个寄存器
*(uint32_t*)(GPIOE_BASE+4) = 0x1;  // 需要手动计算偏移

//所以采用结构体定义寄存器，一是因为其寄存器本就在一块内存排布，二是结构体只需定义首地址，剩余的寄存器地址会根据数据类型依次往下排布
typedef struct {
    uint32_t MODER;   // 偏移0
    uint32_t OTYPER;  // 偏移4
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t BSRR;
    uint32_t LCKR;
    uint16_t AFRL;  // 注意：16位类型可能需要特殊处理
    uint16_t AFRH;
} GPIO_TypeDef;

// 假设GPIOE外设的基地址是0x40021000
#define GPIOE_BASE  (0x40021000U)
#define GPIOE  ((GPIO_TypeDef *) GPIOE_BASE)

//((uin32_t *) GPIOE_BASE)是指向uint32_t类型的指针，所以((GPIO_TypeDef *) GPIOE_BASE)是指向GPIO_TypeDef类型的指针
//故GPIO_TypeDef结构体的首地址即为#define GPIOE_BASE  (0x40021000U)，结构体内部变量地址按照数据类型一次往下排布

int main() {
    // 将MODER寄存器（偏移0）设置为0xff
    GPIOE->MODER = 0xff;  //这是一个语法糖，能够简便的访问结构体内部变量
    // 编译器将其转换为：
    // *((uint32_t *) (GPIOE_BASE + 0)) = 0xff;
}