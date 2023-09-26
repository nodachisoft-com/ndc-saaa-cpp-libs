# ライブラリ概要

ndc-cpp-libs ライブラリはアプリケーションのビジネスロジックに依存しない
共通的な機能を集約したものです。
UnrealEngine などから外部ライブラリとして呼び出し可能なよう設計・実装します。

# コンパイル環境

- Windows 上の Visual Studio 2022 同梱の CL.exe によるコンパイル
- Linux 上の Clang によるコンパイル
  - clang version 10.0.0-4ubuntu1 ( code-server @ github code-spaces)

# Windows 上でのコンパイル

# make コマンドによるコンパイル

@TODO

# UnitTest

gtest の portable を利用してテストを行います。
project フォルダの直下で以下を実行します。

@TODO

# ライブラリに含む機能

すべてライブラリは名前空間 nl にて定義されます。

## cmdInvoker


## csv

CsvReader 型で CSV データの読み込み、指定セル値の取得、指定行の複数セル値の取得できます。
std::string 型を CsvReader に読み込ませると、内部でセルデータに解析され、その後、指定のセル、行データを取得できます。
現時点で、データ読み取り専用のクラスです。（CSVデータのセル値更新には未対応）

## 




