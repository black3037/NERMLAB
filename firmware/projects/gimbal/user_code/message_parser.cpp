#include "message_parser.h"
#include "nerm_lab_constants.h"

char input_string[2049];

void sendDataRequest(void)
{
    char * initRequest = "{\"NERMLAB\":\"DATA REQUEST\"}";
    SendCharArrayUSART1(initRequest,strlen(initRequest));
}

void requestDataParams(void)
{
    sendDataRequest();
    getAllValues();   
}

void getAllValues(void)
{
    jsmn_parser parser;
    jsmntok_t tokens[50];
    jsmn_init(&parser);
    int numTokens = jsmn_parse(&parser,input_string,strlen(input_string),tokens,50);
    extract_value(input_string,  "Mode",       mode,       numTokens, tokens);
    extract_value(input_string,  "Kp",         Kp,         numTokens, tokens);
    extract_value(input_string,  "Ki",         Ki,         numTokens, tokens);
    extract_value(input_string,  "Kd",         Kd,         numTokens, tokens);
    extract_value(input_string,  "WaveType",   waveType,   numTokens, tokens);
    extract_value(input_string,  "Magnitude",  magnitude,  numTokens, tokens);
    extract_value(input_string,  "Frequency",  frequency,  numTokens, tokens);
    extract_value(input_string,  "Controller", controller, numTokens, tokens);
    extract_value(input_string,  "Duration",   duration,   numTokens, tokens);
    extract_value(input_string,  "SampleRate", sampleRate, numTokens, tokens);
}

void getMode(void)
{
    jsmn_parser parser;
    jsmntok_t tokens[50];
    jsmn_init(&parser);
    int numTokens = jsmn_parse(&parser,input_string,strlen(input_string),tokens,50);
    extract_value(input_string,  "Mode",       mode,       numTokens, tokens);
}