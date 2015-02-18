#pragma once
#include "Common.h"
#include <map>
#include <string>
#include <algorithm>
#include <time.h>
#include <sstream>

#ifdef __cplusplus
extern "C" {
#endif			
	class Appender
	{
		protected:
			SEVERITY minLogSeverity;
			std::string GetTime();

		public:			
			Appender();
			Appender(SEVERITY minSev);
			void SetSeverity(SEVERITY sev);
			virtual void Log(SEVERITY msgSev, std::string& Message);
			virtual void Log(std::string& Message);
	};
#ifdef __cplusplus
}
#endif