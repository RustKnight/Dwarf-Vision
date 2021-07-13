#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Core.h"
#include "Console.h"
#include "DataDefs.h"
#include "Export.h"
#include "MiscUtils.h"
#include "PluginManager.h"

#include "modules/Gui.h"
#include "modules/Translation.h"
#include "modules/Units.h"
#include "modules/Filesystem.h"
#include "modules/World.h"

#include "df/creature_raw.h"
#include "df/caste_raw.h"
#include "df/world.h"
#include "df/world_data.h"
#include "df/descriptor_color.h"
#include "df/descriptor_pattern.h"
#include "df/color_modifier_raw.h"
#include "df/tissue_style_raw.h"


using namespace DFHack;
using namespace Units;
using namespace Translation;
//using namespace World;

using namespace df::enums;
using df::global::world;



DFHACK_PLUGIN("dwarfinfo");

const char help[] = "'dwarfinfo' when in look mode to write info about neighboring dwarfs.\n\
'dwarfinfo -n 7' to look 7 cells around cursor(by default n=5)\n\
'dwarfinfo -o filename.txt' to choose name of the recorded file(by default temp/message.txt\n\
'dwarfinfo -h' to show this help\n\
'dwarfinfo -v' toggle verboose mode\n";



struct dwarf
{
	int32_t id;
	bool targeted;
};

bool compareDwarfs(dwarf *a, dwarf *b) { return a->id < b->id; }
bool equalDwarfs(dwarf *a, dwarf *b) { return a->id == b->id && a->targeted==b->targeted; }


command_result getDwarfInfo(color_ostream &out, std::vector<std::string> & params)
{
	std::string subdirName = "temp";
	std::string filename = subdirName + "/" + "message.txt";
	int32_t n = 5;
	bool verbooseOn = false;
	int32_t x, y, z;
	
	
	for (size_t i = 0; i < params.size(); i++){
		
		if (params[i] == "-h")
		{
			out.print("%s\n", help);
			return CR_OK;
		}
		else if (params[i] == "-n")
		{
			int32_t area;
			try
			{
				area = stoi(params[i + 1]);
			}
			catch (std::invalid_argument)
			{
				//out.print("%s\n", help);
				return CR_WRONG_USAGE;
			}
			n = area;
			i++;
		}
		else if (params[i] == "-o")
		{
			filename = params[i + 1];
			i++;
		}
		
		else if (params[i] == "-v")
		{
			verbooseOn = !verbooseOn;
		}
		
		else
		{
			//out.print("%s\n", help);
			return CR_WRONG_USAGE;
		}
	}
	
	
	if (Gui::getCursorCoords(x, y, z))
	{
		//out.print("%d %d %d\n",x,y,z);
	}
	
	else
	{
		if (verbooseOn){
			
			out.printerr("You're not in look mode!\n");	
			return CR_WRONG_USAGE;
		}
		
		else
			return CR_OK;
	}



	std::string curDir = Filesystem::getcwd();
	Filesystem::mkdir(curDir + "/" + subdirName);

	std::vector<df::unit*> units;
	getUnitsInBox(units,x-n,y-n,z,x+n,y+n,z);
	uint32_t countDwarfs = 0;

	//Get new info
	std::vector<dwarf*> dwarfs;
	for (size_t i = 0; i < units.size(); i++)
	{
		if (!isDwarf(units[i])) continue;
		dwarf *d = new dwarf;
		d->id = units[i]->id;
		df::coord unitPos = getPosition(units[i]);
		d->targeted = unitPos.x == x && unitPos.y == y && unitPos.z == z;
		dwarfs.push_back(d);
	}
	std::sort(dwarfs.begin(), dwarfs.end(), compareDwarfs);

	//Read old info from file
	std::vector<dwarf*> prevDwarfs;
	std::ifstream infile(filename);
	if (infile)
	{
		

		std::string oldWorldName;
		std::getline(infile, oldWorldName);

		while (!infile.eof())
		{
			dwarf *d = new dwarf;
			infile >> d->targeted;
			infile.ignore(1);
			infile >> d->id;
			prevDwarfs.push_back(d);
			infile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//out.print("%d $d\n",d->id,d->targeted);
		}

		std::sort(prevDwarfs.begin(), prevDwarfs.end(), compareDwarfs);

		/*for (size_t i = 0; i < prevDwarfs.size(); i++)
		{
			out.print("%d %d, ", prevDwarfs[i]->id, prevDwarfs[i]->targeted);
		}
		out.print("\n");
		for (size_t i = 0; i < dwarfs.size(); i++)
		{
			out.print("%d %d, ", dwarfs[i]->id, dwarfs[i]->targeted);
		}*/

		//here checking changes
		if (std::equal(dwarfs.begin(), dwarfs.end(), prevDwarfs.begin(), prevDwarfs.end(), equalDwarfs))
		{
			if (verbooseOn)
				out.print("Nothing was changed\n");
			
			infile.close();//needed?
			return CR_OK;
		}
	}
	//Output if changed
	std::ofstream outfile(filename);
	df::world_data *data = df::global::world->world_data;
	outfile << TranslateName(&data->name, false);
	for (size_t i = 0; i < dwarfs.size(); i++)
	{
		outfile << endl;
		int32_t index = findIndexById(dwarfs[i]->id);//id and index? wtf
		df::unit *u = getUnit(index);
		//df::unit *u = getUnit(dwarfs[i]->id);
		outfile << dwarfs[i]->targeted << "=";

		df::language_name *unitName = getVisibleName(u);
		std::string PhysDesc = getPhysicalDescription(u);
		outfile << dwarfs[i]->id << "=";
		outfile << DF2UTF(TranslateName(unitName, false)) << "=";
		outfile << u->sex << "=";
		outfile << getAge(u, false) << "=";
		outfile << PhysDesc << "=";
		
		
		std::string bpDataStr;
		for (int j = 0; j < u->appearance.bp_modifiers.size(); ++j)
			bpDataStr.append(std::to_string(u->appearance.bp_modifiers[j]) + ", ");
		
		outfile << bpDataStr;

		//END of bp
		outfile << "=";


		
		// guard against dupplicates
		// write string instead of ints for hairstyles
		// find an order to write to temp ; seperate with =

		auto creatureRaw = world->raws.creatures.all[u->race];
		auto casteRaw = creatureRaw->caste[u->caste];
		std::vector<std::string> styleTokens;	// a check-list of what styles have already been printed

		for (size_t j = 0; j < u->appearance.tissue_style_type.size(); j++)
		{


			auto type = u->appearance.tissue_style_type[j];
			if (type < 0)
				continue;
			int style_raw_index = binsearch_index(casteRaw->tissue_styles, &df::tissue_style_raw::id, type);
			auto styleRaw = casteRaw->tissue_styles[style_raw_index];

			// skip over dupplicate styleTokens
			bool duplicateFound = false;
			for (int i = 0; i < styleTokens.size(); ++i) {

				if (styleTokens[i] == styleRaw->token)
					duplicateFound = true;
			}
			if (duplicateFound)
				continue;
			
			auto tissueLen = u->appearance.tissue_length[j];
			auto tissueStyle = u->appearance.tissue_style[j];
			/*hair style, got it from RemoteFortressReader.pb.h
				UNKEMPT = -1,
				NEATLY_COMBED = 0,
				BRAIDED = 1,
				DOUBLE_BRAID = 2,
				PONY_TAILS = 3,
				CLEAN_SHAVEN = 4
			*/
			outfile << styleRaw->token.c_str() << ":";
			outfile << (tissueLen) << ":";
			//out.print("Style: %d\n", tissueStyle);
			std::string style;
			switch (tissueStyle) {

			case -1: style = "unkept";			break;
			case  0: style = "neatlycombed";	break;
			case  1: style = "braided";			break;
			case  2: style = "doublebraided";	break;
			case  3: style = "ponytail";		break;
			case  4: style = "clean-shaven";	break;
			}
			outfile << style.c_str();

			//add ; if we're not at last loop iteration
			if (j < (u->appearance.tissue_style_type.size() -1))
				outfile << ";";

			styleTokens.push_back(styleRaw->token);	// keep a list of what we already added so far
		}

		//END of hairStyles
		outfile << "=";

		auto colorsList = casteRaw->color_modifiers;
		 
		auto strukt = u->appearance.colors;
		//out.print("cm.size: %d\n",casteRaw->color_modifiers.size());
		

		for (int j = 0; j < strukt.size(); j++) {


			auto color = u->appearance.colors[j];
			auto p = casteRaw->color_modifiers[j]->pattern_index[color];
			// out.print("part: %s ", casteRaw->color_modifiers[j]->part.c_str());		-> part name
			// out.print("%d ", p);														-> value 
			

			auto dc = df::descriptor_pattern::find(p);

			std::string str = dc->id;
			outfile << str.c_str();	// -> actual color string
			outfile << ";";
		}

		outfile << "=";
		
		int curly_idx;
		for (size_t j = 0; j < casteRaw->bp_appearance.modifier_idx.size(); j++)
		{
			if (casteRaw->bp_appearance.modifier_idx[j] == 28)//!!!must get it from casteRaw->bp_appearance.modifiers, but for type == 8 (CURLY) it has id 33, not 28...
			{
				curly_idx = j;
				break;
			}
		}

		outfile << u->appearance.bp_modifiers[curly_idx];

		// hairstyle[n]		= MOUSTACHE:neatlycombed;HAIR:braided
		// colors	[n]		= [hair]string; [skin]string; [iris]string
	}

	outfile.close();

	for (size_t i = 0; i < prevDwarfs.size(); i++)
	{
		delete prevDwarfs[i];
	}
	for (size_t i = 0; i < dwarfs.size(); i++)
	{
		delete dwarfs[i];
	}

	if (verbooseOn)
		out.print("Info about %u dwarfs was written in file '%s'\n", dwarfs.size(), filename.c_str());



	return CR_OK;
}

DFhackCExport command_result plugin_init ( color_ostream &out, std::vector <PluginCommand> &commands)
{
	commands.push_back(PluginCommand("dwarfinfo", "Get info about dwarfs and write into file", getDwarfInfo, false,
		help));
    return CR_OK;
}
