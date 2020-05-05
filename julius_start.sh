#!/bin/sh

~/workspace/julius/julius/julius -C ~/workspace/julius-kit/dictation-kit-v4.4/am-gmm.jconf -nostrip -gram ~/workspace/dict/word -input mic -module > /dev/null &
echo $! #プロセスIDを出力
sleep 2 #2秒間スリープ
