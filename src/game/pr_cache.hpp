
#ifndef PRCACHE_H
#define PRCACHE_H

/*
#include <boost/serialization/access.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/assume_abstract.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/bitset.hpp>
#include <boost/serialization/collections_load_imp.hpp>
#include <boost/serialization/collections_save_imp.hpp>
#include <boost/serialization/collection_size_type.hpp>
#include <boost/serialization/collection_traits.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/serialization/config.hpp>
#include <boost/serialization/deque.hpp>
#include <boost/serialization/ephemeral.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/extended_type_info_no_rtti.hpp>
#include <boost/serialization/extended_type_info_typeid.hpp>
#include <boost/serialization/factory.hpp>
#include <boost/serialization/force_include.hpp>
#include <boost/serialization/forward_list.hpp>
#include <boost/serialization/hash_collections_load_imp.hpp>
#include <boost/serialization/hash_collections_save_imp.hpp>
#include <boost/serialization/hash_map.hpp>
#include <boost/serialization/hash_set.hpp>
#include <boost/serialization/is_bitwise_serializable.hpp>
#include <boost/serialization/item_version_type.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/level_enum.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/priority_queue.hpp>
#include <boost/serialization/queue.hpp>
#include <boost/serialization/scoped_ptr.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/shared_ptr_132.hpp>
#include <boost/serialization/shared_ptr_helper.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/serialization/slist.hpp>
#include <boost/serialization/smart_cast.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/stack.hpp>
#include <boost/serialization/state_saver.hpp>
#include <boost/serialization/static_warning.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/serialization/throw_exception.hpp>
#include <boost/serialization/tracking.hpp>
#include <boost/serialization/tracking_enum.hpp>
#include <boost/serialization/traits.hpp>
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include <boost/serialization/unordered_collections_load_imp.hpp>
#include <boost/serialization/unordered_collections_save_imp.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/valarray.hpp>
#include <boost/serialization/variant.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/void_cast.hpp>
#include <boost/serialization/void_cast_fwd.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/serialization/wrapper.hpp>
#include "boost/serialization/throw_exception.hpp" 
#include "boost/exception/exception.hpp" 
#include <fstream>
*/
#include <string>
#include "ObjectMgr.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include "Log.h"
#include <boost/serialization/split_free.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include "boost/iostreams/stream.hpp"
#include "pr_md5lib.h"
#include "pr_base64.h"
#include "pr_zip.h"
#include "pr_aes.h"

namespace boost {
	namespace serialization {

		template<class Archive, typename T>
		 void serialize(Archive & ar,	tbb::concurrent_unordered_map<uint32, T> & map,	const unsigned int file_version){
			split_free(ar, map, file_version);
		}

		template<class Archive, typename T>
		 void save(Archive & ar, const tbb::concurrent_unordered_map<uint32, T> & map, unsigned int version)
		{
			std::unordered_map<uint32, T> saveMap;
			for (tbb::concurrent_unordered_map<uint32, T>::const_iterator itr = map.begin(); itr != map.end(); ++itr)
				saveMap[(uint32)itr->first] = itr->second;
			ar&saveMap;
		}
		template<class Archive, typename T>
		 void load(Archive & ar, tbb::concurrent_unordered_map<uint32, T> & map, unsigned int version)
		{
			 std::unordered_map<uint32, T> saveMap;
			 ar & saveMap;
			 for (std::unordered_map<uint32, T>::iterator itr = saveMap.begin(); itr != saveMap.end(); ++itr)
				 map[(uint32)itr->first] = itr->second;
		}

	} // namespace serialization
} // namespace boost


class MANGOS_DLL_SPEC PR_Cache{
	public:
		bool fileExist(std::string file){
			return !access(file.c_str(), 0);
		}

		template<typename T>
		void saveCache(T& data, std::string filename)
		{

			std::string md5 = "./cache/" + getMD5(filename);
			if (!fileExist(md5))
			{
				std::ostringstream archive_stream;//定义字符串流
				boost::archive::text_oarchive buffer(archive_stream, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking);//准备内存块
				buffer << data;//将数据写入内存块
				
				std::string bufferedStr = archive_stream.str();//将内存数据块转换为字符串
				//sLog.outString(">>> before %u", bufferedStr.length()/1024);
				std::string zipedStr = pr_compress(bufferedStr);	//压缩字符串
				//sLog.outString(">>> after %u", zipedStr.length() / 1024);
				//std::string base64Str = base64_encode(bufferedStr);//进行简单base64加密
				std::string encodStr = pr_encrypt(zipedStr);  //将数据进行AES加密
				

				std::ofstream ofs(md5, std::ios::binary | std::ios::out);
				boost::archive::binary_oarchive  oa(ofs, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking);
				oa << encodStr; //将加密数据写入文件
			}
			

		}

		template<typename T>
		bool loadCache(T& data, std::string filename)
		{
			std::string md5 = "./cache/" + getMD5(filename);
			if (fileExist(md5))
			{
				ifstream ifs(md5, std::ios::binary | std::ios::in);
				boost::archive::binary_iarchive ia(ifs, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking);

				std::string encodStr;//定义数据

				ia >> encodStr;//读入字符串

				

				std::string zipedStr = pr_decrypt(encodStr); //解密

				//std::string bufferedStr = base64_decode(base64Str); //将Base64的密文还原为原始字符串

				std::string bufferedStr = pr_decompress(zipedStr);//解压缩zip字符串

				std::istringstream archive_stream(bufferedStr);//定义输入字符串流，并将解密后数据作为流
				boost::archive::text_iarchive buffer(archive_stream, boost::archive::no_header | boost::archive::no_codecvt | boost::archive::no_xml_tag_checking);//准备内存块

				buffer >> data;//将内存块数据还原为data

				return true;
			}
			return false;
		}
};

#endif //PRCACHE_H