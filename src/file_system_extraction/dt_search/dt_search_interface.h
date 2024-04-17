#ifndef DT_SEARCH_INTERFACE_H
#define DT_SEARCH_INTERFACE_H

#include "stdafx.h"
#ifdef _WIN32
#include <shellapi.h> // for ShellExecute
#endif
#include <string>
#include "dtconfig.h"
#include "settings.h"
#include <QtCore>
#include <QObject>
#include "recon_generic_functions/recon_static_functions.h"
#include "recon_generic_functions/recon_helper_standard.h"
//
//  Console-mode indexing and searching application
//  using the dtSearch Text Retrieval Engine
//

using namespace std;

#ifdef _MSC_VER
#   include <direct.h>
#   define true 1
#   define false 0
#endif

#ifdef __UNIX__
#   define sSysSlash "/"
#   define cSysSlash '/'
//const char *tempFolder = "/tmp";
#else
#   define sSysSlash "\\"
#   define cSysSlash '\\'
//const char *tempFolder = "c:\\tmp";
#endif


#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif

const long DemoAppId = 0x00abcdef;

class dt_search_interface : public QObject
{
    Q_OBJECT

public:
    dt_search_interface(istream& aIn, ostream & aOut, const char *home = 0, const char *work = 0);
    ~dt_search_interface();
    void run();
    void init();
    void enableLogging();
    void runScript(const char *name);
    bool createIndex(bool caching, QString entered_dir_path);
    bool updateIndex(int fClearFirst = false, QString dir_to_index = "", QString txt_file_with_paths = "");
    void searchIndex(int nIterations, QString search_label);

    void pub_set_list_found_word_file_info_ptr(QList<struct_global_indexed_keyword_found_file_info> *list_struct_info);
    qint64 pub_get_processed_files_count();
    void pub_set_bool_cancel_extraction(bool status);
protected:
    dtsIndexJob *m_indexJob;
    dtsSearchJob *m_searchJob;
    dtsSearchResults *m_results;

    CIndexLibrary m_indexLibrary;

    DString m_logName;
    DFilename m_libName;
    DString m_homeDir;
    DString m_privateDir;
    dtsOptions m_options;
    long id;
    istream & in;
    ostream & out;
    int fOutputDetails;

    QList<struct_global_indexed_keyword_found_file_info> *list_struct_found_keyword_file_info;

    static dt_search_interface *safeCast(void *p);
    static int indexingCB(void * pData, struct dtsMessage& msg);
    static int searchingCB(void *pData, struct dtsMessage& msg);
    static int errorNotifyFn(dtsErrorInfo *info);

    int inputString(const char *prompt, char *dest, int maxLen);

    int selectIndex(DString& dest);
    void indexInverted();
    void searchReport();
    void searchFiles();
    QString reportErrors(dtsErrorInfo& info);
    void showItem(dtsSearchResultsItem& item);
    QString listResults(int fWithDetails);

    void convertFile();
    int searchingCB(struct dtsMessage& msg);
    void allocResults();

private:
    recon_helper_standard *recon_helper_standard_obj;

};

//const char *menu =
//        "C=Create index, "
//        "CC= Create index with caching enabled, "
//        "I=Index files, "
//        "S=Search index, "
//        "D=List search results with details, "
//        "U=Unindexed search\n"
//        "V=Convert file using dtssConvertFile\n"
//        "X=Exit\n";

const char nl = '\n';

#ifdef __UNIX__
#define Platform "Linux"
#else
#define Platform "Windows"
#endif

#ifdef _X64
#define Bits "64-bit"
#else
#define Bits "32-bit"
#endif

#if defined(_MSC_VER)
#define BuiltBy      "VC"
#elif defined(__BORLANDC__)
#define BuiltBy      "BC"
#elif defined(__GNUC__)
#define BuiltBy      "GCC " __VERSION__
#else
#define BuildBy "XC"
#endif

#define BuiltWhen    __DATE__ " " __TIME__


#endif // DT_SEARCH_INTERFACE_H
