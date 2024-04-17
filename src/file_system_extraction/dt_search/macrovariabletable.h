#ifndef MacroVariableTable_H
#   define MacroVariableTable_H

#ifndef DSTRING_H
    #include <dstring.h>
#endif

#ifdef USE_DTSEARCH_NAMESPACE
	using namespace dtSearch;
#endif

// CMacroVariableTable handles substitution of the macro strings
// used in the search results formatting options.
// For example, "%%HitCount%%" is replaced with the hit count
// for a document.
//
class CMacroVariableTable {
    public:
        CMacroVariableTable();
		void clear() { 
			nameTable.clear();
			valueTable.clear();
			}
        void addHtmlVar(const char *varName, const char *value);
        void addTextVar(const char *varName, const char *value);
        void addTextVarT(const char *varName, const TCHAR *value);
        void addIntVar(const char *varName, long value);
		void addCommaIntVar(const char *varName, __int64 v, const char *comma = 0);
        void addDateVar(const char *varName, int year, int month, int day);
        void addDateVar(const char *varName, SYSTEMTIME& st);
        void doReplacements(DString& s, const char *inctest = 0);
        void formatAsString(DString& dest);
        void replaceIncludes(DString& s, const char *inctest);
        void setSearchFormText(const char *s) {
			searchFormText = s;
			}
        void replaceVars(DString& s);
        void cleanUpMacros(DString& s);
    protected:
        DStringSet nameTable;
        DStringSet valueTable;
		DString tmp;
		DString searchFormText;
		DWString wTemp;
        void addVarName(const char *name);
        void addValue(const char *value);
        void safeHtmlEncode(DString& s);
    };

#endif
