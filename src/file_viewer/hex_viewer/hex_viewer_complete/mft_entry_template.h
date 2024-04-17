#ifndef MFT_ENTRY_TEMPLATE_H
#define MFT_ENTRY_TEMPLATE_H

#include <QDialog>
#include "common_header.h"
#include "NTFS/ntfs_parser.h"
#include <QtCore>
#include <QtGui>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QAbstractItemView>

namespace Ui {
class MFT_entry_template;
}

class MFT_entry_template : public QDialog
{
    Q_OBJECT

public:
    explicit MFT_entry_template(QWidget *parent = 0);
    ~MFT_entry_template();

    void pub_set_data(QByteArray data);

private:
    Ui::MFT_entry_template *ui;

    QByteArray mft_entry_data;

};

#endif // MFT_ENTRY_TEMPLATE_H
