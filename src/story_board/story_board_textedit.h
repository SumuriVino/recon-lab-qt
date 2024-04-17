#ifndef STORY_BOARD_TEXTEDIT_H
#define STORY_BOARD_TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>
#include <QImageReader>
#include <QTextEdit>
#include <QTextDocumentFragment>
#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
#include <QEvent>

#ifndef QT_NO_PRINTER
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#endif

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpacerItem>
#include <QFont>
#include <QDockWidget>
#include <QGraphicsBlurEffect>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWebEngineView>

#include "recon_generic_functions/recon_static_functions.h"
#include "header_global_variables_3.h"
#include "display_loading_progress_bar/display_loading_progress_bar.h"
#include "recon_generic_functions/recon_helper_standard.h"
#include "export/export_except_reports/export_rsync/export_rsync.h"
#include "message_box/message_dialog.h"



QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
QT_END_NAMESPACE

class story_board_textedit : public QMainWindow
{
    Q_OBJECT

public:
    story_board_textedit(QWidget *parent = 0);
    ~story_board_textedit();

    void pub_set_story_file_name_and_path(QString file_name, QString file_path);
    void pub_add_data_to_story_board(QString data, QString media);

    bool load_file(const QString &f);
    void pub_set_case_details_to_story_board(QString data);

    void pub_save_storyboard();
    void pub_add_image_file_to_story(QString filepath, bool blurr_status);
    void pub_add_other_file_to_story(QString actual_filename, QString actual_filepath);
    void pub_set_story_default_export_path(QString file_path);

    void pub_cancel_copying_data();
    void pub_add_file_to_story_board(QString actual_filename, QString filepath, bool blurr_status, QString label_file_path);
    void pub_set_blurr_status(bool status);
protected:
    virtual void closeEvent(QCloseEvent *e) Q_DECL_OVERRIDE;
    virtual bool eventFilter(QObject *target, QEvent *event);


private:
    bool load(const QString &f);
    bool maybeSave();
    void setCurrentFileName(const QString &fileName);

signals:
    void linkActivated(QString);

private slots:
    bool slot_fileSave();
    bool slot_fileSaveAs();
    void slot_add_external_file(bool status);
    void filePrintPdf();
    void filePrintOdt();
    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(int styleIndex);
    void slot_textColor();
    void slot_align_left();
    void slot_align_centre();
    void slot_align_right();
    void slot_align_justify();
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();

    void filePrinthtml();
    void slot_file_load_finished(bool status);
    void slot_hide_loading_display_dialog_box(bool cancel_status);
    void slot_message_box_yes_no_clicked(bool yes_no_clicked);
private:

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &font);
    void colorChanged(const QColor &color);


    QTextEdit   *textedit_obj;

    QComboBox     *comboStyle;
    QFontComboBox *comboFont;
    QComboBox     *comboSize;
    QString     fileName;
    QTextCursor current_curser;
    QString     story_file_name;
    QString     story_file_path;
    qint64      current_curser_position_qint;
    QString default_export_path;

    int text_edit_fixed_width;

    QPushButton *pushbutton_undo ;
    QPushButton *pushbutton_redo ;
    QPushButton *pushbutton_cut  ;
    QPushButton *pushbutton_copy ;
    QPushButton *pushbutton_paste;
    QPushButton *pushbutton_add_align_color ;
    QDockWidget *dockwodget_obj;

    void create_ui();

    void add_image(QString filepath);

    QWebEngineView *web_view;
    QString pdf_export_filepath;

    QFileDialog file_dialog_obj;

    display_loading_progress_bar *display_loading_progress_bar_obj;
    bool bool_cancel_loading;

    recon_helper_standard *recon_helper_standard_obj;

    export_rsync *export_rsync_obj;
    bool bool_message_box_yes_button_clicked;
    message_dialog *message_dialog_obj;

    bool bool_message_box_clicked;
    bool bool_blurr_status = false;

};

#endif // STORY_BOARD_TEXTEDIT_H
