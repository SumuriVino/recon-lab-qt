#include "detached_metadata_viewer_module.h"
#include "ui_detached_metadata_viewer_module.h"

detached_metadata_viewer_module::detached_metadata_viewer_module(const QString &title, QWidget *parent) :
    QDialog(parent), w_title(title),
    ui(new Ui::detached_metadata_viewer_module)
{
    ui->setupUi(this);

//    QString set_title = w_title + " - Detached [Detailed Information]";
//    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));
}

detached_metadata_viewer_module::~detached_metadata_viewer_module()
{
    delete ui;
}

void detached_metadata_viewer_module::set_metadata_content(QString data, QString searched_string)
{

    ui->textEdit_metadata->setHtml(data);

    highlight_search_text(searched_string);
}

void detached_metadata_viewer_module::highlight_search_text(QString pattern)
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

void detached_metadata_viewer_module::set_window_title(QString module_name)
{
    QString set_title = module_name + " - Detached [Detailed Information]";
    setWindowTitle(QObject::tr(set_title.toLocal8Bit().data()));

}
