/* mbed Library for FTDI FT800  Enbedded Video Engine "EVE"
 * based on Original Code Sample from FTDI 
 * ported to mbed by Peter Drescher, DC2PD 2014
 * Released under the MIT License: http://mbed.org/license/mit */

#include "FT_Platform.h"


ft_void_t FT800::Ft_Gpu_Copro_SendCmd( ft_uint32_t cmd)
{ 
   Ft_Gpu_Hal_Transfer32( cmd);
}

ft_void_t FT800::Ft_Gpu_CoCmd_SendStr( const ft_char8_t *s)
{
  Ft_Gpu_Hal_TransferString( s);
}


ft_void_t FT800::Ft_Gpu_CoCmd_StartFunc( ft_uint16_t count)
{
  Ft_Gpu_Hal_CheckCmdBuffer( count);
  Ft_Gpu_Hal_StartCmdTransfer( FT_GPU_WRITE,count);
}

ft_void_t FT800::Ft_Gpu_CoCmd_EndFunc( ft_uint16_t count)
{
  Ft_Gpu_Hal_EndTransfer( );
  Ft_Gpu_Hal_Updatecmdfifo( count);
}

ft_void_t FT800::Ft_Gpu_CoCmd_Text( ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3 + strlen(s) + 1);
  Ft_Gpu_Copro_SendCmd(  CMD_TEXT);
  //Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(ft_uint32_t)x));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)options<<16)|(ft_uint32_t)font));
  Ft_Gpu_CoCmd_SendStr(  s);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3 + strlen(s) + 1));  
}

ft_void_t FT800::Ft_Gpu_CoCmd_Number( ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, ft_int32_t n)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);  
  Ft_Gpu_Copro_SendCmd(  CMD_NUMBER);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)options<<16)|font));
  Ft_Gpu_Copro_SendCmd(  n);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));  
}

ft_void_t FT800::Ft_Gpu_CoCmd_LoadIdentity( )
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);    
  Ft_Gpu_Copro_SendCmd(  CMD_LOADIDENTITY);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));  
}

ft_void_t FT800::Ft_Gpu_CoCmd_Toggle( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t font, ft_uint16_t options, ft_uint16_t state, const ft_char8_t* s)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4 + strlen(s) + 1);     
  Ft_Gpu_Copro_SendCmd(  CMD_TOGGLE);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)font<<16)|w));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)state<<16)|options));
  Ft_Gpu_CoCmd_SendStr(  s);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4 + strlen(s) + 1));       
}

/* Error handling for val is not done, so better to always use range of 65535 in order that needle is drawn within display region */
ft_void_t FT800::Ft_Gpu_CoCmd_Gauge( ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t major, ft_uint16_t minor, ft_uint16_t val, ft_uint16_t range)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);       
  Ft_Gpu_Copro_SendCmd(  CMD_GAUGE);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)options<<16)|r));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)minor<<16)|major));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)range<<16)|val));
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_RegRead( ft_uint32_t ptr, ft_uint32_t result)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);         
  Ft_Gpu_Copro_SendCmd(  CMD_REGREAD);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  0);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3)); 
    
}

ft_void_t FT800::Ft_Gpu_CoCmd_GetProps( ft_uint32_t ptr, ft_uint32_t w, ft_uint32_t h)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);           
  Ft_Gpu_Copro_SendCmd(  CMD_GETPROPS);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  w);
  Ft_Gpu_Copro_SendCmd(  h);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));           
}

ft_void_t FT800::Ft_Gpu_CoCmd_Memcpy( ft_uint32_t dest, ft_uint32_t src, ft_uint32_t num)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);         
  Ft_Gpu_Copro_SendCmd(  CMD_MEMCPY);
  Ft_Gpu_Copro_SendCmd(  dest);
  Ft_Gpu_Copro_SendCmd(  src);
  Ft_Gpu_Copro_SendCmd(  num);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));           
}

ft_void_t FT800::Ft_Gpu_CoCmd_Spinner( ft_int16_t x, ft_int16_t y, ft_uint16_t style, ft_uint16_t scale)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);         
  Ft_Gpu_Copro_SendCmd(  CMD_SPINNER);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)scale<<16)|style));
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));           
}

ft_void_t FT800::Ft_Gpu_CoCmd_BgColor( ft_uint32_t c)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);           
  Ft_Gpu_Copro_SendCmd(  CMD_BGCOLOR);
  Ft_Gpu_Copro_SendCmd(  c);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));           
}

ft_void_t FT800::Ft_Gpu_CoCmd_Swap()
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);         
  Ft_Gpu_Copro_SendCmd(  CMD_SWAP);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_Inflate( ft_uint32_t ptr)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);
  Ft_Gpu_Copro_SendCmd(  CMD_INFLATE);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));  
}

ft_void_t FT800::Ft_Gpu_CoCmd_Translate( ft_int32_t tx, ft_int32_t ty)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);  
  Ft_Gpu_Copro_SendCmd(  CMD_TRANSLATE);
  Ft_Gpu_Copro_SendCmd(  tx);
  Ft_Gpu_Copro_SendCmd(  ty);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));  
}

ft_void_t FT800::Ft_Gpu_CoCmd_Stop()
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);    
  Ft_Gpu_Copro_SendCmd(  CMD_STOP);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));    
}

ft_void_t FT800::Ft_Gpu_CoCmd_Slider( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);    
  Ft_Gpu_Copro_SendCmd(  CMD_SLIDER);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)val<<16)|options));
  Ft_Gpu_Copro_SendCmd(  range);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));    
}

ft_void_t FT800::Ft_Gpu_CoCmd_TouchTransform( ft_int32_t x0, ft_int32_t y0, ft_int32_t x1, ft_int32_t y1, ft_int32_t x2, ft_int32_t y2, ft_int32_t tx0, ft_int32_t ty0, ft_int32_t tx1, ft_int32_t ty1, ft_int32_t tx2, ft_int32_t ty2, ft_uint16_t result)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*6*2+FT_CMD_SIZE*2);      
  Ft_Gpu_Copro_SendCmd(  CMD_TOUCH_TRANSFORM);
  Ft_Gpu_Copro_SendCmd(  x0);
  Ft_Gpu_Copro_SendCmd(  y0);
  Ft_Gpu_Copro_SendCmd(  x1);
  Ft_Gpu_Copro_SendCmd(  y1);
  Ft_Gpu_Copro_SendCmd(  x2);
  Ft_Gpu_Copro_SendCmd(  y2);
  Ft_Gpu_Copro_SendCmd(  tx0);
  Ft_Gpu_Copro_SendCmd(  ty0);
  Ft_Gpu_Copro_SendCmd(  tx1);
  Ft_Gpu_Copro_SendCmd(  ty1);
  Ft_Gpu_Copro_SendCmd(  tx2);
  Ft_Gpu_Copro_SendCmd(  ty2);
  Ft_Gpu_Copro_SendCmd(  result);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*6*2+FT_CMD_SIZE*2));        
}

ft_void_t FT800::Ft_Gpu_CoCmd_Interrupt( ft_uint32_t ms)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);        
  Ft_Gpu_Copro_SendCmd(  CMD_INTERRUPT);
  Ft_Gpu_Copro_SendCmd(  ms);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));          
}

ft_void_t FT800::Ft_Gpu_CoCmd_FgColor( ft_uint32_t c)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);          
  Ft_Gpu_Copro_SendCmd(  CMD_FGCOLOR);
  Ft_Gpu_Copro_SendCmd(  c);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));          
}

ft_void_t FT800::Ft_Gpu_CoCmd_Rotate( ft_int32_t a)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);            
  Ft_Gpu_Copro_SendCmd(  CMD_ROTATE);
  Ft_Gpu_Copro_SendCmd(  a);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));          
}

ft_void_t FT800::Ft_Gpu_CoCmd_Button( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4 + strlen(s) + 1);            
  Ft_Gpu_Copro_SendCmd(  CMD_BUTTON);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|font));
  Ft_Gpu_CoCmd_SendStr(  s);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4 + strlen(s) + 1));              
}

ft_void_t FT800::Ft_Gpu_CoCmd_MemWrite( ft_uint32_t ptr, ft_uint32_t num)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);       
  Ft_Gpu_Copro_SendCmd(  CMD_MEMWRITE);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  num);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));       
}

ft_void_t FT800::Ft_Gpu_CoCmd_Scrollbar( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t size, ft_uint16_t range)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);       
  Ft_Gpu_Copro_SendCmd(  CMD_SCROLLBAR);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)val<<16)|options));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)range<<16)|size));
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));       
}

ft_void_t FT800::Ft_Gpu_CoCmd_GetMatrix( ft_int32_t a, ft_int32_t b, ft_int32_t c, ft_int32_t d, ft_int32_t e, ft_int32_t f)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*7);       
  Ft_Gpu_Copro_SendCmd(  CMD_GETMATRIX);
  Ft_Gpu_Copro_SendCmd(  a);
  Ft_Gpu_Copro_SendCmd(  b);
  Ft_Gpu_Copro_SendCmd(  c);
  Ft_Gpu_Copro_SendCmd(  d);
  Ft_Gpu_Copro_SendCmd(  e);
  Ft_Gpu_Copro_SendCmd(  f);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*7));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_Sketch( ft_int16_t x, ft_int16_t y, ft_uint16_t w, ft_uint16_t h, ft_uint32_t ptr, ft_uint16_t format)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);         
  Ft_Gpu_Copro_SendCmd(  CMD_SKETCH);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  format);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));         
}
ft_void_t FT800::Ft_Gpu_CoCmd_MemSet( ft_uint32_t ptr, ft_uint32_t value, ft_uint32_t num)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);         
  Ft_Gpu_Copro_SendCmd(  CMD_MEMSET);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  value);
  Ft_Gpu_Copro_SendCmd(  num);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));         
}
ft_void_t FT800::Ft_Gpu_CoCmd_GradColor( ft_uint32_t c)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);         
  Ft_Gpu_Copro_SendCmd(  CMD_GRADCOLOR);
  Ft_Gpu_Copro_SendCmd(  c);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));         
}
ft_void_t FT800::Ft_Gpu_CoCmd_BitmapTransform( ft_int32_t x0, ft_int32_t y0, ft_int32_t x1, ft_int32_t y1, ft_int32_t x2, ft_int32_t y2, ft_int32_t tx0, ft_int32_t ty0, ft_int32_t tx1, ft_int32_t ty1, ft_int32_t tx2, ft_int32_t ty2, ft_uint16_t result)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*6*2+FT_CMD_SIZE*2);         
  Ft_Gpu_Copro_SendCmd(  CMD_BITMAP_TRANSFORM);
  Ft_Gpu_Copro_SendCmd(  x0);
  Ft_Gpu_Copro_SendCmd(  y0);
  Ft_Gpu_Copro_SendCmd(  x1);
  Ft_Gpu_Copro_SendCmd(  y1);
  Ft_Gpu_Copro_SendCmd(  x2);
  Ft_Gpu_Copro_SendCmd(  y2);
  Ft_Gpu_Copro_SendCmd(  tx0);
  Ft_Gpu_Copro_SendCmd(  ty0);
  Ft_Gpu_Copro_SendCmd(  tx1);
  Ft_Gpu_Copro_SendCmd(  ty1);
  Ft_Gpu_Copro_SendCmd(  tx2);
  Ft_Gpu_Copro_SendCmd(  ty2);
  Ft_Gpu_Copro_SendCmd(  result);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*6*2+FT_CMD_SIZE*2));     
}
ft_void_t FT800::Ft_Gpu_CoCmd_Calibrate( ft_uint32_t result)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);     
  Ft_Gpu_Copro_SendCmd(  CMD_CALIBRATE);
  Ft_Gpu_Copro_SendCmd(  result);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));   
  Ft_Gpu_Hal_WaitCmdfifo_empty( );
  
}
ft_void_t FT800::Ft_Gpu_CoCmd_SetFont( ft_uint32_t font, ft_uint32_t ptr)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);       
  Ft_Gpu_Copro_SendCmd(  CMD_SETFONT);
  Ft_Gpu_Copro_SendCmd(  font);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));       
}
ft_void_t FT800::Ft_Gpu_CoCmd_Logo(  )
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);       
  Ft_Gpu_Copro_SendCmd(  CMD_LOGO);  
  Ft_Gpu_CoCmd_EndFunc( FT_CMD_SIZE*1); 
}
ft_void_t FT800::Ft_Gpu_CoCmd_Append( ft_uint32_t ptr, ft_uint32_t num)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);       
  Ft_Gpu_Copro_SendCmd(  CMD_APPEND);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  num);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));       
}
ft_void_t FT800::Ft_Gpu_CoCmd_MemZero( ft_uint32_t ptr, ft_uint32_t num)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);       
  Ft_Gpu_Copro_SendCmd(  CMD_MEMZERO);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  num);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));       
}
ft_void_t FT800::Ft_Gpu_CoCmd_Scale( ft_int32_t sx, ft_int32_t sy)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);       
  Ft_Gpu_Copro_SendCmd(  CMD_SCALE);
  Ft_Gpu_Copro_SendCmd(  sx);
  Ft_Gpu_Copro_SendCmd(  sy);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));       
}
ft_void_t FT800::Ft_Gpu_CoCmd_Clock( ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t h, ft_uint16_t m, ft_uint16_t s, ft_uint16_t ms)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);         
  Ft_Gpu_Copro_SendCmd(  CMD_CLOCK);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)options<<16)|r));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)m<<16)|h));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)ms<<16)|s));
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));       
}

ft_void_t FT800::Ft_Gpu_CoCmd_Gradient( ft_int16_t x0, ft_int16_t y0, ft_uint32_t rgb0, ft_int16_t x1, ft_int16_t y1, ft_uint32_t rgb1)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);         
  Ft_Gpu_Copro_SendCmd(  CMD_GRADIENT);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y0<<16)|(x0 & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  rgb0);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y1<<16)|(x1 & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  rgb1);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_SetMatrix(  )
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);         
  Ft_Gpu_Copro_SendCmd(  CMD_SETMATRIX);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_Track( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t tag)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);         
  Ft_Gpu_Copro_SendCmd(  CMD_TRACK);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  tag);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));       
}

ft_void_t FT800::Ft_Gpu_CoCmd_GetPtr( ft_uint32_t result)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);         
  Ft_Gpu_Copro_SendCmd(  CMD_GETPTR);
  Ft_Gpu_Copro_SendCmd(  result);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_Progress( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*5);         
  Ft_Gpu_Copro_SendCmd(  CMD_PROGRESS);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)val<<16)|options));
  Ft_Gpu_Copro_SendCmd(  range);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*5));         
}

ft_void_t FT800::Ft_Gpu_CoCmd_ColdStart(  )
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);       
  Ft_Gpu_Copro_SendCmd(  CMD_COLDSTART);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));       
}

ft_void_t FT800::Ft_Gpu_CoCmd_Keys( ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4 + strlen(s) + 1);         
  Ft_Gpu_Copro_SendCmd(  CMD_KEYS);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)h<<16)|w));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)options<<16)|font));
  Ft_Gpu_CoCmd_SendStr(  s);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4 + strlen(s) + 1));           
}

ft_void_t FT800::Ft_Gpu_CoCmd_Dial( ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t val)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);           
  Ft_Gpu_Copro_SendCmd(  CMD_DIAL);
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)y<<16)|(x & 0xffff)));
  Ft_Gpu_Copro_SendCmd(  (((ft_uint32_t)options<<16)|r));
  Ft_Gpu_Copro_SendCmd(  val);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));           
}

// The Loadimage command has to transfer bitmap data ... no start end 
ft_void_t FT800::Ft_Gpu_CoCmd_LoadImage( ft_uint32_t ptr, ft_uint32_t options)
{
  //Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*3);           
  Ft_Gpu_Copro_SendCmd(  CMD_LOADIMAGE);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  options);
  //Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*3));           
}

ft_void_t FT800::Ft_Gpu_CoCmd_Dlstart(  )
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);           
  Ft_Gpu_Copro_SendCmd(  CMD_DLSTART);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));             
}

ft_void_t FT800::Ft_Gpu_CoCmd_Snapshot( ft_uint32_t ptr)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*2);             
  Ft_Gpu_Copro_SendCmd(  CMD_SNAPSHOT);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*2));             
}

ft_void_t FT800::Ft_Gpu_CoCmd_ScreenSaver(  )
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*1);             
  Ft_Gpu_Copro_SendCmd(  CMD_SCREENSAVER);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*1));             
}

ft_void_t FT800::Ft_Gpu_CoCmd_MemCrc( ft_uint32_t ptr, ft_uint32_t num, ft_uint32_t result)
{
  Ft_Gpu_CoCmd_StartFunc( FT_CMD_SIZE*4);             
  Ft_Gpu_Copro_SendCmd(  CMD_MEMCRC);
  Ft_Gpu_Copro_SendCmd(  ptr);
  Ft_Gpu_Copro_SendCmd(  num);
  Ft_Gpu_Copro_SendCmd(  result);
  Ft_Gpu_CoCmd_EndFunc( (FT_CMD_SIZE*4));             
}


ft_void_t FT800::Ft_App_WrCoCmd_Buffer(ft_uint32_t cmd)
{
   Ft_Gpu_Hal_WrCmd32(cmd);
   /* Increment the command index */
   Ft_CmdBuffer_Index += FT_CMD_SIZE;  
}

ft_void_t FT800::Ft_App_WrDlCmd_Buffer(ft_uint32_t cmd)
{
   Ft_Gpu_Hal_Wr32((RAM_DL+Ft_DlBuffer_Index),cmd);
   /* Increment the command index */
   Ft_DlBuffer_Index += FT_CMD_SIZE;  
}

ft_void_t FT800::Ft_App_Flush_DL_Buffer()
{
   Ft_DlBuffer_Index = 0;
   
}

ft_void_t FT800::Ft_App_Flush_Co_Buffer()
{
   Ft_CmdBuffer_Index = 0;
}


/* API to check the status of previous DLSWAP and perform DLSWAP of new DL */
/* Check for the status of previous DLSWAP and if still not done wait for few ms and check again */
ft_void_t FT800::GPU_DLSwap(ft_uint8_t DL_Swap_Type)
{
    ft_uint8_t Swap_Type = DLSWAP_FRAME,Swap_Done = DLSWAP_FRAME;

    if(DL_Swap_Type == DLSWAP_LINE)
    {
        Swap_Type = DLSWAP_LINE;
    }

    /* Perform a new DL swap */
    Ft_Gpu_Hal_Wr8(REG_DLSWAP,Swap_Type);

    /* Wait till the swap is done */
    while(Swap_Done)
    {
        Swap_Done = Ft_Gpu_Hal_Rd8(REG_DLSWAP);

        if(DLSWAP_DONE != Swap_Done)
        {
            Ft_Gpu_Hal_Sleep(10);//wait for 10ms
        }
    }   
}



/* Nothing beyond this */




