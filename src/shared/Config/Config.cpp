/*
 * This file is part of the CMaNGOS Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <algorithm>
#include <mutex>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>


#include "Config.h"
//#include "ace/Configuration_Import_Export.h"


#include "Policies/Singleton.h"

INSTANTIATE_SINGLETON_1(Config);


Config::Config()
    : mConf(nullptr)
{
}

Config::~Config()
{
    delete mConf;
}

bool Config::SetSource(const char* file)
{
    mFilename = file;

    return Reload();
}

bool Config::Reload()
{
    delete mConf;
    mConf = new ConfigMgr;
	std::string configError;
	if (!mConf->LoadInitial(mFilename, configError))
	{
		printf("Error in config file: %s\n", configError.c_str());
		return false;
	}
	DB_CREATURE_DamageMultiplier = GetFloatDefault("DB_CREATURE_DamageMultiplier", false);
	DB_CREATURE_ArmorMultiplier = GetFloatDefault("DB_CREATURE_ArmorMultiplier", false);
	DB_CREATURE_HealthMultiplier = GetFloatDefault("DB_CREATURE_HealthMultiplier", false);

    return true;
}

std::string Config::GetStringDefault(const char* name, const char* def)
{
	return mConf->GetStringDefault(name, def);
}

bool Config::GetBoolDefault(const char* name, bool def)
{
	return mConf->GetBoolDefault(name, def);
}

int32 Config::GetIntDefault(const char* name, int32 def)
{
	return mConf->GetIntDefault(name, def);
}

float Config::GetFloatDefault(const char* name, float def)
{
	return mConf->GetFloatDefault(name, def);
}


using namespace boost::property_tree;

bool ConfigMgr::LoadInitial(std::string const& file, std::string& error)
{
	std::lock_guard<std::mutex> lock(_configLock);

	_filename = file;

	try
	{
		ptree fullTree;
		ini_parser::read_ini(file, fullTree);

		if (fullTree.empty())
		{
			error = "empty file (" + file + ")";
			return false;
		}

		// Since we're using only one section per config file, we skip the section and have direct property access
		_config = fullTree.begin()->second;
	}
	catch (ini_parser::ini_parser_error const& e)
	{
		if (e.line() == 0)
			error = e.message() + " (" + e.filename() + ")";
		else
			error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
		return false;
	}

	return true;
}

bool ConfigMgr::Reload(std::string& error)
{
	return LoadInitial(_filename, error);
}

std::string ConfigMgr::GetStringDefault(std::string const& name, const std::string& def)
{
	std::string value = _config.get<std::string>(ptree::path_type(name, '/'), def);

	value.erase(std::remove(value.begin(), value.end(), '"'), value.end());

	return value;
}

bool ConfigMgr::GetBoolDefault(std::string const& name, bool def)
{
	try
	{
		std::string val = _config.get<std::string>(ptree::path_type(name, '/'));
		val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
		return (val == "true" || val == "TRUE" || val == "yes" || val == "YES" || val == "1");
	}
	catch (std::exception const&  /*ex*/)
	{
		return def;
	}
}

int ConfigMgr::GetIntDefault(std::string const& name, int def)
{
	return _config.get<int>(ptree::path_type(name, '/'), def);
}

float ConfigMgr::GetFloatDefault(std::string const& name, float def)
{
	return _config.get<float>(ptree::path_type(name, '/'), def);
}

std::string const& ConfigMgr::GetFilename()
{
	std::lock_guard<std::mutex> lock(_configLock);
	return _filename;
}

std::list<std::string> ConfigMgr::GetKeysByString(std::string const& name)
{
	std::lock_guard<std::mutex> lock(_configLock);

	std::list<std::string> keys;

	for (const ptree::value_type& child : _config)
	if (child.first.compare(0, name.length(), name) == 0)
		keys.push_back(child.first);

	return keys;
}
