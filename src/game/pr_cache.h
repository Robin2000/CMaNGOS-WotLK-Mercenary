#if !defined(PRCACHE_H)
#define PRCACHE_H

#include <string>
#include "md5lib.h"
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>

namespace boost {
	namespace serialization {

		template<class Archive>
		void serialize(Archive & ar, QueryResult & result, const unsigned int version)
		{
			ar & result;

			/*
			ar & result.GetRowCount();
			ar & result.GetFieldCount();
			Field* pointer = result.Fetch();
			Field field;
			for (int i = 0; i < result.GetRowCount();i++)
			for (int j = 0; j < result.GetFieldCount(); j++)
			{
				pointer = i*j;
				field= *pointer;
				ar & field.GetType();
				ar & field.GetCppString();
			}*/
		}

	} // namespace serialization
} // namespace boost


class PrCache
{
public:
	PrCache(){}

	void load(QueryResult & result, const char* sql){

		std::string md5 = "./cache/" + getMD5(sql, string_length(sql));

		if (fileNotExist(md5)){

			QueryResult * result2 = WorldDatabase.Query(sql);/*����ʱ���������ѯ���*/
			result = *result2;                               /*����ʱ����ʵ�ʵ�ַ����ò���*/
			//delete result2  �����ͷſռ䣬ʹ���������ʩ�Ÿÿռ䡣

			std::ofstream ofs(md5, ios::binary);
			boost::archive::binary_oarchive oa(ofs);
			oa << result2;
			
			//ar & result;
			//SerializeCStringHelper helper(sql);
			//ar & helper; //���Լ򵥵��ַ���

			//ofs.close();
			//delete &md5;
			return;

		}
		//result = readQueryResultToFile(md5);
		//���д����ȡ


		std::ifstream ifs(md5, ios::binary);
		boost::archive::binary_iarchive ia(ifs);
		//ar & result;

		QueryResult * result2;
		ia >> result2; 
		result = *result2;
		
		//ifs.close();
		//delete &md5;

	}

	int string_length(const char *str)
	{
		int len = 0;
		while (*str++ != '\0')
		{
			len++;
		}
		return len;
	}
	bool fileNotExist(std::string file){
		fstream _file;
		_file.open(file, ios::in);
		return (!_file);
	}
	
};

//extern PrCache* prCache; //����ȫ�ֶ���

#endif //PRCACHE_H