
# Refarence : 参考資料

## raspberrypi
[raspberry pi 初期セットアップ](https://qiita.com/HeRo/items/c1c30d7267faeb304538#sshでの接続)

## julius

[juliusのセットアップ](https://www.pc-koubou.jp/magazine/19743)　
[juliusの辞書の作成](https://qiita.com/fishkiller/items/40c271d1216921e29b91)

### juliusのdirectory構成
$HOME
  |- workspace
        |------- julius(juliusセットアップ済み)
        |------- julius-kit
                    |-------- dictation-kit-v4.4
        |------- dict
                    |-------- word.yomi(辞書ファイル)
                    |-------- word.phone(音素ファイル)
                    |-------- word.grammar(構文ファイル)
                    |-------- word.voca(語彙ファイル)
                    |-------- 辞書ファイル郡

### juliusの辞書の作成

word.yomiを作成(word + TABキー + ひらがな)
```
おはよう      おはよう
こんにちわ      こんにちわ
```

辞書ファイルの作成
* 音素ファイルの作成
```
iconv ~/workspace/dict/word.yomi | ~/workspace/julius/gramtools/yomi2voca/yomi2voca.pl > ~/workspace/dict/word.phone
```

word.phone
```
おはよう        o h a y o u
こんにちわ      k o N n i ch i w a
```


* 構文ファイルの作成
```
vim word.grammar
```

word.grammar
```
S : NS_B GREET NS_E
GREET : OHAYOU
GREET : KONNICHIWA
```

* 語彙ファイルの作成
```
vim word.voca
```

word.voca
```
% OHAYOU
おはよう        o h a y o u
% KONNICHIWA
こんにちは      k o N n i ch i w a
% NS_B
[s] silB
% NS_E
[s] silE
```

* 辞書ファイルの作成
音素ファイル、構文ファイル、語彙ファイルとjuliusのスクリプト（mkdfa.pl）を使用して辞書ファイルを作成する
```
cp -b ~/workspace/julius/gramtools/mkdfa/mkfa-1.44-flex/mkfa ~/workspace/julius/gramtools/mkdfa/mkfa
cp -b ~/workspace/julius/gramtools/dfa_minimize/dfa_minimize ~/workspace/julius/gramtools/mkdfa/dfa_minimize
~/workspace/julius/gramtools/mkdfa/mkdfa.pl ~/workspace/dict/word
```

* 作成した辞書でjuliusの実行
```
~/workspace/julius/julius/julius -C ~/workspace/julius-kit/dictation-kit-v4.4/am-gmm.jconf -nostrip -gram ~/workspace/dict/word -input mic
```

## python環境の構築
```
cd ~/workspace/
python3 -m venv venv
source venv/bin/activate
pip3 install numpy --upgrade
export PYTHONPATH="/usr/lib/python3/dist-packages"
```