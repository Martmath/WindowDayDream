# WindowDayDream
DayDream controller for windows 10


  The daydream controllers is adapted to work with the mouse or CAD CATIA V5 in windows. I understand that it's a too late - but I can't bring myself to throw this code away ( .
Based on [Matteo Pisani article](https://hackernoon.com/how-i-hacked-google-daydream-controller-c4619ef318e4) .
Environment is 64win10 VS2019 (v142 msvc)/16.3.6/std:c++latest environment/64. Tested on built-in Intel8265(nuc 7i5).
  Devices (max 2count) must have the connected status, paired mode is not enough for operation. 
  ![fig. 0](https://github.com/Martmath/WindowDayDream/tree/master/WindowDayDream/picture/0.png?raw=true)
  
  if the device itself goes from connected to paired mode, check the charging of the device. 
Try pressing controller home button before clicking OK button in the desktop app.
Sometimes it helped to plug the charging cable into the controller or pull it out.
After restarting the PC, all devices are in paired mode - this should be remembered. 

  CATIA V5 is likely to be 64-bit. If CATIA is loaded after VS its ole server may not be detected.

  The implementation of the Madgwick filter that I found, takes too long the final values of X Y(or I likely didn't find how to speed it up). So I took the accelerometer for X Y and Madgwick for Z.
The magnet from my column and Aida64 app on my phone showed that my LG has a magnetometer, but my daydreams don't(.
In any case, the first three numbers from Mateo's article is definitely not it.

Great alternative technology - [BTSTACK](https://github.com/bluekitchen/btstack).
I was able to work via 4.0 usb dongle using this (of course on Windows 10). But there is no longer any desire or strength to formalize it(.

I didn't work professionally with C++ or hardware, so use it at your own risk.

Below-an * indicates the position of the touchpad. 

Operating procedure:

Launch the application.

1) Set the device to its base position, as in ![fig. 1](https://github.com/Martmath/WindowDayDream/tree/master/WindowDayDream/picture/1.png?raw=true). Press the home button (the far button from touchpad): |* | ; 
![fig. 1](https://github.com/Martmath/WindowDayDream/tree/master/WindowDayDream/picture/1.png?raw=true)
2) Turn it clockwise on a horizontal plane 90 degrees and press again home button: =* ;
![fig. 2](https://github.com/Martmath/WindowDayDream/tree/master/WindowDayDream/picture/2.png?raw=true)
3) On the old path, return the device to its original starting position. We turn along the long side clockwise. Click home button. |* ;
![fig. 3](https://github.com/Martmath/WindowDayDream/tree/master/WindowDayDream/picture/3.png?raw=true)
4) Return to the starting position. Turn vertically 90 degrees along short side. Click home button. _ 
![fig. 4](https://github.com/Martmath/WindowDayDream/tree/master/WindowDayDream/picture/4.png?raw=true)

 Now you can choose the mode - work with CAD CATIA or the mouse. 
 Long press the home button.
If CATIA is running, we can activate the CAD mode. Click the app button, next to
touchpad (after a long press of the home button). Now by rotating the device we can rotate the CATPart model.
 Buttons to add sound and buttons to decrease sound remove and bring the model closer.
 
 If after a long press of the home button, press the touchpad - the mouse mode is activated.
 Now rotating the device and working through the touchpad, you can move the cursor. Pressing the touchpad will correspond to the left mouse buttons, clicking on the app button is clicking on the right mouse button.
 
  If the CATIA application is not running, thus, after a Long press of the home button, the mouse mode is automatically activated.
To return to device original state, press the home button for a long time again.

 
 Daydream контроллер для windows, адаптирован для работы с мышью и CAD CATIA V5. Понимаю что поздновато - но выбросить код рука не поднялась (.
Основано на [статье Matteo Pisani](https://hackernoon.com/how-i-hacked-google-daydream-controller-c4619ef318e4).
Окружение 64win10 VS2019(v142 msvc)/16.3.6/std:c++latest environment/64. Проверялось на встроенном Intel8265(nuc 7i5)
Девайсы (макс - 2шт.) должны быть приконекчены, сопряжения для работы недостаточно. Если устройство само переходит из connected в paired режим - проверьте зарядку устройства. 
Иногда помогало воткнуть кабель в контроллер или наоборот вытащить.
После перезагрузки ПК все устройства находятся в paired режиме - это надо учитывать.

 Катия скорее всего должна быть 64 битной. Если катия загружена после VS, ole сервер возможно не увидится - подозреваю что придется перегружаться. 
 
 Реализация фильтра Маджвика которую я нашел - слишком долго находит конечные значения X Y (или я, скорее всего, не нашел как ускорить), поэтому для X Y взял акселерометр. Зато она отлично ловит ось Z...

Магнит от моей колонки и аида на телефоне показали что мой lg имеет вполне себе кошерный магнитомер, а мои дейдримы (разные - в т.ч. и косящий под "родной") -нет(. 
Во всяком случае -первые три 13ти битных числа из статьи Матео это точно не он.

Отличная альтернативная технология - [BTSTACK](https://github.com/bluekitchen/btstack).
 Получалось взаимодействовать через 4.0 usb dongle (разумеется на 10ке - грабил потоковые данные в 20бит). Но как-то оформить уже нет ни желания, ни сил(.
 
Ни с плюсами ни с железом профессионально не работал, так что - юзайте на свой страх и риск.

Ниже - * - положнение тачпада:

Запускаем приложение:

 1) Выставляем девайс в базовое положение. Нажимаем home button(дальнюю от тачпада): |* | ;
 2) Поворачиваем по часовой стрелке на горизонтальной плоскости на 90 градусов. Снова нажимаем home button: =* ;
 3) По старому пути возвращаем на прежнее положение девайс. Поворачиваем вдоль длинной стороны по часовой стрелке. Жмем home button. |* ;
 4) Возвращаем в начальное положение. Поворачиваем вверх вертикально на 90 градусов (вдоль короткой стороны). Жмем home button. _ .
 
  Теперь можно выбрать режим - работа с CAD CATIA или мышью. Жмем долго home button.
Если запущена CATIA - мы можем активировать режим работы с CAD. Жмем арр button, рядом с тачпадом (после долгого нажатия home button). Теперь вращая девайс мы можем вращать модель.
Кнопки прибавить/убавить звук удаляют и приближают модель.

 Если после долгого нажатия home button, нажать тачпад - активируется режим работы с мышью. Теперь вращая девайс и работая через тачпад можно перемещать курсор.
 Нажатие на тачпад - левая кнопка мыши, нажатие на app button - правая кнопка мыши.
 
Для возврата в исходное состояние опять долго жмем home button.
