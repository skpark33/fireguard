#include <atlstr.h>
#include <skpark/TraceLog.h>
#include "ciElCryptoAes256.h"
#include "ci/libBase/ciStringUtil.h"
#include "ci/libBase/ciUtil.h"
#include "ci/libBase/ciTime.h"
#include <io.h>                // skpark
#include <WinCrypt.h>        // skpark


//ciSET_DEBUG(10, "ciElCryptoAes256");

/*
ciElCryptoAes256*	ciElCryptoAes256::_instance;
ciMutex				ciElCryptoAes256::_instLock;

ciElCryptoAes256*
ciElCryptoAes256::getInstance()
{
	ciGuard aGurad(_instLock);
	if(_instance==0) {
		_instance = new ciElCryptoAes256();
		_instance->Init();
		return _instance;
	}
	return _instance;
}
*/
ciElCryptoAes256::ciElCryptoAes256(void)
{
    aes256_ = NULL;
	Init();
}

ciElCryptoAes256::~ciElCryptoAes256(void)
{
    if (aes256_) {
        delete aes256_;
        aes256_ = NULL;
    }
}

bool ciElCryptoAes256::Init(std::string key32, std::string iv16, ciAES256::Chaining_Mode mode)
{
    if (key32.size() != 32) {
        TraceLog(("KEY size=%d, (%s)", key32.size(), key32.c_str()));
        return false;
    }
    if (iv16.size() != 16) {
        TraceLog(("IV size=%d, (%s)", iv16.size(), iv16.c_str()));
        return false;
    }

    key32_ = key32;
    iv16_ = iv16;

    if (aes256_) {
        delete aes256_;
        aes256_ = NULL;
    }
    aes256_ = new ciAES256(key32, mode);
    aes256_->set_IV(iv16);
    
    return true;
}

std::string ciElCryptoAes256::Encrypt(std::string plain_text, bool useGUID)
{
	if (!aes256_ || plain_text.empty()) {
        return plain_text;
    }

	if(useGUID) {
		ciString guid;
		if(CreateGUID(guid)) {
			ciString ip = ciUtil::getIpAddress();
			if(ip.size() < 16) {
				int padSize = 16-ip.size();
				while(padSize>0){
					ip += "/";
					padSize--;
				}
			}else{
				ip = ip.substr(0,15);
				ip += "/";
			}
			ciTime now;

			char prefix[69];
			memset(prefix,0x00,69);
			sprintf_s(prefix,"%04d%02d%02d%02d%02d%02d%s%s", 
				now.getYear(),now.getMonth(),now.getDay(), 
				now.getHour(), now.getMinute(), now.getSecond(),
				guid.substr(0,38).c_str(), ip.c_str());
	

			plain_text = prefix + plain_text;
			//�ð�14��{36��}10.00.00.0//// = 14+38+16 = 68����.
			// [14] == '{';
			// [51] == '}';
			// [67] == '/' 
		}
	}
	//TraceLog(("Encript(%d,%s)", useGUID, plain_text.c_str()));

    // ����е��� ����. no_padding_block = false 
    std::string cipher_text("");
	try
	{
		if (block_cipher_mode_ != ciAES256::ECB) {
			cipher_text = aes256_->encrypt(plain_text, false ).substr(16);
		} else {
			cipher_text = aes256_->encrypt(plain_text, false );
		}
	}
	catch(...)
	{
		//TraceLog(("encrypt fail !!! (%s)", plain_text.c_str()));
		cipher_text = plain_text;
	}

	std::string buf = ciStringUtil::base64_encode((const unsigned char*)cipher_text.c_str()
															, cipher_text.size());
	//TraceLog(("Encripted values=%s", buf.c_str()));

	std::string retval = ("#" + buf);

	//TraceLog(("Encripted values=%s", retval.c_str()));

	return retval;
}


std::string ciElCryptoAes256::Decrypt(std::string cipher_text)
{
	TraceLog(("Decrypt(%s)", cipher_text.c_str()));
    if (!aes256_) {
		TraceLog(("aes is null"));
        return std::string("");
    }
    if (cipher_text.empty()) {
		TraceLog(("source text is null"));
        return std::string("");
    }

    std::string res("");
	if ( block_cipher_mode_ != ciAES256::ECB ){
        res = iv16_;
    }

	if (cipher_text[0] != '#') {
		TraceLog(("It's not ciphered text"));
        return cipher_text;
    }
	cipher_text = cipher_text.substr(1);
	//TraceLog(("Decrypt2(%s)", cipher_text.c_str()));

	res.append(ciStringUtil::base64_decode(cipher_text));

    // ����е��� ����. no_padding_block = false 
	std::string retval = "";
	try
	{
		retval = aes256_->decrypt( res, false );
	}
	catch(...)
	{
		TraceLog(("decrypt fail !!! (%s)", cipher_text.c_str()));
	}

	//TraceLog(("Decripted value2=%s", retval.c_str()));
	return retval;
}


#define __DUMP(s, buf, sz)  {printf(s);                   \
                              for (int i = 0; i < (sz);i++)    \
                                  printf("%c ", (unsigned char) buf[i]); \
                              printf("\n");}


bool
ciElCryptoAes256::CreateGUID(ciString& outval)
{
#ifdef _COP_MSC_
	GUID guid;
	WCHAR szPackID[40];
	memset(szPackID,0x00,40);

	if(CoCreateGuid(&guid) != S_OK)
	{
		TraceLog(("CoCreateGuid failed"));
		return false;
	}

	if(!StringFromGUID2(guid, szPackID, 39))
	{
		TraceLog(("StringFromGUID2 failed"));
		return false;
	}

	//strGUID.Remove('-');
	//strGUID.Remove('{');
	//strGUID.Remove('}');
	//strGUID = "G_" + sGUID_plane;
	for (int i = 0; i < 40;i++){
		outval += szPackID[i]; 
	}
	TraceLog(("GUID=%s", outval.c_str()));
	return true;
#else
	return false;
#endif
}


std::string  ciAes256Util::Encrypt(std::string plain_text, bool useGUID)
{
	ciElCryptoAes256 aes;
	return aes.Encrypt(plain_text, useGUID);
}
std::string  ciAes256Util::Decrypt(std::string cipher_text)
{
	ciElCryptoAes256 aes;
	return aes.Decrypt(cipher_text);
}


bool ciAes256Util::EncryptFile(LPCTSTR inFilePath, LPCTSTR outFilePath, bool removeInFile)
{
	// ������ ������ ���̳ʸ� �������� ����
	FILE* pFile = fopen(inFilePath, "rb");
	if (!pFile)
	{
		TraceLog(("Encrypt (%s) file read error ", inFilePath));
		return false;
	}

	DWORD dwFileLen = _filelength(_fileno(pFile));
	BYTE* bBuff = new BYTE[dwFileLen];

	// ���� �о ���ۿ� ����
	fread(bBuff, 1, dwFileLen, pFile);
	fclose(pFile);


	HCRYPTPROV    hProv;
	HCRYPTHASH    hHash;
	HCRYPTKEY    hKey;
	CString        csPass = "JYT20140315197402171994082501234";

	// CSP(Crystographic Service Provider) �ڵ� ���
	if (!CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		if (!CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		{
			TraceLog(("Fail to Encrypt"));
			return false;
		}
	}

	// �ؽ� �����
	CryptCreateHash(hProv, CALG_SHA, 0, 0, &hHash);
	// �ؽ� �� ���
	CryptHashData(hHash, (BYTE*)(LPCTSTR)csPass, csPass.GetLength(), 0);
	// Ű �����\tab
	CryptDeriveKey(hProv, CALG_RC4, hHash, 0x0080 * 0x10000, &hKey);
	// ��ȣȭ\tab
	CryptEncrypt(hKey, 0, TRUE, 0, bBuff, &dwFileLen, dwFileLen);
	// �ؽ� ���ֱ�
	CryptDestroyHash(hHash);
	// CSP �ڵ� Ǯ���ֱ�
	CryptReleaseContext(hProv, 0);

	// ��ȣȭ�� ���� �����ϱ�
	pFile = fopen(outFilePath, "wb");
	fwrite(bBuff, 1, dwFileLen, pFile);
	fclose(pFile);

	// ���� ����
	delete[] bBuff;

	if (removeInFile)
	{
		if (!::DeleteFile(inFilePath))
		{
			TraceLog(("WARN : (%s) delete failed", inFilePath));
		}
	}
	return true;
}

bool ciAes256Util::DecryptFile(LPCTSTR inFilePath, LPCTSTR outFilePath, bool removeInFile)
{
	// ������ ������ ���̳ʸ� �������� ����
	FILE* pFile = fopen(inFilePath, "rb");
	if (!pFile)
	{
		TraceLog(("Decrypt (%s) file read error ", inFilePath));
		return false;
	}

	DWORD dwFileLen = _filelength(_fileno(pFile));
	BYTE* bBuff = new BYTE[dwFileLen];

	// ���� �о ���ۿ� ����
	fread(bBuff, 1, dwFileLen, pFile);
	fclose(pFile);

	HCRYPTPROV    hProv;
	HCRYPTHASH    hHash;
	HCRYPTKEY    hKey;
	CString        csPass = "JYT20140315197402171994082501234";

	// CSP(Crystographic Service Provider) �ڵ� ���
	if (!CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		if (!CryptAcquireContext(&hProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, CRYPT_NEWKEYSET))
		{
			TraceLog(("Fail to Decrypt"));
			return false;
		}
	}

	// �ؽ� �����
	CryptCreateHash(hProv, CALG_SHA, 0, 0, &hHash);
	// �ؽ� �� ���
	CryptHashData(hHash, (BYTE*)(LPCTSTR)csPass, csPass.GetLength(), 0);
	// Ű �����\tab
	CryptDeriveKey(hProv, CALG_RC4, hHash, 0x0080 * 0x10000, &hKey);
	// ��ȣȭ
	CryptDecrypt(hKey, 0, TRUE, 0, bBuff, &dwFileLen);
	// �ؽ� ���ֱ�
	CryptDestroyHash(hHash);

	// CSP �ڵ� Ǯ���ֱ�
	CryptReleaseContext(hProv, 0);


	// ��ȣȭ�� ���� �����ϱ�
	pFile = fopen(outFilePath, "wb");
	fwrite(bBuff, 1, dwFileLen, pFile);
	fclose(pFile);

	// ���� ����
	delete[] bBuff;

	if (removeInFile)
	{
		if (!::DeleteFile(inFilePath))
		{
			TraceLog(("WARN : (%s) delete failed", inFilePath));
		}
	}
	return true;
}