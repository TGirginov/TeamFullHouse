/*!
 * \file FT_GPU_HAL.h
 *
 * \author FTDI
 * \date 2013.04.24
 *
 * Copyright 2013 Future Technology Devices International Limited
 *
 * Project: FT800 or EVE compatible silicon
 * File Description:
 *    This file defines the generic APIs of host access layer for the FT800 or EVE compatible silicon.
 *    Application shall access FT800 or EVE resources over these APIs. In addition, there are
 *    some helper functions defined for FT800 coprocessor engine as well as host commands.
 * Rivision History:
 * ported to mbed by Peter Drescher, DC2PD 2014
 */
 
#ifndef FT_GPU_HAL_H
#define FT_GPU_HAL_H

#include "mbed.h"
#include "FT_DataTypes.h"

typedef enum {
    FT_GPU_I2C_MODE = 0,
    FT_GPU_SPI_MODE,

    FT_GPU_MODE_COUNT,
    FT_GPU_MODE_UNKNOWN = FT_GPU_MODE_COUNT
} FT_GPU_HAL_MODE_E;

typedef enum {
    FT_GPU_HAL_OPENED,
    FT_GPU_HAL_READING,
    FT_GPU_HAL_WRITING,
    FT_GPU_HAL_CLOSED,

    FT_GPU_HAL_STATUS_COUNT,
    FT_GPU_HAL_STATUS_ERROR = FT_GPU_HAL_STATUS_COUNT
} FT_GPU_HAL_STATUS_E;

typedef struct {
    ft_uint8_t reserved;
} Ft_Gpu_App_Context_t;

typedef struct {
    /* Total number channels for libmpsse */
    ft_uint32_t TotalChannelNum;
} Ft_Gpu_HalInit_t;

typedef enum {
    FT_GPU_READ = 0,
    FT_GPU_WRITE,
} FT_GPU_TRANSFERDIR_T;


typedef struct {
    ft_uint32_t length; //IN and OUT
    ft_uint32_t address;
    ft_uint8_t  *buffer;
} Ft_Gpu_App_Transfer_t;

class FT800
{
public:
    FT800(PinName mosi,
          PinName miso,
          PinName sck,
          PinName ss,
          PinName intr,
          PinName pd);

private:
    SPI _spi;
    DigitalOut	_ss;
    DigitalOut	_pd;
    InterruptIn	_f800_isr;
public:
    /* Global used for buffer optimization */
    //Ft_Gpu_Hal_Context_t host,*phost;
    Ft_Gpu_App_Context_t        app_header;
    ft_uint16_t ft_cmd_fifo_wp; //coprocessor fifo write pointer
    ft_uint16_t ft_dl_buff_wp;  //display command memory write pointer
    FT_GPU_HAL_STATUS_E        status;        	  //OUT
    ft_void_t*                 hal_handle;        //IN/OUT
    ft_uint32_t Ft_CmdBuffer_Index;
    ft_uint32_t Ft_DlBuffer_Index;
    ft_int16_t FT_DispWidth;
    ft_int16_t FT_DispHeight;
    ft_int16_t FT_DispHCycle;
    ft_int16_t FT_DispHOffset;
    ft_int16_t FT_DispHSync0;
    ft_int16_t FT_DispHSync1;
    ft_int16_t FT_DispVCycle;
    ft_int16_t FT_DispVOffset;
    ft_int16_t FT_DispVSync0;
    ft_int16_t FT_DispVSync1;
    ft_uint8_t FT_DispPCLK;
    ft_char8_t FT_DispSwizzle;
    ft_char8_t FT_DispPCLKPol;


    ft_void_t BootupConfig(void);
    ft_bool_t Bootup(void);


    /*The basic APIs Level 1*/
    ft_bool_t              Ft_Gpu_Hal_Init( );
    ft_bool_t              Ft_Gpu_Hal_Open( );

    /*The APIs for reading/writing transfer continuously only with small buffer system*/
    ft_void_t               Ft_Gpu_Hal_StartTransfer(FT_GPU_TRANSFERDIR_T rw,ft_uint32_t addr);
    ft_uint8_t              Ft_Gpu_Hal_Transfer8(ft_uint8_t value);
    ft_uint16_t             Ft_Gpu_Hal_Transfer16(ft_uint16_t value);
    ft_uint32_t             Ft_Gpu_Hal_Transfer32(ft_uint32_t value);
    ft_void_t               Ft_Gpu_Hal_EndTransfer( );

    /*Read & Write APIs for both burst and single transfer,depending on buffer size*/
    ft_void_t              Ft_Gpu_Hal_Read(Ft_Gpu_App_Transfer_t *transfer);
    ft_void_t              Ft_Gpu_Hal_Write(Ft_Gpu_App_Transfer_t *transfer);

    ft_void_t              Ft_Gpu_Hal_Close();
    ft_void_t              Ft_Gpu_Hal_DeInit();

    /*Helper function APIs Read*/
    ft_uint8_t  Ft_Gpu_Hal_Rd8(ft_uint32_t addr);
    ft_uint16_t Ft_Gpu_Hal_Rd16(ft_uint32_t addr);
    ft_uint32_t Ft_Gpu_Hal_Rd32(ft_uint32_t addr);

    /*Helper function APIs Write*/
    ft_void_t Ft_Gpu_Hal_Wr8(ft_uint32_t addr, ft_uint8_t v);
    ft_void_t Ft_Gpu_Hal_Wr16(ft_uint32_t addr, ft_uint16_t v);
    ft_void_t Ft_Gpu_Hal_Wr32(ft_uint32_t addr, ft_uint32_t v);

    /*******************************************************************************/
    /*******************************************************************************/
    /*APIs for coprocessor Fifo read/write and space management*/
    ft_void_t Ft_Gpu_Hal_Updatecmdfifo(ft_uint16_t count);
    ft_void_t Ft_Gpu_Hal_WrCmd32(ft_uint32_t cmd);
    ft_void_t Ft_Gpu_Hal_WrCmdBuf(ft_uint8_t *buffer,ft_uint16_t count);
    ft_void_t Ft_Gpu_Hal_WaitCmdfifo_empty();
    ft_void_t Ft_Gpu_Hal_ResetCmdFifo();
    ft_void_t Ft_Gpu_Hal_CheckCmdBuffer(ft_uint16_t count);
    ft_void_t Ft_Gpu_Hal_ResetDLBuffer();

    ft_void_t  Ft_Gpu_Hal_StartCmdTransfer(FT_GPU_TRANSFERDIR_T rw, ft_uint16_t count);
    ft_void_t Ft_Gpu_Hal_Powercycle(ft_bool_t up);


    /*******************************************************************************/
    /*******************************************************************************/
    /*APIs for Host Commands*/
    typedef enum {
        FT_GPU_INTERNAL_OSC = 0x48, //default
        FT_GPU_EXTERNAL_OSC = 0x44,
    } FT_GPU_PLL_SOURCE_T;
    typedef enum {
        FT_GPU_PLL_48M = 0x62,  //default
        FT_GPU_PLL_36M = 0x61,
        FT_GPU_PLL_24M = 0x64,
    } FT_GPU_PLL_FREQ_T;

    typedef enum {
        FT_GPU_ACTIVE_M =  0x00,
        FT_GPU_STANDBY_M = 0x41,//default
        FT_GPU_SLEEP_M =   0x42,
        FT_GPU_POWERDOWN_M = 0x50,
    } FT_GPU_POWER_MODE_T;

#define FT_GPU_CORE_RESET  (0x68)

    ft_int32_t hal_strlen(const ft_char8_t *s);
    ft_void_t Ft_Gpu_Hal_Sleep(ft_uint16_t ms);
    ft_void_t Ft_Gpu_ClockSelect(FT_GPU_PLL_SOURCE_T pllsource);
    ft_void_t Ft_Gpu_PLL_FreqSelect(FT_GPU_PLL_FREQ_T freq);
    ft_void_t Ft_Gpu_PowerModeSwitch(FT_GPU_POWER_MODE_T pwrmode);
    ft_void_t Ft_Gpu_CoreReset();
//ft_void_t Ft_Gpu_Hal_StartTransfer( ,FT_GPU_TRANSFERDIR_T rw,ft_uint32_t addr);
    ft_void_t Ft_Gpu_Hal_WrMem(ft_uint32_t addr, const ft_uint8_t *buffer, ft_uint32_t length);
    ft_void_t Ft_Gpu_Hal_WrMemFromFlash(ft_uint32_t addr,const ft_prog_uchar8_t *buffer, ft_uint32_t length);
    ft_void_t Ft_Gpu_Hal_WrCmdBufFromFlash(FT_PROGMEM ft_prog_uchar8_t *buffer,ft_uint16_t count);
    ft_void_t Ft_Gpu_Hal_RdMem(ft_uint32_t addr, ft_uint8_t *buffer, ft_uint32_t length);
    ft_void_t Ft_Gpu_Hal_WaitLogo_Finish();
    ft_uint8_t Ft_Gpu_Hal_TransferString(const ft_char8_t *string);
    ft_void_t Ft_Gpu_HostCommand(ft_uint8_t cmd);
    ft_int32_t Ft_Gpu_Hal_Dec2Ascii(ft_char8_t *pSrc,ft_int32_t value);

    ft_void_t Ft_Gpu_CoCmd_Text(ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);
    ft_void_t Ft_Gpu_CoCmd_Number(ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, ft_int32_t n);
    ft_void_t Ft_Gpu_CoCmd_LoadIdentity();
    ft_void_t Ft_Gpu_CoCmd_Toggle(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t font, ft_uint16_t options, ft_uint16_t state, const ft_char8_t* s);
    ft_void_t Ft_Gpu_CoCmd_Gauge(ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t major, ft_uint16_t minor, ft_uint16_t val, ft_uint16_t range);
    ft_void_t Ft_Gpu_CoCmd_RegRead(ft_uint32_t ptr, ft_uint32_t result);
    ft_void_t Ft_Gpu_CoCmd_GetProps(ft_uint32_t ptr, ft_uint32_t w, ft_uint32_t h);
    ft_void_t Ft_Gpu_CoCmd_Memcpy(ft_uint32_t dest, ft_uint32_t src, ft_uint32_t num);
    ft_void_t Ft_Gpu_CoCmd_Spinner(ft_int16_t x, ft_int16_t y, ft_uint16_t style, ft_uint16_t scale);
    ft_void_t Ft_Gpu_CoCmd_BgColor(ft_uint32_t c);
    ft_void_t Ft_Gpu_CoCmd_Swap();
    ft_void_t Ft_Gpu_CoCmd_Inflate(ft_uint32_t ptr);
    ft_void_t Ft_Gpu_CoCmd_Translate(ft_int32_t tx, ft_int32_t ty);
    ft_void_t Ft_Gpu_CoCmd_Stop();
    ft_void_t Ft_Gpu_CoCmd_Slider(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range);
    ft_void_t Ft_Gpu_CoCmd_Interrupt(ft_uint32_t ms);
    ft_void_t Ft_Gpu_CoCmd_FgColor(ft_uint32_t c);
    ft_void_t Ft_Gpu_CoCmd_Rotate(ft_int32_t a);
    ft_void_t Ft_Gpu_CoCmd_Button(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);
    ft_void_t Ft_Gpu_CoCmd_MemWrite(ft_uint32_t ptr, ft_uint32_t num);
    ft_void_t Ft_Gpu_CoCmd_Scrollbar(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t size, ft_uint16_t range);
    ft_void_t Ft_Gpu_CoCmd_GetMatrix(ft_int32_t a, ft_int32_t b, ft_int32_t c, ft_int32_t d, ft_int32_t e, ft_int32_t f);
    ft_void_t Ft_Gpu_CoCmd_Sketch(ft_int16_t x, ft_int16_t y, ft_uint16_t w, ft_uint16_t h, ft_uint32_t ptr, ft_uint16_t format);
    ft_void_t Ft_Gpu_CoCmd_MemSet(ft_uint32_t ptr, ft_uint32_t value, ft_uint32_t num);
    ft_void_t Ft_Gpu_CoCmd_Calibrate(ft_uint32_t result);
    ft_void_t Ft_Gpu_CoCmd_SetFont(ft_uint32_t font, ft_uint32_t ptr);
    ft_void_t Ft_Gpu_CoCmd_Bitmap_Transform(ft_int32_t x0, ft_int32_t y0, ft_int32_t x1, ft_int32_t y1, ft_int32_t x2, ft_int32_t y2, ft_int32_t tx0, ft_int32_t ty0, ft_int32_t tx1, ft_int32_t ty1, ft_int32_t tx2, ft_int32_t ty2, ft_uint16_t result);
    ft_void_t Ft_Gpu_CoCmd_GradColor(ft_uint32_t c);
    ft_void_t Ft_Gpu_CoCmd_Append(ft_uint32_t ptr, ft_uint32_t num);
    ft_void_t Ft_Gpu_CoCmd_MemZero(ft_uint32_t ptr, ft_uint32_t num);
    ft_void_t Ft_Gpu_CoCmd_Scale(ft_int32_t sx, ft_int32_t sy);
    ft_void_t Ft_Gpu_CoCmd_Clock(ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t h, ft_uint16_t m, ft_uint16_t s, ft_uint16_t ms);
    ft_void_t Ft_Gpu_CoCmd_Gradient(ft_int16_t x0, ft_int16_t y0, ft_uint32_t rgb0, ft_int16_t x1, ft_int16_t y1, ft_uint32_t rgb1);
    ft_void_t Ft_Gpu_CoCmd_SetMatrix();
    ft_void_t Ft_Gpu_CoCmd_Track(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t tag);
    ft_void_t Ft_Gpu_CoCmd_GetPtr(ft_uint32_t result);
    ft_void_t Ft_Gpu_CoCmd_Progress(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range);
    ft_void_t Ft_Gpu_CoCmd_ColdStart();
    ft_void_t Ft_Gpu_CoCmd_Keys(ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);
    ft_void_t Ft_Gpu_CoCmd_Dial(ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t val);
    ft_void_t Ft_Gpu_CoCmd_LoadImage(ft_uint32_t ptr, ft_uint32_t options);
    ft_void_t Ft_Gpu_CoCmd_Dlstart();
    ft_void_t Ft_Gpu_CoCmd_Snapshot(ft_uint32_t ptr);
    ft_void_t Ft_Gpu_CoCmd_ScreenSaver();
    ft_void_t Ft_Gpu_CoCmd_Memcrc(ft_uint32_t ptr, ft_uint32_t num, ft_uint32_t result);

    ft_void_t Ft_Gpu_CoCmd_Logo();

    ft_void_t Ft_Gpu_Copro_SendCmd( ft_uint32_t cmd);
    ft_void_t Ft_Gpu_CoCmd_SendStr( const ft_char8_t *s);
    ft_void_t Ft_Gpu_CoCmd_StartFunc( ft_uint16_t count);
    ft_void_t Ft_Gpu_CoCmd_EndFunc( ft_uint16_t count);
    ft_void_t Ft_Gpu_CoCmd_TouchTransform( ft_int32_t x0, ft_int32_t y0, ft_int32_t x1, ft_int32_t y1, ft_int32_t x2, ft_int32_t y2, ft_int32_t tx0, ft_int32_t ty0, ft_int32_t tx1, ft_int32_t ty1, ft_int32_t tx2, ft_int32_t ty2, ft_uint16_t result);
    ft_void_t Ft_Gpu_CoCmd_BitmapTransform( ft_int32_t x0, ft_int32_t y0, ft_int32_t x1, ft_int32_t y1, ft_int32_t x2, ft_int32_t y2, ft_int32_t tx0, ft_int32_t ty0, ft_int32_t tx1, ft_int32_t ty1, ft_int32_t tx2, ft_int32_t ty2, ft_uint16_t result);
    ft_void_t Ft_Gpu_CoCmd_MemCrc( ft_uint32_t ptr, ft_uint32_t num, ft_uint32_t result);

    ft_uint16_t Ft_Gpu_Cmdfifo_Freespace( );

    ft_void_t Ft_App_WrCoCmd_Buffer(ft_uint32_t cmd);
    ft_void_t Ft_App_WrDlCmd_Buffer(ft_uint32_t cmd);
    ft_void_t Ft_App_Flush_DL_Buffer();
    ft_void_t Ft_App_Flush_Co_Buffer();
    ft_void_t TFT_fadeout();
    ft_void_t TFT_fadein();
    ft_void_t GPU_DLSwap(ft_uint8_t DL_Swap_Type);

	ft_void_t Ft_Sound_ON();
	ft_void_t Ft_Sound_OFF();

};  // end of class

#endif  /*FT_GPU_HAL_H*/



