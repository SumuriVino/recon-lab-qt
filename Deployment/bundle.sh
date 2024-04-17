#!/bin/bash

# NOTE: This script should run from the build directory

# Default value for CODE_SIGN_ONLY flag
CODE_SIGN_ONLY=0

# Parse command line arguments
for arg in "$@"; do
    case $arg in
        -code-sign-only)
            CODE_SIGN_ONLY=1
            shift
            ;;
        *)
            # Assume that the first non-recognized argument is MACDEPLOYQT_PATH
            MACDEPLOYQT_PATH=$arg
            shift
            ;;
    esac
done

# Check if MACDEPLOYQT_PATH is empty
if [[ -z $MACDEPLOYQT_PATH ]] && [[ $CODE_SIGN_ONLY -eq 0 ]]; then
    echo "Error: macdeployqt path is required and was not specified." >&2
    exit 1
fi

# Signature identifier
signature="Developer ID Application: SUMURI LLC (M2UAN8S5M3)"
dylib_dir="RECON_LAB.app/Contents/Frameworks"

if [[ $CODE_SIGN_ONLY -eq 0 ]]; then
    # Run mac deploy tool
    $MACDEPLOYQT_PATH/macdeployqt Recon_LAB.app -qmldir=../src/m_qml/ -sign-for-notarization="$signature"

    # The following steps are needed to clean up some of the issues that can occur during the macdeployqt process
    # Change the install name of libdtsearch64.dylib
    install_name_tool -change @loader_path/../bin64/libdtsearch64.dylib @executable_path/../Frameworks/mlibs/libdtsearch64.dylib RECON_LAB.app/Contents/MacOS/RECON_LAB
fi

echo "Signing files..."

# Use the find command to search for dylib files recursively
# The -type f option ensures that only files (not directories) are processed
# The -name "*.dylib" option restricts the search to files with the .dylib extension
find "$dylib_dir" -type f -name "*.dylib" | while read -r dylib; do
    codesign -s "$signature" -f --timestamp --options runtime "$dylib"
done

# Sign the RECON_LAB executable
codesign -s "$signature" -f --timestamp --options runtime RECON_LAB.app/Contents/MacOS/RECON_LAB

# Sign the RECON_LAB application bundle
codesign -s "$signature" -f --timestamp --options runtime RECON_LAB.app

echo "Signing complete"
