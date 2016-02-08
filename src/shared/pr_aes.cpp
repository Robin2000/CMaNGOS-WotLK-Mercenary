#include <openssl\aes.h>
#include <openssl\rand.h>
#include <openssl\evp.h>
#include <string>
//#pragma comment(lib,"libeay32.lib")
//#pragma comment(lib,"ssleay32.lib")

#define BIG_TEST_SIZE 10240
using namespace std;

std::string EncodeAES( /*const std::string&*/char * strPassword, const std::string& strData)
{
	AES_KEY aes_key;
	if (AES_set_encrypt_key((const unsigned char*)strPassword, AES_BLOCK_SIZE * 8/*strlen(strPassword)*8 *//*strPassword.length() * 8*/, &aes_key) < 0)
	{
		return "";
	}
	std::string strRet;
	for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		AES_encrypt((const unsigned char*)str16.c_str(), out, &aes_key);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	return strRet;
}
std::string EncodeAES_little( /*const std::string&*/char * strPassword, const std::string& strData)
{
	AES_KEY aes_key;
	if (AES_set_encrypt_key((const unsigned char*)strPassword, AES_BLOCK_SIZE * 8/*strlen(strPassword)*8*/ /*strPassword.length() * 8*/, &aes_key) < 0)
	{
		return "";
	}
	unsigned char out[AES_BLOCK_SIZE];
	AES_encrypt((const unsigned char*)strData.c_str(), out, &aes_key);
	return std::string((const char*)out);
}

std::string EncodeAES_Big( /*const std::string&*/char * strPassword, const std::string& strData)
{
	AES_KEY aes_key;
	if (AES_set_encrypt_key((const unsigned char*)strPassword, AES_BLOCK_SIZE * 8/*strlen(strPassword)*8 *//*strPassword.length() * 8*/, &aes_key) < 0)
	{
		return "";
	}
	std::string strRet;
	unsigned int i = 0;
	std::string str16;
	unsigned char out[AES_BLOCK_SIZE];
	for (; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		AES_encrypt((const unsigned char*)str16.c_str(), out, &aes_key);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	str16 = strData.substr(i*AES_BLOCK_SIZE, strData.length() - i*AES_BLOCK_SIZE);
	AES_encrypt((const unsigned char*)str16.c_str(), out, &aes_key);
	strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	return strRet;
}


std::string DecodeAES( /*const std::string&*/char * strPassword, const std::string& strData)
{
	AES_KEY aes_key;
	if (AES_set_decrypt_key((const unsigned char*)strPassword, AES_BLOCK_SIZE * 8/*strlen(strPassword)*8*/ /*strPassword.length() * 8*/, &aes_key) < 0)
	{
		return "";
	}
	std::string strRet;
	for (unsigned int i = 0; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		unsigned char out[AES_BLOCK_SIZE];
		AES_decrypt((const unsigned char*)str16.c_str(), out, &aes_key);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}
	return strRet;
}
std::string DecodeAES_little( /*const std::string&*/char * strPassword, const std::string& strData)
{
	AES_KEY aes_key;
	if (AES_set_decrypt_key((const unsigned char*)strPassword, AES_BLOCK_SIZE * 8/*strlen(strPassword)*8*/ /*strPassword.length() * 8*/, &aes_key) < 0)
	{
		return "";
	}
	unsigned char out[AES_BLOCK_SIZE];
	AES_decrypt((const unsigned char*)strData.c_str(), out, &aes_key);
	return std::string((const char*)out);
}

std::string DecodeAES_Big( /*const std::string&*/char * strPassword, const std::string& strData)
{
	AES_KEY aes_key;
	if (AES_set_decrypt_key((const unsigned char*)strPassword, AES_BLOCK_SIZE * 8/*strlen(strPassword)*8*/ /*strPassword.length() * 8*/, &aes_key) < 0)
	{
		return "";
	}
	std::string strRet;
	unsigned int i = 0;
	unsigned char out[AES_BLOCK_SIZE];
	for (; i < strData.length() / AES_BLOCK_SIZE; i++)
	{
		std::string str16 = strData.substr(i*AES_BLOCK_SIZE, AES_BLOCK_SIZE);
		AES_decrypt((const unsigned char*)str16.c_str(), out, &aes_key);
		strRet += std::string((const char*)out, AES_BLOCK_SIZE);
	}

	std::string str16 = strData.substr(i*AES_BLOCK_SIZE, strData.length() - i*AES_BLOCK_SIZE);
	AES_decrypt((const unsigned char*)str16.c_str(), out, &aes_key);
	strRet += std::string((const char*)out, AES_BLOCK_SIZE);

	return strRet;
}
static unsigned char userkey[AES_BLOCK_SIZE] = { 'm', 'n', 'a', 'd', 'b', 'f', 'g', 'h', 'o', 'j', 's', 'l', 'c', 'e', 'i','k'};
std::string pr_encrypt(const std::string in){//¼ÓÃÜ

	if (in.length() % 16 == 0)
		return EncodeAES((char*)userkey, in);

	if (in.length()<16)
		return EncodeAES_little((char*)userkey, in);

	return EncodeAES_Big((char*)userkey, in);


}
std::string pr_decrypt(const std::string in){//½âÃÜ

	if (in.length() % 16 == 0)
		return DecodeAES((char*)userkey, in);

	if (in.length()<16)
		return DecodeAES_little((char*)userkey, in);

	return DecodeAES_Big((char*)userkey, in);
}