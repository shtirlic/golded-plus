#!/bin/bash
# $Id$
# Create a archive 'gpl-*.zip' (gplYMMDD.zip) with binary linux build
# of the Golded+.

srcdatefile=srcdate.h
build=`sed -n 's/.*"\([[:digit:]]\{8\}\)".*/\1/p' $srcdatefile`
date="$build"
shortdate=${date/???/}

build_flavor="minimal"
platform="lnx"
binsuffix="lnx"
binesdir="bin"
dizfile="$binesdir/file_id.diz"
shortname=gpl${shortdate}.zip
name=gpl115-${date}.zip
requirements_file="${binesdir}/requirements.txt"

echo Build a Golded+/lnx binary package:  ${name} and ${shortname}

if [ ! -f golded3/mygolded.h ]; then
  cp golded3/mygolded.__h golded3/mygolded.h
  echo "golded3/mygolded.h is created now. Please edit this file"
  exit 1
fi

bines="${binesdir}/ged${binsuffix} ${binesdir}/gn${binsuffix} ${binesdir}/rddt${binsuffix}"
files="${bines} docs/copying docs/copying.lib ${requirements_file}"
files="${files} bin/golded bin/openbsd-vt220.sh  ${dizfile}"
files="${files} docs/golded.html docs/golded.txt docs/goldnode.html"
files="${files} docs/goldnode.txt docs/license.txt docs/notework.txt"
files="${files} docs/rddt.html docs/rddt.txt docs/readme.txt docs/notework.rus"
files="${files} docs/rusfaq.txt docs/rusfaq.utf8 docs/tips.txt docs/todowork.txt"
files="${files} docs/tokencfg.txt docs/tokentpl.txt"
files="${files} docs/notework.rus docs/notework.utf8"

compilerver=`gcc --version | grep ^gcc | sed 's/^.* //g'`
printf "Golded NG $build ($build_flavor) \\r\\n"  >${dizfile}
printf '%-5s binaries compiled using ' "`uname`" >>${dizfile}
printf "GCC:$compilerver" >>${dizfile}

exit 0
for f in ${bines}; do rm $f; done
make PLATFORM=${platform} BUILD=${build_flavor} clean all strip docs || (echo "Error $?" ; exit)

for i in ${bines} ; do
  if [ ! -f ${i} ] ; then echo "File ${i} not exists, stop!"; exit 1 ; fi
done

echo Required libraries:  >bin/requirements.txt
ldd ${binesdir}/ged${binsuffix} | sed -e "s/ =.*//" -e /\\/lib\\//d -e "s/\\.so\\./ /" >>${requirements_file}

zip -9DXj ${name} $files
ln ${name} ${shortname} || cp ${name} ${shortname}
