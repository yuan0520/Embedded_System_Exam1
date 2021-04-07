# Embedded System Exam 1

## (1) how to setup and run my program

在這次的作業中，我在 **main.cpp** 中實作了包含 uLCD、button、調整波的頻率，以及 sample data 等等工作。並利用和 Lab 時相同的 **FFT.py** 去畫出 waveform 以及 frequency 的圖。


### 1. 變數宣告

接下來我宣告了一些變數，因為我要 sample 1000 個 data，所以將變數 `sample` 值定為 1000，而 `ADCdata` 這個 array 就是用來存 sample 出來的 data 的，我將他的大小開超過 1000 以利安全。而因為我只要在 screen 中 print 一次我 sample 出來的資料，所以我設一個 `flag`，如果 print 過了就將 `flag` 設為 1，就不會再跑進去 print data 的地方了。`idx` 則是在存 data 時控制的 index 值。

再來我宣告了一些關於 thread 跟 event queue 的變數，`menu_t` 是控制選單的 thread，`menu_queue` 是控制跟 menu 有關的 event 的 queue，`wave_t` 和 `wave_queue` 則依此類推。

再來是跟波形有關的變數，`wavelength_scale` 的值 always 為 1，因為 aout 最大的值為 1。`freq_pos` 的值從 0~3 分別代表 slew rate 等於 1/8、1/4、1/2、1。`freq_scale` 就代表 slew rate 大小。S 代表在 slew rate 為 1 的情況下，波要經過 80ms 才會到 peak。所以 aout 的上升及下降大小即為 1.0 / (S*freq_scale)，可以對應到每種 slew rate。



<div align=center><img src="https://i.imgur.com/z1hObOf.png" width="60%" height="60%" /></div>

### 2. uLCD 選單 

我設計了 `menu` 這個 function 來控制 uLCD 的顯示，利用傳入的 `freq_pos` 的值來選擇顯示的樣式，如下圖，`freq_pos` 的初始值為 1，代表選擇的 slew rate 為 1，可以透過 button 來操作選單，進而改變 `freq_pos` 的值，也就是 slew rate的值。

<div align=center><img src="https://i.imgur.com/HhTxnMz.png" width="50%" height="50%" /></div>


### 3. button 之操作

首先我先宣告了三個 `InterruptIn` 的變數 `btn_up`、`btn_down`、`btn_sel`，分別對應到三個 button，再來是定義這三個 button 的 ISR，分別為下面三個函式 `up_btn_task`、`down_btn_task`、`sel_btn_task`，up、down button 負責控制螢幕顯示，sel button 負責選擇 slew rate，兩者分別是使用不同 event queue 來進行，前者是使用 `menu_queue`，後者是使用 `wave_queue`。最後則是在 main function 中 call `btn.rise`，將他們的 ISR attach 上去。

<div align=center><img src="https://i.imgur.com/MsXyQQ6.png" width="50%" height="50%" /></div>
<br>

<div align=center><img src="https://i.imgur.com/S79o2mk.png" width="50%" height="50%" /></div>
<br>

<div align=center><img src="https://i.imgur.com/KmRvuGj.png" width="50%" height="50%" /></div>


### 4. 控制波的部分、I/O

因為要將波呈現在 picoscope 中，所以我將電壓上升和下將分成兩個迴圈來寫，len 就是上升和下降的大小，剛剛已經定義過了，控制著不同 slew rate 的波的上升和下降速度。

中間的 if 判斷式們，判斷在各 slew rate下，電壓應該要維持在 peak 多久。

最後就是 sample 波的迴圈，我總共 sample 1000 個 電壓來畫圖，為了讓他接近原本的波形，我在電壓大小每次更變時也順便 sample，然後會用 `flag` 來判斷是不是已經 sample 過了，總共只會 sample 一次。


<div align=center><img src="https://i.imgur.com/wDZrVG1.png" width="50%" height="50%" /></div>
<div align=center><img src="https://i.imgur.com/M8lCRjP.png" width="50%" height="50%" /></div>

---

## (2) what are the results


### 1. slew rate = 1

<div align=center><img src="https://i.imgur.com/32Med4P.jpg" width="50%" height="50%" /></div><br>

<div align=center><img src="https://i.imgur.com/uHRSWzK.png" width="50%" height="50%" /></div>


### 2. slew rate = 1/2

<div align=center><img src="https://i.imgur.com/ejgKDma.png" width="50%" height="50%" /></div>
<br>
<div align=center><img src="https://i.imgur.com/I8hYGED.png" width="50%" height="50%" /></div>



### 3. slew rate = 1/4

<div align=center><img src="https://i.imgur.com/oexwA0W.png" width="50%" height="50%" /></div>
<br>
<div align=center><img src="https://i.imgur.com/e9saSQC.png" width="50%" height="50%" /></div>


### 4. slew rate = 1/8

<div align=center><img src="https://i.imgur.com/KKh0nYO.png" width="50%" height="50%" /></div>
<br>
<div align=center><img src="https://i.imgur.com/qd60h2A.png" width="50%" height="50%" /></div>

---

### If you can't open the circuit image file

<img src="https://i.imgur.com/eJUPVvD.jpg" width="50%" height="50%" />
<br><br>
<img src="https://i.imgur.com/ks5253R.jpg" width="50%" height="50%" />
<br><br>
<img src="https://i.imgur.com/rgQpHfl.jpg" width="50%" height="50%" />
<br><br>
<img src="https://i.imgur.com/mRLXKWm.jpg" width="50%" height="50%" />
