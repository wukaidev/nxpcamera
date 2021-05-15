#define ROW  160  //行数   320*240;160*120;100*80;80*60;  
#define LINE  380   //列数

extern u8 Is_Finish;
extern u8 Is_Finish1;

extern void pro_init(void);

extern void DMA_transmit_init(void *SADDR, void *DADDR,uint32 count,uint8 source);
//extern void DMA_transmit_init1(void *SADDR, void *DADDR,uint32 count,uint8 source);

extern void SendImage(void);
extern void SendImage1(void);