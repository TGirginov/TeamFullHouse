/* mbed Library for FTDI FT800  Enbedded Video Engine "EVE"
 * based on Original Code Sample from FTDI 
 * ported to mbed by Peter Drescher, DC2PD 2014
 * Released under the MIT License: http://mbed.org/license/mit */
 
#include "FT_Platform.h"
#include "mbed.h"
#include "FT_LCD_Type.h"

FT800::FT800(PinName mosi,
			PinName miso,
			PinName sck,
			PinName ss,
			PinName intr,
			PinName pd)
	:_spi(mosi, miso, sck),
	 _ss(ss),
	 _f800_isr(InterruptIn(intr)),
	 _pd(pd)
	 {
	 	 _ss = 0;
	 	 _spi.format(8,0);                  // 8 bit spi mode 0
    	 _spi.frequency(4000000);          // start with 10 Mhz SPI clock
    	 _ss = 1;                           // cs high
    	 _pd = 1;                           // PD high 
		 Bootup();   	
	 }


ft_bool_t FT800::Bootup(void){
	Ft_Gpu_Hal_Open();
	BootupConfig();
	
	return(1);
	}
	

ft_void_t FT800::BootupConfig(void){
	ft_uint8_t chipid;
	/* Do a power cycle for safer side */
	Ft_Gpu_Hal_Powercycle( FT_TRUE);

	/* Access address 0 to wake up the FT800 */
	Ft_Gpu_HostCommand( FT_GPU_ACTIVE_M);  
	Ft_Gpu_Hal_Sleep(20);

	/* Set the clk to external clock */
	Ft_Gpu_HostCommand( FT_GPU_EXTERNAL_OSC);  
	Ft_Gpu_Hal_Sleep(10);
	  

	/* Switch PLL output to 48MHz */
	Ft_Gpu_HostCommand( FT_GPU_PLL_48M);  
	Ft_Gpu_Hal_Sleep(10);

	/* Do a core reset for safer side */
	Ft_Gpu_HostCommand( FT_GPU_CORE_RESET);     

	//Read Register ID to check if FT800 is ready. 
	chipid = Ft_Gpu_Hal_Rd8(  REG_ID);
	while(chipid != 0x7C)
		chipid = Ft_Gpu_Hal_Rd8(  REG_ID);

	
	// Speed up 
	//_spi.frequency(16000000);           // 20 Mhz SPI clock
	
	/* Configuration of LCD display */
	Ft_Gpu_Hal_Wr8(  REG_PCLK,0);
	Ft_Gpu_Hal_Wr16(  REG_PWM_DUTY, 0);
    FT_DispHCycle = my_DispHCycle;
    Ft_Gpu_Hal_Wr16(  REG_HCYCLE, FT_DispHCycle);
    FT_DispHOffset = my_DispHOffset;
    Ft_Gpu_Hal_Wr16(  REG_HOFFSET, FT_DispHOffset);
    FT_DispWidth = my_DispWidth;
    Ft_Gpu_Hal_Wr16(  REG_HSIZE, FT_DispWidth);
    FT_DispHSync0 = my_DispHSync0;
    Ft_Gpu_Hal_Wr16(  REG_HSYNC0, FT_DispHSync0);
    FT_DispHSync1 = my_DispHSync1;
    Ft_Gpu_Hal_Wr16(  REG_HSYNC1, FT_DispHSync1);
    FT_DispVCycle = my_DispVCycle;
    Ft_Gpu_Hal_Wr16(  REG_VCYCLE, FT_DispVCycle);
    FT_DispVOffset = my_DispVOffset;
    Ft_Gpu_Hal_Wr16(  REG_VOFFSET, FT_DispVOffset);
    FT_DispHeight = my_DispHeight;
    Ft_Gpu_Hal_Wr16(  REG_VSIZE, FT_DispHeight);
    FT_DispVSync0 = my_DispVSync0;
    Ft_Gpu_Hal_Wr16(  REG_VSYNC0, FT_DispVSync0);
    FT_DispVSync1 = my_DispVSync1;
    Ft_Gpu_Hal_Wr16(  REG_VSYNC1, FT_DispVSync1);
    FT_DispSwizzle = my_DispSwizzle;
    Ft_Gpu_Hal_Wr8(  REG_SWIZZLE, FT_DispSwizzle);
    FT_DispPCLKPol = my_DispPCLKPol;
    Ft_Gpu_Hal_Wr8(  REG_PCLK_POL, FT_DispPCLKPol);
    FT_DispPCLK = my_DispPCLK;
    //Ft_Gpu_Hal_Wr8(  REG_PCLK,FT_DispPCLK);//after this display is visible on the LCD

	//Ft_Gpu_Hal_Wr16(  REG_PWM_HZ, 1000);
	
//#ifdef Inv_Backlite	
//	Ft_Gpu_Hal_Wr16(  REG_PWM_DUTY, 0);
//#else
//	Ft_Gpu_Hal_Wr16(  REG_PWM_DUTY, 100);
//#endif		
//	
    Ft_Gpu_Hal_Wr8(  REG_GPIO_DIR,0x82);  //| Ft_Gpu_Hal_Rd8( REG_GPIO_DIR));
    Ft_Gpu_Hal_Wr8(  REG_GPIO,0x080);     //| Ft_Gpu_Hal_Rd8( REG_GPIO));

	Ft_Gpu_Hal_Wr32(  RAM_DL, CLEAR_COLOR_RGB(0,0,0));	
	Ft_Gpu_Hal_Wr32(  RAM_DL+4, CLEAR(1,1,1));
	Ft_Gpu_Hal_Wr32(  RAM_DL+8, DISPLAY());
	Ft_Gpu_Hal_Wr32(  REG_DLSWAP,2);
	
	Ft_Gpu_Hal_Wr16(  REG_PCLK, FT_DispPCLK);
	
    /* Touch configuration - configure the resistance value to 1200 - this value is specific to customer requirement and derived by experiment */
    Ft_Gpu_Hal_Wr16(  REG_TOUCH_RZTHRESH,1200);
    
    for(int duty = 0; duty <= 128; duty++) {
        Ft_Gpu_Hal_Wr8(REG_PWM_DUTY, duty);     // Turn on backlight - ramp up slowly to full brighness
        wait(0.01);
    }

}



/* API to initialize the SPI interface */
ft_bool_t  FT800::Ft_Gpu_Hal_Init()
{
	// is done in constructor
	return 1;
}


ft_bool_t  FT800::Ft_Gpu_Hal_Open()
{
	ft_cmd_fifo_wp = ft_dl_buff_wp = 0;
	status = FT_GPU_HAL_OPENED;
	return 1;
}

ft_void_t  FT800::Ft_Gpu_Hal_Close( )
{
	status = FT_GPU_HAL_CLOSED;
}

ft_void_t FT800::Ft_Gpu_Hal_DeInit()
{

}

/*The APIs for reading/writing transfer continuously only with small buffer system*/
ft_void_t  FT800::Ft_Gpu_Hal_StartTransfer( FT_GPU_TRANSFERDIR_T rw,ft_uint32_t addr)
{
	if (FT_GPU_READ == rw){
		_ss = 0;       // cs low
		_spi.write(addr >> 16);
		_spi.write(addr >> 8);
		_spi.write(addr & 0xff);
		_spi.write(0); //Dummy Read Byte
		status = FT_GPU_HAL_READING;
	}else{
		_ss = 0;       // cs low
		_spi.write(0x80 | (addr >> 16));
		_spi.write(addr >> 8);
		_spi.write(addr & 0xff);
		status = FT_GPU_HAL_WRITING;
	}
}


/*The APIs for writing transfer continuously only*/
ft_void_t  FT800::Ft_Gpu_Hal_StartCmdTransfer( FT_GPU_TRANSFERDIR_T rw, ft_uint16_t count)
{
	Ft_Gpu_Hal_StartTransfer( rw, ft_cmd_fifo_wp + RAM_CMD);
}

ft_uint8_t  FT800::Ft_Gpu_Hal_TransferString( const ft_char8_t *string)
{
    ft_uint16_t length = strlen(string);
    while(length --){
       Ft_Gpu_Hal_Transfer8( *string);
       string ++;
    }
    //Append one null as ending flag
    return Ft_Gpu_Hal_Transfer8( 0);
}


ft_uint8_t  FT800::Ft_Gpu_Hal_Transfer8( ft_uint8_t value)
{
        return _spi.write(value);	
}


ft_uint16_t  FT800::Ft_Gpu_Hal_Transfer16( ft_uint16_t value)
{
	ft_uint16_t retVal = 0;

        if (status == FT_GPU_HAL_WRITING){
		Ft_Gpu_Hal_Transfer8( value & 0xFF);//LSB first
		Ft_Gpu_Hal_Transfer8( (value >> 8) & 0xFF);
	}else{
		retVal = Ft_Gpu_Hal_Transfer8( 0);
		retVal |= (ft_uint16_t)Ft_Gpu_Hal_Transfer8( 0) << 8;
	}

	return retVal;
}

ft_uint32_t  FT800::Ft_Gpu_Hal_Transfer32( ft_uint32_t value)
{
	ft_uint32_t retVal = 0;
	if (status == FT_GPU_HAL_WRITING){
		Ft_Gpu_Hal_Transfer16( value & 0xFFFF);//LSB first
		Ft_Gpu_Hal_Transfer16( (value >> 16) & 0xFFFF);
	}else{
		retVal = Ft_Gpu_Hal_Transfer16( 0);
		retVal |= (ft_uint32_t)Ft_Gpu_Hal_Transfer16( 0) << 16;
	}
	return retVal;
}

ft_void_t   FT800::Ft_Gpu_Hal_EndTransfer( )
{
	_ss = 1; 
	status = FT_GPU_HAL_OPENED;
}


ft_uint8_t  FT800::Ft_Gpu_Hal_Rd8( ft_uint32_t addr)
{
	ft_uint8_t value;
	Ft_Gpu_Hal_StartTransfer( FT_GPU_READ,addr);
	value = Ft_Gpu_Hal_Transfer8( 0);
	Ft_Gpu_Hal_EndTransfer( );
	return value;
}
ft_uint16_t FT800::Ft_Gpu_Hal_Rd16( ft_uint32_t addr)
{
	ft_uint16_t value;
	Ft_Gpu_Hal_StartTransfer( FT_GPU_READ,addr);
	value = Ft_Gpu_Hal_Transfer16( 0);
	Ft_Gpu_Hal_EndTransfer( );
	return value;
}
ft_uint32_t FT800::Ft_Gpu_Hal_Rd32( ft_uint32_t addr)
{
	ft_uint32_t value;
	Ft_Gpu_Hal_StartTransfer( FT_GPU_READ,addr);
	value = Ft_Gpu_Hal_Transfer32( 0);
	Ft_Gpu_Hal_EndTransfer( );
	return value;
}

ft_void_t FT800::Ft_Gpu_Hal_Wr8( ft_uint32_t addr, ft_uint8_t v)
{	
	Ft_Gpu_Hal_StartTransfer( FT_GPU_WRITE,addr);
	Ft_Gpu_Hal_Transfer8( v);
	Ft_Gpu_Hal_EndTransfer( );
}
ft_void_t FT800::Ft_Gpu_Hal_Wr16( ft_uint32_t addr, ft_uint16_t v)
{
	Ft_Gpu_Hal_StartTransfer( FT_GPU_WRITE,addr);
	Ft_Gpu_Hal_Transfer16( v);
	Ft_Gpu_Hal_EndTransfer( );
}
ft_void_t FT800::Ft_Gpu_Hal_Wr32( ft_uint32_t addr, ft_uint32_t v)
{
	Ft_Gpu_Hal_StartTransfer( FT_GPU_WRITE,addr);
	Ft_Gpu_Hal_Transfer32( v);
	Ft_Gpu_Hal_EndTransfer( );
}

ft_void_t FT800::Ft_Gpu_HostCommand( ft_uint8_t cmd)
{
  _ss = 0;
  _spi.write(cmd);
  _spi.write(0x00);
  _spi.write(0x00);
  _ss = 1;
}

ft_void_t FT800::Ft_Gpu_ClockSelect( FT_GPU_PLL_SOURCE_T pllsource)
{
   Ft_Gpu_HostCommand( pllsource);
}

ft_void_t FT800::Ft_Gpu_PLL_FreqSelect( FT_GPU_PLL_FREQ_T freq)
{
   Ft_Gpu_HostCommand( freq);
}

ft_void_t FT800::Ft_Gpu_PowerModeSwitch( FT_GPU_POWER_MODE_T pwrmode)
{
   Ft_Gpu_HostCommand( pwrmode);
}

ft_void_t FT800::Ft_Gpu_CoreReset( )
{
   Ft_Gpu_HostCommand( 0x68);
}


ft_void_t FT800::Ft_Gpu_Hal_Updatecmdfifo( ft_uint16_t count)
{
	 ft_cmd_fifo_wp  = ( ft_cmd_fifo_wp + count) & 4095;
	//4 byte alignment
	 ft_cmd_fifo_wp = ( ft_cmd_fifo_wp + 3) & 0xffc;
	Ft_Gpu_Hal_Wr16( REG_CMD_WRITE, ft_cmd_fifo_wp);
}


ft_uint16_t FT800::Ft_Gpu_Cmdfifo_Freespace( )
{
	ft_uint16_t fullness,retval;

	fullness = ( ft_cmd_fifo_wp - Ft_Gpu_Hal_Rd16( REG_CMD_READ)) & 4095;
	retval = (FT_CMD_FIFO_SIZE - 4) - fullness;
	return (retval);
}

ft_void_t FT800::Ft_Gpu_Hal_WrCmdBuf( ft_uint8_t *buffer,ft_uint16_t count)
{
	ft_uint32_t length =0, SizeTransfered = 0;   

#define MAX_CMD_FIFO_TRANSFER   Ft_Gpu_Cmdfifo_Freespace( )  
	do {                
		length = count;
		if (length > MAX_CMD_FIFO_TRANSFER){
		    length = MAX_CMD_FIFO_TRANSFER;
		}
      	        Ft_Gpu_Hal_CheckCmdBuffer( length);

                Ft_Gpu_Hal_StartCmdTransfer( FT_GPU_WRITE,length);

                SizeTransfered = 0;
		while (length--) {
                    Ft_Gpu_Hal_Transfer8( *buffer);
		    		buffer++;
                    SizeTransfered ++;
		}
                length = SizeTransfered;

		Ft_Gpu_Hal_EndTransfer( );
		Ft_Gpu_Hal_Updatecmdfifo( length);

		Ft_Gpu_Hal_WaitCmdfifo_empty( );

		count -= length;
	}while (count > 0);
}


ft_void_t FT800::Ft_Gpu_Hal_WrCmdBufFromFlash( FT_PROGMEM ft_prog_uchar8_t *buffer,ft_uint16_t count)
{
	ft_uint32_t length =0, SizeTransfered = 0;   

#define MAX_CMD_FIFO_TRANSFER   Ft_Gpu_Cmdfifo_Freespace( )  
	do {                
		length = count;
		if (length > MAX_CMD_FIFO_TRANSFER){
		    length = MAX_CMD_FIFO_TRANSFER;
		}
      	        Ft_Gpu_Hal_CheckCmdBuffer( length);

                Ft_Gpu_Hal_StartCmdTransfer( FT_GPU_WRITE,length);


                SizeTransfered = 0;
		while (length--) {
                    Ft_Gpu_Hal_Transfer8( ft_pgm_read_byte_near(buffer));
		    buffer++;
                    SizeTransfered ++;
		}
                length = SizeTransfered;

    	        Ft_Gpu_Hal_EndTransfer( );
		Ft_Gpu_Hal_Updatecmdfifo( length);

		Ft_Gpu_Hal_WaitCmdfifo_empty( );

		count -= length;
	}while (count > 0);
}


ft_void_t FT800::Ft_Gpu_Hal_CheckCmdBuffer( ft_uint16_t count)
{
   ft_uint16_t getfreespace;
   do{
        getfreespace = Ft_Gpu_Cmdfifo_Freespace( );
   }while(getfreespace < count);
}

ft_void_t FT800::Ft_Gpu_Hal_WaitCmdfifo_empty( )
{
   while(Ft_Gpu_Hal_Rd16( REG_CMD_READ) != Ft_Gpu_Hal_Rd16( REG_CMD_WRITE));
   
    ft_cmd_fifo_wp = Ft_Gpu_Hal_Rd16( REG_CMD_WRITE);
}

ft_void_t FT800::Ft_Gpu_Hal_WaitLogo_Finish( )
{
    ft_int16_t cmdrdptr,cmdwrptr;

    do{
         cmdrdptr = Ft_Gpu_Hal_Rd16( REG_CMD_READ);
         cmdwrptr = Ft_Gpu_Hal_Rd16( REG_CMD_WRITE);
    }while ((cmdwrptr != cmdrdptr) || (cmdrdptr != 0));
     ft_cmd_fifo_wp = 0;
}


ft_void_t FT800::Ft_Gpu_Hal_ResetCmdFifo( )
{
    ft_cmd_fifo_wp = 0;
}


ft_void_t FT800::Ft_Gpu_Hal_WrCmd32( ft_uint32_t cmd)
{
         Ft_Gpu_Hal_CheckCmdBuffer( sizeof(cmd));
      
         Ft_Gpu_Hal_Wr32( RAM_CMD +  ft_cmd_fifo_wp,cmd);
      
         Ft_Gpu_Hal_Updatecmdfifo( sizeof(cmd));
}


ft_void_t FT800::Ft_Gpu_Hal_ResetDLBuffer( )
{
            ft_dl_buff_wp = 0;
}

/* Toggle PD_N pin of FT800 board for a power cycle*/
ft_void_t FT800::Ft_Gpu_Hal_Powercycle(  ft_bool_t up)
{
	if (up)
	{
             //Toggle PD_N from low to high for power up switch  
            _pd = 0; 
            Ft_Gpu_Hal_Sleep(20);

            _pd = 1;
            Ft_Gpu_Hal_Sleep(20);
	}else
	{
             //Toggle PD_N from high to low for power down switch
            _pd = 1;
            Ft_Gpu_Hal_Sleep(20);
            
            _pd = 0;
            Ft_Gpu_Hal_Sleep(20);
	}
}

ft_void_t FT800::Ft_Gpu_Hal_WrMemFromFlash( ft_uint32_t addr,const ft_prog_uchar8_t *buffer, ft_uint32_t length)
{
	ft_uint32_t SizeTransfered = 0;      

	Ft_Gpu_Hal_StartTransfer( FT_GPU_WRITE,addr);

	while (length--) {
            Ft_Gpu_Hal_Transfer8( ft_pgm_read_byte_near(buffer));
	    buffer++;
	}

	Ft_Gpu_Hal_EndTransfer( );
}

ft_void_t FT800::Ft_Gpu_Hal_WrMem( ft_uint32_t addr,const ft_uint8_t *buffer, ft_uint32_t length)
{
	ft_uint32_t SizeTransfered = 0;      

	Ft_Gpu_Hal_StartTransfer( FT_GPU_WRITE,addr);

	while (length--) {
            Ft_Gpu_Hal_Transfer8( *buffer);
	    buffer++;
	}

	Ft_Gpu_Hal_EndTransfer( );
}


ft_void_t FT800::Ft_Gpu_Hal_RdMem( ft_uint32_t addr, ft_uint8_t *buffer, ft_uint32_t length)
{
	ft_uint32_t SizeTransfered = 0;      

	Ft_Gpu_Hal_StartTransfer( FT_GPU_READ,addr);

	while (length--) {
	   *buffer = Ft_Gpu_Hal_Transfer8( 0);
	   buffer++;
	}

	Ft_Gpu_Hal_EndTransfer( );
}

ft_int32_t FT800::Ft_Gpu_Hal_Dec2Ascii(ft_char8_t *pSrc,ft_int32_t value)
{
	ft_int16_t Length;
	ft_char8_t *pdst,charval;
	ft_int32_t CurrVal = value,tmpval,i;
	ft_char8_t tmparray[16],idx = 0;

	Length = strlen(pSrc);
	pdst = pSrc + Length;

	if(0 == value)
	{
		*pdst++ = '0';
		*pdst++ = '\0';
		return 0;
	}

	if(CurrVal < 0)
	{
		*pdst++ = '-';
		CurrVal = - CurrVal;
	}
	/* insert the value */
	while(CurrVal > 0){
		tmpval = CurrVal;
		CurrVal /= 10;
		tmpval = tmpval - CurrVal*10;
		charval = '0' + tmpval;
		tmparray[idx++] = charval;
	}

	for(i=0;i<idx;i++)
	{
		*pdst++ = tmparray[idx - i - 1];
	}
	*pdst++ = '\0';

	return 0;
}


ft_void_t FT800::Ft_Gpu_Hal_Sleep(ft_uint16_t ms)
{
	wait_ms(ms);
}

ft_void_t FT800::Ft_Sound_ON(){
	 Ft_Gpu_Hal_Wr8(  REG_GPIO, 0x02 | Ft_Gpu_Hal_Rd8( REG_GPIO));
}

ft_void_t FT800::Ft_Sound_OFF(){
	 Ft_Gpu_Hal_Wr8(  REG_GPIO, 0xFD & Ft_Gpu_Hal_Rd8( REG_GPIO));
}




