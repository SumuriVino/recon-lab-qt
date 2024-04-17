#ifndef SETTINGS_H
#   define SETTINGS_H

#ifndef XMLMAKER_H
#   include <xmlmaker.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE

namespace dtSearch {

#endif


#define fnItem "Item"

// Generate read/write for a table of "Item" members
template <class Type>
class CXmlTableOfItems : public DArrayAsVector<Type> {
    public:

        virtual ~CXmlTableOfItems() {}
        virtual const char *getTableName() {
            return "GenericTable";
            }
        virtual void writeTableProperties(CXmlMaker& /* maker */) {
            }
        virtual void readTableProperties(CXmlReader& /* input */, CXmlReader::Selection& /* sel */) {
            }

        void writeTo(CXmlMaker& xmlMaker, const char *tableName) {
            xmlMaker.beginField(tableName);
            writeTableProperties(xmlMaker);
            for (int i = 0; i < this->getCount(); ++i) {
                xmlMaker.beginField(fnItem);
                this->getRef(i).writeTo(xmlMaker);
                xmlMaker.endField(fnItem);
                }
            xmlMaker.endField(tableName);
			}
        void writeToString(DString& dest) {
            CXmlMaker xmlMaker;
            const char *tableName = getTableName();
            writeTo(xmlMaker, tableName);
            xmlMaker.getText(dest);
            }
        int write(const TCHAR *fn) {
			return writeU8(UiToUtf8(fn));
            }
        int writeU8(const char *fn) {
            DString temp;
            writeToString(temp);
            return temp.writeToU8(fn);
            }
        virtual int readV5(const char * /* fn */) {
            return FAIL;
            }
		void readFrom(CXmlReader& input, CXmlReader::Selection sel) {
            readTableProperties(input, sel);
            CXmlReader::FieldSet items = input.find(fnItem, sel);
            for (int i = 0; i < items.getCount(); ++i) {
                Type item;
                CXmlReader::Selection itemFields = input.select(items, i);
                item.readFrom(input, itemFields);
                this->append(item);
                }

			}
		int read(const TCHAR *fn) {
			return readU8(UiToUtf8(fn));
			}
		int readU8(const char *fn) {
            // this-> needed for gcc template compiler error
			this->clear();
            CXmlReader xmlReader;
            const char *tableName = getTableName();
            xmlReader.getFromU8(fn, tableName);
            CXmlReader::Selection sel = xmlReader.select(tableName);
            if (!sel)
                return readV5(fn);

            readFrom(xmlReader, sel);

            return SUCCESS;
            }
			
        int readFromBuf(const char *buf) {
            this->clear();
			CXmlReader xmlReader;
			xmlReader.setXml(buf);
            CXmlReader::Selection sel = xmlReader.select(getTableName());
            if (!sel)
                return FAIL;

            readFrom(xmlReader, sel);
            return SUCCESS;
			}
        int readW(const wchar_t *fn) {
            CUiStringFromUnicode uiName = fn;
            return read(uiName);
            }
        void writeW(const wchar_t *fn) {
            CUiStringFromUnicode uiName = fn;
            write(uiName);
            }

        };


const int ftOverrideInternal = dtsFtOverride;
const int ftDisable = dtsFtDisable;

struct CFileTypeTableItem {
    DString name;
    long typeId;
    DStringSet filters;
    long flags;

    CFileTypeTableItem() {
        typeId = it_Ansi;
        flags = 0;
        }
    CFileTypeTableItem(const CFileTypeTableItem& other) {
        *this = other;
        }
    CFileTypeTableItem& operator=(const CFileTypeTableItem& other) {
        typeId = other.typeId;
        filters = other.filters;
        flags = other.flags;
        name = other.name;
        return *this;
        }
    int operator==(const CFileTypeTableItem& ) {
        return false;
        }
    void setName(const char *n) {
        name = n;
        }
    void writeTo(CXmlMaker& xmlMaker);
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel);
    };

class CFileTypeTable : public CXmlTableOfItems<CFileTypeTableItem>  {
    public:
        DString defaultEncoding;
        CFileTypeTable& operator=(const CFileTypeTable& other) {
            defaultEncoding = other.defaultEncoding;
            copyArray(other);
            return *this;
            }
        virtual const char *getTableName() {
            return "dtSearchFileTypeRules";
            }
        virtual void writeTableProperties(CXmlMaker& maker) {
            maker.storeField("DefaultEncoding", defaultEncoding);
            }
        virtual void readTableProperties(CXmlReader& input, CXmlReader::Selection& sel ) {
            input.getFirstIn(sel, "DefaultEncoding", defaultEncoding);
            }

    };



const int mtExact = 1;
const int mtCaseInsensitive = 2;
const int mtRegularExpression = 4;
const int mtPrefix = 8;
const int mtFirstSegmentIsHeader = 16;

struct CSplitRulesTableItem {
    long matchType;
    DString name;
    DString splitMark;
    DStringSet filters;
    CSplitRulesTableItem() {
        matchType = mtExact;
        }
    CSplitRulesTableItem(const CSplitRulesTableItem& other) {
        *this = other;
        }
    CSplitRulesTableItem& operator=(const CSplitRulesTableItem& other) {
        name = other.name;
        splitMark = other.splitMark;
        filters = other.filters;
        matchType = other.matchType;
        return *this;
        }
    int operator==(const CSplitRulesTableItem& ) {
        return false;
        }
    void setName(const char *n) {
        name = n;
        }
    void writeTo(CXmlMaker& xmlMaker);
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel);
    };

class CSplitRulesTable : public CXmlTableOfItems<CSplitRulesTableItem> {
    public:
        int readV5(const char *fn);
        const char *getTableName() {
            return "dtSearchFileSegmentationRules";
            }
    };


const int tfStoreFieldText = 8;
const int tfRegularExpression = 4;
const int tfCaseInsensitive = 2;

struct CTextFieldTableItem {
    DString80 name;
    DString80 start;
    DString80 end;
    DStringSet includeFilters;
    long linesToScan;
    long flags;
    bool fCaseInsensitive;
    bool fRegularExpression;
    bool fStoreFieldText;
    bool fExactMatch;
    CTextFieldTableItem() : linesToScan(0), fCaseInsensitive(0),
        fRegularExpression(false), fStoreFieldText(false), flags(false), fExactMatch(false) {
        }
    CTextFieldTableItem& operator=(const CTextFieldTableItem& other) {
        name = other.name;
        start = other.start;
        end = other.end;
        includeFilters = other.includeFilters;
        linesToScan = other.linesToScan;
        fCaseInsensitive = other.fCaseInsensitive;
        fRegularExpression = other.fRegularExpression;
        fStoreFieldText = other.fStoreFieldText;
        fExactMatch = other.fExactMatch;
        flags = other.flags;
        return *this; }
    void getFlags() {
        fCaseInsensitive = makeBool(flags & tfCaseInsensitive);
        fRegularExpression = makeBool(flags & tfRegularExpression);
        fStoreFieldText = makeBool(flags & tfStoreFieldText);
        fExactMatch = makeBool(!fCaseInsensitive && !fRegularExpression);
        }

    int operator==(const CTextFieldTableItem& other) const {
        return !strcmp(name, other.name);
        }
    void setName(const char *aName) {
        name = aName;
        }
    void writeTo(CXmlMaker& xmlMaker);
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel);
    };

class CTextFieldTable : public CXmlTableOfItems<CTextFieldTableItem> {
    public:
        int readV5(const char *fn);
        const char *getTableName() {
            return "dtSearchTextFieldsDefinitions";
            }
    };

struct CUserThesaurusItem {
    DString name;
    DStringSet synonyms;

    CUserThesaurusItem() { }
    void setName(const char *aName) {
        name = aName;
        }
    CUserThesaurusItem(const CUserThesaurusItem& other) {
        *this = other;
        }
    CUserThesaurusItem& operator=(const CUserThesaurusItem& other) {
        name = other.name;
        synonyms = other.synonyms;
        return *this;
        }
    void writeTo(CXmlMaker& xmlMaker);
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel);
    };

class CUserThesaurusTable : public CXmlTableOfItems<CUserThesaurusItem> {
    public:
        const char *getTableName() {
            return "dtSearchUserThesaurus";
            }
        int readV5(const char *fn); // read version 5 format file
    };


struct CXmlSearchResultsItem {
    CXmlSearchResultsItem() {
        }
    CXmlSearchResultsItem(const CXmlSearchResultsItem& other) {
        *this = other;
        }
    CXmlSearchResultsItem& operator=(const CXmlSearchResultsItem& other) {
        filename = other.filename;
        hitCount = other.hitCount;
        shortName = other.shortName;
        location = other.location;
        return *this;
        }
    DFilename filename;
    DFilename shortName;
    DFilename location;
    long hitCount;

    void writeTo(CXmlMaker& /* xmlMaker */) {
        // write not supported
        }
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel) {
        input.getFirstIn(sel, "Filename", filename);
        input.getFirstIn(sel, "HitCount", hitCount);
        input.getFirstIn(sel, "ShortName", shortName);
        input.getFirstIn(sel, "Location", location);
        }
    };

struct CMacroTableItem {
    DString name;
    DString expansion;
    CMacroTableItem() {
        }
    CMacroTableItem& operator=(const CMacroTableItem& other) {
        name = other.name;
        expansion = other.expansion;
        return *this; }

    int operator==(const CMacroTableItem& other) const {
        return !strcmp(name, other.name);
        }
    void setName(const char *aName) {
        name = aName;
        }
    void writeTo(CXmlMaker& xmlMaker) {
        xmlMaker.storeField("Name", name);
        xmlMaker.storeField("Expansion", expansion);
        }
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel) {
        input.getFirstIn(sel, "Name", name);
        input.getFirstIn(sel, "Expansion", expansion);
        }
    };

class CMacroTable : public CXmlTableOfItems<CMacroTableItem> {
    public:
        CMacroTable()
            {
            }
        const char *getTableName() {
            return "dtSearchMacros";
            }
        virtual int readV5(const char *fn);
    };




struct CStemmingRule {
    DString suffix;
    DString replace;
    long minPrefix;

    CStemmingRule() {
        minPrefix = 0;
        }
    CStemmingRule& operator=(const CStemmingRule& other) {
        minPrefix = other.minPrefix;
        replace = other.replace;
        suffix = other.suffix;
        return *this;
        }

    int operator==(const CStemmingRule& other) const {
        return !strcmp(suffix, other.suffix) && (minPrefix == other.minPrefix);
        }
    void writeTo(CXmlMaker& xmlMaker) {
        xmlMaker.storeField("Suffix", suffix);
        xmlMaker.storeField("Replace", replace);
        xmlMaker.storeField("MinPrefix", minPrefix);
        }
    void readFrom(CXmlReader& input, CXmlReader::Selection& sel) {
        input.getFirstIn(sel, "Suffix", suffix);
        input.getFirstIn(sel, "Replace", replace);
        input.getFirstIn(sel, "MinPrefix", minPrefix);
        }
    };

class CXmlStemmingRulesTable : public CXmlTableOfItems<CStemmingRule> {
    public:
        const char *getTableName() {
            return "dtSearchStemmingRules";
            }
        int readV5(const char *fn);
    };

class CXmlSearchResults : public CXmlTableOfItems<CXmlSearchResultsItem> {
    public:
        dtsFileDate searchDate;
        DString request;
        long totalFiles;
        long totalHits;

        CXmlSearchResults() {
            totalFiles = 0;
            totalHits = 0;
            }
        CXmlSearchResults& operator=(const CXmlSearchResults& other) {
            memmove(&searchDate, &other.searchDate, sizeof searchDate);
            request = other.request;
            totalFiles = other.totalFiles;
            totalHits = other.totalHits;
            copyArray(other);
            return *this;
            }

        const char *getTableName() {
            return "dtSearchResults";
            }
        virtual void readTableProperties(CXmlReader& input, CXmlReader::Selection& sel) {
            input.getFirstIn(sel, "SearchDate", searchDate);
            input.getFirstIn(sel, "Request", request);
            input.getFirstIn(sel, "TotalFiles", totalFiles);
            input.getFirstIn(sel, "TotalHits", totalHits);
            }
        void readHeader(const TCHAR *fn) {
			readHeaderU8(UiToUtf8(fn));
			}
        void readHeaderU8(const char *fn) {
            clear();

            DString tmp;
            tmp.getFromU8(fn, 1024L + 8192);
            ptrdiff_t iFirstItem = tmp.find("<Item>");
            if (iFirstItem > 0)
                tmp.truncate(iFirstItem);
            CXmlReader reader;
            reader.setXml(tmp);
            CXmlReader::Selection sel = reader.selectAll();
            readTableProperties(reader, sel);
            }
    };

// Data for HTML forms-based authentication
struct CHttpFormAuthInfo {
	long fHaveData;
	long fAuthViaPost;
	DString authUrl;
	DStringSet authData;

	CHttpFormAuthInfo() {
		clear();
		}
	CHttpFormAuthInfo(const CHttpFormAuthInfo& other) {
		copy(other);
		}
	void clear() {
		fHaveData = 0;
		fAuthViaPost = 0;
		authUrl.clear();
		authData.clear();
		}
	void copy(const CHttpFormAuthInfo& other) {
		fHaveData = other.fHaveData;
		fAuthViaPost = other.fAuthViaPost;
		authUrl = other.authUrl;
		authData = other.authData;
		}
	CHttpFormAuthInfo& operator=(const CHttpFormAuthInfo& other) {
		copy(other);
		return *this;
		}
    void getFrom(CXmlReader& reader, CXmlReader::Selection sel);
    void writeTo(CXmlMaker& writer);
	};
struct IndexLibraryEntry {
    DWString name;
    DWString path;
    DWString serverUrl;
    DWString serverPath;
    DWString searchForm;

    IndexLibraryEntry() { }
    IndexLibraryEntry(const IndexLibraryEntry& other) {
        *this = other;
        }
    int operator!() {
        return path.isBlank();
        }
    IndexLibraryEntry& operator=(const IndexLibraryEntry& other) {
        name = other.name;
        path = other.path;
        serverUrl = other.serverUrl;
        searchForm = other.searchForm;
        serverPath = other.serverPath;
        return *this;
        }
    int isLocal() {
        return serverUrl.isBlank();
        }
    int isWeb() {
        return !isLocal();
        }
    int operator==(const IndexLibraryEntry & other) const {
        return !_wcsicmp(name, other.name);
        }
    int operator>(const IndexLibraryEntry& other) const {
        return _wcsicmp(name, other.name) > 0;
        }
    int operator<(const IndexLibraryEntry& other) const {
        return _wcsicmp(name, other.name) < 0;
        }
    void setA(const char *aName, const char *aPath) {
        name.storeAnsiAsUnicode(aName);
        path.storeAnsiAsUnicode(aPath);
        };
    void setU8(const char *aName, const char *aPath) {
        name.storeUtf8AsUnicode(aName);
        path.storeUtf8AsUnicode(aPath);
        }
    void setW(const wchar_t *aName, const wchar_t *aPath) {
        name = aName;
        path = aPath;
        }
	void setUi(const TCHAR *tName, const TCHAR *tPath) {
#ifdef _UNICODE
		setW(tName, tPath);
#else
		setA(tName, tPath);
#endif
		}		
    void generateServerPath() {
        path.clear();
        path << serverUrl << L"?index=" << serverPath;
        }
    void extractServerInfo() {
        const wchar_t *splitPoint = wcsstr(path, L"?index=");
        if (splitPoint) {
            serverUrl.clear();
            serverUrl.store(path, (int) (splitPoint - path.str()));
            serverPath = splitPoint + 7;
            }
        }
    };

// The type of a library determines what happens when a user double-clicks it.
// Merge: entries stored in current library
// Copy:  index library is copied locally
// Standard: a link to the library is created
#define IndexLibraryActionMerge     "Merge"
#define IndexLibraryActionCopy      "Copy"
#define IndexLibraryActionStandard  "Standard"
#define IndexLibraryActionUse       "Use"

class CIndexLibrary {
    public:
        CIndexLibrary() {
            m_name = "Library";
            }
        CIndexLibrary(const CIndexLibrary& other) {
            *this = other;
            }
        CIndexLibrary& operator=(const CIndexLibrary& other) {
            m_table = other.m_table;
            m_name = other.m_name;
            m_filename = other.m_filename;
            return *this;
            }
        int operator!=(const CIndexLibrary& other) const {
            return !(*this == other);
            }
        int operator==(const CIndexLibrary& other) const;
        void add(const CIndexLibrary& other);
        void clear() {
            m_table.clear();
            m_name.clear();
            m_filename.clear();
            }
        int getCount() const {
            return m_table.getCount();
            }
        void addA(const char *aName, const char *aPath, int fCheckIfUnique = false) {
            IndexLibraryEntry e;
            e.setA(aName, aPath);
            add(e, fCheckIfUnique);
            }
        int add(const IndexLibraryEntry& e, int fCheckIfUnique = true) {
            if (!fCheckIfUnique || ((findByNameW(e.name) == FAIL) && (findByPathW(e.path) == FAIL))) {
                m_table.append(e);
                return SUCCESS;
                }
            else
                return FAIL;
            }
        // Add an entry and make the m_name unique if another index has the same m_name
        int addWithUniqueName(IndexLibraryEntry& e);
        void addW(const wchar_t *aName, const wchar_t *aPath, int fCheckIfUnique = false) {
            IndexLibraryEntry e;
            e.setW(aName, aPath);
            add(e, fCheckIfUnique);
            }
        int findByNameW(const wchar_t *aName);
        int findByNameA(const char *aName) {
            DWString temp;
            temp.storeAnsiAsUnicode(aName);
            return findByNameW(temp);
            }
        int findByPathW(const wchar_t *aPath);
        int findByPathA(const char *aPath) {
            DWString temp;
            temp.storeAnsiAsUnicode(aPath);
            return findByPathW(temp);
            }
        const wchar_t *nameToPathW(const wchar_t *aName) {
            int i = findByNameW(aName);
            if (i > FAIL)
                return m_table[i].path;
            else
                return 0;
            }
        const wchar_t *nameToPathA(const char *aName) {
            int i = findByNameA(aName);
            if (i > FAIL)
                return m_table[i].path;
            else
                return 0;
            }
        const wchar_t *pathToNameW(const wchar_t *aPath) {
            int i = findByPathW(aPath);
            if (i > FAIL)
                return m_table[i].name;
            else
                return 0;
            }
        const wchar_t *pathToNameA(const char *aPath) {
            int i = findByPathA(aPath);
            if (i > FAIL)
                return m_table[i].name;
            else
                return 0;
            }
        int writeU8(const char *fn, int fMakeRelative = false);
        int write(const TCHAR *fn, int fMakeRelative = false) {
			return writeU8(UiToUtf8(fn), fMakeRelative);
			}
        int read(const TCHAR *fn) {
			return readU8(UiToUtf8(fn));
			}
		int readU8(const char *fn);
        int readFromXml(const char *xml, const char *fn= 0);
        int readOld(const char *fn);
        IndexLibraryEntry& get(int i) {
            return m_table[i];
            }
        int save() {
            return writeU8(m_filename, true);
            }
        int saveAs(const TCHAR *filename) {
            m_filename = UiToUtf8(filename);
            return save();
            }
        void remove(int iIndex) {
            m_table.remove(iIndex);
        }
        void setName(const char *aName) {
            m_name = aName;
            }
        const char *getName() {
            return m_name;
            }
        const IndexLibraryEntry& get(int i) const {
            if ((i >= 0) && (i < m_table.getCount()))
                return m_table.get(i);
            else
                return m_blankEntry;
            }
        void setAction(const char *s) {
            m_action = s;
            }
        int isActionType(const char *s) {
            return !_stricmp(s, m_action);
            }
        DArrayAsVector<IndexLibraryEntry> m_table;
        DFilename m_baseFolder;
    protected:
        IndexLibraryEntry m_blankEntry;
        DFilename m_filename;
        DString m_name;
        DString m_action;
    };


#ifdef USE_DTSEARCH_NAMESPACE
}  // dtSearch namespace
#endif



#endif
