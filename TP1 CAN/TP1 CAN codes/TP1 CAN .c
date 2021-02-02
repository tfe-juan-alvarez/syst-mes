
//
// Created by Juan & Oli on 5/14/2020.
// on  vérifie si ça marche
//

unsigned char Can_Init_Flags, Can_Send_Flags, Can_Rcv_Flags; // can flags
unsigned char Rx_Data_Len;                                   // received data length in bytes
char RxTx_Data[8] = {0};                                           // can rx/tx data buffer
char Msg_Rcvd;                                               // reception flag
const long ID_1st = 12111, ID_2nd = 3;                       // node IDs
long Rx_ID;
int recu;
int i;
int j;
char bufferLCD[16];
int nombre,oldnbr;
char bufferUART[16];

// LCD module connections
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
// End LCD module connections

// CANSPI module connections
sbit CanSpi_CS            at  RC0_bit;
sbit CanSpi_CS_Direction  at  TRISC0_bit;
sbit CanSpi_Rst           at  RC2_bit;
sbit CanSpi_Rst_Direction at  TRISC2_bit;
// End CANSPI module connections



void main()
{
  ANSELA = 0x01;                      // Configure AN pins as digital I/O
  ANSELB = 0;
  ANSELC = 0;
  ANSELD = 0;

  C1ON_bit = 0;               // Disable comparators
  C2ON_bit = 0;

  TRISD = 0x00;
  TRISA = 0x01;                          // OUTPUT = 0 Iput = 1
  TRISB = 0x00;
  TRISC = 0x00;



  Can_Init_Flags = 0;                                         //
  Can_Send_Flags = 0;                                         // clear flags
  Can_Rcv_Flags  = 0;                                         //

  ADC_Init();

  Lcd_Init();

UART1_Init(9600);

  Delay_ms(200);
  
  Can_Send_Flags = _CANSPI_TX_PRIORITY_0 &                    // form value to be used
                   _CANSPI_TX_XTD_FRAME &                     //     with CANSPIWrite
                   _CANSPI_TX_NO_RTR_FRAME;

  Can_Init_Flags = _CANSPI_CONFIG_SAMPLE_THRICE &             // Form value to be used
                   _CANSPI_CONFIG_PHSEG2_PRG_ON &             //  with CANSPIInit
                   _CANSPI_CONFIG_XTD_MSG &
                   _CANSPI_CONFIG_DBL_BUFFER_ON &
                   _CANSPI_CONFIG_VALID_XTD_MSG;


  SPI1_Init();                                                     // initialize SPI1 module

  CANSPIInitialize(1,3,3,3,1,Can_Init_Flags);                      // Initialize external CANSPI module
  CANSPISetOperationMode(_CANSPI_MODE_CONFIG,0xFF);                // set CONFIGURATION mode
  CANSPISetMask(_CANSPI_MASK_B1,-1,_CANSPI_CONFIG_XTD_MSG);        // set all mask1 bits to ones
  CANSPISetMask(_CANSPI_MASK_B2,-1,_CANSPI_CONFIG_XTD_MSG);        // set all mask2 bits to ones
  CANSPISetFilter(_CANSPI_FILTER_B2_F4,ID_1st,_CANSPI_CONFIG_XTD_MSG);// set id of filter B2_F4 to 2nd node ID

  CANSPISetOperationMode(_CANSPI_MODE_NORMAL,0xFF);                // set NORMAL mode

  //RxTx_Data[0] = {};                                                // set initial data to be sent
   Lcd_Cmd(_LCD_CLEAR);
   Lcd_Out(1,1,"yol0");

  for(;;)
    {
    memset(Rxtx_Data,0,sizeof(RxTx_Data));
      Msg_Rcvd = CANSPIRead(&Rx_ID , RxTx_Data , &Rx_Data_Len, &Can_Rcv_Flags);// receive message
      if ((Rx_ID == ID_1st) && Msg_Rcvd) // if message received check id
        {

        for(i=0; i < 8 ; i++)
        {

          recu = RxTx_Data[i];
          //memset(bufferUART,0,sizeof(bufferUART));

          sprintf(bufferUART,"nbr rcvd : %04d",recu);
          UART1_Write_Text(bufferUART);
          UART1_Write_Text("\n\r");
          delay_ms(10);
          
        }
        UART1_Write_Text("end");
        UART1_Write_Text("\n\r");
      }
      //}
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
        CANSPIWrite(ID_2nd, RxTx_Data, 1, Can_Send_Flags);     // send data
        j++;
      }
      delay_ms(1000);
    }
}
