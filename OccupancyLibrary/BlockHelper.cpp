#include "BlockHelper.h"

/// <summary>
/// Reads the XML block file.
/// </summary>
/// <param name="logger">The logger.</param>
/// <param name="filename">The filename of the block file</param>
/// File Format:
/// <Block Name="Descriptive Block Name"><Sensor SensorID="1"/><Connected BlockName="Block Name"/></Block>
/// <returns>
/// Vector of Block*
/// </returns>
vector<Block*> BlockHelper::ReadXMLBlockFile(Logger logger, std::string filename)
{
	ostringstream msg;
	msg << "Entering BlockHelper::ReadXMLBlockFile(logger, " << filename;
	logger.log(DEBUG_LOG_LEVEL, msg.str());

	vector<Block*> returnValue;

	xml_document xdoc;
	xml_parse_result result = xdoc.load_file(filename.c_str());
	if (!result)
	{
		msg.clear();
		msg << "Failed to parse file: " << filename << "\nError description: " << result.description() << "\nError offset: " << result.offset;
		logger.log(ERROR_LOG_LEVEL, msg.str());
		return returnValue;
	}

	// Step one, create all of the blocks
	// We are looking for top level nodes named Block
	map<int, Block*> blocks;
	int blockID = 0;
	pugi::xml_node baseNode = xdoc.child("Blocks");
	for (pugi::xml_node node = baseNode.child("Block"); node; node = node.next_sibling("Block"))
	{
		Block* block = new Block(logger, blockID);
		if (node.attribute("Name"))
		{
			xml_attribute nameAttribute = node.attribute("Name");
			block->SetBlockName(nameAttribute.value());
			msg.clear();
			msg << "Block ID: " << blockID << " name set to: " << nameAttribute.value();
			logger.log(DEBUG_LOG_LEVEL, msg.str());
		}
		else
		{
			msg.clear();
			msg << "Block ID: " << blockID << " doesnt have a name set to it";
			logger.log(WARN_LOG_LEVEL, msg.str());
		}

		for (pugi::xml_node activationNode = node.child("Sensor"); activationNode; activationNode = activationNode.next_sibling("Sensor"))
		{
			xml_attribute activationAttribute = activationNode.attribute("SensorID");
			if (activationAttribute)
			{
				msg.clear();
				msg << "BlockID: " << blockID << " has an sensor defined, ID=" << activationAttribute.value();
				logger.log(DEBUG_LOG_LEVEL, msg.str());
				block->AddDetector(activationAttribute.as_int());
			}
		}		
		
		returnValue.push_back(block);
		blockID++;
	}

	// Step 2, added blocks, now add connections by name
	blockID = 0;
	for (pugi::xml_node node = baseNode.child("Block"); node; node = node.next_sibling("Block"))
	{
		Block* thisBlock = NULL;
		if (node.attribute("Name"))
		{
			xml_attribute nameAttribute = node.attribute("Name");
			thisBlock = GetBlockByName(returnValue, nameAttribute.value());
			if (thisBlock == NULL)
			{
				// This shouldn't ever happen, if it did its a pretty catastrophic error
				logger.log(FATAL_LOG_LEVEL, "Failed to find the block in pass 2, failing to create blocks from XML");
				return returnValue;
			}
		}
		else
		{
			// We didnt find it by name, lets just assume its by block ID
			thisBlock = returnValue[blockID];
		}

		// We have our block, now lets work on connections
		for (pugi::xml_node connectedNode = node.child("Connected"); connectedNode; connectedNode = connectedNode.next_sibling("Connected"))
		{
			xml_attribute connectedAttribute = connectedNode.attribute("BlockName");
			if (connectedAttribute)
			{
				msg.clear();
				msg << "BlockID: " << blockID << " has a connection defined to: " << connectedAttribute.value();
				logger.log(DEBUG_LOG_LEVEL, msg.str());
				Block* connectedBlock = GetBlockByName(returnValue, connectedAttribute.value());
				if (connectedBlock)
				{
					thisBlock->AddNeighbor(connectedBlock->GetID());
				}
				else
				{
					logger.log(ERROR_LOG_LEVEL, "Connected block not found, was it defined?");
				}
			}
		}
		blockID++;
	}

	// Step 3 (Eventually, allow for tasks..)
	// Now we return what we got
	msg << "Exiting BlockHelper::ReadXMLBlockFile(logger, " << filename<<"), with "<<returnValue.size()<<" blocks defined";
	logger.log(DEBUG_LOG_LEVEL, msg.str());
	return returnValue;
}

Block* BlockHelper::GetBlockByName(vector<Block*> input, std::string name)
{	
	for (int a = 0; a < input.size(); a++)
	{
		if (input[a]->GetBlockName().compare(name) == 0)
		{
			return input[a];
		}
	}

	// Didnt get it by name, return null
	return NULL;
}