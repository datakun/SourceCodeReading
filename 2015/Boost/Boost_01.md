# Boost 준비하기
1. Visual Studio 2015 Preview 설치
  - http://www.visualstudio.com/en-us/downloads/visual-studio-2015-downloads-vs.aspx
2. Boost 라이브러리 설치
  - http://www.boost.org/ 에서 최신버전 다운로드
  - 적절한 위치에 압축 해제
3. b2.exe 만들기
  - vs2014 x86 Native Tools Command Prompt 실행
  - cd 명령어로 2번에서 압축 푼 위치로 이동
  - ```> bootstrap.bat``` 입력
4. 빌드
  - vs2014 x86 Native Tools Command Prompt 실행
  - cd 명령어로 2번에서 압축 푼 위치로 이동
  - ```> b2 -a --stagedir=x86 variant=debug,release link=static threading=multi stage``` 입력
5. Visual Studio 프로젝트에 적용하기
  - 메뉴 Project > Properties > Configuration Properties > C/C++ > General > Additional Include Directories 에 "2번의 위치" 입력
  - 메뉴 Project > Properties > Configuration Properties > Linker > General > Additional Library Directories 에 "(2번의 위치)\stage\lib" 입력
6. Boost 1.57 버전 기준으로 stage\lib 폴더의 용량이 736MB 가 아니라면 2번의 압축 푼 위치에서 *.lib으로 검색하여 모든 lib 파일들을 stage\lib에 옮기자.

- 윈도우 Visual Studio 에서 Boost 라이브러리 이용하기
- Visual Studio 2015 Preview 기준으로 진행
