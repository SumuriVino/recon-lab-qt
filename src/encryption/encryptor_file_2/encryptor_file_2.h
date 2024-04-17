#ifndef ENCRYPTOR_FILE_2
#define ENCRYPTOR_FILE_2

#include "qaesencryption.h"
#include <QtCore>

class encryptor_file_2
{
public:
    encryptor_file_2();

    bool pub_encrypt_file(QString source_file, QString dest_file);
    bool pub_decrypt_file(QString source_file, QString dest_file);

private:
    QString key;
    QString iv;

};

#endif // ENCRYPTOR_FILE_2
