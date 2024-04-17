#include "m_treewidget.h"

m_treewidget::m_treewidget(QWidget *parent) :
    QTreeWidget(parent)
{
    setAlternatingRowColors(true);

//    setStyleSheet("QTreeWidget {"
//                           "selection-color: white;"
//                           "}"
//                           "QTreeWidget {"
//                           " alternate-background-color: white;"
//                           " background-color:  #f1f1f1;"
//                           "}"
//                           "QTreeWidget::item {"
//                               "height: 23px;"

//                           "}"
//                           "QTreeWidget::item:selected:!active {"
//                               "color:white;"
//                              " background-color:  #CDCDCD;"

//                           "}"
//                           );

    QString stylesheet = QString("QTreeView::item:selected:!active {"
                                 "color:white;"
                                 " }"

                                 "QTreeView {"
                                 "   }"

                                 "QTreeView::item:selected:!active {"
                                 "color:white;"
                                 "background-color:  #CDCDCD;"
                                 "}");

    setStyleSheet(stylesheet);
    setAttribute(Qt::WA_MacShowFocusRect, false);
}

m_treewidget::~m_treewidget()
{

}

