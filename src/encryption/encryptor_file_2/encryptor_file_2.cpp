#include "encryptor_file_2.h"

encryptor_file_2::encryptor_file_2()
{
    key = "#$knfgcoivf3r2r35rccr3p9r8c4pcqr@#R$#@";
    iv = "cgnrioy34T#$T%$%^r32crp8m4tr2e52f%$f235" ;
}


bool encryptor_file_2::pub_encrypt_file(QString source_file, QString dest_file)
{
    QFile mfile(source_file);

    if(!mfile.open(QIODevice::ReadOnly))
        return false;

    QByteArray source_data = mfile.readAll();

    mfile.close();

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);


    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray encodeText = encryption.encode(source_data, hashKey, hashIV);


    QFile tfile(dest_file);

    if(!tfile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    tfile.write(encodeText);

    tfile.close();

    return true;

}

bool encryptor_file_2::pub_decrypt_file(QString source_file, QString dest_file)
{
    QFile mfile(source_file);

    if(!mfile.open(QIODevice::ReadOnly))
        return false;
    QByteArray source_data = mfile.readAll();

    mfile.close();

    QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);


    QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
    QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

    QByteArray decodeText = encryption.decode(source_data, hashKey, hashIV);


    QFile tfile(dest_file);

    if(!tfile.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    tfile.write(decodeText);

    tfile.close();

    return true;

}
