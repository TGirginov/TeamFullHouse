#include "mbed.h"
#include "FT_Platform.h"
 
FT800 TFT(p11,p12,p13,p14,p16,p17);       // MOSI, MISO, CLK, CS, INT, PD
 
/* API to demonstrate calibrate widget/functionality */
ft_void_t CoPro_Widget_Calibrate()
{
    /*************************************************************************/
    /* Below code demonstrates the usage of calibrate function. Calibrate    */
    /* function will wait untill user presses all the three dots. Only way to*/
    /* come out of this api is to reset the coprocessor bit.                 */
    /*************************************************************************/
    {
 
        TFT.Ft_Gpu_CoCmd_Dlstart();                                 // start a new display command list
        TFT.Ft_App_WrCoCmd_Buffer(CLEAR_COLOR_RGB(64,64,64));       // set the clear color R, G, B
        TFT.Ft_App_WrCoCmd_Buffer(CLEAR(1,1,1));                    // clear buffers -> color buffer,stencil buffer, tag buffer
        TFT.Ft_App_WrCoCmd_Buffer(COLOR_RGB(0xff,0xff,0xff));       // set the current color R, G, B
        //TFT.Ft_Gpu_CoCmd_Text((TFT.FT_DispWidth/2), (TFT.FT_DispHeight/2), 27, OPT_CENTER, "Please Tap on the dot");  // draw Text at x,y, font 27, centered
        //TFT.Ft_Gpu_CoCmd_Calibrate(0);                              // start the calibration of touch screen
        TFT.Ft_App_Flush_Co_Buffer();                               // download the commands into FT800 FIFO
        TFT.Ft_Gpu_Hal_WaitCmdfifo_empty();                         // Wait till coprocessor completes the operation
    }
}

/***************************************************************************/
/* Show a Screen with Text for 5 seconds                                   */
/* A spinner shows the delay                                               */
/***************************************************************************/
 
ft_void_t API_Screen(ft_char8_t *str)
{
    TFT.Ft_Gpu_CoCmd_Dlstart();                                   // start a new display command list
    TFT.Ft_App_WrCoCmd_Buffer(CLEAR_COLOR_RGB(255,255,255));      // set the clear color to white
    TFT.Ft_App_WrCoCmd_Buffer(CLEAR(1,1,1));                      // clear buffers -> color buffer,stencil buffer, tag buffer
 
    TFT.Ft_App_WrCoCmd_Buffer(COLOR_RGB(0x80,0x80,0x00));         // set current color
    TFT.Ft_Gpu_CoCmd_Text((TFT.FT_DispWidth/2), TFT.FT_DispHeight/2, 31, OPT_CENTERX, str); // draw Text with font 31
    TFT.Ft_App_WrCoCmd_Buffer(COLOR_RGB(0xFF,0x00,0x00));         // change current color
    TFT.Ft_Gpu_CoCmd_Spinner((TFT.FT_DispWidth/2),TFT.FT_DispHeight/4, 0,0); // draw a animated spinner
 
    TFT.Ft_App_WrCoCmd_Buffer(DISPLAY());                         // Display the image
    TFT.Ft_Gpu_CoCmd_Swap();                                      // Swap the current display list
    TFT.Ft_App_Flush_Co_Buffer();                                 // Download the command list into fifo
 
    TFT.Ft_Gpu_Hal_WaitCmdfifo_empty();                           // Wait till coprocessor completes the operation
    TFT.Ft_Gpu_Hal_Sleep(5000);                                   // Wait 5 s
}
 
int main()
{
 
    /*************************************************************************/
    /* Below code demonstrates the usage of track function. Track function   */
    /* tracks the pen touch on any specific object. Track function supports  */
    /* rotary and horizontal/vertical tracks. Rotary is given by rotation    */
    /* angle and horizontal/vertucal track is offset position.               */
    /*************************************************************************/
 
    //API_Screen("DEMO  START");                  // Show start screen
    //CoPro_Widget_Calibrate();                   // calibrate the touch screen

    /* update the background color continuously for the color change in any of the trackers */
    /* the demo is updating the screen in a endless loop                                    */
    while(1) {
        /* Display a rotary dial, horizontal slider and vertical scroll */

        //TFT.Ft_App_WrCoCmd_Buffer(CMD_DLSTART);                     // start a new display command list
        TFT.Ft_Gpu_CoCmd_Dlstart();
        TFT.Ft_App_WrCoCmd_Buffer(CLEAR_COLOR_RGB(0,0,0));          // set the new clear color
        TFT.Ft_App_WrCoCmd_Buffer(CLEAR(1,1,1));                    // clear buffers -> color buffer,stencil buffer, tag buffer
        TFT.Ft_App_WrCoCmd_Buffer(COLOR_RGB(255,255,255));       // set current color to white

        TFT.Ft_Gpu_CoCmd_Text((TFT.FT_DispWidth/6), (TFT.FT_DispHeight/6), 26, OPT_CENTER, "Speed"); // display text
        TFT.Ft_Gpu_CoCmd_Text((TFT.FT_DispWidth/2), (TFT.FT_DispHeight/6), 26, OPT_CENTER, "Gear");  // display text
        
        TFT.Ft_Gpu_CoCmd_Number((TFT.FT_DispWidth/6), (TFT.FT_DispHeight/3), 26, OPT_CENTER, 35); //display number
        TFT.Ft_Gpu_CoCmd_Number((TFT.FT_DispWidth/2), (TFT.FT_DispHeight/3), 31, OPT_CENTER, 4); //display number
        
        TFT.Ft_Gpu_CoCmd_Number((TFT.FT_DispWidth/6), ((2*TFT.FT_DispHeight)/3), 26, OPT_CENTER, 18); //display number
        TFT.Ft_Gpu_CoCmd_Number((TFT.FT_DispWidth/2), ((2*TFT.FT_DispHeight)/3), 26, OPT_CENTER, 500); //display number
        TFT.Ft_Gpu_CoCmd_Number(((5*TFT.FT_DispWidth)/6), ((2*TFT.FT_DispHeight)/3), 26, OPT_CENTER, 6); //display number
        
        TFT.Ft_Gpu_CoCmd_Text((TFT.FT_DispWidth/6), ((5*TFT.FT_DispHeight)/6), 26, OPT_CENTER, "Oil Temp");  // display text
        TFT.Ft_Gpu_CoCmd_Text((TFT.FT_DispWidth/2), ((5*TFT.FT_DispHeight)/6), 26, OPT_CENTER, "Fuel Level");  // display text
        TFT.Ft_Gpu_CoCmd_Text(((5*TFT.FT_DispWidth)/6), ((5*TFT.FT_DispHeight)/6), 26, OPT_CENTER, "Coolant Temp");  // display text

        TFT.Ft_App_WrCoCmd_Buffer(DISPLAY());                                   // Display the image
        TFT.Ft_Gpu_CoCmd_Swap();                                                // Swap the current display list

        TFT.Ft_App_Flush_Co_Buffer();                                           // Download the commands into fifo

        TFT.Ft_Gpu_Hal_WaitCmdfifo_empty();                                     // Wait till coprocessor completes the operation

        TFT.Ft_Gpu_Hal_Sleep(100);                                               // wait 10ms
    }  // end of display loop 
}
 
            