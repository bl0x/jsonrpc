#ifndef JsonRPC_h
#define JsonRPC_h

#include "Arduino.h"
#include "aJSON.h"

typedef void (*ajson_callback)(aJsonObject*, int);

struct Mapping
{
    String name;
    ajson_callback callback;
};

struct FuncMap
{
    Mapping* mappings;
    unsigned int capacity;
    unsigned int used;
};

class JsonRPC
{
    public:
	JsonRPC(int capacity, Stream *serial = &Serial);
	void registerMethod(String methodname, ajson_callback);
	void processMessage(aJsonObject *msg);
    private:
	FuncMap* mymap;
	Stream *serial;
};

#endif
