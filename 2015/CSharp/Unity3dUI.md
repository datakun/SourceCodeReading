# Unity3D로 대화 상자를 받아보자. (uGUI)
## 참고사항
- http://ncanis.tistory.com/429 를 참고
- Unity3D UI Tutorial은 http://unity3d.com/learn/tutorials/modules/beginner/ui 참고

## 레이아웃 잡기!
1. Canvas를 생성 (GameObject > UI > Canvas)
2. Canvas 하위에 Panel 생성 (GameObject > UI > Panel)
3. Panel에 ```Vertical Layout Group```과 ```Content Size Fitter``` script를 생성 (Inspector의 Add Component에서 검색)
4. 3번의 ```Vertical Layout Group``` 속성 중 ```padding```을 적절한 값으로 설정 (여기서는 10)
5. 3번의 ```Content Size Fitter``` 속성 중 ```Vertical Fit```을 ```Preferred Size```로 변경, ```Horizontal Fit```을 ```Unconstrained```로 변경
6. Panel 하위에 Text 생성 (GameObject > UI > Text)
7. Text의 ```Font Size```를 적절한 값으로 설정
  - HTML 태그 사용 가능 ([링크](http://docs.unity3d.com/Manual/StyledText.html))
8. Panel 하위에 빈 GameObject 생성 (GameObject > Create Empty)
9. 빈 GameObject 하위에 Button 생성 (GameObject > UI > Button)
10. 생성한 Button이 너무 작다면 최소 보장 높이를 설정하자
  - Button에 ```Layout Element``` script 생성 (Inspector의 Add Component에서 검색)
  - ```Layout Element``` script에서 Preferred Height를 적절한 값으로 설정 (여기서는 50)

## 버튼을 눌렀을 때 해보자! (텍스트 변경)
1. 버튼을 선택한 후, Inspector의 On Click () 부분의 No Object에 Text 오브젝트를 끌어옴
2. No Funtion이 활성화 되면, Text > string text를 선택하고 변경할 텍스트를 입력
3. 끝. 버튼을 누르면 결과 확인 가능 
