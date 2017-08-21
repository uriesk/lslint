#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: archive_lslint.sh RELEASE_NAME(e.g. v1.0.4)" 1>&2
  exit 1
fi

LSLINT_ZIP_VERSION=$1
mkdir -p binary/zips

pushd binary/windows/; zip ../zips/lslint_${LSLINT_ZIP_VERSION}_win.zip lslint.exe; popd
pushd binary/windows32/; zip ../zips/lslint_${LSLINT_ZIP_VERSION}_win32.zip lslint.exe; popd
pushd binary/windows64/; zip ../zips/lslint_${LSLINT_ZIP_VERSION}_win64.zip lslint.exe; popd
pushd binary/osx/; zip ../zips/lslint_${LSLINT_ZIP_VERSION}_osx.zip lslint; popd
pushd binary/linux/; zip ../zips/lslint_${LSLINT_ZIP_VERSION}_linux.zip lslint; popd

