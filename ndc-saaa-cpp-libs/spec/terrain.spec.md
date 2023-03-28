# terrain フォルダ内のプログラム概要

## TerrainEngine

地形情報を外部に公開するための主な機能を提供する。

内部では BaseLayer（乱数シードから一意に生成される地形データ）と
ModLayer（変更差分レイヤー）を管理する機能を持つ。

## TerrainBaseConfig

BaseLayer に渡され、地形生成のパラメータとして読み取られる Config 。


## TerrainBaseLayerGenerator

乱数シードを元に一意に生成される BaseLayer を生成する。
