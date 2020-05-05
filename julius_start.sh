#!/bin/sh

~/workspace/julius/julius/julius -C /home/pi/workspace/julius-kit/dictation-kit-v4.4/am-gmm.jconf -nostrip -gram /home/pi/workspace/dict/word -input mic -module > /dev/null &
echo $! #プロセスIDを出力
sleep 2 #2秒間スリープ
