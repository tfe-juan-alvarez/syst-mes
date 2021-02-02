
_main:

;MyProjectffffffffff.c,44 :: 		void main()
;MyProjectffffffffff.c,46 :: 		ANSELA = 0x01;                      // Configure AN pins as digital I/O
	MOVLW       1
	MOVWF       ANSELA+0 
;MyProjectffffffffff.c,47 :: 		ANSELB = 0;
	CLRF        ANSELB+0 
;MyProjectffffffffff.c,48 :: 		ANSELC = 0;
	CLRF        ANSELC+0 
;MyProjectffffffffff.c,49 :: 		ANSELD = 0;
	CLRF        ANSELD+0 
;MyProjectffffffffff.c,51 :: 		C1ON_bit = 0;               // Disable comparators
	BCF         C1ON_bit+0, BitPos(C1ON_bit+0) 
;MyProjectffffffffff.c,52 :: 		C2ON_bit = 0;
	BCF         C2ON_bit+0, BitPos(C2ON_bit+0) 
;MyProjectffffffffff.c,54 :: 		TRISD = 0x00;
	CLRF        TRISD+0 
;MyProjectffffffffff.c,55 :: 		TRISA = 0x01;                          // OUTPUT = 0 Iput = 1
	MOVLW       1
	MOVWF       TRISA+0 
;MyProjectffffffffff.c,56 :: 		TRISB = 0x00;
	CLRF        TRISB+0 
;MyProjectffffffffff.c,57 :: 		TRISC = 0x00;
	CLRF        TRISC+0 
;MyProjectffffffffff.c,61 :: 		Can_Init_Flags = 0;                                         //
	CLRF        _Can_Init_Flags+0 
;MyProjectffffffffff.c,62 :: 		Can_Send_Flags = 0;                                         // clear flags
	CLRF        _Can_Send_Flags+0 
;MyProjectffffffffff.c,63 :: 		Can_Rcv_Flags  = 0;                                         //
	CLRF        _Can_Rcv_Flags+0 
;MyProjectffffffffff.c,65 :: 		ADC_Init();
	CALL        _ADC_Init+0, 0
;MyProjectffffffffff.c,67 :: 		Lcd_Init();
	CALL        _Lcd_Init+0, 0
;MyProjectffffffffff.c,69 :: 		UART1_Init(9600);
	BSF         BAUDCON+0, 3, 0
	CLRF        SPBRGH+0 
	MOVLW       207
	MOVWF       SPBRG+0 
	BSF         TXSTA+0, 2, 0
	CALL        _UART1_Init+0, 0
;MyProjectffffffffff.c,71 :: 		Delay_ms(200);
	MOVLW       3
	MOVWF       R11, 0
	MOVLW       8
	MOVWF       R12, 0
	MOVLW       119
	MOVWF       R13, 0
L_main0:
	DECFSZ      R13, 1, 1
	BRA         L_main0
	DECFSZ      R12, 1, 1
	BRA         L_main0
	DECFSZ      R11, 1, 1
	BRA         L_main0
;MyProjectffffffffff.c,75 :: 		_CANSPI_TX_NO_RTR_FRAME;
	MOVLW       244
	MOVWF       _Can_Send_Flags+0 
;MyProjectffffffffff.c,81 :: 		_CANSPI_CONFIG_VALID_XTD_MSG;
	MOVLW       211
	MOVWF       _Can_Init_Flags+0 
;MyProjectffffffffff.c,84 :: 		SPI1_Init();                                                     // initialize SPI1 module
	CALL        _SPI1_Init+0, 0
;MyProjectffffffffff.c,86 :: 		CANSPIInitialize(1,3,3,3,1,Can_Init_Flags);                      // Initialize external CANSPI module
	MOVLW       1
	MOVWF       FARG_CANSPIInitialize_SJW+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_BRP+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_PHSEG1+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_PHSEG2+0 
	MOVLW       1
	MOVWF       FARG_CANSPIInitialize_PROPSEG+0 
	MOVF        _Can_Init_Flags+0, 0 
	MOVWF       FARG_CANSPIInitialize_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPIInitialize+0, 0
;MyProjectffffffffff.c,87 :: 		CANSPISetOperationMode(_CANSPI_MODE_CONFIG,0xFF);                // set CONFIGURATION mode
	MOVLW       128
	MOVWF       FARG_CANSPISetOperationMode_mode+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetOperationMode_WAIT+0 
	CALL        _CANSPISetOperationMode+0, 0
;MyProjectffffffffff.c,88 :: 		CANSPISetMask(_CANSPI_MASK_B1,-1,_CANSPI_CONFIG_XTD_MSG);        // set all mask1 bits to ones
	CLRF        FARG_CANSPISetMask_CANSPI_MASK+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+1 
	MOVWF       FARG_CANSPISetMask_val+2 
	MOVWF       FARG_CANSPISetMask_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetMask_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetMask+0, 0
;MyProjectffffffffff.c,89 :: 		CANSPISetMask(_CANSPI_MASK_B2,-1,_CANSPI_CONFIG_XTD_MSG);        // set all mask2 bits to ones
	MOVLW       1
	MOVWF       FARG_CANSPISetMask_CANSPI_MASK+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+1 
	MOVWF       FARG_CANSPISetMask_val+2 
	MOVWF       FARG_CANSPISetMask_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetMask_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetMask+0, 0
;MyProjectffffffffff.c,90 :: 		CANSPISetFilter(_CANSPI_FILTER_B2_F4,ID_1st,_CANSPI_CONFIG_XTD_MSG);// set id of filter B2_F4 to 2nd node ID
	MOVLW       5
	MOVWF       FARG_CANSPISetFilter_CANSPI_FILTER+0 
	MOVLW       79
	MOVWF       FARG_CANSPISetFilter_val+0 
	MOVLW       47
	MOVWF       FARG_CANSPISetFilter_val+1 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+2 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetFilter_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetFilter+0, 0
;MyProjectffffffffff.c,92 :: 		CANSPISetOperationMode(_CANSPI_MODE_NORMAL,0xFF);                // set NORMAL mode
	CLRF        FARG_CANSPISetOperationMode_mode+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetOperationMode_WAIT+0 
	CALL        _CANSPISetOperationMode+0, 0
;MyProjectffffffffff.c,95 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectffffffffff.c,96 :: 		Lcd_Out(1,1,"yol0");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr1_MyProjectffffffffff+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr1_MyProjectffffffffff+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectffffffffff.c,98 :: 		for(;;)
L_main1:
;MyProjectffffffffff.c,100 :: 		memset(Rxtx_Data,0,sizeof(RxTx_Data));
	MOVLW       _RxTx_Data+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_RxTx_Data+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       8
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;MyProjectffffffffff.c,101 :: 		Msg_Rcvd = CANSPIRead(&Rx_ID , RxTx_Data , &Rx_Data_Len, &Can_Rcv_Flags);// receive message
	MOVLW       _Rx_ID+0
	MOVWF       FARG_CANSPIRead_id+0 
	MOVLW       hi_addr(_Rx_ID+0)
	MOVWF       FARG_CANSPIRead_id+1 
	MOVLW       _RxTx_Data+0
	MOVWF       FARG_CANSPIRead_rd_data+0 
	MOVLW       hi_addr(_RxTx_Data+0)
	MOVWF       FARG_CANSPIRead_rd_data+1 
	MOVLW       _Rx_Data_Len+0
	MOVWF       FARG_CANSPIRead_data_len+0 
	MOVLW       hi_addr(_Rx_Data_Len+0)
	MOVWF       FARG_CANSPIRead_data_len+1 
	MOVLW       _Can_Rcv_Flags+0
	MOVWF       FARG_CANSPIRead_CANSPI_RX_MSG_FLAGS+0 
	MOVLW       hi_addr(_Can_Rcv_Flags+0)
	MOVWF       FARG_CANSPIRead_CANSPI_RX_MSG_FLAGS+1 
	CALL        _CANSPIRead+0, 0
	MOVF        R0, 0 
	MOVWF       _Msg_Rcvd+0 
;MyProjectffffffffff.c,102 :: 		if ((Rx_ID == ID_1st) && Msg_Rcvd) // if message received check id
	MOVF        _Rx_ID+3, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L__main15
	MOVF        _Rx_ID+2, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L__main15
	MOVF        _Rx_ID+1, 0 
	XORLW       47
	BTFSS       STATUS+0, 2 
	GOTO        L__main15
	MOVF        _Rx_ID+0, 0 
	XORLW       79
L__main15:
	BTFSS       STATUS+0, 2 
	GOTO        L_main6
	MOVF        _Msg_Rcvd+0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main6
L__main13:
;MyProjectffffffffff.c,105 :: 		for(i=0; i < 8 ; i++)
	CLRF        _i+0 
	CLRF        _i+1 
L_main7:
	MOVLW       128
	XORWF       _i+1, 0 
	MOVWF       R0 
	MOVLW       128
	SUBWF       R0, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main16
	MOVLW       8
	SUBWF       _i+0, 0 
L__main16:
	BTFSC       STATUS+0, 0 
	GOTO        L_main8
;MyProjectffffffffff.c,108 :: 		recu = RxTx_Data[i];
	MOVLW       _RxTx_Data+0
	ADDWF       _i+0, 0 
	MOVWF       FSR0 
	MOVLW       hi_addr(_RxTx_Data+0)
	ADDWFC      _i+1, 0 
	MOVWF       FSR0H 
	MOVF        POSTINC0+0, 0 
	MOVWF       _recu+0 
	MOVLW       0
	MOVWF       _recu+1 
	MOVLW       0
	MOVWF       _recu+1 
;MyProjectffffffffff.c,111 :: 		sprintf(bufferUART,"nbr rcvd : %04d",recu);
	MOVLW       _bufferUART+0
	MOVWF       FARG_sprintf_wh+0 
	MOVLW       hi_addr(_bufferUART+0)
	MOVWF       FARG_sprintf_wh+1 
	MOVLW       ?lstr_2_MyProjectffffffffff+0
	MOVWF       FARG_sprintf_f+0 
	MOVLW       hi_addr(?lstr_2_MyProjectffffffffff+0)
	MOVWF       FARG_sprintf_f+1 
	MOVLW       higher_addr(?lstr_2_MyProjectffffffffff+0)
	MOVWF       FARG_sprintf_f+2 
	MOVF        _recu+0, 0 
	MOVWF       FARG_sprintf_wh+5 
	MOVF        _recu+1, 0 
	MOVWF       FARG_sprintf_wh+6 
	CALL        _sprintf+0, 0
;MyProjectffffffffff.c,112 :: 		UART1_Write_Text(bufferUART);
	MOVLW       _bufferUART+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(_bufferUART+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;MyProjectffffffffff.c,113 :: 		UART1_Write_Text("\n\r");
	MOVLW       ?lstr3_MyProjectffffffffff+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(?lstr3_MyProjectffffffffff+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;MyProjectffffffffff.c,114 :: 		delay_ms(10);
	MOVLW       26
	MOVWF       R12, 0
	MOVLW       248
	MOVWF       R13, 0
L_main10:
	DECFSZ      R13, 1, 1
	BRA         L_main10
	DECFSZ      R12, 1, 1
	BRA         L_main10
	NOP
;MyProjectffffffffff.c,105 :: 		for(i=0; i < 8 ; i++)
	INFSNZ      _i+0, 1 
	INCF        _i+1, 1 
;MyProjectffffffffff.c,116 :: 		}
	GOTO        L_main7
L_main8:
;MyProjectffffffffff.c,117 :: 		UART1_Write_Text("end");
	MOVLW       ?lstr4_MyProjectffffffffff+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(?lstr4_MyProjectffffffffff+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;MyProjectffffffffff.c,118 :: 		UART1_Write_Text("\n\r");
	MOVLW       ?lstr5_MyProjectffffffffff+0
	MOVWF       FARG_UART1_Write_Text_uart_text+0 
	MOVLW       hi_addr(?lstr5_MyProjectffffffffff+0)
	MOVWF       FARG_UART1_Write_Text_uart_text+1 
	CALL        _UART1_Write_Text+0, 0
;MyProjectffffffffff.c,119 :: 		}
L_main6:
;MyProjectffffffffff.c,121 :: 		memset(Rxtx_Data,0,sizeof(RxTx_Data));
	MOVLW       _RxTx_Data+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_RxTx_Data+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       8
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;MyProjectffffffffff.c,122 :: 		nombre = ADC_Read(0) >> 2;
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       R2 
	MOVF        R1, 0 
	MOVWF       R3 
	RRCF        R3, 1 
	RRCF        R2, 1 
	BCF         R3, 7 
	RRCF        R3, 1 
	RRCF        R2, 1 
	BCF         R3, 7 
	MOVF        R2, 0 
	MOVWF       _nombre+0 
	MOVF        R3, 0 
	MOVWF       _nombre+1 
;MyProjectffffffffff.c,125 :: 		if(oldnbr != nombre)
	MOVF        _oldnbr+1, 0 
	XORWF       R3, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main17
	MOVF        R2, 0 
	XORWF       _oldnbr+0, 0 
L__main17:
	BTFSC       STATUS+0, 2 
	GOTO        L_main11
;MyProjectffffffffff.c,127 :: 		oldnbr = nombre;
	MOVF        _nombre+0, 0 
	MOVWF       _oldnbr+0 
	MOVF        _nombre+1, 0 
	MOVWF       _oldnbr+1 
;MyProjectffffffffff.c,128 :: 		memset(bufferLCD,0,sizeof(bufferLCD));
	MOVLW       _bufferLCD+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       16
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;MyProjectffffffffff.c,129 :: 		sprintf(bufferLCD,"nbr : %04d", nombre);
	MOVLW       _bufferLCD+0
	MOVWF       FARG_sprintf_wh+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_sprintf_wh+1 
	MOVLW       ?lstr_6_MyProjectffffffffff+0
	MOVWF       FARG_sprintf_f+0 
	MOVLW       hi_addr(?lstr_6_MyProjectffffffffff+0)
	MOVWF       FARG_sprintf_f+1 
	MOVLW       higher_addr(?lstr_6_MyProjectffffffffff+0)
	MOVWF       FARG_sprintf_f+2 
	MOVF        _nombre+0, 0 
	MOVWF       FARG_sprintf_wh+5 
	MOVF        _nombre+1, 0 
	MOVWF       FARG_sprintf_wh+6 
	CALL        _sprintf+0, 0
;MyProjectffffffffff.c,130 :: 		Lcd_Out(2,1,bufferLCD);
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _bufferLCD+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;MyProjectffffffffff.c,131 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);
	MOVLW       12
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;MyProjectffffffffff.c,132 :: 		RxTx_Data [0] = nombre;
	MOVF        _nombre+0, 0 
	MOVWF       _RxTx_Data+0 
;MyProjectffffffffff.c,133 :: 		RxTx_Data[1] = j;
	MOVF        _j+0, 0 
	MOVWF       _RxTx_Data+1 
;MyProjectffffffffff.c,134 :: 		CANSPIWrite(ID_2nd, RxTx_Data, 1, Can_Send_Flags);     // send data
	MOVLW       3
	MOVWF       FARG_CANSPIWrite_id+0 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+1 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+2 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+3 
	MOVLW       _RxTx_Data+0
	MOVWF       FARG_CANSPIWrite_wr_data+0 
	MOVLW       hi_addr(_RxTx_Data+0)
	MOVWF       FARG_CANSPIWrite_wr_data+1 
	MOVLW       1
	MOVWF       FARG_CANSPIWrite_data_len+0 
	MOVF        _Can_Send_Flags+0, 0 
	MOVWF       FARG_CANSPIWrite_CANSPI_TX_MSG_FLAGS+0 
	CALL        _CANSPIWrite+0, 0
;MyProjectffffffffff.c,135 :: 		j++;
	INFSNZ      _j+0, 1 
	INCF        _j+1, 1 
;MyProjectffffffffff.c,136 :: 		}
L_main11:
;MyProjectffffffffff.c,137 :: 		delay_ms(1000);
	MOVLW       11
	MOVWF       R11, 0
	MOVLW       38
	MOVWF       R12, 0
	MOVLW       93
	MOVWF       R13, 0
L_main12:
	DECFSZ      R13, 1, 1
	BRA         L_main12
	DECFSZ      R12, 1, 1
	BRA         L_main12
	DECFSZ      R11, 1, 1
	BRA         L_main12
	NOP
	NOP
;MyProjectffffffffff.c,138 :: 		}
	GOTO        L_main1
;MyProjectffffffffff.c,139 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
