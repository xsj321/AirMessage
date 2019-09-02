#include"web_control.h"
int WebControl::FindPage(String req)
{
    Serial.println(req.indexOf(PAGE_High_Urls));
    if(req.indexOf(PAGE_Home_Urls)!=-1)return PAGE_HOME;
    else if(req.indexOf(PAGE_High_Urls)!=-1)return PAGE_HIGH;
    else if(req.indexOf(PAGE_Low_Urls)!=-1)return PAGE_LOW;
    else
    {
        return -1;
    }
    
}

