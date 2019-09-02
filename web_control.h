#ifndef _WEB_CONTROL_H__
#define _WEB_CONTROL_H__
#define PAGE_HOME                                       0x0001
#define PAGE_HIGH                                       0x0002
#define PAGE_LOW                                        0x0003
#include "urls.h"
#include <Arduino.h>
#include <WString.h>

class WebControl
{
private:
    /* data */
public:
    int FindPage(String req);
};


#endif