/*-----------------------------------------*\
|  HyperXKeyboardController.cpp             |
|                                           |
|  Driver for HyperX RGB Keyboard lighting  |
|  controller                               |
|                                           |
|  Adam Honse (CalcProgrammer1) 1/30/2020   |
\*-----------------------------------------*/

#include "HyperXKeyboardController.h"

#ifdef WIN32
#include <Windows.h>
#else
#include <unistd.h>

static void Sleep(unsigned int milliseconds)
{
    usleep(1000 * milliseconds);
}
#endif


static unsigned int keys[] = {0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
                              0x15, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1D, 0x1E, 0x20, 0x21, 0x22, 0x23, 0x24,
                              0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
                              0x33, 0x34, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3E, 0x3F, 0x41, 0x44, 0x45,
                              0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x51, 0x54, 0x55, 0x58, 0x59,
                              0x5A, 0x5B, 0x5C, 0x5E, 0x5F, 0x61, 0x64, 0x65, 0x68, 0x69, 0x6A, 0x6B, 0x6C,
                              0x6E, 0x6F, 0x74, 0x75, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F, 0x81,
                              0x84, 0x85, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F, 0x91, 0x94, 0x95 };

static unsigned int extended_red[] = {0x08, 0x48, 0x88, 0x09, 0x89, 0x0A, 0x8A, 0x0B, 0x8B, 0x0C, 0x8C, 0x0D, 0x8D, 0x0E, 0x8F, 0x8E, 0x0F, 0x4F, 0x92, 0x13, 0x93, 0x12 };
static unsigned int extended_grn[] = {0x29, 0x28, 0x78, 0x19, 0x79, 0x1A, 0x7A, 0x1B, 0x7B, 0x1C, 0x7C, 0x1D, 0x7D, 0x1E, 0x6E, 0x7E, 0x1F, 0x6F, 0x82, 0x23, 0x83, 0x22 };
static unsigned int extended_blu[] = {0x39, 0x38, 0x68, 0x3A, 0x69, 0x2A, 0x6A, 0x2B, 0x6B, 0x2C, 0x6C, 0x2D, 0x6D, 0x2E, 0x5E, 0x5D, 0x2F, 0x5F, 0x72, 0x33, 0x73, 0x32 };

HyperXKeyboardController::HyperXKeyboardController(hid_device* dev_handle)
{
    dev = dev_handle;
}

HyperXKeyboardController::~HyperXKeyboardController()
{

}

void HyperXKeyboardController::SetMode(unsigned char mode, unsigned char direction, unsigned char speed)
{
    active_mode = mode;
    active_direction = direction;
    active_speed = speed;

    SendEffect
        (
        0x01,
        active_mode,
        active_direction,
        HYPERX_REACTIVE_MODE_NONE,
        active_speed,
        HYPERX_COLOR_MODE_SPECTRUM
        );
}

void HyperXKeyboardController::SetLEDsDirect(std::vector<RGBColor> colors)
{
    unsigned char red_color_data[104];
    unsigned char grn_color_data[104];
    unsigned char blu_color_data[104];
    unsigned char ext_color_data[150];

    for(std::size_t i = 0; i < 104; i++)
    {
        red_color_data[i] = RGBGetRValue(colors[i]);
        grn_color_data[i] = RGBGetGValue(colors[i]);
        blu_color_data[i] = RGBGetBValue(colors[i]);
    }

    for(std::size_t i = 0; i < 22; i++)
    {
        ext_color_data[extended_red[i]] = RGBGetRValue(colors[i + 104]);
        ext_color_data[extended_grn[i]] = RGBGetGValue(colors[i + 104]);
        ext_color_data[extended_blu[i]] = RGBGetBValue(colors[i + 104]);
    }

    SendDirect
        (
        HYPERX_COLOR_CHANNEL_RED,
        red_color_data
        );

    Sleep(2);

    SendDirect
        (
        HYPERX_COLOR_CHANNEL_GREEN,
        grn_color_data
        );

    Sleep(2);

    SendDirect
        (
        HYPERX_COLOR_CHANNEL_BLUE,
        blu_color_data
        );

    Sleep(2);

    SendDirectExtended
        (
        ext_color_data
        );
}

void HyperXKeyboardController::SetLEDs(std::vector<RGBColor> colors)
{
    unsigned char red_color_data[104];
    unsigned char grn_color_data[104];
    unsigned char blu_color_data[104];
    unsigned char ext_color_data[150];

    for(std::size_t i = 0; i < 104; i++)
    {
        red_color_data[i] = RGBGetRValue(colors[i]);
        grn_color_data[i] = RGBGetGValue(colors[i]);
        blu_color_data[i] = RGBGetBValue(colors[i]);
    }

    for(std::size_t i = 0; i < 22; i++)
    {
        ext_color_data[extended_red[i]] = RGBGetRValue(colors[i + 104]);
        ext_color_data[extended_grn[i]] = RGBGetGValue(colors[i + 104]);
        ext_color_data[extended_blu[i]] = RGBGetBValue(colors[i + 104]);
    }

    SendColor
        (
        0x01,
        HYPERX_COLOR_CHANNEL_RED,
        red_color_data
        );

    SendColor
        (
        0x01,
        HYPERX_COLOR_CHANNEL_GREEN,
        grn_color_data
        );

    SendColor
        (
        0x01,
        HYPERX_COLOR_CHANNEL_BLUE,
        blu_color_data
        );

    SendExtendedColor
        (
        0x01,
        ext_color_data
        );
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXKeyboardController::SelectProfile
    (
    unsigned char   profile
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = 0x01;
    buf[0x02]   = profile;

    buf[0x06]   = 0x03;
    buf[0x07]   = 0x01;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXKeyboardController::SendEffect
    (
    unsigned char   profile,
    unsigned char   mode,
    unsigned char   direction,
    unsigned char   reactive_mode,
    unsigned char   speed,
    unsigned char   color_mode
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Effect packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PACKET_ID_SET_EFFECT;
    buf[0x02]   = profile;

    /*-----------------------------------------------------*\
    | Set mode                                              |
    \*-----------------------------------------------------*/
    buf[0x09]   = 0x01;
    buf[0x0A]   = mode;

    /*-----------------------------------------------------*\
    | Set direction                                         |
    \*-----------------------------------------------------*/
    buf[0x0D]   = direction;
    buf[0x0E]   = direction;

    /*-----------------------------------------------------*\
    | Set reactive mode                                     |
    \*-----------------------------------------------------*/
    buf[0x1B]   = reactive_mode;
    buf[0x1C]   = reactive_mode;
    buf[0x1D]   = reactive_mode;
    buf[0x1E]   = reactive_mode;
    buf[0x1F]   = reactive_mode;
    buf[0x20]   = reactive_mode;
    buf[0x21]   = reactive_mode;
    buf[0x22]   = reactive_mode;

    buf[0x6B]   = 0x09;
    buf[0x6C]   = 0x09;
    buf[0x6D]   = 0x05;
    buf[0x6E]   = 0x05;
    buf[0x6F]   = 0x06;
    buf[0x70]   = 0x05;

    /*-----------------------------------------------------*\
    | Set speed                                             |
    \*-----------------------------------------------------*/
    buf[0x71]   = speed;

    buf[0x72]   = 0x09;

    /*-----------------------------------------------------*\
    | Set color mode                                        |
    \*-----------------------------------------------------*/
    buf[0x73]   = color_mode;
    buf[0x74]   = color_mode;
    buf[0x75]   = color_mode;
    buf[0x76]   = color_mode;
    buf[0x77]   = color_mode;
    buf[0x78]   = color_mode;
    buf[0x79]   = color_mode;
    buf[0x7A]   = color_mode;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXKeyboardController::SendColor
    (
    unsigned char   profile,
    unsigned char   color_channel,
    unsigned char*  color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Color packet                                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PACKET_ID_SET_COLOR;
    buf[0x02]   = profile;
    buf[0x03]   = color_channel;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < 104; i++)
    {
        buf[keys[i]] = color_data[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXKeyboardController::SendExtendedColor
    (
    unsigned char   profile,
    unsigned char*  color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Color packet                                   |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PACKET_ID_SET_COLOR;
    buf[0x02]   = profile;
    buf[0x03]   = HYPERX_COLOR_CHANNEL_EXTENDED;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0x08; i <= 0x93; i++)
    {
        buf[i] = color_data[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXKeyboardController::SendDirect
    (
    unsigned char   color_channel,
    unsigned char*  color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PACKET_ID_DIRECT;
    buf[0x02]   = color_channel;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0; i < 104; i++)
    {
        buf[keys[i]] = color_data[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXKeyboardController::SendDirectExtended
    (
    unsigned char*  color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PACKET_ID_DIRECT;
    buf[0x02]   = HYPERX_COLOR_CHANNEL_EXTENDED;

    /*-----------------------------------------------------*\
    | Fill in color data                                    |
    \*-----------------------------------------------------*/
    for(int i = 0x08; i <= 0x93; i++)
    {
        buf[i] = color_data[i];
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}