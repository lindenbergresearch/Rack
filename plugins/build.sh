#!/bin/zsh
#       __   ___  ______
#      / /  / _ \/_  __/
#     / /__/ , _/ / /    Lindenberg
#    /____/_/|_| /_/  Research Tec.
#
#
#	   https://github.com/lindenbergresearch/LRTRack
#    heapdump@icloud.com
#
#    Sound Modules for VCV Rack
#    Copyright 2017-2020 by Patrick Lindenberg / LRT
#
#    For Redistribution and use in source and binary forms,
#    with or without modification please see LICENSE.
#
ARG1=$1
LOGDIR=./log

mkdir -p $LOGDIR

if [[ ${ARG1} == "" ]]; then
  printf "build: no target given.\n"
  exit 1
fi

ok=00
nok=00

if [ -f logo.txt ]; then
  cat logo.txt
fi

printf "logfiles stored at  : ${LOGDIR}\n"
printf "total cpu cores     : $(sysctl -n hw.ncpu)\n"
printf "machine id          : $(uname -mpsr)\n\n"

printf "$(pwd):\n"

if [[ $1 == "cleanup" ]]; then
  printf "just cleanup logs.\n"
  rm -rf $LOGDIR
  exit
fi

for i in */Makefile; do
  DIR=$(echo "$i" | cut -d"/" -f1)
  printf '* make: "%s" for: [%-26s] => ' "$ARG1" "$DIR"

  pushd $DIR >/dev/null

  CMD="time make $ARG1 -j $(sysctl -n hw.ncpu)"

  bash -c "$CMD" >../$LOGDIR/${DIR}.log 2>>../$LOGDIR/${DIR}.log

  if [ $? -ne 0 ]; then
    nok=$((nok + 1))
    printf "fail!\n"
  else
    ok=$((ok + 1))
    T=$(cat ../$LOGDIR/${DIR}.log | grep real)
    printf "ok. [$T]\n"
  fi

  popd >/dev/null

done

COUNT=$(ls */Makefile | wc -l)
printf '\n%s\n%s\n%s\n%s\n\n' "succeeded    : $ok" "failed       : $nok" "total        : $((ok + nok))" "disk usage   : $(du -hac | grep total | cut -d"t" -f1)"

# EOF
