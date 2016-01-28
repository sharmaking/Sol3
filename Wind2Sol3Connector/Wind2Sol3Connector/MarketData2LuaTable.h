#pragma once

#include "Sol3MessageGenerator.h"

using namespace sol3_message_generator;

struct MarketData2Sol3
{
	static int nGetSol3Message(out_msg::CBaseOutMessage *, char * szOutBuffer,size_t stLength);
};
