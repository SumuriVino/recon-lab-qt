#ifndef GPT_TEMPLATE_H
#define GPT_TEMPLATE_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QAbstractItemView>
#include "file_viewer/hex_viewer/hex_viewer_complete/common_header.h"
#include "file_viewer/hex_viewer/hex_viewer_complete/GPT/gpt_parser.h"


namespace Ui {
class gpt_template;
}

class gpt_template : public QDialog
{
    Q_OBJECT

public:
    explicit gpt_template(QWidget *parent = 0);
    ~gpt_template();
    void pub_set_data(QByteArray data);

private:
    Ui::gpt_template *ui;

    QByteArray gpt_data;
};

#endif // GPT_TEMPLATE_H
