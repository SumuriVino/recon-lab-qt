#include "detached_metadata_viewer.h"
#include "ui_detached_metadata_viewer.h"

detached_metadata_viewer::detached_metadata_viewer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::detached_metadata_viewer)
{
    ui->setupUi(this);
}

detached_metadata_viewer::~detached_metadata_viewer()
{
    delete ui;
}

void detached_metadata_viewer::set_metadata_content(QString data, QString searched_string)
{

    ui->textEdit_metadata->setHtml(data);

    highlight_search_text(searched_string);
}

void detached_metadata_viewer::set_window_title(QString title)
{
    setWindowTitle(title);
}

void detached_metadata_viewer::highlight_search_text(QString pattern)
{
    ui->textEdit_metadata->moveCursor(QTextCursor::Start);

    pattern = pattern.trimmed();

    if(pattern.size() < 1)
        return;

    QStringList patternlist;

    if(pattern.contains(","))
    {
        QStringList keyword_list;
        keyword_list = pattern.split(",", Qt::SkipEmptyParts);

        for(int i = 0; i < keyword_list.size(); i++)
        {
            QString temp_string = keyword_list.at(i);
            temp_string = temp_string.trimmed();
            if(temp_string == "")
            {
                continue;
            }

            patternlist.append(temp_string);
        }
    }
    else
        patternlist.append(pattern);

    QTextCharFormat fmt;
    fmt.setBackground(QColor("#D59B3F"));


    QTextCursor cursor(ui->textEdit_metadata->document());

    QString text = ui->textEdit_metadata->toPlainText().toLower();

    for(int i = 0; i < patternlist.size(); i++)
    {
        QString pattern = patternlist.at(i);
        cursor.setCharFormat(fmt);
        int length = pattern.size();

        int index = 0;
        while (index >= 0)
        {

            index = text.indexOf(pattern.toLower(), index);
            if(index < 0)
            {
                break;
            }

            cursor.setPosition(index, QTextCursor::MoveAnchor);
            cursor.setPosition(index + length, QTextCursor::KeepAnchor);

            index = index + length;

            cursor.setCharFormat(fmt);
        }
    }

}
