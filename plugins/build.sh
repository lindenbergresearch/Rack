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
ARG2=$2
LOG_DIR=$(pwd)/log

mkdir -p $LOG_DIR

if [[ ${ARG1} == "-h" ]]; then
  print "\u001b[34;1m"
  cat logo.txt
  print "\u001b[0m"
  printf "build: nothing to do.\n\n"
  printf "USAGE: $0 [<TARGET>|CLEANUP] <OPTIONS>\n"
  printf "\nTARGET\n  make target to execute on all plugins.\n"
  printf "\nOPTIONS\n"
  printf "  -n build only virgin plugins\n"
  printf "\n"
  exit 1
fi

ok=00
nok=00

print "\u001b[34;1m"
if [ -f logo.txt ]; then
  cat logo.txt
fi

printf "\u001b[37;1mlogfiles stored at  : ${LOG_DIR}\n"
printf "total cpu cores     : $(sysctl -n hw.ncpu)\n"
printf "machine id          : $(uname -mpsr)\n\n"

print "\u001b[37;0m"

printf "\u001b[35m$(pwd):\n\n"


if [[ $ARG1 == "cleanup" ]]; then
  printf "just cleanup logs.\n"
  rm -rf $LOG_DIR
    print "\u001b[0m"
  exit
fi

if [[ ${ARG1} == "run" ]]; then
  ARG1=""
fi


for i in */Makefile; do
  DIR=$(echo "$i" | cut -d"/" -f1)

  printf '\u001b[37;1m* make: "%s" for: [%-26s] => ' "$ARG1" "$DIR"

  if [[ "${ARG2}" == "-n" ]] && [ -f ./"$DIR"/plugin.dylib ]; then
    printf "ignore.\n"
    continue
  fi

  pushd $DIR >/dev/null

#  CMD="time make $ARG1 -j $(sysctl -n hw.ncpu)"
  CMD="time make $ARG1 -j16"

  bash -c "$CMD" >$LOG_DIR/${DIR}.log 2>>$LOG_DIR/${DIR}.log

  if [ $? -ne 0 ]; then
    nok=$((nok + 1))
    printf "\u001b[31mfail! :-( \n"
    print "\u001b[0m"
  else
    ok=$((ok + 1))
    T=$(cat $LOG_DIR/${DIR}.log | grep real | sed "s/real\t//g")
    printf "ok. \u001b[33;1m [$T]\n"
  fi

  popd >/dev/null

done

COUNT=$(ls */Makefile | wc -l)
printf '\u001b[37m\n%s\n%s\n%s\n%s\n\n' "succeeded    : $ok" "failed       : $nok" "total        : $((ok + nok))" "disk usage   : $(du -hac | grep total | cut -d"t" -f1)"

print "\u001b[0m"
if [[ $1 == "run" ]]; then
  printf "Running App..\n"
  cd ..
  make run
fi


# EOF
