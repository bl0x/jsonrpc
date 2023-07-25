#include "Arduino.h"
#include "aJSON.h"
#include "JsonRPC.h"

JsonRPC::JsonRPC(int capacity, Stream *stream)
{
    mymap = (FuncMap *)malloc(sizeof(FuncMap));
    mymap->capacity = capacity;
    mymap->used = 0;
    mymap->mappings = (Mapping*)malloc(capacity * sizeof(Mapping));
    serial = stream;
    memset(mymap->mappings, 0, capacity * sizeof(Mapping));
}

void JsonRPC::registerMethod(String methodName, ajson_callback callback)
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
        serial->flush();
        return;
    }

    aJsonObject* params = aJson.getObjectItem(msg, "params");
    if (!params)
    {
	serial->flush();
	return;
    }

    aJsonObject* id = aJson.getObjectItem(msg, "id");
    int id_value = 0;
    if (!id)
    {
	    id_value = -1;
    } else {
	    id_value = id->valueint;
    }

    String methodName = method->valuestring;
    for (int i=0; i<mymap->used; i++)
    {
        Mapping* mapping = &(mymap->mappings[i]);
        if (methodName.equals(mapping->name))
        {
	    mapping->callback(params, id_value);
	}
    }
}

