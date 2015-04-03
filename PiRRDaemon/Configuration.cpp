#include "Configuration.h"

Configuration::Configuration()
{
	configFile = "config.json";
	configParsed = parseConfig();
}

Configuration::Configuration(string configurationFile)
{	
	configFile = configurationFile;
	configParsed = parseConfig();
}

bool Configuration::parseConfig()
{
	string jsonText;
	try
	{
		std::ifstream inFile;
		inFile.open(configFile.c_str());
		if (!inFile)
		{
			return false;
		}
		
		// Read file into a string
		inFile.seekg(0, std::ios::end);
		jsonText.reserve(inFile.tellg());
		inFile.seekg(0, std::ios::beg);
		jsonText.assign((std::istreambuf_iterator<char>(inFile)),
			std::istreambuf_iterator<char>());
		inFile.close();
	}
	catch (exception e)
	{
		return false;
	}

	// File is read, lets parse
	JSONNODE *node = json_parse(jsonText.c_str());
	
	if (!parseJSON(node))
		return false;
}

bool Configuration::parseJSON(const JSONNODE *n)
{
	if (n == NULL)
		return false;
	
	
	
	return true;
}