#ifndef MANAGE_PLIST_KEY_H
#define MANAGE_PLIST_KEY_H

#include <QFrame>
#include <QtCore>

#include <QComboBox>
#include "ui_custom_classes/m_tablewidget.h"
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"

namespace Ui {
class manage_plist_key;
}

class manage_plist_key : public QFrame
{
    Q_OBJECT

public:
    explicit manage_plist_key(QWidget *parent = 0);
    ~manage_plist_key();

    void draw_table_manage_plist_key(QStringList &keys_list);
    void pub_set_plist_table_name(QString db_name);
    void pub_set_plist_db_path(QString db_name);

private:
    Ui::manage_plist_key *ui;

    recon_helper_standard *recon_helper_standard_obj;
    QString get_conversion_type_of_the_current_selected_key(QString current_key);
    void update_conversion_type_value_in_db_for_removed_key(QString key_name);

    QString plist_db_table_name;
    QString plist_viewer_db_path;

    enum enum_plist_key_column
    {
        enum_keys_column = 0,
        enum_data_conversion_column
    };

    QMenu *menu_manage_keys;
    QStringList *all_key_QStringList_pointer;

    void create_right_click_actions(QMenu *mainMenu, QStringList submenuList);
    void contextMenuEvent(QContextMenuEvent *);

public slots:
    void slot_current_index_changed_data_conversion_combobox(QString);

private slots:
    void on_pushButton_clicked();
    void slot_manage_keys_menu_clicked(QAction *);

signals:

    void signal_data_conversion_timestamp_type_change(QString,QString);
};

#endif // MANAGE_PLIST_KEY_H
