@REM
@REM
cd Wpa_supplicant/wpa_supplicant-2.9/
copy src/utils/build-config.h   ../../AmiCryptoPkg/CryptoLib
copy src/utils/includes.h       ../../AmiCryptoPkg/CryptoLib
copy src/utils/common.h         ../../AmiCryptoPkg/CryptoLib
copy src/utils/os.h             ../../AmiCryptoPkg/CryptoLib
copy src/utils/os.h             ../../AmiCryptoPkg/CryptoLib
copy src/utils/wpa_debug.h      ../../AmiCryptoPkg/CryptoLib
copy src/utils/wpa_debug.c      ../../AmiCryptoPkg/CryptoLib
copy src/crypto/crypto.h        ../../AmiCryptoPkg/CryptoLib
copy src/crypto/md5.h           ../../AmiCryptoPkg/CryptoLib
copy src/crypto/md5.c           ../../AmiCryptoPkg/CryptoLib
copy src/crypto/sha1.h          ../../AmiCryptoPkg/CryptoLib
copy src/crypto/sha1.c          ../../AmiCryptoPkg/CryptoLib
copy src/crypto/sha256.h        ../../AmiCryptoPkg/CryptoLib
copy src/crypto/sha256.c        ../../AmiCryptoPkg/CryptoLib
copy src/crypto/sha384.c        ../../AmiCryptoPkg/CryptoLib
copy src/crypto/sha512.c        ../../AmiCryptoPkg/CryptoLib
copy src/tls/asn1.h             ../../AmiCryptoPkg/CryptoLib
copy src/tls/asn1.c             ../../AmiCryptoPkg/CryptoLib
copy src/tls/rsa.h              ../../AmiCryptoPkg/CryptoLib
copy src/tls/rsa.c              ../../AmiCryptoPkg/CryptoLib
copy src/tls/bignum.h           ../../AmiCryptoPkg/CryptoLib
copy src/tls/bignum.c           ../../AmiCryptoPkg/CryptoLib
copy src/tls/libtommath.h       ../../AmiCryptoPkg/CryptoLib
copy src/tls/x509v3.c             ../../AmiCryptoPkg/CryptoLib
copy src/tls/x509v3.h             ../../AmiCryptoPkg/CryptoLib
cd ..
