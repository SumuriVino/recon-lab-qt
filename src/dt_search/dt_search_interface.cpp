#include "dt_search_interface.h"

dt_search_interface::dt_search_interface(istream & aIn, ostream & aOut, const char *home, const char *work) :
    in(aIn),
    out(aOut),
    id(DemoAppId),
    m_indexJob(new dtsIndexJob),
    m_searchJob(new dtsSearchJob),
    m_results(0)
{   char cwd[256];
#ifdef _WIN32
    _getcwd(cwd, sizeof cwd);
#else
    getcwd(cwd, sizeof cwd);
#endif
    m_homeDir = home;
    if (m_homeDir.isBlank())
        m_homeDir = cwd;
    m_privateDir = work;
    if (m_privateDir.isBlank())
        m_privateDir = cwd;

}

dt_search_interface::~dt_search_interface()
{
    delete m_indexJob;
    m_indexJob = 0;

    delete m_searchJob;
    m_searchJob = 0;

    delete m_results;
    m_results = 0;
}

void dt_search_interface::init()
{
    //    out << "dtSearch Text Retrieval Engine Text-Mode Demo" << nl;
    //    out << "---------------------------------------------" << nl;
    //    out << Platform << " " << Bits << " " << BuiltBy << " " << BuiltWhen << nl;

    short result = 0;

    dtssGetOptions(m_options, result);
    strCopy(m_options.homeDir, m_homeDir, sizeof m_options.homeDir);
    strCopy(m_options.privateDir, m_privateDir, sizeof m_options.privateDir);
    m_options.matchDigitChar = '=';
    dtssSetOptions(m_options, result);

    dtsVersionInfo version;
    dtssVersionEx(version);
    DString ver;
    ver << version.majorVersion << "." << PadNum((int) version.minorVersion, 2) << " (" << version.build << ")";
    //    out << "dtSearch Engine " << ver << nl;

    //    out << "Home directory =    " << m_options.homeDir << nl;
    //    out << "Private directory = " << m_options.privateDir << nl;
    //    out << "Log file:           " << m_logName << nl;

    m_libName = m_privateDir;
    m_libName.addTail("ixlib.ilb");
    m_indexLibrary.read(Utf8ToUi(m_libName));
}

void dt_search_interface::enableLogging()
{
    m_logName.clear();
    m_logName << m_privateDir << sSysSlash << "dtsearch.log";
    dtssDebugLogEx(m_logName, dtsLogCommit);
}

void dt_search_interface::allocResults()
{
    if (m_results)
        delete m_results;

    m_results = new dtsSearchResults(256);
    m_searchJob->resultsHandle = m_results->getHandle();
    m_searchJob->output.listingFlags |= dtsListNoDisk;
    m_searchJob->maxFilesToRetrieve = 256;
}

dt_search_interface *dt_search_interface::safeCast(void *p)
{
    dt_search_interface *pDemoApp = (dt_search_interface *) p;
    if (pDemoApp && pDemoApp->id == DemoAppId)
        return pDemoApp;
    else
        return NULL;
}


void dt_search_interface::runScript(const char *name)
{
    short result;
    char tmp[256];
    strCopy(tmp, name, sizeof tmp);
    dtssRunScript(tmp, result);
}

void dt_search_interface::run()
{
    //    while(1)
    //    {
    //        char cmd[80];
    //        strClear(cmd);
    //        //        out << menu;
    //        inputString("Enter command", cmd, sizeof cmd);
    //        char c = (char) toupper(cmd[0]);
    //        char c2 = (char) toupper(cmd[1]);
    //        int intArg = 0;

    //        if (isdigit(cmd[1]))
    //            intArg = atoi(cmd+1);

    //        switch(c)
    //        {
    //        case 'C':
    //            createIndex(c2 == 'C');
    //            break;
    //        case 'I':
    //            updateIndex(c2 == 'C');
    //            break;
    //        case 'R':
    //            indexInverted();
    //            break;
    //        case 'S':
//                searchIndex(intArg);
    //            break;
    //        case 'U':
    //            searchFiles();
    //            break;
    //        case 'D':
    //            listResults(true);
    //            break;
    //        case 'X':
    //            return;
    //        case 'L':
    //            dtssDebugLog("");
    //            break;
    //        case 'V':
    //            convertFile();
    //            break;
    //        case 'P':
    //            searchReport();
    //            break;
    //        case 'W':
    //            fOutputDetails = true;
    //            break;
    //        default:
    //            return;
    //        }
    //    }
}

int dt_search_interface::inputString(const char *prompt, char *dest, int maxLen)
{
    memset(dest, 0, maxLen);
#ifdef __UNIX__
    strncpy(dest, prompt, maxLen);
#else
    in.getline(dest, maxLen, '\n');
#endif

    if (strlen(dest) > 0)
        return SUCCESS;
    else
        return FAIL;
}

int dt_search_interface::selectIndex(DString& dest)
{
//    //    out << "--------------------Select Index---------------------------" << nl;
//        for (int i = 0; i < m_indexLibrary.getCount(); ++i)
//        {
//            IndexLibraryEntry& e = m_indexLibrary.get(i);
//            DString name, path;
//            name.storeUnicodeAsAnsi(e.name);
//            path.storeUnicodeAsAnsi(e.path);
//        }
//    //    out << "-----------------------------------------------------------" << nl;

    QString sel_val = 0;
    if(m_indexLibrary.getCount() > 0)
        sel_val = QString::number(m_indexLibrary.getCount() - 1);

    char sel[40];
    inputString(sel_val.toLocal8Bit().data(), sel, sizeof sel);

    int k = FAIL;
    if (isdigit(sel[0]))
    {
        k = atoi(sel);
        if ((k >= 0) && (k < m_indexLibrary.getCount()))
            dest.storeUnicodeAsAnsi(m_indexLibrary.get(k).path);
        else
            k = FAIL;
    }

    if (k == FAIL)
        return FAIL;
    else
        return SUCCESS;
}


int dt_search_interface::indexingCB(void * pData, struct dtsMessage& msg)
{
    dt_search_interface *pDemoApp = dt_search_interface::safeCast(pData);

    dtsIndexProgressInfo *prog = (dtsIndexProgressInfo *)msg.ptrParam;

    QCoreApplication::processEvents();

    switch(msg.command)
    {
    case dtsnCheckForAbort:
        msg.result = dtsContinue;
        return dtsContinue;
    case dtsnIndexStartingFile:
    {
        QString msg = "Indexing ";
        msg.append(prog->file.name);
        recon_static_functions::app_debug(msg, Q_FUNC_INFO);
        break;
    }
    default:
        break;
    }

    return dtsContinue;
}

bool dt_search_interface::createIndex(bool caching, QString entered_dir_path)
{
    m_indexJob->clear();
    QString index_name = "Index_1_" + QString::number(qrand()) + QString::number(QDateTime::currentMSecsSinceEpoch());

    if (inputString(entered_dir_path.toLocal8Bit().data(), m_indexJob->indexPath, sizeof m_indexJob->indexPath) == SUCCESS)
    {
        if (inputString(index_name.toLocal8Bit().data(), m_indexJob->indexName, sizeof m_indexJob->indexName) == SUCCESS)
        {
            m_indexJob->action.create = true;
            m_indexJob->createFlags.relativePaths = true;
            m_indexJob->createFlags.addToLibrary = true;

            if (caching)
                m_indexJob->indexingFlags = (dtsIndexCacheText | dtsIndexCacheOriginalFile);

            short result;
            dtssDoIndexJob(*m_indexJob, result);

            if (result == FAIL)
                return false;
            else
            {
                IndexLibraryEntry libEntry;
                libEntry.name.storeAnsiAsUnicode(m_indexJob->indexName);
                libEntry.path.storeAnsiAsUnicode(m_indexJob->indexPath);
                m_indexLibrary.add(libEntry, true);
                m_indexLibrary.write(Utf8ToUi(m_libName), true);
                return true;
            }
        }
    }

    return false;
}

bool dt_search_interface::updateIndex(int fClearFirst, QString dir_to_index)
{
    char includeFilters[80];
    char excludeFilters[80];
    short result = 0;

    m_indexJob->clear();

    dtsErrorInfo errorInfo;
    errorInfo.pUserData = this;
    errorInfo.setNotify(errorNotifyFn);
    m_indexJob->errorHandler = &errorInfo;

    DString path(m_indexJob->indexPath, sizeof m_indexJob->indexPath);

    if (selectIndex(path) == FAIL)
        return false;

    char dir[256];
    inputString(dir_to_index.toLocal8Bit().data(), dir, sizeof dir);

    if (strIsBlank(dir))
        return false;

    if (dir[0] == '@')  // list of filenames
        strCopy(m_indexJob->toAdd.fileListName, &dir[1], sizeof m_indexJob->toAdd.fileListName);
    else
    {
        strCopy(m_indexJob->toAdd.dir, dir, sizeof m_indexJob->toAdd.dir);
        inputString("", includeFilters, sizeof includeFilters);
        inputString("", excludeFilters, sizeof excludeFilters);

        m_indexJob->toAdd.includeFilters = includeFilters;
        m_indexJob->toAdd.excludeFilters = excludeFilters;
    }

    m_indexJob->action.add = true;
    m_indexJob->action.create = (short) fClearFirst;
    m_indexJob->pReportCallBack = indexingCB;
    m_indexJob->pReportData = this;
    dtssDoIndexJob(*m_indexJob, result);

    if (result == FAIL)
        return false;
    else
        return true;
}

int dt_search_interface::searchingCB(void * pData, dtsMessage& msg)
{
    dt_search_interface *pDemoApp = (dt_search_interface *) pData;

    return pDemoApp->searchingCB(msg);
}

void dt_search_interface::showItem(dtsSearchResultsItem& item)
{
    struct_global_indexed_keyword_found_file_info struct_info;

    //    out << "    Retrieved file information:\n";
    //    out << "        Filename:       " << item.filename << nl;
    //    out << "        DisplayName:    " << item.displayName << nl;
    //    out << "        File size:      " << item.size << nl;
    //    out << "        Word count:     " << item.wordCount << nl;

    struct_info.file_name = item.displayName;
    struct_info.file_path = item.filename;
    struct_info.file_size = QString::number(item.size);
    struct_info.file_total_word_count = item.wordCount;
    struct_info.index_path = m_searchJob->indexesToSearch;

    DString d;
    d.ymd(item.modified.year, item.modified.month, item.modified.day);
    d << " ";
    d.hms(item.modified.hour, item.modified.minute, item.modified.second);

    //    out << "        Modified:       " << d << nl;

    struct_info.modified_date = d;

    //
    // List any user-defined fields for this document.  User-defined
    // fields are created using the fieldsToStore parameter in
    // dtsIndexJob based on text or database field definitions,
    // and are returned as a null-delimited series of fieldname, value
    // pairs.
    //
    if (item.userFields && *item.userFields)
    {
        const char *p = item.userFields;

        while(*p) {
            //            out << "        " << p << ":    ";
            p += (strlen(p) + 1);
            //            out << p << nl;
            p += (strlen(p) + 1);
        }
    }

    if (item.hitsByWord && *item.hitsByWord)
    {
        const char *p = item.hitsByWord;
        //        out << "        Hits by word:   ";
        while(*p) {
            //            out << p;
            p += (strlen(p) + 1);
            //            if (*p)
            //                out << "; ";
        }
        //        out << nl;
    }

    if (item.hitsByWord2 && *item.hitsByWord2)
    {
        const char *p = item.hitsByWord2;
        //        out << "        Hits by word2:  ";
        while(*p) {
            //            out << p;
            p += (strlen(p) + 1);
            //            if (*p)
            //                out << "; ";
        }
        //        out << nl;
    }

    //    out << "        Hit locations:  ";
    QStringList hits_locations;
    for (int h = 0; h < item.hitCount; ++h)
    {
        QString tmp_str = QString::number(item.hits[h]);
        hits_locations << tmp_str;
        //        out << item.hits[h] << " ";
    }
    //    out << nl;

    struct_info.word_hit_location_list = hits_locations;

    //
    //  List the contents of the file map for this document,
    //  which provides file offset-to-word count data
    //
    //  For plain text files, the filePtr is a simple file offset,
    //  but for other formats it generally is not a simple offset.
    //
    //    out << "        file map:       " << ios::hex;
    int ct = 0;
    for (int m = 0; m < item.fileMapEntryCount; ++m)
    {
        dtsFileMapEntry& entry = item.fileMapTable[m];
        //        out << entry.filePtr << "->" << entry.wordCount << ' ';
        if (ct++ > 5) {
            //            out << "\n                        ";
            ct = 0;
        }
    }

    list_struct_found_keyword_file_info->append(struct_info);
    //    out << nl << ios::dec;
}


int dt_search_interface::searchingCB(dtsMessage& msg)
{
    switch(msg.command)
    {
    case dtsnCheckForAbort:
        msg.result = dtsContinue;
        return dtsContinue;
    case dtsnSearchFound:
    {
        out << msg.paramA << " hits in " << msg.strParam << nl;
        if (fOutputDetails)
        {
            dtsSearchResultsItem *pItem = (dtsSearchResultsItem *) msg.paramB;
            showItem(*pItem);
        }

        break;
    }
    default:
        break;
    }

    out.flush();
    return dtsContinue;
}

//  Output errors collected in a dtsErrorInfo
QString dt_search_interface::reportErrors(dtsErrorInfo& info)
{
    QString error_str;

    if (info.getCount())
        error_str = QString("Errors reported during the operation and logged in dtsErrorInfo: " + QString::number(info.getCount()) + "\n");
    for (int i = 0; i < info.getCount(); ++i)
    {
        error_str.append("(" + QString::number(info.getMessageCode(i)) + ")" + info.getMessage(i) + "\n");

        if (strlen(info.getMessageArg1(i)))
        {
            error_str.append("    [");
            error_str.append(info.getMessageArg1(i));
            error_str.append("]");
            error_str.append("\n");
        }

        if (strlen(info.getMessageArg2(i)))
        {
            error_str.append("    [");
            error_str.append(info.getMessageArg2(i));
            error_str.append("]");
            error_str.append("\n");
        }
    }

    return error_str;
}

//  Output error reported through a dtsErrorInfo's pNotifyFn
int dt_search_interface::errorNotifyFn(dtsErrorInfo *info)
{
    dt_search_interface *d = (dt_search_interface *) info->pUserData;
    recon_static_functions::app_debug("Error reported through dtsErrorInfo: " + QString(info->getMessage()),Q_FUNC_INFO);
//    d->out << "Error reported through dtsErrorInfo: " << info->getMessage() << "\n";
    return 0;
}

void dt_search_interface::pub_set_list_found_word_file_info_ptr(QList<struct_global_indexed_keyword_found_file_info> *list_struct_info)
{
    list_struct_found_keyword_file_info = list_struct_info;
}

void dt_search_interface::searchIndex(int nIterations, QString search_label)
{
    if (nIterations < 1)
        nIterations = 1;

    // Set up search job
    m_searchJob->clear();
    dtsErrorInfo errorInfo;
    errorInfo.pUserData = this;
    errorInfo.pNotifyFn = errorNotifyFn;
    m_searchJob->errorHandler = &errorInfo;
    m_searchJob->action.searchIndexes = true;

    // Get index to search and search request

    char request[512];
    if (inputString(search_label.toLocal8Bit().data(), request, sizeof request) == FAIL)
        return;

    for (int i = 0; i < m_indexLibrary.getCount(); ++i)
    {
        IndexLibraryEntry& e = m_indexLibrary.get(i);
        DString name, path;
        name.storeUnicodeAsAnsi(e.name);
        path.storeUnicodeAsAnsi(e.path);

        m_searchJob->indexesToSearch = path;
        m_searchJob->request2 = request;

        for (int i = 0; i < nIterations; ++i)
        {
            // Set up output search results object
            allocResults();

            short fError = 0;
            dtssDoSearchJob(*m_searchJob, fError);
        }

        listResults(true);
    }

}

QString dt_search_interface::listResults(int fWithDetails)
{
    QString result_list;

    if (!m_results)
    {
        result_list = "No search results to list";
        return result_list;
    }

    m_results->sortByName();
    for (int i = 0; i < m_results->getCount(); ++i)
    {
        dtsSearchResultsItem item;
        m_results->getDocInfo(i, item);
        m_results->logDocInfo(i, &item);
        if (fWithDetails)
            showItem(item);
        else
            result_list.append(item.filename + QString(" ") + QString::number(item.hitCount) + "\n");
    }

    return result_list;
}

void dt_search_interface::searchReport()
{
    // now make a search report form the search results
    //    DFilename fn = tempFolder;
    //    fn.addTail("temp.html");

    dtsSearchReportJob rjob;
    dtsErrorInfo errorInfo;

    rjob.errorHandler = &errorInfo;
    rjob.pReportCallBack = searchingCB;
    rjob.pReportData = this;
    dtsOutputInfo output;
    output.outputFormat = it_HTML;
    //    output.outputFile = fn;
    output.beforeHit = "<B>";
    output.afterHit = "</B>";
    rjob.output = &output;

    rjob.flags = dtsReportByWord | dtsReportIncludeAll;
    rjob.amountOfContext = 10; // two paragraphs around each hit
    rjob.fileHeader =
            "<P><H4>%%Hits%% %%Filename%% in %%Location%%, modified %%Date%% size %%Size%%</H4><HR>";
    rjob.contextHeader = "<P><B>[Page %%Page%% Paragraph %%Paragraph%%]</B><BR>";
    m_results->makeReport(rjob);
#ifdef _WIN32
    // open the output file in a web browser
    ShellExecute(0, _T("open"), Utf8ToUi(output.outputFile), 0, 0, SW_SHOW);
#endif
    reportErrors(errorInfo);

}

void dt_search_interface::searchFiles(void)
{
    short result = 0;
    m_searchJob->clear();
    dtsErrorInfo errorInfo;
    errorInfo.pUserData = this;
    errorInfo.setNotify(errorNotifyFn);
    m_searchJob->errorHandler = &errorInfo;
    m_searchJob->pReportCallBack = searchingCB;
    m_searchJob->pReportData = this;
    m_searchJob->action.searchFiles = true;

    char request[512];
    if (inputString("Request: ", request, sizeof request) == FAIL)
        return;

    m_searchJob->request2 = request;

    char dirToSearch[256];
    char filter[256];
    memset(dirToSearch, 0, sizeof dirToSearch);
    memset(filter, 0, sizeof filter);
    inputString("Directory to search: ", dirToSearch, sizeof dirToSearch);
    inputString("Filter: ", filter, sizeof filter);
    m_searchJob->filesToSearch.dirList = dirToSearch;
    m_searchJob->filesToSearch.includeFilters = filter;

    allocResults();

    /* Write output to a listing file and a dtSearch search results file */
    m_searchJob->sortType = dtsSortByHits;

    if (fOutputDetails)
        m_searchJob->searchFlags |= (dtsSearchWantHitsByWord);

    dtssDoSearchJob(*m_searchJob, result);
    listResults(false);
    reportErrors(errorInfo);
}


void dt_search_interface::convertFile()
{
    char input[80], output[80], fmt[80];

    if (inputString("Input file: ", input, sizeof input) == FAIL)
        return;

    if (inputString("Output file: ", output, sizeof output) == FAIL)
        return;

    if (inputString("Output format: (h=HTML, a=Ansi, r=RTF, u=Utf8)", fmt, sizeof fmt) == FAIL)
        return;

    dtsFileConvertJob job;
    job.inputFile = input;
    job.outputFile = output;
    job.beforeHit = "<<<";
    job.afterHit = ">>>";
    long sampleHits[3] = { 3, 5, 7 };
    job.hits = sampleHits;
    job.hitCount = 3;

    switch(toupper(fmt[0])) {
    case 'H':
        job.outputFormat = it_HTML;
        break;
    case 'R':
        job.outputFormat = it_RTF;
        break;
    case 'A':
        job.outputFormat = it_Ansi;
        break;
    case 'U':
        job.outputFormat = it_Utf8;
        break;
    default:
        job.outputFormat = it_Ansi;
        break;
    }

    short result;
    dtssConvertFile(job, result);

    if (result)
        out << "dtssConvertFile returned error " << result << "\n";

    ifstream in(output);
    DString text;
    text.getFrom(in);
    out << text << nl;
}

// Demonstrates how the OnIndexWord callback function can be used to
// modify text being indexed.  In this case, each word is indexed along with an
// inverted version of the word.
void OnIndexWord_Inverter(void * /* pData */, dtsOnIndexWordInfo* pInfo)
{
    // First time we are called for a word, just request another callback (nAdditionalWords = 1)

    if (pInfo->iWhichCallback == 0)
    {
        pInfo->nAdditionalWords = 1;
        return;
    }

    // Second time we are called for a word, return the inverted version
    if (pInfo->iWhichCallback == 1)
    {
        size_t l = strlen(pInfo->originalWord);
        if (l >= sizeof pInfo->wordToIndex)
            l = sizeof pInfo->wordToIndex-1;
        for (size_t i = 0; i < l; ++i)
            pInfo->wordToIndex[i] = pInfo->originalWord[l-i-1];
        pInfo->wordToIndex[l] = '\0';
        return;
    }
}

void dt_search_interface::indexInverted()
{
    char includeFilters[80];
    char excludeFilters[80];
    short result = 0;

    m_indexJob->clear();
    m_indexJob->pOnIndexWordFn = OnIndexWord_Inverter;
    m_indexJob->pOnIndexWordData = this;

    dtsErrorInfo errorInfo;
    errorInfo.pUserData = this;
    errorInfo.setNotify(errorNotifyFn);
    m_indexJob->errorHandler = &errorInfo;

    DString path(m_indexJob->indexPath, sizeof m_indexJob->indexPath);
    if (selectIndex(path) == FAIL)
        return;

    inputString("Directory to index: ", m_indexJob->toAdd.dir, sizeof m_indexJob->toAdd.dir);
    inputString("Filename filter (i.e., *.DOC): ", includeFilters, sizeof includeFilters);
    inputString("Exclude filter: ", excludeFilters, sizeof excludeFilters);

    m_indexJob->toAdd.includeFilters = includeFilters;
    m_indexJob->toAdd.excludeFilters = excludeFilters;
    m_indexJob->action.add = true;
    m_indexJob->pReportCallBack = indexingCB;
    m_indexJob->pReportData = this;

    dtssDoIndexJob(*m_indexJob, result);

    if (result == FAIL)
        out << "FAILED -- Unable to update index in " << m_indexJob->indexPath << nl;
    else
        out << "OK" << nl;
}


#if defined(_Windows) && !defined(__WIN32__)

#include <windows.h>

#endif
