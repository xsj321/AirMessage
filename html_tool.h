#ifndef _HTML_TOOL__
#define _HTML_TOOL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WString.h>
#include <Arduino.h>
#include <FS.h>

#define MAX_LENTH 99999
class HtmlTool
{
private:
    /* data */
public:
    String ReadHtmlFile(char* FilePath);
};

#endif