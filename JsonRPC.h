#ifndef JsonRPC_h
#define JsonRPC_h

#include "Arduino.h"
#include "aJSON.h"

struct Mapping
{
    String name;
    void (*callback)(aJsonObject*);
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
	JsonRPC(int capacity);
	void registerMethod(String methodname, void(*callback)(aJsonObject*));
	void processMessage(aJsonObject *msg);
    private:
	FuncMap* mymap;
};

#endif
