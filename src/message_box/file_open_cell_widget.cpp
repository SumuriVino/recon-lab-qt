#include "file_open_cell_widget.h"

file_open_cell_widget::file_open_cell_widget(QWidget *parent) :
    QWidget(parent)
{

}

file_open_cell_widget::file_open_cell_widget(QString path)
{
    //    file_path = path;
    QFileInfo info(path);

    file_path = info.absoluteFilePath();
    // file_path = file_path.remove(info.fileName());

    label_file = new QLabel(this);
    if(info.isDir())
        label_file->setText(info.filePath());
    else
        label_file->setText(info.fileName());

    pushbutton_open = new QPushButton(this);
    pushbutton_open->setStyleSheet("QPuhButton:flat{border: none; background-color: transparent;}"
                                   "QPuhButton{background:transparent;}"
                                   "QPushButton:checked{ background-color: transparent; border: none;  }"
                                   "QPushButton:!checked{ background-color: transparent; border: none;  }"
                                   "QPushButton:clicked{ background-color: transparent;  border: none;}"
                                   "QPushButton:pressed { background-color: transparent; border: none; }"
                                   );
    pushbutton_open->setFixedSize(22,22);
    pushbutton_open->setIcon(QIcon("../icons/signs/browse.png"));
    pushbutton_open->setIconSize(QSize(22,22));
    pushbutton_open->setToolTip("Open in Finder");

    connect(pushbutton_open,SIGNAL(clicked()),this,SLOT(slot_pushbutton_clicked()));

    QHBoxLayout *hbox_layout = new QHBoxLayout(this);
    hbox_layout->addWidget(label_file);
    hbox_layout->addWidget(pushbutton_open);
    hbox_layout->setContentsMargins(8,0,10,0);
    hbox_layout->setStretch(0,0);
    hbox_layout->setStretch(1,1);

    this->setLayout(hbox_layout);
}

void file_open_cell_widget::pub_set_global_report_dir_name(QString path)
{
    QStringList tmp_list = path.split("/", Qt::SkipEmptyParts);

    if(tmp_list.size() < 2)
        return;

    QString dir_name = tmp_list.at(tmp_list.size() - 2);

    if(dir_name.contains("_"))
        dir_name.replace("_", " ");

    QFileInfo info(path);

    file_path = info.absoluteFilePath();

    label_file = new QLabel(this);

    // label_file->setText("<a href = "+ dir_name + " > " + dir_name + " </a>");
    label_file->setText("<a href = "+ dir_name + " > " + dir_name + " </a>");

    if(path.endsWith(".zip"))
        label_file->setText("<a href = "+ dir_name + " > " + QFileInfo(path).fileName() + " </a>");


    connect(label_file, SIGNAL(linkActivated(QString)), this, SLOT(slot_label_file_linkactivated(QString)),Qt::QueuedConnection);

    pushbutton_open = new QPushButton(this);
    pushbutton_open->setFixedSize(15,15);
    pushbutton_open->setFlat(true);
    pushbutton_open->setIcon(QIcon("../icons/signs/browse.png"));
    //pushbutton_open->setCheckable(true);
    connect(pushbutton_open,SIGNAL(clicked()),this,SLOT(slot_pushbutton_clicked()));

    QHBoxLayout *hbox_layout = new QHBoxLayout(this);
    hbox_layout->addWidget(label_file);
    hbox_layout->addWidget(pushbutton_open);
    hbox_layout->setContentsMargins(8,0,10,0);
    hbox_layout->setStretch(0,0);
    hbox_layout->setStretch(1,1);

    this->setLayout(hbox_layout);
}

file_open_cell_widget::~file_open_cell_widget()
{

}

void file_open_cell_widget::slot_pushbutton_clicked()
{
    QStringList arg;
    arg << "-R"<<file_path;

    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
    pushbutton_open->setChecked(false);
}

void file_open_cell_widget::slot_label_file_linkactivated(QString m_type)
{
    QStringList arg;
    arg << file_path;

    QProcess *myProcess = new QProcess(this);
    myProcess->startDetached("open",arg);
}
