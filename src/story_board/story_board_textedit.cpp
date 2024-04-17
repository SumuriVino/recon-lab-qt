#include "story_board_textedit.h"


story_board_textedit::story_board_textedit(QWidget *parent) : QMainWindow(parent)
{
    recon_helper_standard_obj = new recon_helper_standard(this);

    export_rsync_obj = new export_rsync(this);

    setWindowFlags(Qt::Window|Qt::WindowTitleHint|Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint|Qt::WindowCloseButtonHint);

    setToolButtonStyle(Qt::ToolButtonFollowStyle);

    textedit_obj = new QTextEdit(this);

    connect(textedit_obj, SIGNAL(currentCharFormatChanged(QTextCharFormat)),this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(textedit_obj, SIGNAL(cursorPositionChanged()),this, SLOT(cursorPositionChanged()));

    textedit_obj->setTextInteractionFlags(Qt::TextEditorInteraction | Qt::TextBrowserInteraction);

    text_edit_fixed_width = 680;
    textedit_obj->setFixedWidth(text_edit_fixed_width);
    textedit_obj->setFocus();
    textedit_obj->document()->setDocumentMargin(100);

    create_ui();

    QFont textFont("Helvetica");
    textFont.setStyleHint(QFont::SansSerif);
    textedit_obj->setFont(textFont);
    fontChanged(textedit_obj->font());
    colorChanged(textedit_obj->textColor());

    connect(textedit_obj->document(), SIGNAL(modificationChanged(bool)),this, SLOT(setWindowModified(bool)));
    setWindowModified(textedit_obj->document()->isModified());

    textedit_obj->installEventFilter(this);
    textedit_obj->setFocus();


    file_dialog_obj.setOption(QFileDialog::DontUseNativeDialog);
    QList<QUrl> drives;
    drives << QUrl::fromLocalFile(QDir("/Volumes/").absolutePath());
    drives << QUrl::fromLocalFile(QDir("/").absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Downloads")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Desktop")).absolutePath());
    drives << QUrl::fromLocalFile(QDir(getenv("HOME") + QString("/Documents")).absolutePath());
    file_dialog_obj.setSidebarUrls(drives);
    file_dialog_obj.setDirectory(getenv("HOME"));

    display_loading_progress_bar_obj = new display_loading_progress_bar(this);
    connect(display_loading_progress_bar_obj,SIGNAL(signal_close_loading_display_dialog(bool)),this,SLOT(slot_hide_loading_display_dialog_box(bool)));

    bool_cancel_loading = false;
    bool_message_box_clicked =  false;
    bool_message_box_yes_button_clicked = false;
    message_dialog_obj = new message_dialog(this);
    connect(message_dialog_obj,SIGNAL(signal_yes_no_check(bool)),this,SLOT(slot_message_box_yes_no_clicked(bool)));

}

story_board_textedit::~story_board_textedit()
{

}


void story_board_textedit::create_ui()
{

    QString storyboard_icons_dir = QString("../icons/Features/storyboard/");

    QFont font_s;
    font_s.setPixelSize(12);

    QVBoxLayout *vboxlayout_storyboard   = new QVBoxLayout(this);
    vboxlayout_storyboard->setContentsMargins(0,0,0,0);

    ///=============file_actions
    QPushButton *pushbutton_save = new QPushButton(QString(""),this);
    pushbutton_save->setFixedWidth(30);
    pushbutton_save->setFont(font_s);
    pushbutton_save->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_save->setIcon(QIcon(storyboard_icons_dir + QString("save.png")));
    pushbutton_save->setToolTip(QString("Save"));
    pushbutton_save->setShortcut(QKeySequence::Save);
    connect(pushbutton_save, SIGNAL(clicked(bool)), this, SLOT(slot_fileSave()));


    QPushButton *pushbutton_export_pdf = new QPushButton(QString(""),this);
    pushbutton_export_pdf->setFixedWidth(30);
    pushbutton_export_pdf->setFont(font_s);
    pushbutton_export_pdf->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_export_pdf->setIcon(QIcon(storyboard_icons_dir + QString("export_pdf.png")));
    pushbutton_export_pdf->setToolTip(QString("Export"));
    //pushbutton_export_pdf->setShortcut(Qt::CTRL + Qt::Key_D);
    //connect(pushbutton_export_pdf, SIGNAL(clicked(bool)), this, SLOT(filePrintPdf()));

    QMenu *menu_export = new QMenu(this);

    QAction *action_export_html  =  new QAction(QString("Export HTML") , this);
    QAction *action_export_pdf   =  new QAction(QString("Export PDF") , this);
    QAction *action_export_odt   =  new QAction(QString("Export ODT") , this);

    connect(action_export_html, SIGNAL(triggered()), this, SLOT(filePrinthtml()));
    connect(action_export_pdf, SIGNAL(triggered()), this, SLOT(filePrintPdf()));
    connect(action_export_odt, SIGNAL(triggered()), this, SLOT(filePrintOdt()));

    menu_export->addAction(action_export_html);
    menu_export->addAction(action_export_pdf);
    menu_export->addAction(action_export_odt);

    pushbutton_export_pdf->setMenu(menu_export);



    QPushButton *pushbutton_add_external_file = new QPushButton(QString(""),this);
    pushbutton_add_external_file->setFixedWidth(30);
    pushbutton_add_external_file->setFont(font_s);
    pushbutton_add_external_file->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_external_file->setIcon(QIcon(storyboard_icons_dir + QString("add_image.png")));
    pushbutton_add_external_file->setToolTip(QString("Add File"));
    connect(pushbutton_add_external_file, SIGNAL(clicked(bool)), this, SLOT(slot_add_external_file(bool)));


    QHBoxLayout *hboxlayout_file_actions = new QHBoxLayout;

    hboxlayout_file_actions->addWidget(pushbutton_save);
    hboxlayout_file_actions->setSpacing(20);
    hboxlayout_file_actions->addWidget(pushbutton_export_pdf);
    hboxlayout_file_actions->setSpacing(20);
    hboxlayout_file_actions->addWidget(pushbutton_add_external_file);
    hboxlayout_file_actions->addStretch(1);

    QFont font_bold;
    font_bold.setBold(true);

    QLabel *label_file = new QLabel(this);
    label_file->setText(QString("File"));
    label_file->setFont(font_bold);



    QVBoxLayout *Vboxlayout_file_actions = new QVBoxLayout(this);
    Vboxlayout_file_actions->addWidget(label_file);
    Vboxlayout_file_actions->addLayout(hboxlayout_file_actions);


    ///=============edit_actions
    pushbutton_undo = new QPushButton(QString(""),this);
    pushbutton_undo->setFixedWidth(30);
    pushbutton_undo->setFont(font_s);
    pushbutton_undo->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_undo->setIcon(QIcon(storyboard_icons_dir + QString("undo.png")));
    pushbutton_undo->setToolTip(QString("Undo"));
    pushbutton_undo->setShortcut(QKeySequence::Undo);

    pushbutton_redo = new QPushButton(QString(""),this);
    pushbutton_redo->setFixedWidth(30);
    pushbutton_redo->setFont(font_s);
    pushbutton_redo->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_redo->setIcon(QIcon(storyboard_icons_dir + QString("redo.png")));
    pushbutton_redo->setToolTip(QString("Redo"));
    pushbutton_redo->setShortcut(QKeySequence::Redo);

    pushbutton_cut = new QPushButton(QString(""),this);
    pushbutton_cut->setFixedWidth(30);
    pushbutton_cut->setFont(font_s);
    pushbutton_cut->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_cut->setIcon(QIcon(storyboard_icons_dir + QString("cut.png")));
    pushbutton_cut->setToolTip(QString("Cut"));
    pushbutton_cut->setShortcut(QKeySequence::Cut);

    pushbutton_copy = new QPushButton(QString(""),this);
    pushbutton_copy->setFixedWidth(30);
    pushbutton_copy->setFont(font_s);
    pushbutton_copy->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_copy->setIcon(QIcon(storyboard_icons_dir + QString("copy.png")));
    pushbutton_copy->setToolTip(QString("Copy"));
    pushbutton_copy->setShortcut(QKeySequence::Copy);

    pushbutton_paste = new QPushButton(QString(""),this);
    pushbutton_paste->setFixedWidth(30);
    pushbutton_paste->setFont(font_s);
    pushbutton_paste->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_paste->setIcon(QIcon(storyboard_icons_dir + QString("paste.png")));
    pushbutton_paste->setToolTip(QString("Paste"));
    pushbutton_paste->setShortcut(QKeySequence::Paste);


    QHBoxLayout *hboxlayout_edit_actions = new QHBoxLayout;

    hboxlayout_edit_actions->addWidget(pushbutton_undo);
    hboxlayout_edit_actions->setSpacing(20);
    hboxlayout_edit_actions->addWidget(pushbutton_redo);
    hboxlayout_edit_actions->setSpacing(20);
    hboxlayout_edit_actions->addWidget(pushbutton_cut);
    hboxlayout_edit_actions->setSpacing(20);
    hboxlayout_edit_actions->addWidget(pushbutton_copy);
    hboxlayout_edit_actions->setSpacing(20);
    hboxlayout_edit_actions->addWidget(pushbutton_paste);
    hboxlayout_edit_actions->addStretch(1);

    QLabel *label_edit = new QLabel(this);
    label_edit->setText(QString("Edit"));
    label_edit->setFont(font_bold);

    QVBoxLayout *Vboxlayout_edit_actions = new QVBoxLayout(this);
    Vboxlayout_edit_actions->addWidget(label_edit);
    Vboxlayout_edit_actions->addLayout(hboxlayout_edit_actions);


    connect(pushbutton_undo,  SIGNAL(clicked(bool)), textedit_obj, SLOT(undo()));
    connect(pushbutton_redo,  SIGNAL(clicked(bool)), textedit_obj, SLOT(redo()));
    connect(pushbutton_cut,   SIGNAL(clicked(bool)), textedit_obj, SLOT(cut()));
    connect(pushbutton_copy,  SIGNAL(clicked(bool)), textedit_obj, SLOT(copy()));
    connect(pushbutton_paste, SIGNAL(clicked(bool)), textedit_obj, SLOT(paste()));

    pushbutton_cut->setEnabled(false);
    pushbutton_copy->setEnabled(false);

    connect(textedit_obj, SIGNAL(copyAvailable(bool)), pushbutton_cut, SLOT(setEnabled(bool)));
    connect(textedit_obj, SIGNAL(copyAvailable(bool)), pushbutton_copy, SLOT(setEnabled(bool)));



    ///================Text Actions Font==============///
    QPushButton *pushbutton_bold = new QPushButton(QString(""),this);
    pushbutton_bold->setFixedWidth(30);
    pushbutton_bold->setFont(font_s);
    pushbutton_bold->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_bold->setIcon(QIcon(storyboard_icons_dir + QString("bold.png")));
    pushbutton_bold->setToolTip(QString("Bold"));
    pushbutton_bold->setShortcut(Qt::CTRL + Qt::Key_B);
    connect(pushbutton_bold, SIGNAL(clicked(bool)), this, SLOT(textBold()));

    QPushButton *pushbutton_italic = new QPushButton(QString(""),this);
    pushbutton_italic->setFixedWidth(30);
    pushbutton_italic->setFont(font_s);
    pushbutton_italic->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_italic->setIcon(QIcon(storyboard_icons_dir + QString("italic.png")));
    pushbutton_italic->setToolTip(QString("Italic"));
    pushbutton_italic->setShortcut(Qt::CTRL + Qt::Key_I);
    connect(pushbutton_italic, SIGNAL(clicked(bool)), this, SLOT(textItalic()));


    QPushButton *pushbutton_underline = new QPushButton(QString(""),this);
    pushbutton_underline->setFixedWidth(30);
    pushbutton_underline->setFont(font_s);
    pushbutton_underline->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_underline->setIcon(QIcon(storyboard_icons_dir + QString("underline.png")));
    pushbutton_underline->setToolTip(QString("Underline"));
    pushbutton_underline->setShortcut(Qt::CTRL + Qt::Key_U);
    connect(pushbutton_underline, SIGNAL(clicked(bool)), this, SLOT(textUnderline()));


    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    pushbutton_add_align_color = new QPushButton(pix, QString(""),this);
    pushbutton_add_align_color->setFixedWidth(30);
    pushbutton_add_align_color->setFont(font_s);
    pushbutton_add_align_color->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_align_color->setToolTip(QString("Color"));
    connect(pushbutton_add_align_color, SIGNAL(clicked(bool)), this, SLOT(slot_textColor()));


    comboStyle = new QComboBox(this);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Dot");
    comboStyle->addItem("1. 2. 3.");
    comboStyle->addItem("a. b. c.");
    comboStyle->addItem("A. B. C.");
    comboStyle->addItem("i. ii. iii.");
    comboStyle->addItem("I. II. III.");
    connect(comboStyle, SIGNAL(activated(int)), this, SLOT(textStyle(int)));

    comboStyle->setFixedWidth(233);

    comboFont = new QFontComboBox(this);
    connect(comboFont, SIGNAL(textActivated(QString)), this, SLOT(textFamily(QString)));
    comboFont->setEditable(false);

    comboSize = new QComboBox(this);
    comboSize->setObjectName("comboSize");
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(textActivated(QString)), this, SLOT(textSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font()
                                                                   .pointSize())));

    QHBoxLayout *hboxlayout_text_actions_font = new QHBoxLayout;
    hboxlayout_text_actions_font->addStretch(1);
    hboxlayout_text_actions_font->addWidget(comboFont);
    hboxlayout_text_actions_font->addSpacing(20);
    hboxlayout_text_actions_font->addWidget(comboSize);
    hboxlayout_text_actions_font->addStretch(1);

    QHBoxLayout *hboxlayout_text_actions_formatting = new QHBoxLayout;
    hboxlayout_text_actions_formatting->addWidget(pushbutton_bold);
    hboxlayout_text_actions_formatting->addSpacing(20);
    hboxlayout_text_actions_formatting->addWidget(pushbutton_italic);
    hboxlayout_text_actions_formatting->addSpacing(20);
    hboxlayout_text_actions_formatting->addWidget(pushbutton_underline);
    hboxlayout_text_actions_formatting->addSpacing(20);
    hboxlayout_text_actions_formatting->addWidget(pushbutton_add_align_color);
    hboxlayout_text_actions_formatting->addStretch(1);

    QHBoxLayout *hboxlayout_text_actions_style = new QHBoxLayout;
    hboxlayout_text_actions_style->addStretch(1);
    hboxlayout_text_actions_style->addWidget(comboStyle);
    hboxlayout_text_actions_style->addStretch(1);


    QLabel *label_font = new QLabel(this);
    label_font->setText(QString("Font"));
    label_font->setFont(font_bold);


    QLabel *label_align = new QLabel(this);
    label_align->setText(QString("Alignment"));
    label_align->setFont(font_bold);


    QVBoxLayout *Vboxlayout_text_actions = new QVBoxLayout;
    Vboxlayout_text_actions->addWidget(label_font);
    Vboxlayout_text_actions->addLayout(hboxlayout_text_actions_font);
    Vboxlayout_text_actions->addLayout(hboxlayout_text_actions_formatting);
    Vboxlayout_text_actions->addWidget(label_align);

    ///================Text Actions alignment==============///
    QPushButton *pushbutton_align_left = new QPushButton(QString(""),this);
    pushbutton_align_left->setFixedWidth(30);
    pushbutton_align_left->setFont(font_s);
    pushbutton_align_left->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_align_left->setIcon(QIcon(storyboard_icons_dir + QString("align_left.png")));
    pushbutton_align_left->setToolTip(QString("Left"));
    pushbutton_align_left->setShortcut(Qt::CTRL + Qt::Key_L);
    connect(pushbutton_align_left, SIGNAL(clicked(bool)), this, SLOT(slot_align_left()));


    QPushButton *pushbutton_align_centre = new QPushButton(QString(""),this);
    pushbutton_align_centre->setFixedWidth(30);
    pushbutton_align_centre->setFont(font_s);
    pushbutton_align_centre->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_align_centre->setIcon(QIcon(storyboard_icons_dir + QString("align_centre.png")));
    pushbutton_align_centre->setToolTip(QString("Centre"));
    pushbutton_align_centre->setShortcut(Qt::CTRL + Qt::Key_E);
    connect(pushbutton_align_centre, SIGNAL(clicked(bool)), this, SLOT(slot_align_centre()));


    QPushButton *pushbutton_add_align_right = new QPushButton(QString(""),this);
    pushbutton_add_align_right->setFixedWidth(30);
    pushbutton_add_align_right->setFont(font_s);
    pushbutton_add_align_right->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_align_right->setIcon(QIcon(storyboard_icons_dir + QString("align_right.png")));
    pushbutton_add_align_right->setShortcut(Qt::CTRL + Qt::Key_R);
    pushbutton_add_align_right->setToolTip(QString("Right"));
    connect(pushbutton_add_align_right, SIGNAL(clicked(bool)), this, SLOT(slot_align_right()));


    QPushButton *pushbutton_add_align_justify = new QPushButton(QString(""),this);
    pushbutton_add_align_justify->setFixedWidth(30);
    pushbutton_add_align_justify->setFont(font_s);
    pushbutton_add_align_justify->setAttribute(Qt::WA_MacShowFocusRect,false);
    pushbutton_add_align_justify->setIcon(QIcon(storyboard_icons_dir + QString("align_justify.png")));
    pushbutton_add_align_justify->setToolTip(QString("Justify"));
    pushbutton_add_align_justify->setShortcut(Qt::CTRL + Qt::Key_J);
    connect(pushbutton_add_align_justify, SIGNAL(clicked(bool)), this, SLOT(slot_align_justify()));

    QHBoxLayout *hboxlayout_text_actions_align = new QHBoxLayout;

    hboxlayout_text_actions_align->addWidget(pushbutton_align_left);
    hboxlayout_text_actions_align->addSpacing(20);
    hboxlayout_text_actions_align->addWidget(pushbutton_align_centre);
    hboxlayout_text_actions_align->addSpacing(20);
    hboxlayout_text_actions_align->addWidget(pushbutton_add_align_right);
    hboxlayout_text_actions_align->addSpacing(20);
    hboxlayout_text_actions_align->addWidget(pushbutton_add_align_justify);
    hboxlayout_text_actions_align->addStretch(1);

    QSpacerItem *spacer_up   = new QSpacerItem(10,20,QSizePolicy::Maximum,QSizePolicy::Expanding);
    QSpacerItem *spacer_down = new QSpacerItem(10,20,QSizePolicy::Maximum,QSizePolicy::Expanding);

    QVBoxLayout *vboxlayout_actions = new QVBoxLayout;

    vboxlayout_actions->addSpacerItem(spacer_up);
    vboxlayout_actions->addLayout(Vboxlayout_file_actions);
    vboxlayout_actions->addLayout(Vboxlayout_edit_actions);
    vboxlayout_actions->addLayout(Vboxlayout_text_actions);
    vboxlayout_actions->addLayout(hboxlayout_text_actions_align);
    vboxlayout_actions->addLayout(hboxlayout_text_actions_style);
    vboxlayout_actions->addSpacerItem(spacer_down);


    QWidget *widget_main_edit_options = new QWidget;
    widget_main_edit_options->setLayout(vboxlayout_actions);
    widget_main_edit_options->setFixedWidth(250);

    widget_main_edit_options->setGeometry(0, 0, 250, 330);
    QPalette pal = palette();

    // set background
    QColor back_color;
    back_color.setRgb(247,248,247);
    pal.setColor(QPalette::Window, back_color);
    widget_main_edit_options->setAutoFillBackground(true);

    QHBoxLayout *hboxlayout_storyboard = new QHBoxLayout;

    QSpacerItem *spacer_right = new QSpacerItem(10,20,QSizePolicy::Expanding,QSizePolicy::Expanding);
    QSpacerItem *spacer_left  = new QSpacerItem(10,20,QSizePolicy::Expanding,QSizePolicy::Expanding);

    hboxlayout_storyboard->addSpacing(5);
    hboxlayout_storyboard->addSpacerItem(spacer_left);
    hboxlayout_storyboard->addWidget(textedit_obj);
    hboxlayout_storyboard->addSpacerItem(spacer_right);
    hboxlayout_storyboard->addWidget(widget_main_edit_options);
    hboxlayout_storyboard->setStretchFactor(widget_main_edit_options, 0);
    hboxlayout_storyboard->addSpacing(5);

    vboxlayout_storyboard->addLayout(hboxlayout_storyboard);

    QWidget *widget_main_storyboard = new QWidget;
    widget_main_storyboard->setLayout(vboxlayout_storyboard);

    dockwodget_obj = new QDockWidget(tr(story_file_name.toLocal8Bit().data()), this);
    dockwodget_obj->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
    dockwodget_obj->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dockwodget_obj->setWidget(widget_main_storyboard);
    addDockWidget(Qt::BottomDockWidgetArea , dockwodget_obj);

}

void story_board_textedit::pub_save_storyboard()
{
    slot_fileSave();
}

void story_board_textedit::pub_set_story_file_name_and_path(QString file_name, QString file_path)
{
    story_file_name = file_name;
    story_file_path = file_path;
    dockwodget_obj->setWindowTitle(story_file_name);

    setCurrentFileName(file_path);
}

void story_board_textedit::pub_set_story_default_export_path(QString file_path)
{
    default_export_path =  file_path;
}

bool story_board_textedit::eventFilter(QObject *target, QEvent *event)
{
    //   if (target == textedit_obj)
    //  {
    //        if (event->type() == QEvent::KeyPress)
    //        {
    //            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    //            std::string str = keyEvent->text().toStdString();
    //            char ch = str[0];
    //            if (!((ch <= '9' && ch >= '0') | (ch <= 'F' && ch >= 'A') |
    //                  (ch <= 'f' && ch >= 'a')))
    //            {
    //                if (ch != 9) return true;
    //                else
    //                    return QWidget::eventFilter(target, event);
    //            }
    //        }
    //   }

    return false;
}


void story_board_textedit::closeEvent(QCloseEvent *e)
{
    if (maybeSave())
        e->accept();
    else
        e->ignore();
}


void story_board_textedit::pub_add_file_to_story_board(QString actual_filename, QString filepath, bool blurr_status, QString label_file_path)
{
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    filepath = filepath.trimmed();
    if(filepath.isEmpty())
        return;

    if(!label_file_path.trimmed().isEmpty() && !label_file_path.startsWith("/"))
        label_file_path.prepend("/");

    if(recon_helper_standard_obj->check_is_it_image_file(filepath ,label_file_path, Q_FUNC_INFO))
    {
        pub_add_image_file_to_story(filepath, blurr_status);
    }
    else
        pub_add_other_file_to_story(actual_filename, filepath);

    pub_add_data_to_story_board(label_file_path, QString(""));
    pub_add_data_to_story_board(QString("<br>"), QString(""));

}

void story_board_textedit::pub_add_data_to_story_board(QString data, QString media)
{
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    textedit_obj->append(data);
    textedit_obj->append("<br>");

}

void story_board_textedit::pub_set_case_details_to_story_board(QString data)
{
    textedit_obj->setHtml(data);
}

void story_board_textedit::pub_add_image_file_to_story(QString filepath, bool blurr_status)
{
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    filepath = filepath.trimmed();

    if(filepath.isEmpty())
        return;


    QImage img(filepath);
    img = img.scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QString base_64_data;
    if(blurr_status)
    {
        QGraphicsBlurEffect *effect = new QGraphicsBlurEffect;

        effect->setBlurRadius(8);
        int extent = 0;

        QGraphicsScene scene;
        QGraphicsPixmapItem item;
        item.setPixmap(QPixmap::fromImage(img));
        item.setGraphicsEffect(effect);
        scene.addItem(&item);
        QImage res(img.size()+QSize(extent*2, extent*2), QImage::Format_ARGB32);
        res.fill(Qt::transparent);
        QPainter ptr(&res);
        scene.render(&ptr, QRectF(), QRectF( -extent, -extent, img.width()+extent*2, img.height()+extent*2 ) );

        QByteArray arr;
        QBuffer buf(&arr);
        res.save(&buf, "JPG" );

        base_64_data = QString::fromLocal8Bit(arr.toBase64());

        delete effect;
    }
    else
    {
        QByteArray arr;
        QBuffer buf(&arr);
        img.save(&buf, "JPG" );

        base_64_data = QString::fromLocal8Bit(arr.toBase64());

    }

    //QString image = QString("<img src=data:image/png;base64,") + base_64_data  + ">";

    //QString data_str = QString("<td align=\"center\">") +  image + QString("</td>");
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);
    QString file_name = QFileInfo(filepath).fileName();
    QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
    QString file_copy_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(filepath ,resource_dir_path , file_name , Q_FUNC_INFO);
    QString copied_file_name = QFileInfo(file_copy_path).fileName();

    QString relative_path = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(copied_file_name,Q_FUNC_INFO);

    QString image = QString("<img src=data:image/png;base64,") + base_64_data  + " width=\"140\" height=\"150\" >";
    QString hyperlink = QString("<a href=") +  relative_path + QString(" target=\"_blank\" onClick=\"window.open(") + "'" + relative_path + "'" + QString(" , '_blank', 'width=800, height=600, left=10,top=10 scrollbars=yes');return false\">");
    QString data_str = QString("<td align=\"center\">") +  QString(hyperlink + image) + QString("</td>");

    textedit_obj->append(data_str);
    textedit_obj->append("<br>");

}

void story_board_textedit::pub_add_other_file_to_story(QString actual_filename, QString actual_filepath)
{
    actual_filepath = actual_filepath.trimmed();

    if(actual_filepath.isEmpty())
        return;

    //actual_filename.replace(" ", "%20");
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    QString resource_dir_path = global_narad_muni_class_obj->get_field(MACRO_NARAD_Feature_Path_Location_Story_Board_In_Result_QString).toString() + story_file_name + "/Resources/";
    QString file_copy_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(actual_filepath ,resource_dir_path , actual_filename , Q_FUNC_INFO);

    QString relative_path = "./Resources/" + recon_helper_standard_obj->pub_get_html_formated_filename(QFileInfo(file_copy_path).fileName(),Q_FUNC_INFO);

    QString data_str = QString("<br>");
    //data_str.append(QString("<a href='") + "./Resources/" + QFileInfo(file_copy_path).fileName() + QString("' target=\"_blank\"> ") + actual_filename + QString("</a>"));
    data_str.append(QString("<a href='") + relative_path + QString("' target=\"_blank\"> ") + actual_filename + QString("</a>"));
    data_str.append("<br>");

    QTextCursor cursor = textedit_obj->textCursor();
    cursor.insertHtml(data_str);

    slot_fileSave();
}

bool story_board_textedit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;

    QFile file(f);
    if (!file.open(QFile::ReadWrite))
        return false;

    QByteArray data = file.readAll();
    file.close();

    QTextCodec *codec = Qt::codecForHtml(data);

    QString str = codec->toUnicode(data);
    str.replace("<!--StartFragment-->","");
    str.replace("<!--EndFragment-->","");


    if (Qt::mightBeRichText(str))
    {
        textedit_obj->setHtml(str);
    }
    else
    {
        str = QString::fromLocal8Bit(data);
        textedit_obj->setPlainText(str);
    }

    setCurrentFileName(f);
    return true;
}

bool story_board_textedit::load_file(const QString &f)
{
    load(f);
}

bool story_board_textedit::maybeSave()
{
    if (!textedit_obj->document()->isModified())
        return true;

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return slot_fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void story_board_textedit::setCurrentFileName(const QString &fileName)
{
    QFile file(fileName);

    if(file.exists())
    {
        this->fileName = fileName;
        textedit_obj->document()->setModified(false);
        setWindowTitle(tr("%1[*] - %2").arg(fileName).arg(tr("Rich Text")));

        setWindowModified(false);

        return;
    }


    if(!file.open(QIODevice::ReadWrite  | QIODevice::Truncate ))
    {
        recon_static_functions::app_debug(" : File Creation -------FAILED------ ",Q_FUNC_INFO);
        recon_static_functions::app_debug(file.errorString() , Q_FUNC_INFO);
        return;
    }

    file.flush();
    file.close();


    this->fileName = fileName;
    textedit_obj->document()->setModified(false);

    //    QString shownName;
    //    if (fileName.isEmpty())
    //        shownName = "untitled.txt";
    //    else
    //        shownName = QFileInfo(fileName).fileName();
    //    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Rich Text")));

    setWindowTitle(tr("%1[*] - %2").arg(fileName).arg(tr("Rich Text")));

    setWindowModified(false);
    //setWindowModified(true);
}

bool story_board_textedit::slot_fileSave()
{
    if (fileName.isEmpty())
        return slot_fileSaveAs();
    if (fileName.startsWith(QStringLiteral(":/")))
        return slot_fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textedit_obj->document());
    if (success)
        textedit_obj->document()->setModified(false);
    return success;
}

bool story_board_textedit::slot_fileSaveAs()
{
    QString file_name = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),
                                                     tr("ODF Text Document (*.odt);;"
                                                        "Text (.txt)"
                                                        "HTML-Files (*.htm *.html);;"
                                                        "All Files (*)"));

    if(file_name.isEmpty())
        return false;

    if (!(file_name.endsWith(".odt", Qt::CaseInsensitive) || file_name.endsWith(".htm", Qt::CaseInsensitive)
          || file_name.endsWith(".html", Qt::CaseInsensitive) || file_name.endsWith(".txt", Qt::CaseInsensitive)))
    {
        file_name += ".odt"; // default
    }

    setCurrentFileName(file_name);
    return slot_fileSave();
}

void story_board_textedit::pub_set_blurr_status(bool status)
{
    bool_blurr_status = status;
}

void story_board_textedit::slot_add_external_file(bool status)
{
    Q_UNUSED(status);

    QString filepath; // = QFileDialog::getOpenFileName(this,tr("Add File"), "/home", tr("Files (*)"));

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::ExistingFile);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            filepath = selected_files.at(0);
        }
    }

    if(filepath.trimmed().isEmpty())
        return;

    QString filename = recon_helper_standard_obj->get_filename_by_filepath_without_fileinfo(filepath);
    pub_add_file_to_story_board(filename, filepath, bool_blurr_status, filepath);
    return;

    //    if(recon_helper_standard_obj->check_is_it_image_file(filepath, Q_FUNC_INFO))
    //    {
    //        add_image(filepath);
    //    }
    //    else
    //    {
    //        QString filename_actl = QFileInfo(filepath).fileName();
    //        pub_add_other_file_to_story(filename_actl,filepath);
    //    }

    //    pub_add_data_to_story_board(filepath, QString(""));

}

void story_board_textedit::add_image(QString filepath)
{
    filepath = filepath.trimmed();

    if(filepath.isEmpty())
        return;

    QUrl Uri (QString("file://%1").arg (filepath));
    QImage image = QImageReader(filepath).read();

    QTextDocument * textDocument = textedit_obj->document();
    textDocument->addResource(QTextDocument::ImageResource, Uri, QVariant(image));

    QTextCursor cursor = textedit_obj->textCursor();
    QTextImageFormat imageFormat;

    if(image.width() > text_edit_fixed_width)
    {
        imageFormat.setWidth(text_edit_fixed_width - 30);
        imageFormat.setHeight(text_edit_fixed_width - 200);
    }
    else
    {
        imageFormat.setWidth(image.width());
        imageFormat.setHeight(image.height());
    }

    imageFormat.setName( Uri.toString() );
    cursor.insertImage(imageFormat);

    slot_fileSave();

}


void story_board_textedit::filePrintPdf()
{
    pdf_export_filepath.clear();

    QString export_dir; //=QFileDialog::getExistingDirectory(this, "Export PDF", default_export_path);

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            export_dir = selected_files.at(0);
        }
    }

    export_dir = export_dir.trimmed();
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    if(!export_dir.isEmpty())
    {

        QString source_dir_path = default_export_path + story_file_name + QString("/");

        QString exported_f_path =  export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_dir_path ,export_dir ,story_file_name,Q_FUNC_INFO);

        QString export_filepath = QFileInfo(exported_f_path).absoluteFilePath() + QString("/") + QFileInfo(exported_f_path).fileName();

        if (QFileInfo(export_filepath).suffix().isEmpty())
            export_filepath.append(".pdf");

        pdf_export_filepath = export_filepath;

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(export_filepath);
        printer.setPaperSource(QPrinter::LargeFormat);
        textedit_obj->document()->print(&printer);

        QFileInfo info(export_filepath);
        if(info.exists())
        {
            QString report_dir_path = info.absolutePath();

            QProcess proc;
            QStringList args;
            args << "-R" << "0777" << report_dir_path;;
            proc.execute("chmod", args);

            QCoreApplication::processEvents();

            bool_message_box_yes_button_clicked = false;
            bool_message_box_clicked = false;
            message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("File(s) has been exported. Do you want to open it?")));
            message_dialog_obj->show();

            recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
            int proc_counter = 0;
            while(1)
            {
                proc_counter++;
                if(proc_counter % 100 == 0)
                {
                    proc_counter = 0;
                    QCoreApplication::processEvents();
                }

                if(bool_message_box_clicked)
                    break ;
            }
            recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

            if(bool_message_box_yes_button_clicked)
            {
                QProcess myProcess;
                myProcess.startDetached("open",QStringList(pdf_export_filepath));
            }

        }

    }

}

void story_board_textedit::filePrintOdt()
{
    QString export_dir; //= QFileDialog::getExistingDirectory(this, "Export ODT", default_export_path);

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            export_dir = selected_files.at(0);
        }
    }

    export_dir = export_dir.trimmed();
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    if(!export_dir.isEmpty())
    {

        QString source_dir_path = default_export_path + story_file_name + QString("/");

        QString exported_f_path = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_dir_path ,export_dir ,story_file_name,Q_FUNC_INFO);

        QString export_filepath = QFileInfo(exported_f_path).absoluteFilePath() + QString("/") + QFileInfo(exported_f_path).fileName();

        if (QFileInfo(export_filepath).suffix().isEmpty())
            export_filepath.append(".odt");

        QFile file(export_filepath);
        if (!file.open(QIODevice::ReadWrite))
        {
            recon_static_functions::app_debug("Error creating odt file -----Failed---- " + file.fileName() , Q_FUNC_INFO);
            recon_static_functions::app_debug(file.errorString() , Q_FUNC_INFO);
            return;
        }

        QByteArray text_edit_data =  textedit_obj->document()->toHtml().toLocal8Bit();

        file.write(text_edit_data);
        file.flush();

        file.close();

        QFileInfo info(export_filepath);

        if(info.exists())
        {
            QString report_dir_path = info.absolutePath();

            QProcess proc;
            QStringList args;
            args << "-R" << "0777" << report_dir_path;;
            proc.execute("chmod", args);

            QCoreApplication::processEvents();

            bool_message_box_yes_button_clicked = false;
            bool_message_box_clicked = false;
            message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, global_lang_tr_obj.get_translated_string(QString("File(s) has been exported. Do you want to open it?")));
            message_dialog_obj->show();

            recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
            int proc_counter = 0;
            while(1)
            {
                proc_counter++;
                if(proc_counter % 100 == 0)
                {
                    proc_counter = 0;
                    QCoreApplication::processEvents();
                }

                if(bool_message_box_clicked)
                    break ;
            }
            recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

            if(bool_message_box_yes_button_clicked)
            {
                QProcess myProcess;
                myProcess.startDetached("open",QStringList(export_filepath));
            }

        }

    }


}

void story_board_textedit::filePrinthtml()
{
    QString export_dir; //= QFileDialog::getExistingDirectory(this, "Export HTML", default_export_path);

    file_dialog_obj.setNameFilter(("*"));
    file_dialog_obj.setFileMode(QFileDialog::Directory);
    if(file_dialog_obj.exec())
    {
        QStringList selected_files = file_dialog_obj.selectedFiles();
        if(selected_files.size() > 0)
        {
            export_dir = selected_files.at(0);
        }
    }

    export_dir = export_dir.trimmed();
    export_rsync_obj->set_rsync_copy_essentials(Q_FUNC_INFO);

    if(!export_dir.isEmpty())
    {
        if(!export_dir.endsWith("/"))
            export_dir.append("/");

        //        QString filename = recon_static_functions::get_available_filename(story_file_name, export_dir, Q_FUNC_INFO);

        //        export_dir.append(filename);
        //        QDir dir(export_dir);
        //        if(!dir.exists())
        //            dir.mkdir(export_dir);

        QString source_dir_path = default_export_path + story_file_name + QString("/");

        export_dir = export_rsync_obj->pub_copy_file_using_rsync_for_export_as_folder(source_dir_path ,export_dir ,story_file_name,Q_FUNC_INFO);

        QProcess proc;
        QStringList args;
        args << "-R" << "0777" << export_dir;;
        proc.execute("chmod", args);

        QCoreApplication::processEvents();

        bool_message_box_yes_button_clicked = false;
        bool_message_box_clicked = false;
        message_dialog_obj->pub_set_message(MACRO_MSG_CHECK_YES_NO, ( "File(s) has been exported. Do you want to open it?"));
        message_dialog_obj->show();

        recon_static_functions::debug_conditional(" : infinite loop start", Q_FUNC_INFO);
        int proc_counter = 0;
        while(1)
        {
            proc_counter++;
            if(proc_counter % 100 == 0)
            {
                proc_counter = 0;
                QCoreApplication::processEvents();
            }

            if(bool_message_box_clicked)
                break ;
        }
        recon_static_functions::debug_conditional(" : infinite loop end", Q_FUNC_INFO);

        if(bool_message_box_yes_button_clicked)
        {
            QString html_file_path;
            html_file_path = export_dir;
            if(!html_file_path.endsWith("/"))
                html_file_path.append("/");

            html_file_path.append(story_file_name).append(".html");

            QProcess myProcess;
            myProcess.startDetached("open",QStringList(html_file_path));
        }

    }
}


void story_board_textedit::textBold()
{

    QTextCursor cursor = textedit_obj->textCursor();
    if(!cursor.charFormat().font().bold())
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(QFont::Bold);
        mergeFormatOnWordOrSelection(fmt);
    }
    else
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(QFont::Normal);
        mergeFormatOnWordOrSelection(fmt);
    }


}

void story_board_textedit::textUnderline()
{
    QTextCursor cursor = textedit_obj->textCursor();
    if(!cursor.charFormat().font().underline())
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(true);
        mergeFormatOnWordOrSelection(fmt);
    }
    else
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(false);
        mergeFormatOnWordOrSelection(fmt);
    }


}

void story_board_textedit::textItalic()
{

    QTextCursor cursor = textedit_obj->textCursor();
    if(!cursor.charFormat().font().italic())
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(true);
        mergeFormatOnWordOrSelection(fmt);
    }
    else
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(false);
        mergeFormatOnWordOrSelection(fmt);
    }

}

void story_board_textedit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void story_board_textedit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}


void story_board_textedit::textStyle(int styleIndex)
{
    QTextCursor cursor = textedit_obj->textCursor();

    if (styleIndex != 0) {
        QTextListFormat::Style style = QTextListFormat::ListDisc;

        switch (styleIndex) {
        default:
        case 1:
            style = QTextListFormat::ListDisc;
            break;
        case 2:
            style = QTextListFormat::ListDecimal;
            break;
        case 3:
            style = QTextListFormat::ListLowerAlpha;
            break;
        case 4:
            style = QTextListFormat::ListUpperAlpha;
            break;
        case 5:
            style = QTextListFormat::ListLowerRoman;
            break;
        case 6:
            style = QTextListFormat::ListUpperRoman;
            break;


        }

        cursor.beginEditBlock();

        QTextBlockFormat blockFmt = cursor.blockFormat();

        QTextListFormat listFmt;

        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }

        listFmt.setStyle(style);

        cursor.createList(listFmt);

        cursor.endEditBlock();
    } else {
        // ####
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }
}

void story_board_textedit::slot_textColor()
{

    QColor col = QColorDialog::getColor(textedit_obj->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);

}


void story_board_textedit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}


void story_board_textedit::cursorPositionChanged()
{
    current_curser = textedit_obj->textCursor();
    current_curser_position_qint = current_curser.position();

}

void story_board_textedit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textedit_obj->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    textedit_obj->mergeCurrentCharFormat(format);
}

void story_board_textedit::fontChanged(const QFont &font)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(font).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(font.pointSize())));
}

void story_board_textedit::colorChanged(const QColor &color)
{
    QPixmap pix(16, 16);
    pix.fill(color);

    pushbutton_add_align_color->setIcon(pix);
}

void story_board_textedit::slot_align_left()
{
    textedit_obj->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
}

void story_board_textedit::slot_align_centre()
{
    textedit_obj->setAlignment(Qt::AlignHCenter);
}

void story_board_textedit::slot_align_right()
{
    textedit_obj->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
}

void story_board_textedit::slot_align_justify()
{
    textedit_obj->setAlignment(Qt::AlignJustify);
}

void story_board_textedit::slot_file_load_finished(bool status)
{
}

void story_board_textedit::slot_hide_loading_display_dialog_box(bool cancel_status)
{
    bool_cancel_loading =  cancel_status;
    display_loading_progress_bar_obj->hide();
}

void story_board_textedit::pub_cancel_copying_data()
{
    bool_cancel_loading = true;
}

void story_board_textedit::slot_message_box_yes_no_clicked(bool yes_no_clicked)
{
    bool_message_box_yes_button_clicked = yes_no_clicked;
    bool_message_box_clicked = true;

}
