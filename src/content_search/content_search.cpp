#include "content_search.h"
#include "ui_content_search.h"

content_search::content_search(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::content_search)
{
    ui->setupUi(this);

    create_ui();

}

content_search::~content_search()
{
    delete ui;
}

void content_search::pub_display_keyword_list()
{

}

void content_search::pub_set_essentials()
{// set caller type (pre launcher or post launcher lab)
    recon_configuration_page_keyword_list_obj->pub_set_caller_type(MACRO_CONFIGURATION_CALLER_TYPE_POST_LAUNCHER);
}

void content_search::pub_display_for_popup()
{
    QDialog dialog;
    setWindowFlags(dialog.windowFlags());
}

void content_search::slot_pushButton_done_clicked()
{ // content searched with search label
    QString search_label = lineedit_search_label->text().trimmed();
    if(search_label.isEmpty())
        search_label = lineedit_search_label->placeholderText().trimmed();

    emit signal_content_search_pushbutton_done_clicked(search_label);

    hide();
}

void content_search::slot_pushButton_cancel_clicked()
{ // hide previous window if cancel the content search
    hide();
}

void content_search::create_ui()
{ // create ui stack widget and add content search window on it
    stack_widget_main = new QStackedWidget(this);

    create_done_cancel_button_box();

    vbox_main = new QVBoxLayout;

    vbox_main->addWidget(stack_widget_main);
    vbox_main->addLayout(hbox_done_cancel_buttons);

    recon_configuration_page_keyword_list_obj = new recon_configuration_page_keyword_list(this);

    stack_widget_main->addWidget(recon_configuration_page_keyword_list_obj);

    stack_widget_main->setCurrentIndex(0);


    vbox_main->setContentsMargins(0,0,0,0);
    vbox_main->setSpacing(0);


    setLayout(vbox_main);
}

void content_search::pub_clear_search_label()
{ // clear search label
    lineedit_search_label->clear();
    lineedit_search_label->setPlaceholderText(QString("Content Search"));
}


void content_search::create_done_cancel_button_box()
{ // call when we search or cancel the content search accordingly
    //pushButton_done = new QPushButton("Done", this);
    pushButton_done = new QPushButton("Start", this);

    lineedit_search_label = new QLineEdit(this);
    lineedit_search_label->setAttribute(Qt::WA_MacShowFocusRect, false);

    pushButton_cancel = new QPushButton("Cancel", this);

    hbox_done_cancel_buttons = new QHBoxLayout;

    hbox_done_cancel_buttons->addWidget(lineedit_search_label);
    //hbox_done_cancel_buttons->addStretch();
    hbox_done_cancel_buttons->addWidget(pushButton_cancel);
    hbox_done_cancel_buttons->addWidget(pushButton_done);
    hbox_done_cancel_buttons->setContentsMargins(10,0,2,0);

    hbox_done_cancel_buttons->setSpacing(5);

    connect(pushButton_cancel, SIGNAL(clicked()), this, SLOT(slot_pushButton_cancel_clicked()));
    connect(pushButton_done, SIGNAL(clicked()), this, SLOT(slot_pushButton_done_clicked()));

}
