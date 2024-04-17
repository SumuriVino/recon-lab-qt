#include "stdafx.h"
#include "settings.h"
#include "dtconfig.h"
//#include <stringtools.h>

#ifdef USE_DTSEARCH_NAMESPACE
using namespace dtSearch;
#endif

using namespace std;



#define fnFieldEnd          "End"
#define fnFieldStart        "Start"
#define fnFilters           "Filters"
#define fnFlags             "Flags"
#define fnHtmlHead          "HtmlHead"
#define fnIncludeFilters    "Filters"
#define fnBaseFolder        "BaseFolder"
#define fnIndexName         "IndexName"
#define fnItem              "Item"
#define fnItemName          "Name"
#define fnItemSynonyms      "Synonyms"
#define fnLinesToScan       "LinesToScan"
#define fnMatchType         "MatchType"
#define fnName              "Name"
#define fnRecursive         "Recursive"
#define fnSplitMark         "SplitMark"
#define fnTypeId            "TypeId"
#define fnUserThesaurus     "dtSearchUserThesaurus"
void CFileTypeTableItem::writeTo(CXmlMaker& xmlMaker) {
    xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnTypeId, typeId);
    xmlMaker.storeField(fnFilters, filters);
    xmlMaker.storeField(fnFlags, flags);
}

void CFileTypeTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnTypeId, typeId);
    input.getFirstIn(sel, fnFilters, filters);
    input.getFirstIn(sel, fnFlags, flags);
}

void CSplitRulesTableItem::writeTo(CXmlMaker& xmlMaker) {
    xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnMatchType, matchType);
    xmlMaker.storeField(fnFilters, filters);
    xmlMaker.storeField(fnSplitMark, splitMark);
}

void CSplitRulesTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnMatchType, matchType);
    input.getFirstIn(sel, fnFilters, filters);
    input.getFirstIn(sel, fnSplitMark, splitMark);
}

int CSplitRulesTable::readV5(const char *fn)
{
   const char *CSplitRulesTableId = "dtSearch File Splitting Rules";

    clear();
    DStringSet lines;
    lines.getLinesFromU8(fn);
    if (lines.getCount() == 0)
		return FAIL;
		
	int iLine = 0;
    DString line;
    line = lines.getString(iLine++);
    line.trim();
    if (_stricmp(line, CSplitRulesTableId))
        return FAIL;

    while(iLine < lines.getCount()) {
        line = lines.getString(iLine++);
        if (!line.isBlank()) {
            DString filters;
            DStringSet s;
            s.tokenizeq(line, ",", '\'', true);
            if (s.getCount() >= 4) {
                CSplitRulesTableItem item;
                item.name = s.getString(0);
                item.splitMark = s.getString(1);
                filters = s.getString(2);
                item.matchType = s.getInt(3);
                item.filters.tokenizeq(filters, ",", '\"');
                append(item);
                }
            }
        }
    return SUCCESS;
}

const char *CTextFieldTableId = "dtSearch Text Field Table";

void CTextFieldTableItem::writeTo(CXmlMaker& xmlMaker)
{   xmlMaker.storeField(fnName, name);
    xmlMaker.storeField(fnFieldStart, start);
    xmlMaker.storeField(fnFieldEnd, end);
    xmlMaker.storeField(fnIncludeFilters, includeFilters);
    xmlMaker.storeField(fnLinesToScan, linesToScan);
    xmlMaker.storeField(fnFlags, flags);
}

void CTextFieldTableItem::readFrom(CXmlReader& input, CXmlReader::Selection& sel)
{   input.getFirstIn(sel, fnName, name);
    input.getFirstIn(sel, fnFieldStart, start);
    input.getFirstIn(sel, fnFieldEnd, end);
    input.getFirstIn(sel, fnIncludeFilters, includeFilters);
    input.getFirstIn(sel, fnLinesToScan, linesToScan);
    input.getFirstIn(sel, fnFlags, flags);
    getFlags();
}

int CTextFieldTable::readV5(const char *fn)
{
    clear();
    DStringSet lines;
    lines.getLinesFromU8(fn);
    if (!lines.getCount())
		return FAIL;
		
    DString line;
    int iLine = 0;
    line = lines.getString(iLine++);
    
    if (_stricmp(line, CTextFieldTableId))
        return FAIL;
    while(iLine < lines.getCount()) {
	    line = lines.getString(iLine++);

        if (!line.isBlank()) {
            DString filters;
            CTextFieldTableItem item;
            DStringSet s;
            s.tokenizeq(line, ",", '\'', true);
            if (s.getCount() >= 6) {
                DString sFilters;
                CTextFieldTableItem ftItem;
                ftItem.name = s.getString(0);
                ftItem.start = s.getString(1);
                ftItem.end = s.getString(2);
                ftItem.name.trim();
                ftItem.start.trim();
                ftItem.end.trim();
                ftItem.linesToScan = s.getInt(3);
                sFilters = s.getString(4);
                ftItem.flags = s.getInt(5);
                ftItem.includeFilters.tokenizeq(sFilters, ";, ", '\"');
                append(ftItem);
                }
            }
        }
    return SUCCESS;
}

void CUserThesaurusItem::writeTo(CXmlMaker& xmlMaker)
{   xmlMaker.storeField(fnItemName, name);
    xmlMaker.storeField(fnItemSynonyms, synonyms);
}

void CUserThesaurusItem::readFrom(CXmlReader& xmlReader, CXmlReader::Selection& sel)
{   xmlReader.getFirstIn(sel, fnItemName, name);
    xmlReader.getFirstIn(sel, fnItemSynonyms, synonyms);
}

int CUserThesaurusTable::readV5(const char *fn)
{   clear();

    const char *UserThesaurusId = "dtSearch User Thesaurus";
    DStringSet lines;
    lines.getLinesFromU8(fn);
    int ct = 0;
    DString line = lines.getString(ct++);
    line.trim();

    if (strcmp(line, UserThesaurusId))
        return FAIL;

    int i = 1;
    while(!line.isBlank() && (ct < lines.getCount())) {
        line = lines.getString(ct++);
        CUserThesaurusItem item;
        item.synonyms.tokenizeq(line, " \r\n\t", '\"');
        if (item.synonyms.getCount() > 0) {
            int fCanUseFirst = true;
            DString first = item.synonyms.getString(0);
            for (int j = 0; j < getCount(); ++j) {
                if (!_stricmp(get(j).name, first))
                    fCanUseFirst = false;
                }
            if (fCanUseFirst)
                item.name = first;
            else
                item.name << "#" << i++;
            append(item);
            }
        }
    return SUCCESS;
}


int CMacroTable::readV5(const char *fn)
{   DString s;
    s.getFromU8(fn);
    if (s.isBlank())
        return FAIL;
    if (_strnicmp(s, "dtSearch Macros", 15))
        return FAIL;
    for(const char *p = strstr(s, "Name = "); p; p = strstr(p + 1, "Name = ")) {
        const char *n = p + 7;
        const char *q = strstr(n, "Exp = ");
        if (q) {
            CMacroTableItem item;
            const char *e = strchr(q, '\n');
            if (!e)
                e = q + strlen(q);
            item.name.store(n, q - n);
            q += 6;
            item.expansion.store(q, e-q);
            item.name.trim();
            item.expansion.trim();
            append(item);
            }
        }
    return SUCCESS;
}

int CXmlStemmingRulesTable::readV5(const char *fn)
{	
    clear();
    DStringSet lines;
    lines.getLinesFromU8(fn);
    DString line;
    for (int i = 0; i < lines.getCount(); ++i) {
        CStemmingRule rule;
        line = lines.getString(i);
        if (line.first() == '*')
            continue;
        if (line.find("-----") > FAIL)
            break;

        char *s = line.str();
        char *p = strchr(s, '+');
        if (p && atoi(s)) {
            rule.minPrefix = atoi(s);
            s = p + 1;
            }
        else
            rule.minPrefix = 0;
        p = strstr(s, "->");
        if (!p)
            continue;
        *(p-1) = 0;
        rule.suffix = s;
        rule.suffix.trim();
        rule.replace = p + 2;
        rule.replace.trim();
        append(rule);
        }
    if (getCount() > 0)
        return SUCCESS;
    else
        return FAIL;
}


void CIndexLibrary::add(const CIndexLibrary& other)
{   for (int i = 0; i < other.getCount(); ++i) {
        IndexLibraryEntry e = other.get(i);
        addWithUniqueName(e);
        }
}

int CIndexLibrary::operator ==(const CIndexLibrary& other) const
{   if (getCount() != other.getCount())
        return false;

    for (int i = 0; i < other.getCount(); ++i) {
        const IndexLibraryEntry &a = m_table.get(i);
        const IndexLibraryEntry &b = other.m_table.get(i);
        if (wcscmp(a.path, b.path))
            return false;
        if (wcscmp(a.name, b.name))
            return false;
        }
    return true;
}

int CIndexLibrary::findByPathW(const wchar_t *aPath)
{   for (int i = 0; i < m_table.getCount(); ++i) {
        if (!_wcsicmp(aPath, m_table[i].path))
            return i;
        }
    return FAIL;
}

int CIndexLibrary::findByNameW(const wchar_t *aName)
{   for (int i = 0; i < m_table.getCount(); ++i) {
        if (!_wcsicmp(aName, m_table[i].name))
            return i;
        }
    return FAIL;
}

#define IndexLibraryId		"dtSearch Index Library"
#define fnIndexLibrary		"dtSearchIndexLibrary"
#define fnLibName			"Name"
#define fnLibAction			"Action"
#define	fnLibItem			"Item"
#define fnIndexPath			"IndexPath"
#define fnServerUrl			"ServerUrl"
#define fnSearchForm		"SearchForm"
#define fnServerPath		"ServerPath"

int CIndexLibrary::writeU8(const char *fn, int fMakeRelative)
{
    debugLog("CIndexLibrary::write ", fn);
    CXmlMaker xmlMaker;
    xmlMaker.setOutputU8(fn);
    xmlMaker.clearNoHeader();
    xmlMaker.beginField(fnIndexLibrary);
    xmlMaker.storeField(fnLibName, m_name);
    xmlMaker.storeField(fnLibAction, m_action);
    xmlMaker.storeRelativeField(fnBaseFolder, m_baseFolder);
    DFilename basePath;
    if (fMakeRelative && !DFilename::isRelative(fn)) {
        basePath = fn;
        basePath.rmTail();
        }
    for(int i = 0; i < getCount(); ++i) {
        IndexLibraryEntry& e = get(i);
        xmlMaker.beginField(fnLibItem);
        xmlMaker.storeField(fnIndexName, e.name);
        if (e.serverPath.isBlank()) {
            DFilename path;
            path.storeUnicodeAsUtf8(e.path);
            if (!basePath.isBlank())
                path.makeRelativeTo(basePath);
            xmlMaker.storeField(fnIndexPath, path);
            }
        if (!e.searchForm.isBlank())
            xmlMaker.storeField(fnSearchForm, e.searchForm);
        if (!e.serverUrl.isBlank())
            xmlMaker.storeField(fnServerUrl, e.serverUrl);
        if (!e.serverPath.isBlank())
            xmlMaker.storeField(fnServerPath, e.serverPath);
        xmlMaker.endField(fnLibItem);
        }
    xmlMaker.endField(fnIndexLibrary);
    xmlMaker.writeU8(fn);

    m_filename = fn;

    return SUCCESS;
}

int CIndexLibrary::readU8(const char *fn)
{   debugLog("CIndexLibrary::read ", fn);
    clear();
    DString xml;
    xml.getFromU8(fn);
    if (xml.isBlank())
        return FAIL;
    if (readFromXml(xml, fn) == FAIL)
        return readOld(fn);
    else
        return SUCCESS;
}

int CIndexLibrary::readFromXml(const char *xml, const char *fn)
{
    CXmlReader xmlReader;
    xmlReader.setXml(xml);
    CXmlReader::Selection sel = xmlReader.select(fnIndexLibrary);

    if (!sel)
        return FAIL;

    if (!fn)
        fn = "";
    DFilename basePath = fn;
    basePath.rmTail();

    xmlReader.setFilenameU8(fn);
    xmlReader.getFirstIn(sel, fnLibName, m_name);
    xmlReader.getFirstIn(sel, fnLibAction, m_action);
    xmlReader.getFirstInRelative(sel, fnBaseFolder, m_baseFolder);

    CXmlReader::FieldSet items = xmlReader.find(fnLibItem, sel);
    for (int i = 0; i < items.getCount(); ++i) {
        CXmlReader::Selection itemFields = xmlReader.select(items, i);
        IndexLibraryEntry e;
        DFilename path;
        xmlReader.getFirstIn(itemFields, fnIndexName, e.name);
        xmlReader.getFirstIn(itemFields, fnSearchForm, e.searchForm);
        xmlReader.getFirstIn(itemFields, fnIndexPath, path);
        xmlReader.getFirstIn(itemFields, fnServerPath, e.serverPath);
        xmlReader.getFirstIn(itemFields, fnServerUrl, e.serverUrl);
        if (e.serverPath.isBlank()) {
            path.makeAbsoluteFrom(basePath);
            e.path.storeUtf8AsUnicode(path);
            }
        else
            e.generateServerPath();
        addWithUniqueName(e);
        }
    m_filename = fn;
    return SUCCESS;
}

int CIndexLibrary::readOld(const char *fn)
{

    clear();

	DStringSet lines;
	lines.getLinesFromU8(fn);
	if (lines.getCount() == 0) {
        return FAIL;
        }
    DString line;
    int iLine = 0;
    line = lines.getString(iLine++);
    line.trim();
    if (_stricmp(line, IndexLibraryId)) {
        debugLog("  Read failed, header was not a library id, = ", line);
        return FAIL;
        }
    m_name = lines.getString(iLine++);

    DFilename basePath;
    basePath = fn;
    basePath.rmTail();

    while(iLine < lines.getCount()) {
        line = lines.getString(iLine++);

        if (!line.isBlank()) {
            DStringSet s;
            s.tokenizeq(line, ", ");
            IndexLibraryEntry e;
            e.name.storeUtf8AsUnicode(s.getString(0));
            DFilename path = s.getString(1);
            path.makeAbsoluteFrom(basePath);

            e.path.storeUtf8AsUnicode(path);
            addWithUniqueName(e);
            }
        }
    m_filename = fn;
    return SUCCESS;

}

int CIndexLibrary::addWithUniqueName(IndexLibraryEntry& e)
{   if (add(e, true) == SUCCESS)
        return SUCCESS;
    // do not add the same index twice
    if (findByPathW(e.path) == SUCCESS)
        return FAIL;
    DString origName;
    e.name.getUtf8(origName);
    for (int i = 2; i < 999; ++i) {
        DString newName;
        newName << origName << " #" << i;
        e.name.storeUtf8AsUnicode(newName);
        if (add(e, true) == SUCCESS)
            return SUCCESS;
        }
    return FAIL;
}

