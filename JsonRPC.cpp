#include "Arduino.h"
#include "aJSON.h"
#include "JsonRPC.h"

JsonRPC::JsonRPC(int capacity)
{
    mymap = (FuncMap *)malloc(sizeof(FuncMap));
    mymap->capacity = capacity;
    mymap->used = 0;
    mymap->mappings = (Mapping*)malloc(capacity * sizeof(Mapping));
    memset(mymap->mappings, 0, capacity * sizeof(Mapping));
}

void JsonRPC::registerMethod(String methodName, void(*callback)(aJsonObject*))
{
    // only write keyvalue pair if we allocated enough memory for it
    if (mymap->used < mymap->capacity)
    {
	Mapping* mapping = &(mymap->mappings[mymap->used++]);
	mapping->name = methodName;
	mapping->callback = callback;
    }
}

void JsonRPC::processMessage(aJsonObject *msg)
{
    aJsonObject* method = aJson.getObjectItem(msg, "method");
    if (!method)
    {
	// not a valid Json-RPC message
        Serial.flush();
        return;
    }
    
    aJsonObject* params = aJson.getObjectItem(msg, "params");
    if (!params)
    {
	Serial.flush();
	return;
    }
    
    String methodName = method->valuestring;
    for (int i=0; i<mymap->used; i++)
    {
        Mapping* mapping = &(mymap->mappings[i]);
        if (methodName.equals(mapping->name))
        {
	    mapping->callback(params);
	}
    }
}

