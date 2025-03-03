# Проект "Crack Me"

## Основные задачи

Нужно было написать программу, принимающую пароль и проверяющую его на истинность. Так же, в задаче нужно было целенаправленно допустить две уязвимости (простую и среднюю по сложности). Программа исполнялась в виртуальной машине операционной системы DOS - DOSBox, так как в данной ОС отсутствуют какие-либо ограничения на использование памяти, что сильно упрощает взлом и увеличивает его вариативность.

## Моя программа

Моя программа записана в файле [CrackMe.asm](CrackMe.asm). Она спрашивает у пользователя пароль и сравнивает его с истинным значением ('\0\0\0'), а затем, в зависимости от его истинности, выводит сообщение о подтверждении входа или об ошибке. Пароль, введённый пользователем, размещался в верхушке стека, где было выделено заранее некоторое количество байт специально для него.

#### Случай ввода верного пароля

![alt text](data/CorrectInput.png)

#### Случай ввода неверного пароля

![alt text](data/IncorrectInput.png)

### Лёгкая уязвимость

Лёгкой уязвимостью в моей программе считался пустой пароль (простое нажатие клавиши Enter при запуске программы). Она появлялась, так как память, выделенная под пароль была заполнена нулями, а сам пароль в свою очередь и являлся набором нулей, то есть при вводе пустого пароля в буфер ничего не записывалось, а так как нет проверки на количество введённых символов, то перебор шёл по пустому буферу, значение которого совпадало с паролем, что и давало пользователю верный результат ввода.

```
    mov ah, 0Ah                     ; Функция ввода данных из прерывания DOS'а (21h)

    mov cx, STACK_ARRAY_LEN

llFreeStackArray:
    push 0h
    loop llFreeStackArray           ; Выделение 2 * CX = 2 * STACK_ARRAY_LEN байт под пароль

    push INPUT_MAX_LEN              ; Указание, что можно ввести до INPUT_MAX_LEN байт
    mov dx, sp                      ; DX указывает на байт буфера, определяющий максимальный размер ввода

    int 21h

```

#### Ввод от лица пользователя:

![alt text](data/CorrectInput.png)

### Средняя по сложности уязвимость

Данная уязвимость заключалась в том, что размер выделяемого под пароль буфера был меньше максимального количества считываемых байт, из-за чего пользователь мог перезаписать код возврата из функции, который был расположен сразу после буфера. Таким образом, можно было записать в стек адрес команды, вызывающей функцию вывода подтверждения пароля, пропуская проверку пароля. Таким образом нужно было ввести столько байт, сколько было выделено в стеке, а в конце указать адрес возврата.
Адрес возврата, подходящий для взлома был равен 014A. Следовательно, в конце пароля нужно ввести `(char)01h = Ctrl+A` и `(char)4Ah = J`, однако в обратном порядке, так как в DOS реализована память в формате Little Endian, то есть младшие байты идут раньше старших в памяти.

![alt text](data/MyHardWeakness.png)

## Взлом чужой программы

Я обменялся программами с пользователем [Urodish](https://github.com/kzueirf12345). Его файл расположен [тут](VZLOM.COM). Для дизассемблирования я использовал встроенную программу Turbo Debugger, а так же такие программы, как Radare2 и IDA.

### Лёгкая уязвимость

Лёгкой уязвимостью являлась оставленная проверка на то, что, если пароль начинается с символа '\0', то он должен считаться валидным. Таким образом, достаточно было ввести любой пароль с нулевым символом в начале, который вводится в терминале DOSBox комбиначией `Ctrl + @`.

#### Скриншот из Turbo Debugger'а, на котором видна эта проверка:

![alt text](data/EasyWeakness.png)

### Средняя по сложности уязвимость

Средней по сложности уязвимостью считалось "подвешивание" программы на 21h прерывание, которое обрабатывает вызовы функций DOS'а. Нужно было написать программу, подменяющую обработчик прерываний DOS'а и меняющий ip регистр, лежащий в стеке, то есть адрес возврата из прерывания. Данный метод позволяет при завершении обработки прерывания оказаться на вызове функции печати подтверждения пароля, пропуская его проверку. Такая программа прописана мной в файле [crack.asm](crack.asm). В ней я подсчитываю количество раз вызова обработчика прерывания и в нужный момент (при выводе строки с сообщением о неверном пароле) я меняю адрес возврата и убираю резидентную программу из памяти, подменяя её оригинальным обработчиком прерывания.

Другая версия использования данной уязвимости заключается в подмене адреса строки, которую хочет вывести программа в случае неверного пароля. Аналогично методу, описанному выше, я подсчитываю количество вызовов и в тот же момент, что и при замене адреса возврата, я меняю адрес вывода функции (регистр DX). Данная программа прописана в файле [crack_2.asm](crack_2.asm).

### Бинарный патч

[Программа](src/main.cpp) в папке [src](src) открывает файл `.COM` и меняет байт команды **JE** на команду **JMP** в проверке пароля на истинность. Таким образом, программа в любой ситуации считает пароль истинным, так как проверка всегда выдаёт результат совпадения паролей. Адрес нужного байта я так же узнал благодаря дизассемблированию. Итоговый результат записывается в файл [cracked.com](cracked.com).

#### Запуск программы после бинарного патча:

![alt text](data/CorrectOut.png)

Так же данная программа логирует все свои действия и в параллельном потоке открывает окошко, в котором рисует движение картинки из файла [roflan.com](data/roflan.png) по траектории движения логотипа DVD, а так же запускает музыку из файла [VI_KA.mp3](data/VI_KA.mp3). Для работы с графикой используется библиотека SFML.

#### Вид окна, нарисованного программой:

![alt text](data/RoflanScreenshot.png)

<!-- ## Отдельное сравнение дизассемблеров

### IDA (в DOSBox)

![alt text](data/IDA.png)

### Turbo Debugger

![alt text](data/TurboDebugger.png)

### Radare2

#### Вывод в дизассемблере кода с комментариями

![alt text](data/Radare2Comments.png)

#### Отображение зависимостей в **jump**'ах и **call**'ах

![alt text](data/Radare2JMP.png)

### Сравнение

IDA позволяет просмотреть весь дизассемблированный код в одном файле, однако, Turbo Debugger, по сравнению с ним, позволяет так же увидеть данные о состоянии регистров, флагов, содержимом стека и любого фрагмента памяти. Однако, Radare2 всё равно выглядит лучше на их фоне, так как по мимо всего выше перечисленного, он позволяет просмотреть таблицу функций, таблицу переменных, а так же рисует стрелками переходы в **jump**'ах, **call**'ах и **ret**'ах, что упрощает понимание программы и предугадывание дальнейшего её поведения.

Таким образом, наиболее полезным и удобным дизассемблером из этих трёх является Radare2. -->
