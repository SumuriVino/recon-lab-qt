//#include "htmltopdf_webengine.h"

//htmltopdf_webengine::htmltopdf_webengine(QObject *parent)
//{
//    webengineview_obj = new QWebEngineView(this);
//    webengineview_obj->setWindowFlag(Qt::Window);
//    webengineview_obj->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    connect(webengineview_obj,SIGNAL(loadFinished(bool)),this,SLOT(slot_html_page_loadFinished(bool)));
//    connect(webengineview_obj->page(),SIGNAL(pdfPrintingFinished(QString,bool)),this,SLOT(slot_pdfPrintingFinished(QString,bool)));
//}


//void htmltopdf_webengine::slot_create_pdf_webengine(QString source_html_file, QString dest_pdf_file)
//{
//    pdf_file_path = dest_pdf_file;

//    if(!QFile::exists(source_html_file) || !source_html_file.endsWith("html"))
//    {
//        emit signal_htmltopdf_finished_webengine(pdf_file_path, false);
//        return;
//    }

//    QFileInfo info(source_html_file);

//    webengineview_obj->load(QUrl::fromLocalFile(info.filePath()));
//}

//void htmltopdf_webengine::slot_html_page_loadFinished(bool value)
//{
//    if(value)
//    {
//        webengineview_obj->page()->printToPdf(pdf_file_path);
//    }
//    else
//    {
//        emit signal_htmltopdf_finished_webengine(pdf_file_path, false);
//    }
    
//}

//void htmltopdf_webengine::slot_pdfPrintingFinished(QString pdf_path, bool value)
//{
//    emit signal_htmltopdf_finished_webengine(pdf_path, value);
//}
