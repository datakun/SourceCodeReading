# 나중에 정리
- QCheckBox의 크기를 키우고 싶다.
  - http://stackoverflow.com/questions/6302428/increase-checkbox-size-not-its-text-in-qt
  - 다음 코드를 setStyleSheet()로 적용
  ```
  QCheckBox::indicator {
     width: 40px;
     height: 40px;
  }
  QCheckBox::indicator:checked
  {
    image: url(../Checkbox_checked_normal.png);
  }
  QCheckBox::indicator:unchecked
  {
    image: url(../Checkbox_unchecked_normal.png);
  }

  QCheckBox::indicator:checked:hover
  {
    image: url(../Checkbox_checked_hovered.png);
  }
  QCheckBox::indicator:unchecked:hover
  {
    image: url(../Checkbox_unchecked_hovered.png);
  }
  QCheckBox::indicator:checked:pressed
  {
    image: url(../Checkbox_checked_pressed.png);
  }
  QCheckBox::indicator:unchecked:pressed
  {
    image: url(../Checkbox_unchecked_pressed.png);
  }
  QCheckBox::indicator:checked:disabled
  {
    image: url(../Checkbox_checked_disabled.png);
  }
  ```
- 마우스 휠을 이용하여 가로 스크롤바를 움직이고 싶다
  - 다음 코드를 적용
  ```
  int x -= (event->delta() / 120) * 30;
  if (x < 0)
      x = 0;
  else if (x > ui.scrollArea->horizontalScrollBar()->maximum())
      x = ui.scrollArea->horizontalScrollBar()->maximum();
  ui.scrollArea->horizontalScrollBar()->setValue(x);
  ```
