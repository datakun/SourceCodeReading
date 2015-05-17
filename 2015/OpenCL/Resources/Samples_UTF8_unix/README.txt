rev1.1, 28.Nov.2010

サンプルファイル一覧
====================
以下は、書籍のリスト番号と、対応するファイルの一覧です。

 1. リスト3.1  Chapter3/main.cpp
 2. リスト4.1  Chapter4/AddVector1/main.cpp
 3. リスト4.3  Chapter4/AddVector2/main.cpp
 4. リスト5.1  Chapter5/Bitmap/bitmapspec.h
 5. リスト5.2  Chapter5/Bitmap/Bitmap.h
 6. リスト5.3  Chapter5/Bitmap/Bitmap.cpp
 7. リスト5.4  Chapter5/Bitmap/MyError.h
 8. リスト5.8  Chapter5/Mandelbrot/main.cpp
 9. リスト5.9  Chapter5/Mandelbrot/Mandelbrot.h
10. リスト5.10 Chapter5/Mandelbrot/Mandelbrot.cpp
11. リスト5.11 Chapter5/Mandelbrot/VdcMapper.h
12. リスト5.12 Chapter5/Mandelbrot/VdcMapper.cpp
13. リスト5.13 Chapter5/Mandelbrot/ColorTable.h
14. リスト5.14 Chapter5/Mandelbrot/ColorTable.cpp
15. リスト5.15 Chapter5/Mandelbrot/ClHelper.h
16. リスト5.16 Chapter5/Mandelbrot/ClHelper.cpp
17. リスト5.17 Chapter5/Mandelbrot/calc.cl
18. リスト5.18 Chapter5/Softfocus1/main.cpp
19. リスト5.19 Chapter5/Softfocus2/calc.cl
20. リスト5.20 Chapter5/Softfocus2/main.cpp
21. リスト5.21 Chapter5/Softfocus3/main.cpp
22. リスト5.22 Chapter5/Softfocus3/calc.cl
23. リスト6.1  Chapter6/6.1_Logging/main.cpp
24. リスト6.2  Chapter6/6.1_Logging/calc.cl
25. リスト6.3  Chapter6/6.2_Profiling/main.cpp
26. リスト6.4  Chapter6/6.2_Profiling/calc.cl
27. リスト6.5  Chapter6/6.3_MacOS/Stopwatch.h
28. リスト6.6  Chapter6/6.3_MacOS/Stopwatch.cpp
29. リスト6.7  Chapter6/6.3_Linux/Stopwatch.h
30. リスト6.8  Chapter6/6.3_Linux/Stopwatch.cpp
31. リスト6.9  Chapter6/6.3_Windows/Stopwatch.h
32. リスト6.10 Chapter6/6.3_Windows/Stopwatch.cpp


変更履歴
========

rev1.1 (2010年11月28日)
-----------------------
  1. OpenCL 1.1への対応
     OpenCL 1.1で発現すると思われる、以下の問題を修正しています。
     (NVIDIA CUDA Toolkit 3.2 RC 2 (October 2010), Windows版で現象を確認)

    1) clCreateContextFromType が CL_INVALID_PLATFORM を返して失敗する
       clCreateContextFromType のプロパティに NULL を渡した場合、コンテキスト
       の作成に失敗し、ステータスとして CL_INVALID_PLATFORM が返される場合があ
       る。この問題に対応するため、プラットフォーム ID を取得した後、プロパティ
       として clCreateContextFromType に渡すようにコードを変更。

       変更ファイル
       Chapter4/AddVector2/main.cpp
       Chapter5/Mandelbrot/ClHelper.cpp
       Chapter6/6.1_Logging/main.cpp
       Chapter6/6.2_Profiling/main.cpp

    2) カーネル関数のビルドが失敗する
       5章および6章のサンプルプログラム実行時に、カーネルのビルドが失敗する以
       下の不具合を修正しています。

       - 非ポインタ引数にアドレススペースを指定していた不具合を修正
       - 文字列リテラルとして記述していたメッセージ用文字列を __constant 空間
         に配置するように修正

       変更ファイル
       Chapter5/Mandelbrot/calc.cl
       Chapter6/6.1_Logging/calc.cl

  2. 不具合修正
     Chapter5/Softfocus3 が正しく動作しない問題を修正しています。

       変更ファイル
       Chapter5/Softfocus3/main.cpp
       Chapter5/Softfocus3/calc.cl


rev1.0 (2010年5月5日)
-----------------------
  1. 書籍からの変更
     リスト5.16, 6.1, 6.3 には、以下の変更をおこなっています。

      - リスト5.16 (ClHelper.cpp)
         - 関数 ClHelper::loadProgramSource において、以下の変更を適用
           - fclose の呼び出しを追加
           - fopen に渡すパラメータを "r" から "rb" に変更

      - リスト6.1 (main.cpp)
         - 関数 main において、以下の変更を適用
           - fclose の呼び出しを追加
           - fopen に渡すパラメータを "r" から "rb" に変更

      - リスト6.3 (main.cpp)
         - 関数 main において、以下の変更を適用
           - fclose の呼び出しを追加
           - fopen に渡すパラメータを "r" から "rb" に変更
