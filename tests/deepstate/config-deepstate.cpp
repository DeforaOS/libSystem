/* $Id$ */

#include <deepstate/DeepState.hpp>
using namespace deepstate;
#include <assert.h>
extern "C" {
#include "System/config.h"
}

TEST(SystemConfig, Load)
{
	const char * path = DeepState_InputPath(NULL);
	Config * config;

	config = config_new_load(path);
	assert(config != NULL);
	config_delete(config);
}
