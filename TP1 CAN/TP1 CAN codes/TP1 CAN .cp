#line 1 "C:/Users/he303651/Desktop/yolo test/MyProjectffffffffff.c"





unsigned char Can_Init_Flags, Can_Send_Flags, Can_Rcv_Flags;
unsigned char Rx_Data_Len;
char RxTx_Data[8] = {0};
char Msg_Rcvd;
const long ID_1st = 12111, ID_2nd = 3;
long Rx_ID;
int recu;
int i;
int j;
char bufferLCD[16];
int nombre,oldnbr;
char bufferUART[16];


sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;



sbit CanSpi_CS at RC0_bit;
sbit CanSpi_CS_Direction at TRISC0_bit;
sbit CanSpi_Rst at RC2_bit;
sbit CanSpi_Rst_Direction at TRISC2_bit;




void main()
{
 ANSELA = 0x01;
 ANSELB = 0;
 ANSELC = 0;
 ANSELD = 0;

 C1ON_bit = 0;
 C2ON_bit = 0;

 TRISD = 0x00;
 TRISA = 0x01;
 TRISB = 0x00;
 TRISC = 0x00;



 Can_Init_Flags = 0;
 Can_Send_Flags = 0;
 Can_Rcv_Flags = 0;

 ADC_Init();

 Lcd_Init();

UART1_Init(9600);

 Delay_ms(200);

 Can_Send_Flags = _CANSPI_TX_PRIORITY_0 &
 _CANSPI_TX_XTD_FRAME &
 _CANSPI_TX_NO_RTR_FRAME;

 Can_Init_Flags = _CANSPI_CONFIG_SAMPLE_THRICE &
 _CANSPI_CONFIG_PHSEG2_PRG_ON &
 _CANSPI_CONFIG_XTD_MSG &
 _CANSPI_CONFIG_DBL_BUFFER_ON &
 _CANSPI_CONFIG_VALID_XTD_MSG;


 SPI1_Init();

 CANSPIInitialize(1,3,3,3,1,Can_Init_Flags);
 CANSPISetOperationMode(_CANSPI_MODE_CONFIG,0xFF);
 CANSPISetMask(_CANSPI_MASK_B1,-1,_CANSPI_CONFIG_XTD_MSG);
 CANSPISetMask(_CANSPI_MASK_B2,-1,_CANSPI_CONFIG_XTD_MSG);
 CANSPISetFilter(_CANSPI_FILTER_B2_F4,ID_1st,_CANSPI_CONFIG_XTD_MSG);

 CANSPISetOperationMode(_CANSPI_MODE_NORMAL,0xFF);


 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Out(1,1,"yol0");

 for(;;)
 {
 memset(Rxtx_Data,0,sizeof(RxTx_Data));
 Msg_Rcvd = CANSPIRead(&Rx_ID , RxTx_Data , &Rx_Data_Len, &Can_Rcv_Flags);
 if ((Rx_ID == ID_1st) && Msg_Rcvd)
 {

 for(i=0; i < 8 ; i++)
 {

 recu = RxTx_Data[i];


 sprintf(bufferUART,"nbr rcvd : %04d",recu);
 UART1_Write_Text(bufferUART);
 UART1_Write_Text("\n\r");
 delay_ms(10);

 }
 UART1_Write_Text("end");
 UART1_Write_Text("\n\r");
 }

 memset(Rxtx_Data,0,sizeof(RxTx_Data));
 nombre = ADC_Read(0) >> 2;


 if(oldnbr != nombre)
 {
 oldnbr = nombre;
 memset(bufferLCD,0,sizeof(bufferLCD));
 sprintf(bufferLCD,"nbr : %04d", nombre);
 Lcd_Out(2,1,bufferLCD);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 RxTx_Data [0] = nombre;
 RxTx_Data[1] = j;
 CANSPIWrite(ID_2nd, RxTx_Data, 1, Can_Send_Flags);
 j++;
 }
 delay_ms(1000);
 }
}
