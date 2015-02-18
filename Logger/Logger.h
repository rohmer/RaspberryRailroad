#pragma once
#include <iostream>
#include <vector>
#include "Appender.h"
#include "Common.h"

#ifdef __cplusplus
extern "C" {
#endif

	class Logger
	{
		private:
			std::vector<Appender> appenders;

	};

#ifdef __cplusplus
}
#endif
